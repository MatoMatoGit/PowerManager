#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SoftwareSerial.h"
#include "oc_timer.h"
#include "wdt.h"
#include "sleep.h"
#include "soft_timer.h"
#include "ext_int.h"
#include "fw_info.h"

#include "power_mngr.h"

#define ENABLE_SERIAL_OUTPUT 1

#define AWAKE_TIME_MAX_MSEC		600000	/* 10 Mins. */
#define SLEEP_TIME_MIN_SEC		300	/* 5 Mins. */

#define SOFTSERIAL_TX_DDR	DDRB
#define SOFTSERIAL_TX_PORT	PORTB
#define SOFTSERIAL_TX_PIN	PINB2
#define SOFTSERIAL_RX_PIN	PINB1

/* avrdude.exe -p t85 -B 25 -c usbasp -U flash:w:"C:\Users\dorspi\Documents\Atmel Studio\7.0\mvsensor_fw\Debug\mvsensor_fw.hex":i */
/* avrdude.exe -p t85 -B 25 -c usbasp -U hfuse:w:0xDF:m */
/* avrdude.exe -p t85 -B 25 -c usbasp -U lfuse:w:0xE2:m */

static volatile bool wdt_wake = false;

static ExtInt_t UartInt = NULL;
static SoftTimer_t AwakeTimer;

static volatile bool InitSleep = false;
static volatile uint32_t SleepTime = 5;
static uint8_t Status = 0x01;

void WakeupCallback(void);
static void SwitchToStandby(uint32_t sleep_time_s);
static void SerialGpioInit(void);
static void CommandSleep(PowerMngrCmd_e cmd, void *args);
static void CommandStatus(PowerMngrCmd_e cmd, void *args);
static void AwakeTimerCallback(SoftTimer_t timer);

int main(void)
{
	WdtDisable();
	cli();
	MCUCR = 0;
	MCUSR = 0;
	PORTB = 0;
	DDRB &= ~(1 << PINB0) & ~(1 << PINB1) & ~(1 << PINB2) & ~(1 << PINB3) & ~(1 << PINB4);
	DDRB |= (1 << PINB3);
	DDRB |= (1 << PINB4);
	
	static int log_res = 0;

	softSerialInit(&DDRB, &PORTB, &PINB, SOFTSERIAL_RX_PIN, SOFTSERIAL_TX_PIN); //SOFTWARE_SERIAL_RX_DISABLED
	softSerialBegin(2400);
	SerialGpioInit();

	SoftTimerInit();
	AwakeTimer = SoftTimerCreate(AWAKE_TIME_MAX_MSEC, AwakeTimerCallback);

	FwInfoInit();
	
#if ENABLE_SERIAL_OUTPUT==1
	FwInfoPrint();
#endif

	SleepWakeupCallbackSet(WakeupCallback);
	
	ExtIntInit();
	UartInt = ExtIntRegister(SOFTSERIAL_RX_PIN, &PORTB, EXT_INT_PIN_CHANGE, softSerialRxHandler);

	softSerialPrintLn("boot");
	ExtIntEnable(UartInt);
	PowerMngrRegisterCallback(CMD_SLEEP, CommandSleep);
	PowerMngrRegisterCallback(CMD_STATUS, CommandStatus);
	
	SoftTimerStart(AwakeTimer);
	
	PORTB |= (1 << PINB4);
	
    while (1)
    {	
		int c = softSerialRead();
		if(c >= 0) {
			softSerialPrintInt(c);
			softSerialPrintLn("");
			if(c != '\n') {
				PowerMngrRxHandler(c);
			}
		}
		_delay_ms(200);
		
		if(InitSleep == false) {
			continue;
		}
		
		SoftTimerStop(AwakeTimer);
		SoftTimerDeinit();
		
		PORTB &= ~(1 << PINB4);
		SwitchToStandby(SleepTime);
		
		InitSleep = false;
		
		SerialGpioInit();
		ExtIntEnable(UartInt);
				
		DDRB |= (1 << PINB3);
		
		DDRB |= (1 << PINB4);
		PORTB |= (1 << PINB4);
		
		SoftTimerInit();
		SoftTimerStart(AwakeTimer);
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
#if ENABLE_SERIAL_OUTPUT==1
	softSerialPrint("sleep: ");
	softSerialPrintIntLn(sleep_time_s);
#endif
	
	/* Disable all outputs and set all IOs as inputs. */
	PORTB = 0;
	DDRB &= ~(1 << PINB0) & ~(1 << PINB1) & ~(1 << PINB2) & ~(1 << PINB3) & ~(1 << PINB4);
	
	ExtIntDisable();
	SleepForDuration(sleep_time_s);
}

static void SerialGpioInit(void)
{
	DDRB |= (1 << SOFTSERIAL_TX_PIN);
	PORTB |= (1 << SOFTSERIAL_TX_PIN);
	
	DDRB &= ~(1 << SOFTSERIAL_RX_PIN);
}

static void CommandSleep(PowerMngrCmd_e cmd, void *args)
{
	softSerialPrintLn("received sleep command");	
	InitSleep = true;
	SleepTime = *((uint32_t *)args);
}

static void CommandStatus(PowerMngrCmd_e cmd, void *args)
{
	*((uint8_t *)args) = Status;
}

static void AwakeTimerCallback(SoftTimer_t timer)
{
	softSerialPrintLn("awake for max duration, forcing sleep");
	InitSleep = true;
}