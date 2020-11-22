/*
 * soft_timer.h
 *
 * Created: 24-03-2019 14:17:41
 *  Author: dorspi
 */ 


#ifndef SOFT_TIMER_H_
#define SOFT_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#define SOFT_TIMER_CONFIG_NUM_TIMERS 2

typedef struct SoftTimerInstance * SoftTimer_t;

typedef void (*SoftTimerCb_t)(SoftTimer_t timer);

void SoftTimerInit(void);

void SoftTimerDeinit(void);

SoftTimer_t SoftTimerCreate(uint32_t period_ms, SoftTimerCb_t callback);

void SoftTimerStart(SoftTimer_t timer);

void SoftTimerStop(SoftTimer_t timer);

void SoftTimerReset(SoftTimer_t timer);

void SoftTimerPeriodSet(SoftTimer_t timer, uint32_t period_ms);

uint32_t SoftTimerTicksGet(SoftTimer_t timer);

bool SoftTimerIsEnabled(SoftTimer_t timer);

uint32_t SoftTimerTotalTimeGet(void);

#endif /* SOFT_TIMER_H_ */