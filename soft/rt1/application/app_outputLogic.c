/*******************************************************************************
*
*	模块名称 : 输出控制
*	文件名称 : OutputControl.c
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
//------------------------------E N D-------------------------------------------
#define __FUNCTION_KEEP_TEMP__
static bool tempKeepStatusFlag = false;

typedef struct
{
    int16_t setTemp;
    int16_t measTemp;
    int16_t openDiffTemp;
    int16_t closeDiffTemp;
    bool    outputFlag;
}logic_t;

static logic_t logic;


bool app_logic_runAtMakeWarmMode(void)
{
    return true;
}




bool app_output_floorLogic(void)
{
    bool retOutputFlag = logic.outputFlag;
    int16_t diffTemp = 0;
    diffTemp = logic.setTemp - logic.measTemp;
    
    if(diffTemp >= logic.openDiffTemp)
    {
        retOutputFlag = true;
    }
    else if(diffTemp <= -logic.closeDiffTemp)
    {
        retOutputFlag = false;
    }
    
//    /***************************加入恒温标志判定****************************/
//    #ifdef __FUNCTION_KEEP_TEMP__
//    if(retOutputFlag)
//    {
//        tempKeepStatusFlag = false;
//    }
//    else
//    {
//        if((diffTemp <= logic.openDiffTemp) && (diffTemp >= -logic.closeDiffTemp))
//        {
//            tempKeepStatusFlag = true;
//        }
//        else
//        {
//            tempKeepStatusFlag = false;
//        }
//    }
//    #endif
//    /**************************加入恒温标志判定****************************/
    
    return retOutputFlag;
}

bool app_output_getTempKeepStatus(void)
{
    if(tempKeepStatusFlag)
    {
        return true;
    }
    return false;
}

void app_outputLogic_scanTask(void)
{
    static uint16_t lastTick = 0;
           uint16_t diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastTick;
    if(500 <= diffTick)
    {
        lastTick = GetSysTickMillisecond();
        SysPara_t *ptSysPara;
        ptSysPara = controler_getSysParaPt();
//        if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
//        {
//            logic.setTemp = ptSysPara->record.ltpTemp;
//            logic.openDiffTemp = 0;
//            logic.closeDiffTemp = 30;
//        }
//        else
        if(SYS_STATUS_POWER_OFF != ptSysPara->record.sysRunStatus)
        {
            logic.openDiffTemp = ptSysPara->record.diffTemp;
            logic.closeDiffTemp = ptSysPara->record.diffTemp;
            logic.setTemp = ptSysPara->runSetTemp;
        }
        else
        {
            if(MIN_LTP_TEMP < ptSysPara->record.ltpSetTemp)
            {
                logic.openDiffTemp = 0;
                logic.closeDiffTemp = 30;
                logic.setTemp = ptSysPara->record.ltpSetTemp;
            }
            
        }
        logic.measTemp = ptSysPara->runMeasTemp;        
        logic.outputFlag = ptSysPara->floorRelayFlag;
    }
}





