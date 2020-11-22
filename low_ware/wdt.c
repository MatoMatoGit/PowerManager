/*
 * wdt.c
 *
 * Created: 23-03-2019 17:07:11
 *  Author: dorspi
 */ 

#include "wdt.h"

#include <stdlib.h>
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static WdtCallback_t Callback = NULL;

static uint8_t WdtPrescalers[WDT_CYCLES_NUM] = {
	0,									/* 2K cycles */
	(1<<WDP0),							/* 4K cycles. */
	(1<<WDP1),							/* 8K cycles. */
	(1<<WDP1) | (1<<WDP0),				/* 16K cycles. */
	(1<<WDP2),							/* 32K cycles. */
	(1<<WDP2) | (1<<WDP0),				/* 64K cycles. */
	(1<<WDP2) | (1<<WDP1),				/* 128K cycles. */
	(1<<WDP2) | (1<<WDP1) | (1<<WDP0) ,	/* 256K cycles. */
	(1<<WDP3),							/* 512K cycles. */
	(1<<WDP3) | (1<<WDP0),				/* 1024K cycles. */
};

void WdtEnable(WdtCycles_t cycles, WdtCallback_t cb)
{
	Callback = cb;
	
	cli();
	
	// The MCU Status Register (MCUSR) is used to tell the cause of the last
	// reset, such as brown-out reset, watchdog reset, etc.
	// NOTE: for security reasons, there is a timed sequence for clearing the
	// WDE and changing the time-out configuration. If you don't use this
	// sequence properly, you'll get unexpected results.

	// Clear the reset flag on the MCUSR, the WDRF bit (bit 3).
	MCUSR = 0;
	MCUSR &= ~(1<<WDRF);

	// Configure the Watchdog timer Control Register (WDTCSR)
	// The WDTCSR is used for configuring the time-out, mode of operation, etc

	// In order to change WDE or the pre-scaler, we need to set WDCE (This will
	// allow updates for 4 clock cycles).

	// Set the WDCE bit (bit 4) and the WDE bit (bit 3) of the WDTCSR. The WDCE
	// bit must be set in order to change WDE or the watchdog pre-scalers.
	// Setting the WDCE bit will allow updates to the pre-scalers and WDE for 4
	// clock cycles then it will be reset by hardware.
	WDTCR |= (1<<WDCE) | (1<<WDE);

	/**
	 *	Setting the watchdog pre-scaler value with VCC = 5.0V
	 *	WDP3 WDP2 WDP1 WDP0 | Number of WDT | Typical Time-out at Oscillator Cycles
	 *	0    0    0    0    |   2K cycles   | 16 ms
	 *	0    0    0    1    |   4K cycles   | 32 ms
	 *	0    0    1    0    |   8K cycles   | 64 ms
	 *	0    0    1    1    |  16K cycles   | 0.125 s
	 *	0    1    0    0    |  32K cycles   | 0.25 s
	 *	0    1    0    1    |  64K cycles   | 0.5 s
	 *	0    1    1    0    |  128K cycles  | 1.0 s
	 *	0    1    1    1    |  256K cycles  | 2.0 s
	 *	1    0    0    0    |  512K cycles  | 4.0 s
	 *	1    0    0    1    | 1024K cycles  | 8.0 s
	*/
	WDTCR = WdtPrescalers[cycles];
	// Enable the WD interrupt (note: no reset).
	WDTCR |= (1 << WDIE);
	
	sei();
}

void WdtDisable(void)
{
	cli();
	
	MCUSR &= ~(1<<WDRF);
	WDTCR |= (1<<WDCE) | (1<<WDE);
	WDTCR = 0;
	
	sei();
}


ISR(WDT_vect)
{
	if(Callback != NULL) {
		Callback();	
	}
}