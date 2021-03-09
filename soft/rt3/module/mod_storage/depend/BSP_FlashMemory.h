//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_FLASHMEMORY_H
#define _BSP_FLASHMEMORY_H
//-----------------------------------------------------------------------------
#ifndef  __Snail_DataRetype__
#include ".\SnailDataTypedef.h"
#endif
#include "DeviceSelect.h"



void Prg_WriteFlashBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length);
void Prg_ReadFlashBytes(INT8U* pRead,INT32U Flashaddr,INT8U Length);
void Prg_WriteEepromBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length);
void bsp_flash_write_oneBlock(INT16U blockNum,INT8U *ptDat);

#define FLASH_CONFIG_FREQ_4MHZ      0x01   // 华大有效                         
#define FLASH_CONFIG_FREQ_22_12MHZ  0x02 
BOOLEAN bsp_sto_configure(INT32U u32CfgFreq);







#if defined(__STM8L052R8__)
#define RUN_MCU_TYPE                             0X00000000
#define EEPROM_BLOCK_ONE_START_ADDR              0X00001000
#define EPPROM_ONE_BLOCK_SIZE                    128

#define DOWN_LOAD_AREA_FRIST_BLOCK_NUM           264
#define DOWN_LOAD_AREA_LAST_BLOCK_NUM            511
#define DOWN_LOAD_AREA_INFORMATION_BLOCK_ADDR    0X00017F80
#define DOWN_LOAD_AREA_INFORMATION_STATUS_ADDR   0X00017FF4
#define DOWN_LOAD_AREA_INFORMATION_CHECK_ADDR    0X00017FF8
#define DOWN_LOAD_AREA_INFORMATION_SIZE_ADDR     0X00017FFC
#define DOWN_LOAD_AREA_SIZE                      0X00007C00

#elif defined(__HDSC32L136__)
#define RUN_MCU_TYPE                             0X00000001
#define EEPROM_BLOCK_ONE_START_ADDR              0XFC00
#define EPPROM_ONE_BLOCK_SIZE                    512
#define DOWN_LOAD_AREA_FRIST_BLOCK_NUM           264
#define DOWN_LOAD_AREA_LAST_BLOCK_NUM            503
#define DOWN_LOAD_AREA_INFORMATION_BLOCK_ADDR    0XFB80
#define DOWN_LOAD_AREA_INFORMATION_STATUS_ADDR   0XFBF4
#define DOWN_LOAD_AREA_INFORMATION_CHECK_ADDR    0XFBF8
#define DOWN_LOAD_AREA_INFORMATION_SIZE_ADDR     0XFBFC
#define DOWN_LOAD_AREA_SIZE                      0X7800

#endif


#endif
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++


