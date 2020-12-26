//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_CRC8_H
#define _BSP_CRC8_H
//-----------------------------------------------------------------------------
#include "stdint.h"
#include <stdbool.h>


uint8_t bsp_makeCrc8Byte(uint8_t writeCnt,uint8_t len,uint8_t *ptDat);



bool bsp_checkCrc8Byte(uint8_t len,uint8_t *ptDat);
#endif
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++


