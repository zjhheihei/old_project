#include ".\APP_Cfg.h"

typedef enum
{
    RELAY_TASK_STATUS_INIT = 0x00,
    RELAY_TASK_STATUS_DELAY,
    RELAY_TASK_STATUS_RUN,
}relayTaskStatus_t;

static relayTaskStatus_t relayTaskStatus  = RELAY_TASK_STATUS_INIT;
static bool              outsideLimitFlag = false;


#define LOW_TEMP_PROTECT_DIF   30
#define LIF_DIFF               30 

bool relay_out_limitFlag(void)
{
    return outsideLimitFlag;
}


void limitCheck(void)
{
    SysPara_t *ptSys;
    ptSys = controler_getSysParaPt();
    if(ptSys->floorRelayFlag)
    {        
        if(SENSOR_ALL == ptSys->record.sensorMode)
        {
            if(NONE_ERROR != ptSys->systemError)
            {               
                outsideLimitFlag = false;
            }
            else
            {
                if(ptSys->runLifTemp >= ptSys->record.lifTemp)
                {
                    outsideLimitFlag = true;  
                    
                }
                else if(ptSys->runLifTemp <= ptSys->record.lifTemp - LIF_DIFF)
                {
                    outsideLimitFlag = false;
                } 
            }
        }
        else
        {
            outsideLimitFlag = false;           
        }
    }
    else
    {
        outsideLimitFlag = false;
    }      
    
    if(outsideLimitFlag)
    {
        ptSys->floorRelayFlag = false;
    }
}

bool app_relayOutput_enable(void)
{//允许继电器输出
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt();  
    if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
    {
        return false;
    }
    else if(SYS_STATUS_HIGH_SETTING == ptSysPara->record.sysRunStatus)
    {
        return false;
    }
    else if(SYS_STATUS_TIMING_SET == ptSysPara->record.sysRunStatus)
    {
        if(TIMMING_SET_DDR_ON == ptSysPara->timming.setDdr)
        {
            return false;
        }      
    }
    else if(ptSysPara->systemError)
    {
        return false;
    }
    return true;
}

#define TEN_DAY 14400
#define THREE_MINUTE 3

void app_relayAction(bool actionFlag)
{
    if(actionFlag)
    {
         mod_relay_on(RELAY_FLOOR,RELAY_DELAY_TIME); 
         mod_relay_on(RELAY_ASSIST,600);
    }
    else
    {
         mod_relay_off(RELAY_FLOOR,0);  
         mod_relay_off(RELAY_ASSIST,0);  
    }
}

void app_output_timeControlCheck(void)
{
    static uint16_t timeConOneMinuteCyc = 0;
    SysPara_t *ptSys;
    ptSys = controler_getSysParaPt(); 
    bool resetFlag = false;
    if(ptSys->record.outputTimeControFlag)
    {      
        if(ptSys->floorRelayFlag)
        {
            if(TIME_CON_STATUS_IDLE == ptSys->timeContorl.status)
            {
                ptSys->timeContorl.runMinute = 0;
                ptSys->timeContorl.status = TIME_CON_STATUS_RUN;
                timeConOneMinuteCyc = GetSysTickMillisecond();
                app_control_timeOut_set(DELAY_TIME_CONTROL_TIME,ms_5000); 
                ptSys->timeContorl.disTimeControlFlag = true;
            }
            else
            {               
                uint16_t passTick = 0;
                passTick = GetSysTickMillisecond() - timeConOneMinuteCyc;\
                if(ms_60000 <= passTick)
                {
                    timeConOneMinuteCyc = GetSysTickMillisecond();
                    ptSys->timeContorl.runMinute++; 
                    uint8_t *ptLastTime = &ptSys->record.outputOpenLastTime;
                    if(TIME_CON_STATUS_REST == ptSys->timeContorl.status)
                    {
                        ptLastTime++;
                    }
                    if(ptSys->timeContorl.runMinute >= *ptLastTime)
                    {
                        ptSys->timeContorl.runMinute = 0;
                        if(TIME_CON_STATUS_REST == ptSys->timeContorl.status)
                        {
                            ptSys->timeContorl.status = TIME_CON_STATUS_RUN;
                        }
                        else
                        {
                            ptSys->timeContorl.status = TIME_CON_STATUS_REST;                            
                        }
                    }
                }
            }
        }
        else
        {
            resetFlag = true;
        }
    }
    else
    {
        resetFlag = true;
    }
    
    if(resetFlag)
    {
        ptSys->timeContorl.runMinute = 0;
        ptSys->timeContorl.status = TIME_CON_STATUS_IDLE;
        timeConOneMinuteCyc = GetSysTickMillisecond();
        ptSys->timeContorl.disTimeControlFlag = false;
    }
    
    if(TIME_CON_STATUS_REST == ptSys->timeContorl.status)
    {
        ptSys->floorRelayFlag = false;
    }
}

void app_relayOut_scanTask(void)
{    
    static uint16_t initDelayTime = 0;
    mod_relay_scanTask(GetSysTickMillisecond());
    if(RELAY_TASK_STATUS_INIT == relayTaskStatus)
    {
        initDelayTime = GetSysTickMillisecond();
        relayTaskStatus = RELAY_TASK_STATUS_DELAY;
    }
    else if(RELAY_TASK_STATUS_DELAY == relayTaskStatus)
    {
        uint16_t passTick = 0;
        passTick = GetSysTickMillisecond() - initDelayTime;
        if(ms_3000 < passTick)
        {
            relayTaskStatus = RELAY_TASK_STATUS_RUN;
        }
    }
    else
    {
        SysPara_t *ptSysPara;
        ptSysPara = controler_getSysParaPt();              
        if(app_relayOutput_enable()) 
        {      
            ptSysPara->floorRelayFlag = app_output_floorLogic();   
            limitCheck();
        }
        else
        {
            ptSysPara->floorRelayFlag = false;
        }                     
        app_output_timeControlCheck();       
        app_relayAction(ptSysPara->floorRelayFlag);   
    } 
}
//-----------------------APP.ThreeFanOut.C--END---------------------------------


