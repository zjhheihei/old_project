/*******************************************************************************
*
*	模块名称 : APP背光模块
*	文件名称 : APP.Backlight.c
*	版    本 : V1.0
*	说    明 : 1、传出压入背光数据
*                  2、背光扫描
*                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-11-27  Linzl  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/
#include ".\APP_Cfg.h"
/******************************************************************************/
#ifdef NDEBUG
#define LS3_BACKLIGHT_ON_16SECOND   16
#else
#define LS3_BACKLIGHT_ON_16SECOND   0xffff
#endif
void app_backLigth_on(void)
{    
    mod_backLight_on(LS3_BACKLIGHT_ON_16SECOND);             
}


void app_backlight_scanTask(void)
{
    mod_backLight_task(GetSysTickMillisecond());
}
//------------------------APP.Backlight.C--END----------------------------------


