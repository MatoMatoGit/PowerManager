/*
 * sleep.h
 *
 * Created: 23-03-2019 17:45:27
 *  Author: dorspi
 */ 


#ifndef SLEEP_H_
#define SLEEP_H_

#include <stdint.h>

typedef void (*SleepWakeupCallback_t) (void);

void SleepWakeupCallbackSet(SleepWakeupCallback_t wakeup_cb);

void SleepForDuration(uint32_t duration_s);

#endif /* SLEEP_H_ */