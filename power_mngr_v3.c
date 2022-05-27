#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "oc_timer.h"
#include "wdt.h"
#include "sleep.h"
#include "soft_timer.h"
#include "ext_int.h"
#include "fw_info.h"

#define AWAKE_TIME_MAX_MSEC		600000	/* 10 Mins. */
#define SLEEP_TIME_SHORT_SEC	1//300	/* 5 Mins. */
#define SLEEP_TIME_LONG_SEC		5

#define SLEEP_ACTIVATE	PINB1
#define SLEEP_SELECT	PINB2
#define LDO_EN_PIN	PINB3


/* avrdude.exe -p t85 -B 25 -c usbasp -U flash:w:"C:\Users\dorspi\Documents\Atmel Studio\7.0\mvsensor_fw\Debug\mvsensor_fw.hex":i */
/* avrdude.exe -p t85 -B 25 -c usbasp -U hfuse:w:0xDF:m */
/* avrdude.exe -p t85 -B 25 -c usbasp -U lfuse:w:0xE2:m */

typedef enum {
	AWAIT_ACTIVATE = 0,
	SLEEP = 1,
	WAKE = 2
}State_e;

static volatile bool wdt_wake = false;

static SoftTimer_t AwakeTimer;

void WakeupCallback(void);
static void SwitchToStandby(uint32_t sleep_time_s);
static void AwakeTimerCallback(SoftTimer_t timer);

static void GpioInit(void);
static void GpioDeinit(void);
static void LdoEnable(void);
static void LdoDisable(void);

int main(void)
{
	WdtDisable();
	cli();
	MCUCR = 0;
	MCUSR = 0;
	PORTB = 0;
	
	static int log_res = 0;

	softSerialInit(&DDRB, &PORTB, &PINB, SOFTWARE_SERIAL_RX_DISABLED, SOFTSERIAL_TX_PIN);
	// softSerialBegin(2400);
	// SerialGpioInit();

	// SoftTimerInit();
	// AwakeTimer = SoftTimerCreate(AWAKE_TIME_MAX_MSEC, AwakeTimerCallback);

	FwInfoInit();
	
#if ENABLE_SERIAL_OUTPUT==1
	FwInfoPrint();
#endif

	SleepWakeupCallbackSet(WakeupCallback);
	
	ExtIntInit();
	PulseInt = ExtIntRegister(PULSE_PIN, &PORTB, EXT_INT_PIN_CHANGE, PulseInterruptHandler);

	
	// SoftTimerStart(AwakeTimer);
	uint32_t exp = 1;
	uint32_t fact = 1;
	uint32_t sleep_time_sec = 0;

    while (1)
    {	
		static State_e state = WAKE;
		
		switch(state) {
			case AWAIT_ACTIVATE:
			
			break;

			case SLEEP:
			sleep_time_sec = int(pow(10, exp)) * fact;
			softSerialPrintIntLn(sleep_time_sec);
			_delay_ms(1000);
			LdoDisable();
			GpioDeinit();
			SwitchToStandby(sleep_time_sec);
			state = WAKE;
			break;

			case WAKE:
			sleep_time_sec = 0;
			fact = 1;
			exp = 1;
			GpioInit();
			softSerialBegin(2400);
			_delay_ms(100);
			LdoEnable();
			state = AWAIT_SELECT_EXP;
			break;

			default:
			break;
		}


    }
}

void WakeupCallback(void)
{
	if(wdt_wake == false) {
		wdt_wake = true;
		//MoistureInit();
	}
}

static void SwitchToStandby(uint32_t sleep_time_s)
{
	SleepForDuration(sleep_time_s);
}

static void AwakeTimerCallback(SoftTimer_t timer)
{
	softSerialPrintLn("awake for max duration, forcing sleep");
	InitSleep = true;
}

static bool SleepActivatState(void)
{
	return PINB & SLEEP_ACTIVATE;
}

static bool SleepSelectState(void)
{
	return PINB & SLEEP_SELECT;
}

static void GpioInit(void)
{
	DDRB &= (1 << SLEEP_ACTIVATE) & ~(1 << SLEEP_SELECT);
	DDRB |= (1 << LDO_EN_PIN);
	DDRB |= (1 << PINB4);
}

static void GpioDeinit(void)
{
	PORTB = 0;
	DDRB &= ~(1 << PINB0) & ~(1 << PINB1) & ~(1 << PINB2) & ~(1 << PINB3) & ~(1 << PINB4);
}

static void LdoEnable(void)
{
	PORTB |= (1 << LDO_EN_PIN);
}

static void LdoDisable(void)
{
	PORTB &= ~(1 << LDO_EN_PIN);
}