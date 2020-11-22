/*
 * gpio_adc.c
 *
 * Created: 16-03-2019 22:34:52
 *  Author: dorspi
 */

#include "gpio_adc.h"

#include <avr/io.h>

#define ADC_DDR DDRB
#define ADC_PIN_CH_2 PINB4

void GpioAdcInit(void)
{
	ADC_DDR &= ~(1 << ADC_PIN_CH_2);
}