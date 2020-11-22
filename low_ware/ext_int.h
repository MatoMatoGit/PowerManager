#ifndef EXT_INT_H_
#define EXT_INT_H_

#include <stdint.h>
#include <stdbool.h>

#define NUM_EXT_INTS 3

typedef enum {
    EXT_INT_PIN_CHANGE = 0,
    EXT_INT_PIN_LOW,
    EXT_INT_PIN_FALLING,
    EXT_INT_PIN_RISING
}ExtIntType_t;

typedef struct ExtIntDesc_t * ExtInt_t;

typedef void (*ExtIntCallback_t) (bool state);


void ExtIntInit(void);

void ExtIntDeinit(void);


ExtInt_t ExtIntRegister(uint8_t pin, volatile uint8_t *port, ExtIntType_t type, ExtIntCallback_t cb);

void ExtIntUnregister(ExtInt_t ext_int);


ExtIntType_t ExtIntType(ExtInt_t ext_int);

int ExtIntEnable(ExtInt_t ext_int);

void ExtIntDisable(void);

#endif
