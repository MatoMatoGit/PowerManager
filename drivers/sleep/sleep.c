/*
 * sleep.c
 *
 * Created: 23-03-2019 17:45:35
 *  Author: dorspi
 */ 
#include "sleep.h"

#include "wdt.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define WDT_CYCLE_TO_SEC_MAP_SIZE 4

typedef struct  {
	WdtCycles_t cycles;
	uint8_t sec;
} WdtCycleToSec_t;

const WdtCycleToSec_t WdtCycleToSecMapping[WDT_CYCLE_TO_SEC_MAP_SIZE] = {
	{.cycles = WDT_CYCLES_128K, .sec = 1},
	{.cycles = WDT_CYCLES_256K, .sec = 2},
	{.cycles = WDT_CYCLES_512K, .sec = 4},
	{.cycles = WDT_CYCLES_1024K, .sec = 8},
};

static volatile uint8_t wdt_flag = 0;

static SleepWakeupCallback_t WakeupCallback = NULL;

static void WdtTimeoutCallback(void);
static WdtCycles_t WdtCyclesFromSec(uint32_t sec_total, uint8_t *sec_wdt);

void SleepWakeupCallbackSet(SleepWakeupCallback_t wakeup_cb)
{
	WakeupCallback = wakeup_cb;
}

void SleepForDuration(uint32_t duration_s)
{
	//static uint32_t wdt_cnt = 0;
	uint8_t sleep_s = 0;

	do {
		/* Get the sleep time and WDT cycles from the total sleep duration.
		 * Enable the WDT to timeout after that many cycles.  */
		WdtEnable(WdtCyclesFromSec(duration_s, &sleep_s), WdtTimeoutCallback);
			
		/* Set sleep mode and enable sleep. 
		 * Disables power of peripherals. */
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		
		sei();
		
		/* Go to sleep. */
		sleep_mode();
		
		/* Resumed here when woken. */
		
		cli();
		
		/* Disable sleep mode. */
		sleep_disable();

		/* Enable power to peripherals. */
		power_all_enable();
		
		sei();
		
		/* Check if the MCU was woken by the WDT. */
		if(wdt_flag) {
			/* Decrement the total sleep duration by the amount
			 * of seconds slept. */
			duration_s = (duration_s >= sleep_s ? duration_s - sleep_s : 0);
			wdt_flag = 0;
		}
	/* Loop while there is still time left to sleep. */
	} while(duration_s > 0);
	
	if(WakeupCallback != NULL) {
		WakeupCallback();
	}
	
	WdtDisable();
}

static void WdtTimeoutCallback(void)
{
	if(wdt_flag == 0) {
		wdt_flag = 1;
		WdtDisable();
	}
}

static WdtCycles_t WdtCyclesFromSec(uint32_t sec_total, uint8_t *sec_wdt)
{
	WdtCycles_t cycles = WdtCycleToSecMapping[0].cycles;
	*sec_wdt = WdtCycleToSecMapping[0].sec;
	
	for(uint8_t i = 1; i < WDT_CYCLE_TO_SEC_MAP_SIZE; i++) {
		if(sec_total >= WdtCycleToSecMapping[i].sec) {
			cycles = WdtCycleToSecMapping[i].cycles;
			*sec_wdt = WdtCycleToSecMapping[i].sec;
		}
	}
	
	return cycles;
}