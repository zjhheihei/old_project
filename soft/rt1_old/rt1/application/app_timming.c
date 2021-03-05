/*******************************************************************************
*
*	模块名称 : 输出控制
*	文件名称 : app_timming.c
*	版    本 : V1.0
*	说    明 : 
*                  
*                 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-09-21  zhoujh&Linzl  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\APP_Cfg.h"

#define HALF_A_HOUR   1800
#define TIMMING_FINISH   0
static uint8_t oneMinuteCnt = 0;
static bool    halfHourFinishFlag = false;

void app_timing_halfHour_handle(void)
{
    static uint16_t oneMinuteCyc = 0;
           uint16_t passTick = 0;
    passTick = GetSysTickMillisecond() - oneMinuteCyc;
    if(ms_60000 <= passTick)
    {
        oneMinuteCyc = GetSysTickMillisecond();
        if(++oneMinuteCnt > 29)
        {
            oneMinuteCnt = 0;
            halfHourFinishFlag = true;                    
        }
    }
}

void app_timming_scanTask(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    if(TIMMING_STATUS_RUN == ptSys->timming.status)
    {
        app_timing_halfHour_handle();
        if(0 < ptSys->timming.halfHour)
        {
            if(halfHourFinishFlag)
            {
                halfHourFinishFlag = false;
                ptSys->timming.halfHour--;
            }
        }
        if(TIMMING_FINISH == ptSys->timming.halfHour)
        {
            ptSys->timming.status = TIMMING_STATUS_IDLE;
            
            ptSys->record.sysRunStatus = (systemRunStatus_t)ptSys->timming.setDdr;            
        }       
    }
    else
    {
        oneMinuteCnt = 0;
    }
}




