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

#define SELECT_PIN	PINB1
#define PULSE_PIN	PINB2
#define LDO_EN_PIN	PINB3


/* avrdude.exe -p t85 -B 25 -c usbasp -U flash:w:"C:\Users\dorspi\Documents\Atmel Studio\7.0\mvsensor_fw\Debug\mvsensor_fw.hex":i */
/* avrdude.exe -p t85 -B 25 -c usbasp -U hfuse:w:0xDF:m */
/* avrdude.exe -p t85 -B 25 -c usbasp -U lfuse:w:0xE2:m */


static ExtInt_t PulseInt = NULL;
static volatile uint32_t PulseCount = 0;


static void PulseInterruptHandler(bool state);

int main(void)
{

	cli();

	
	ExtIntInit();
	PulseInt = ExtIntRegister(PULSE_PIN, &PORTB, EXT_INT_PIN_CHANGE, PulseInterruptHandler);

	ExtIntEnable(PulseInt);
	
    while (1)
    {	

    }
}

static void PulseInterruptHandler(bool state)
{
	PulseCount++;
	PORTB ^= (1 << LDO_EN_PIN);	
}
