/*
 * oc_timer.c
 *
 * Created: 16-03-2019 22:20:14
 *  Author: dorspi
 */ 

#include "oc_timer.h"

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

OcTimerCb_t Callbacks[OC_TIMER_NUM_CALLBACKS] = {NULL};

void OcTimerInit(uint8_t oc)
{
	cli();
	
	TCCR0A = 0;
	TCCR0B = 0;
	
	OCR0A  = oc;      // number to count up to (0x70 = 112)
	TCCR0A = (1 << WGM01);      // Clear Timer on Compare Match (CTC) mode
	TCCR0B = 0;      // stop timer
	TCNT0 = 0;
		
	sei();
}

void OcTimerStart(void)
{
	cli();
	TIMSK |= (1 << OCIE0A);
	TCCR0B |= (1 << CS00) | (1 << CS02); // clock source CLK/1024, start timer
	sei();
}

void OcTimerStop(void)
{
	cli();
	TCCR0B = 0;      // stop timer
	TIMSK &= ~(1 << OCIE0A);
	sei();
}

uint8_t OcTimerGet(void)
{
	return TCNT0;
}

void OcTimerSet(uint8_t val)
{
	TCNT0 = val;
}

void OcTimerReset(void)
{
	TCNT0 = 0;
}

void OcTimerCallbackRegister(void (*oc_timer_cb)(void))
{
	for(uint8_t i = 0; i < OC_TIMER_NUM_CALLBACKS; i++) {
		if(Callbacks[i] == NULL) {
			Callbacks[i] = oc_timer_cb;
			break;
		}
	}
}

ISR(TIMER0_COMPA_vect) 
{
	for(uint8_t i = 0; i < OC_TIMER_NUM_CALLBACKS; i++) {
		if(Callbacks[i] != NULL) {
			Callbacks[i]();
		}
	}
}