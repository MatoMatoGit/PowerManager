/*
 * oc_timer.h
 *
 * Created: 16-03-2019 22:18:18
 *  Author: dorspi
 */ 


#ifndef OC_TIMER_H_
#define OC_TIMER_H_

#include <stdint.h>

#define OC_TIMER_NUM_CALLBACKS 1

typedef void (*OcTimerCb_t) (void);

void OcTimerInit(uint8_t oc);

void OcTimerStart(void);

void OcTimerStop(void);

uint8_t OcTimerGet(void);

void OcTimerSet(uint8_t val);

void OcTimerReset(void);

void OcTimerCallbackRegister(OcTimerCb_t cb);


#endif /* OC_TIMER_H_ */