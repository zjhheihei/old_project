/*******************************************************************************
*
*	模块名称 : 时段获取
*	文件名称 : APP_TimeInterval.c
*	版    本 : V1.0
*	说    明 : 
*                  
*                 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-11-01  zhoujh&Linzl  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/



#include ".\APP_Cfg.h"

static uint8_t currentInterval;


uint8_t app_interval_getInterval(void)
{    
    if(7 <= currentInterval)
    {
        currentInterval = 7; 
    }
    return currentInterval;
}



void app_interval_check(void)
{
    static uint8_t lastInterval = 0xff;
    static systemRunStatus_t stoRunStatus = SYS_STATUS_POWER_OFF;
    uint8_t i           = 0;
    uint8_t PrgWork     = 0;
    uint8_t PrgRest     = 0; 
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt();
    
    for(;i < 6;i++)
    {
        if(ptSysPara->rtc.hourMinute < ptSysPara->record.interval[i].hourMinute)
        {           
            break;
        }
    }
    if(0 == i)
    {
        PrgWork=5;
    }
    else
    {
        PrgWork=i-1;
    }
    
    
    
    if((ptSysPara->rtc.hourMinute >= ptSysPara->record.interval[6].hourMinute) &&\
      (ptSysPara->rtc.hourMinute < ptSysPara->record.interval[7].hourMinute)) 
    {
        PrgRest = 6;
    }
    else
    {
        PrgRest = 7;
    }
    
    switch(ptSysPara->record.weekPrg)
    {
        case PROGRAM_7:
        {
            currentInterval = PrgWork;
            break;
        }
        case PROGRAM_6:
        {
            if(ptSysPara->rtc.week)
            {
                currentInterval = PrgWork;
            }
            else
            {
                currentInterval = PrgRest;
            }
            break;
        }
        case PROGRAM_5:
        {
            if((0 < ptSysPara->rtc.week) && (6 > ptSysPara->rtc.week))
            {
                currentInterval = PrgWork;
            }
            else
            {
                currentInterval = PrgRest;
            }
        }
    }
    
    if(lastInterval != currentInterval)
    {
        if(TEMP_CONTROL_MODE_TEMPORARY == ptSysPara->record.tempControlMode)
        {
            ptSysPara->record.tempControlMode = TEMP_CONTROL_MODE_TIME;
        }
        lastInterval = currentInterval;          
    }
    
    if(stoRunStatus != ptSysPara->record.sysRunStatus)
    {
        stoRunStatus = ptSysPara->record.sysRunStatus;
        if(TEMP_CONTROL_MODE_TEMPORARY == ptSysPara->record.tempControlMode)
        {
            ptSysPara->record.tempControlMode = TEMP_CONTROL_MODE_TIME;
        }
    }
}

void app_interval_scanTask(void)
{
    static uint16_t lastStoSystick = 0;
           uint16_t diffTick = 0;
    diffTick = GetSysTickMillisecond() -lastStoSystick;
    if(1000 <= diffTick)
    {
        lastStoSystick = GetSysTickMillisecond();
        app_interval_check();
    }
}

   
//-----------------------APP.ThreeFanOut.C--END---------------------------------


