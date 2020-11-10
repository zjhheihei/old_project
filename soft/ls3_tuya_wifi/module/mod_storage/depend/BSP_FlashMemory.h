//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_FLASHMEMORY_H
#define _BSP_FLASHMEMORY_H
//-----------------------------------------------------------------------------
#ifndef  __Snail_DataRetype__
#include ".\SnailDataTypedef.h"
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//Task
//------------------------------------------------------------------------------
//Event
//------------------------------------------------------------------------------
//Status
//------------------------------------------------------------------------------
//Function
//名称:程序空间块写操作
//功能:写入一个块的数据到程序空间
//入口:   _In_BlockNumber   块编号 从0x0000开始
//        _In_pData         需要存入Flash的数据指针
__ramfunc void ProgramFlash_WriteBlock(INT16U _In_BlockNumber,INT8U* _In_pData);
//------------------------------------------------------------------------------
void Prg_WriteFlashBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length);
void Prg_ReadFlashBytes(INT8U* pRead,INT32U Flashaddr,INT8U Length);
void Prg_WriteEepromBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length);

void JumpToApplication(void);
BOOLEAN CheckAppDataIntegrity(void);
void BSP_Restart_TheMcu(void);

#endif
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++


