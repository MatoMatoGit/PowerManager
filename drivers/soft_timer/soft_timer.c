/*
 * soft_timer.c
 *
 * Created: 17-03-2019 16:21:38
 *  Author: dorspi
 */

#include "soft_timer.h"

#include "oc_timer.h"
#include <avr/interrupt.h>
#include  <avr/io.h>

#include <string.h>

#define SOFT_TIMER_UPDATE_INTERVAL_MS 25

struct SoftTimerInstance {
	volatile bool en;
	volatile uint32_t t_start_ms;
	volatile uint32_t t_period_ms;
	SoftTimerCb_t cb;
};

static void OcTimerCallback(void);

static struct SoftTimerInstance SoftTimers[SOFT_TIMER_CONFIG_NUM_TIMERS];

static volatile uint32_t TotalTime = 0;

void SoftTimerInit(void)
{
	memset(SoftTimers, 0, sizeof(SoftTimers));
	
	OcTimerInit(195);
	OcTimerCallbackRegister(OcTimerCallback);
	OcTimerStart();
}

void SoftTimerDeinit(void)
{
	OcTimerStop();
}

SoftTimer_t SoftTimerCreate(uint32_t period_ms, SoftTimerCb_t callback)
{
	SoftTimer_t timer = NULL;
	
	for(uint8_t i = 0; i < SOFT_TIMER_CONFIG_NUM_TIMERS; i++) {
		if(SoftTimers[i].t_period_ms == 0) {
			timer = &SoftTimers[i];
			break;
		}
	}
	
	timer->t_start_ms = 0;
	timer->t_period_ms = period_ms;
	timer->cb = callback;
	
	return timer;
}

void SoftTimerStart(SoftTimer_t timer)
{
	if(timer != NULL) {
		timer->en = true;
		timer->t_start_ms = TotalTime;
	}
}

void SoftTimerStop(SoftTimer_t timer)
{
	if(timer != NULL) {
		timer->en = false;
		timer->t_start_ms = 0;
	}	
}

void SoftTimerReset(SoftTimer_t timer)
{
	if(timer != NULL) {
		timer->t_start_ms = TotalTime;
	}	
}

void SoftTimerPeriodSet(SoftTimer_t timer, uint32_t period_ms)
{
	if(timer != NULL) {
		timer->t_period_ms = period_ms;
	}
}

uint32_t SoftTimerTicksGet(SoftTimer_t timer)
{
	uint32_t ticks = 0;
	
	if(timer != NULL) {
		ticks = TotalTime - timer->t_start_ms;
	}
	
	return ticks;
}

bool SoftTimerIsEnabled(SoftTimer_t timer)
{
	if(timer != NULL) {
		return timer->en;
	}
	
	return false;
}

uint32_t SoftTimerTotalTimeGet(void)
{
	return TotalTime;
}

static void OcTimerCallback(void)
{
	TotalTime += SOFT_TIMER_UPDATE_INTERVAL_MS;

	for(uint8_t i = 0; i < SOFT_TIMER_CONFIG_NUM_TIMERS; i++) {
		if(SoftTimers[i].en == true && SoftTimers[i].t_period_ms != 0) {
			if(TotalTime >= SoftTimers[i].t_start_ms + SoftTimers[i].t_period_ms) {
				SoftTimers[i].cb(&SoftTimers[i]);
				SoftTimers[i].t_start_ms = TotalTime;
			}
		}
	}
}