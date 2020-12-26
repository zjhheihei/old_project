//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_WDT_H
#define _BSP_WDT_H
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
//名称: 看门狗初始化
//功能: 初始化调用，一般第一个调用
//入口: 无
//
//出口: 无

void Watchdog_Configure(void);
//------------------------------------------------------------------------------
//名称: 看门狗定时器清零
//功能: 
//入口: 无
//
//出口: 无
void ClearWatchDog(void);
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++