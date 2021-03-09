#include ".\APP_Cfg.h"

typedef enum
{
    RELAY_TASK_STATUS_INIT = 0x00,
    RELAY_TASK_STATUS_DELAY,
    RELAY_TASK_STATUS_RUN,
}relayTaskStatus_t;

static relayTaskStatus_t relayTaskStatus  = RELAY_TASK_STATUS_INIT;
static bool              outsideLimitFlag = false;
static bool              ltpFlag          = false;
//static bool              timeOutControlEnableFlag = false;



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
                if(ptSys->runLifTemp >= ptSys->record.lifSetTemp)
                {
                    outsideLimitFlag = true;  
                    
                }
                else if(ptSys->runLifTemp <= ptSys->record.lifSetTemp - LIF_DIFF)
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
        //timeOutControlEnableFlag = false;
        return false;
    }
    else if(SYS_STATUS_HIGH_SETTING == ptSysPara->record.sysRunStatus)
    {
        //timeOutControlEnableFlag = false;
        return false;
    }   
    else if(ptSysPara->systemError)
    {
        //timeOutControlEnableFlag = false;
        return false;
    }
    //timeOutControlEnableFlag = true;
    return true;
}

#define TEN_DAY 14400
#define THREE_MINUTE 3

void app_relayAction(bool actionFlag)
{
    bool mainFlag = false;
    bool assistFlag = false;
    SysPara_t *ptSys;
    ptSys = controler_getSysParaPt(); 
    if(actionFlag)
    {
        switch(ptSys->record.relayOutType)
        {
            case RELAY_OUT_TYPE_MAIN_P_ASSIST_P:mainFlag = true;assistFlag = true;break;
            case RELAY_OUT_TYPE_MAIN_P_ASSIST_N:mainFlag = true;assistFlag = false;break;
            case RELAY_OUT_TYPE_MAIN_N_ASSIST_N:mainFlag = false;assistFlag = false;break;
            case RELAY_OUT_TYPE_MAIN_P_ASSIST_N_OFF:mainFlag = false;assistFlag = false;break;
            default:break;
        }
    }
    else
    {
        switch(ptSys->record.relayOutType)
        {
            case RELAY_OUT_TYPE_MAIN_P_ASSIST_P:mainFlag = false;assistFlag = false;break;
            case RELAY_OUT_TYPE_MAIN_P_ASSIST_N:mainFlag = false;assistFlag = true;break;
            case RELAY_OUT_TYPE_MAIN_N_ASSIST_N:mainFlag = true;assistFlag = true;break;
            case RELAY_OUT_TYPE_MAIN_P_ASSIST_N_OFF:
            {
                if(SYS_STATUS_POWER_OFF != ptSys->record.sysRunStatus)
                {
                    mainFlag = true;
                    assistFlag = true;
                }
                else
                {
                    if(MIN_LTP_TEMP == ptSys->record.ltpSetTemp)
                    {
                        mainFlag = false;
                        assistFlag = false;
                    }
                    else
                    {
                        mainFlag = true;
                        assistFlag = true;
                    }
                }
            }
            break;
            default:break;
        }  
    }
    if(mainFlag)
    {
        mod_relay_on(RELAY_FLOOR,RELAY_DELAY_TIME); 
    }
    else
    {
        mod_relay_off(RELAY_FLOOR,0);  
    }
    
    if(assistFlag)
    {
        uint16_t delayTime = 0;
        delayTime = ptSys->record.linkRelayDelayTime * 600;
        if(0 == delayTime)
        {
            delayTime = RELAY_DELAY_TIME * 2;
        }
        mod_relay_on(RELAY_ASSIST,delayTime);
    }
    else
    {
        mod_relay_off(RELAY_ASSIST,0);  
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
            ltpFlag = false;
        }
        else
        {
            ptSysPara->floorRelayFlag = false;
            
            if((SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus) &&\
              (MIN_LTP_TEMP < ptSysPara->record.ltpSetTemp) &&\
                (NONE_ERROR == ptSysPara->systemError))
            {
                ltpFlag = app_output_ltpLogic(ltpFlag); 
            }
            else
            {
                ltpFlag = false;
            } 
            ptSysPara->floorRelayFlag = ltpFlag;
        }  
        
        if(app_openWinow_run())
        {
            ptSysPara->floorRelayFlag = false;
        }
        
        app_relayAction(ptSysPara->floorRelayFlag);   
    } 
}


