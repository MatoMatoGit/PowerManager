/*
 * power_mngr.h
 *
 * Created: 14-11-2020 14:23:04
 *  Author: dorspi
 */ 


#ifndef POWER_MNGR_H_
#define POWER_MNGR_H_

#include <stdint.h>

#define MAX_CMDS 2
#define CMD_INDEX 0
#define ARG_START_INDEX 1

typedef enum {
	CMD_SLEEP = 0,
	CMD_STATUS,
	CMD_NUM,
	CMD_INVALID,
}PowerMngrCmd_e;

#define MAX_CMD_LENGTH 10

typedef void (*PowerMngrCallback_t) (PowerMngrCmd_e cmd, void *args);

void PowerMngrInit(void);

void PowerMngrRegisterCallback(PowerMngrCmd_e cmd, PowerMngrCallback_t cb);

void PowerMngrRxHandler(uint8_t byte);


#endif /* POWER_MNGR_H_ */