/**
 * This library provides an abstraction of (raw) non-volatile memory (nvm) such
 * as EEPROM or flash.
 * The nvm space can be divided in partitions, where each partition contains a fixed
 * type of data. Each entry in a partition of that fixed size is known as an element.
 * Elements can be appended or removed using the Fifo API, or elements can be
 * written / read manually using the Raw API.
 * 
 * The Raw API takes an offset (relative address) argument, which is translated by 
 * the library to a absolute address.
 */
#include <stdint.h>

#define NV_PART_MAX_PARTS 5

typedef uint8_t NvPartId_t;

typedef uint16_t NvSize_t;

typedef enum {
	NV_PART_RES_OK = 0,
	NV_PART_RES_ERR,
	NV_PART_RES_NO_SPACE,
	NV_PART_RES_INV_ARG,
	NV_PART_RES_INV_ID,
}NvPartResult_e;

typedef enum {
	NV_PART_TYPE_RAW = 0,
	NV_PART_TYPE_FIFO,
}NvPartType_e;

typedef struct {
	/* Unique identifer for this partition. */
	NvPartId_t id;
	
	/* Total amount of bytes reserved for this partition. */
	NvSize_t part_size;
	
	/* Maximum amount of elements allowed in memory. 
	 * Used by FIFO API. */
	NvSize_t elem_cap;
	
	/* Size of each element to store. */
	NvSize_t elem_size;
	
	NvPartType_e type;
}NvPartCfg_t;

NvPartCfg_t nv_parts[NV_PART_MAX] = {
	{
	},
	/**
	 * Define parts here.
	 */
};

typedef uint8_t NvData_t;

typedef struct {
	NvPartCfg_t *cfg;
	NvData_t *mem;
	NvSize_t w_offset;
	NvSize_t r_offset;
}NvPart_t;

NvPart_t NvParts [NV_PART_MAX];



NvPartResult_e NvPartInit(const NvPartCfg_t *parts);


NvPartResult_e NvPartRawWrite(NvPartId_t id, void *elem, NvSize_t offset);

NvPartResult_e NvPartRawRead(NvPartId_t id, void *elem, NvSize_t offset);


NvPartResult_e NvPartFifoWrite(NvPartId_t id, void *elem);

NvPartResult_e NvPartFifoRead(NvPartId_t id, void *elem);


NvPartResult_e NvPartErase(NvPartId_t id);
