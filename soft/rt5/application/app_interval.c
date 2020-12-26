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

static uint8_t runInter;


uint8_t app_inter_runInter(void)
{    
    if(5 <= runInter)
    {
        runInter = 5; 
    }
    return runInter;
}


#define WORK_DAY_CHECK_LEN  4
#define REST_DAY_CHECK_LEN  2
uint8_t app_inter_interCheck(realTime_t *rtc,interval_t *inter,uint8_t checkLen)
{
    uint8_t i = 0;
    uint8_t retInter = 0xff;
    for(;i < checkLen;i++)
    {        
        if(rtc->hm < inter[i].hourMinute)
        {           
            break;
        }
    }
    if(0 == i)
    {
        retInter = (checkLen - 1);
    }
    else
    {
        retInter = i - 1;
    }
    return retInter;
}

bool app_inter_restCheck(realTime_t *rtc,weekProgramCfg_t weekCfg)
{
    bool restFlag = false;
    switch(weekCfg)
    {
        case PROGRAM_6:
        case PROGRAM_5:
        {
            if((WEEK_SUNDAY == rtc->week))
            {
                restFlag = true;
            }
            else if((WEEK_SATURDAY == rtc->week))
            {
                if(PROGRAM_5 == weekCfg)
                {
                    restFlag = true;
                }
            }
            break;
        }            
        default:break;
    }
    return restFlag;
}

void app_inter_runIntercheck(realTime_t *rtc,interval_t *inter,weekProgramCfg_t weekCfg)
{   
    if(app_inter_restCheck(rtc,weekCfg))
    {
      runInter = (app_inter_interCheck(rtc,&inter[4],REST_DAY_CHECK_LEN) + 4);           
    }
    else
    {
      runInter = app_inter_interCheck(rtc,inter,WORK_DAY_CHECK_LEN);
    }                   
}

void app_inter_changeCheck(tempControlMode_t *_out_Mode,systemRunStatus_t status)
{
    static uint8_t stoInter = 0xff;
    static systemRunStatus_t stoLastStatus = SYS_STATUS_POWER_OFF;
    if(stoInter != runInter)
    {
        stoInter = runInter;
        if(TEMP_CONTROL_MODE_TEMPORARY == *_out_Mode)
        {
            *_out_Mode = TEMP_CONTROL_MODE_TIME;
        }
    }
    if(stoLastStatus != status)
    {
        stoLastStatus = status;
        *_out_Mode = TEMP_CONTROL_MODE_MANUAL;
    }
    
}

void app_interval_task(void)
{
    static uint16_t lastStoSystick = 0;
           uint16_t diffTick = 0;
    diffTick = GetSysTickMillisecond() -lastStoSystick;
    if(1000 <= diffTick)
    {
        SysPara_t *ptSys;  
        ptSys = controler_getSysParaPt();
        app_inter_runIntercheck(app_real_time_getTime(),ptSys->record.interval,ptSys->record.weekPrg);
        app_inter_changeCheck(&ptSys->record.tempControlMode,ptSys->record.sysRunStatus);
    }
}




   
//-----------------------APP.ThreeFanOut.C--END---------------------------------


