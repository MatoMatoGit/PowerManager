/*
 * wdt.h
 *
 * Created: 23-03-2019 17:07:02
 *  Author: dorspi
 */ 


#ifndef WDT_H_
#define WDT_H_

typedef enum {
	WDT_CYCLES_2K = 0,
	WDT_CYCLES_4K,
	WDT_CYCLES_8K,
	WDT_CYCLES_16K,
	WDT_CYCLES_32K,
	WDT_CYCLES_64K,
	WDT_CYCLES_128K,
	WDT_CYCLES_256K,
	WDT_CYCLES_512K,
	WDT_CYCLES_1024K,
	WDT_CYCLES_NUM
}WdtCycles_t;

typedef void (*WdtCallback_t)(void);

void WdtEnable(WdtCycles_t cycles, WdtCallback_t cb);

void WdtDisable(void);

#endif /* WDT_H_ */