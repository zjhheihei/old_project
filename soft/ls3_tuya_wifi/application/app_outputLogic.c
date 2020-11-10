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

typedef struct
{
    int16_t setTemp;
    int16_t measTemp;
    int16_t openDiffTemp;
    int16_t closeDiffTemp;
    bool    outputFlag;
}logic_t;

static logic_t logic;

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
    return retOutputFlag;
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
        if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
        {
            logic.setTemp = ptSysPara->record.ltpTemp;
            logic.openDiffTemp = 0;
            logic.closeDiffTemp = 30;
        }
        else
        {
            logic.openDiffTemp = ptSysPara->record.diffTemp;
            logic.closeDiffTemp = ptSysPara->record.diffTemp;
            logic.setTemp = ptSysPara->runSetTemp;
        }
        logic.measTemp = ptSysPara->runMeasTemp;
        
        logic.outputFlag = ptSysPara->floorRelayFlag;
    }
}






