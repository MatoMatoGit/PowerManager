/*
 * fw_info.c
 *
 * Created: 08-07-2020 22:06:32
 *  Author: dorspi
 */ 

#include "fw_info.h"

//#include "SoftwareSerial.h"

#include <avr/eeprom.h>
#include <string.h>

uint16_t EEMEM FwVerMajor;
uint16_t EEMEM FwVerMinor;

uint16_t FwVerMajorCache = 0xFFFF;
uint16_t FwVerMinorCache = 0xFFFF;

void FwInfoInit(void)
{
	FwVerMajorCache = eeprom_read_word(&FwVerMajor);
	FwVerMinorCache = eeprom_read_word(&FwVerMinor);
	
	if(FwVerMajorCache == 0xFFFF) {
		FwVerMajorCache = FW_VER_MAJOR;
		FwVerMinorCache = FW_VER_MINOR;
		
		eeprom_update_word(&FwVerMajor, FwVerMajorCache);
		eeprom_update_word(&FwVerMinor, FwVerMinorCache);
		FwVerMajorCache = eeprom_read_word(&FwVerMajor);
		FwVerMinorCache = eeprom_read_word(&FwVerMinor);
	}
}

void FwInfoPrint(void)
{
	//softSerialPrint("FW ver: V");
	//softSerialPrintInt(FwVerMajorCache);
	//softSerialPrint(".");
	//softSerialPrintInt(FwVerMinorCache);
	//softSerialPrintLn("");
}