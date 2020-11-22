#include "adc.h"

#include <avr/io.h>

void AdcInit(void)
{
    // AREF = AVcc
	ADMUX = 0;
    
    // ADC Enable and prescaler of 128
    // 1000000/8 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);	
}

void AdcDeinit(void)
{
	ADCSRA = 0;
}

uint16_t AdcChannelRead(uint8_t ch)
{
  // select the corresponding channel 0~7
  // ANDing with ’7? will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0b00000111;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
  
  // start single conversion
  // write ’1? to ADSC
  ADCSRA |= (1<<ADSC);
  
  // wait for conversion to complete
  // ADSC becomes ’0? again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
  
  return (ADC);
}
