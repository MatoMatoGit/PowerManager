#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void AdcInit(void);

void AdcDeinit(void);

/* Blocking read. */
uint16_t AdcChannelRead(uint8_t ch);

#endif