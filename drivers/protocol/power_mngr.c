/*
 * power_mngr.c
 *
 * Created: 14-11-2020 14:23:19
 *  Author: dorspi
 */ 

#include "power_mngr.h"
#include "SoftwareSerial.h"

#define CMD_CODE_SLEEP 97
#define CMD_CODE_STATUS 100
#define CMD_CODE_VERSION 101

typedef struct Cmd_t * pCmd_t;

typedef void (*ProcessCmdFunc_t) (PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer);

typedef struct Cmd_t {
	PowerMngrCmd_e cmd;
	uint8_t length;
	ProcessCmdFunc_t func;
}Cmd_t;

static void ProcessCmdSleep(PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer);
static void ProcessCmdStatus(PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer);
static void ProcessCmdVersion(PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer);
static PowerMngrCmd_e CmdFromCode(uint8_t code);

const Cmd_t SleepCmd = {
	.cmd = CMD_CODE_SLEEP,
	.length = 5,
	.func = ProcessCmdSleep
};

const Cmd_t StatusCmd = {
	.cmd = CMD_CODE_STATUS,
	.length = 1,
	.func = ProcessCmdStatus
};

const Cmd_t VersionCmd = {
	.cmd = CMD_CODE_VERSION,
	.length = 1,
	.func = ProcessCmdVersion
};




static uint8_t RxBuffer[MAX_CMD_LENGTH];
static uint8_t RxIndex = 0;

static PowerMngrCallback_t CmdCallbacks[MAX_CMDS] = {};

static pCmd_t Cmds[MAX_CMDS] = {
	&SleepCmd,
	&StatusCmd
};

void PowerMngrInit(void)
{
	
}

void PowerMngrRegisterCallback(PowerMngrCmd_e cmd, PowerMngrCallback_t cb)
{
	if(cmd >= CMD_NUM) {
		return;
	}
	
	CmdCallbacks[cmd] = cb;
}

void PowerMngrRxHandler(uint8_t byte)
{
	static PowerMngrCmd_e cmd = CMD_INVALID;
	
	/* Received first byte, contains command code. */
	if(RxIndex == CMD_INDEX) {
		cmd = CmdFromCode(byte);
		softSerialPrintIntLn(cmd);
		if(cmd != CMD_INVALID) {
			RxIndex++;
		}
	} else {
		RxBuffer[RxIndex] = byte;
		RxIndex++;
	}
	
	softSerialPrintIntLn(RxIndex);
	
	
	if(cmd != CMD_INVALID) {
		if(RxIndex >= Cmds[cmd]->length) {
			softSerialPrintLn("Processing cmd");
			Cmds[cmd]->func(cmd, Cmds[cmd], RxBuffer);
			cmd = CMD_INVALID;
			RxIndex = 0;
		}
	}
}

static void ProcessCmdSleep(PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer)
{
	uint32_t time_sec = 0;
	
	time_sec |= buffer[ARG_START_INDEX];
	time_sec <<= 8;
	time_sec |= buffer[ARG_START_INDEX + 1];
	time_sec <<= 8;
	time_sec |= buffer[ARG_START_INDEX + 2];
	time_sec <<= 8;
	time_sec |= buffer[ARG_START_INDEX + 3];
	
	softSerialPrintLn("calc time:");
	softSerialPrintIntLn(time_sec);
	
	CmdCallbacks[cmd_enum](cmd_enum, (void *)&time_sec);
}

static void ProcessCmdStatus(PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer)
{
	uint8_t status = 0;
	CmdCallbacks[cmd_enum](cmd_enum, (void *)&status);
	softSerialPrintInt(status);
}

static void ProcessCmdVersion(PowerMngrCmd_e cmd_enum, pCmd_t cmd, uint8_t *buffer)
{
	uint8_t version[2] = {0};
	CmdCallbacks[cmd_enum](cmd_enum, (void *)&version);
	softSerialPrintInt(version[0]);
	softSerialPrintInt(version[1]);
}

static PowerMngrCmd_e CmdFromCode(uint8_t code)
{
	for(uint8_t i = 0; i < CMD_NUM; i++) {
		if(Cmds[i]->cmd == code) {
			return (PowerMngrCmd_e)i;
		}
	}
	
	return CMD_INVALID;
}