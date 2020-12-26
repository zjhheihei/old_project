#include ".\APP_Cfg.h"




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
    return retOutputFlag;
}


bool app_output_ltpLogic(bool lastLtpFlag)
{
    bool retOutputFlag = lastLtpFlag;
    SysPara_t *para;
        para = controler_getSysParaPt();
    int16_t diffTemp = 0;
    diffTemp = para->record.ltpSetTemp - para->runMeasTemp;
    
    if(diffTemp >= 0)
    {
        retOutputFlag = true;
    }
    else if(diffTemp <= -30)
    {
        retOutputFlag = false;
    }  
    return retOutputFlag;
}


void app_outputLogic_scanTask(void)
{
//    static uint16_t lastTick = 0;
//           uint16_t diffTick = 0;
//    diffTick = GetSysTickMillisecond() - lastTick;
//    if(500 <= diffTick)
//    {
//        lastTick = GetSysTickMillisecond();
        SysPara_t *para;
        para = controler_getSysParaPt();
        if(SYS_STATUS_POWER_OFF != para->record.sysRunStatus)
        {
            logic.openDiffTemp = para->record.diffTemp;
            logic.closeDiffTemp = para->record.diffTemp;
            logic.setTemp = para->runSetTemp;
        }
        else
        {
            if(MIN_LTP_TEMP < para->record.ltpSetTemp)
            {
                logic.openDiffTemp = 0;
                logic.closeDiffTemp = 30;
                logic.setTemp = para->record.ltpSetTemp;
            }
            
        }
        logic.measTemp = para->runMeasTemp;        
        logic.outputFlag = para->floorRelayFlag;
//    }
}





