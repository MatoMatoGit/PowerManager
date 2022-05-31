/*
 * fw_info.h
 *
 * Created: 08-07-2020 22:04:00
 *  Author: dorspi
 */ 


#ifndef FW_INFO_H_
#define FW_INFO_H_

#include <stdint.h>

#define FW_VER_MAJOR	1
#define FW_VER_MINOR	0

void FwInfoInit(void);

void FwInfoPrint(void);

#endif /* FW_INFO_H_ */