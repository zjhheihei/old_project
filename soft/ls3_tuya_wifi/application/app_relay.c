


#include ".\APP_Cfg.h"



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************本模块使用的局部变量***********************************
//------------------------------E N D-------------------------------------------
static bool outsideLimitFlag = false;

#define LOW_TEMP_PROTECT_DIF   30


#define LIF_DIFF   30 
bool relay_out_limitFlag(void)
{
    return outsideLimitFlag;
}

bool app_relayOutput_enable(void)
{//允许继电器输出
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt();
    if(SENSOR_ALL == ptSysPara->record.sersonType)
    {
        if(NONE_ERROR != ptSysPara->systemError)
        {
            return false;
        }
        else
        {
            if(ptSysPara->runLifTemp >= ptSysPara->record.lifTemp)
            {
                outsideLimitFlag = true;
                return false;
            }
            else if(ptSysPara->runLifTemp <= ptSysPara->record.lifTemp - LIF_DIFF)
            {
                outsideLimitFlag = false;
            } 
        }
    }
    else
    {
        outsideLimitFlag = false;
        if(SENSOR_IN == ptSysPara->record.sersonType)
        {
            if(INSIDE_ERROR & ptSysPara->systemError)
            {
                return false;
            }
        }
        else if(SENSOR_OUT == ptSysPara->record.sersonType)
        {
            if(OUTSIDE_ERROR & ptSysPara->systemError)
            {
                return false;
            }
        }
    }
    
    if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
    {
        return false;
    }
    else if(SYS_STATUS_HIGH_SETTING == ptSysPara->record.sysRunStatus)
    {
        return false;
    }
    return true;
}

#define TEN_DAY 14400
#define THREE_MINUTE 3

typedef enum
{
    CLEAN_IDLE=((uint8_t)0x00),
    CLEAN_RUN,
}cleanStatus_t;
static cleanStatus_t cleanStatus = CLEAN_IDLE;

/*除垢功能*/
void app_relayOut_clean(void)
{
    static uint16_t lastCleanTick = 0;
    static bool lastOutputFlag = false;
    static uint16_t mintueCount = 0;  
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt();
    
    if(ptSysPara->record.cleanFlag)
    {
        uint16_t diffTick = 0;
        diffTick = GetSysTickMillisecond() - lastCleanTick;
        switch(cleanStatus)
        {
            case CLEAN_IDLE:
            {
                if(lastOutputFlag != ptSysPara->floorRelayFlag)
                {
                    lastOutputFlag = ptSysPara->floorRelayFlag;
                    mintueCount = 0;
                    lastCleanTick = GetSysTickMillisecond();
                }
                else
                {                   
                    if(60000 <= diffTick)
                    {
                        lastCleanTick = GetSysTickMillisecond();
                        if(++mintueCount >= TEN_DAY)
                        {
                            mintueCount = 0;
                            cleanStatus = CLEAN_RUN;
                            if(ptSysPara->floorRelayFlag)
                            {
                                ptSysPara->floorRelayFlag = false;
                            }
                            else
                            {
                                ptSysPara->floorRelayFlag = true;
                            }
                        }                   
                    }                     
                }
                break;
            }
            case CLEAN_RUN:
            {
                if(60000 <= diffTick)
                {
                    lastCleanTick = GetSysTickMillisecond();
                    if(++mintueCount >= THREE_MINUTE)
                    {
                        cleanStatus = CLEAN_IDLE;
                        mintueCount = 0;
                    }
                }
                break;
            }
        }
    }
    else
    {
        mintueCount = 0;       
        cleanStatus = CLEAN_IDLE;
    }
}

void app_relay_ltp(void)
{
    static bool ltpOutputFlag = false;
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt(); 
    if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
    {
        if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
        {
            ltpOutputFlag = app_output_floorLogic();
            ptSysPara->floorRelayFlag = ltpOutputFlag;
        }
    }
    else
    {
        ltpOutputFlag = false;
    }
}

void app_relayOut_type(void)
{
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt(); 
    volatile bool floorFlag = false;
    volatile bool linkFlag = false;   
    switch(ptSysPara->record.outputType)
    {
        case RELAY_SAME_DIRECTION:
        {
            if(ptSysPara->floorRelayFlag)
            {
                floorFlag = true;
                linkFlag = true;   
            }
            else
            {
                floorFlag = false;
                linkFlag = false;
            }
            break;
        }
        case ASSIST_RELAY_REVERSE:
        {
            if(ptSysPara->floorRelayFlag)
            {
                //mod_relay_on(RELAY_FLOOR,RELAY_DELAY_TIME);     
                //mod_relay_off(RELAY_ASSIST,0); 
                floorFlag = true;
                linkFlag = false;
            }
            else
            {
                //mod_relay_off(RELAY_FLOOR,0); 
                //mod_relay_on(RELAY_ASSIST,delayTime); 
                floorFlag = false;
                linkFlag = true;
            }
            break;
        }
        case RELAY_ALL_REVERSE:
        {
            if(ptSysPara->floorRelayFlag)
            {
                //mod_relay_off(RELAY_FLOOR,0);     
                //mod_relay_off(RELAY_ASSIST,0); 
                floorFlag = false;
                linkFlag = false;
            }
            else
            {
                //mod_relay_on(RELAY_FLOOR,RELAY_DELAY_TIME); 
                //mod_relay_on(RELAY_ASSIST,delayTime);
                floorFlag = true;
                linkFlag = true;
            }
            break;
        }
        case MAIN_RELAY_REVERSE:
        {
            if(ptSysPara->floorRelayFlag)
            {
                floorFlag = false;
                linkFlag = true;
            }
            else
            {
                if(SYS_STATUS_POWER_OFF != ptSysPara->record.sysRunStatus) 
                {
                    //relay_on(RELAY_MAIN,RELAY_DELAY_TIME);
                    floorFlag = true;
                } 
                else
                {
                    //relay_off(RELAY_MAIN,NULL); 
                    floorFlag = false;
                  
                }               
                linkFlag = false;
            }
            break;
        }
    }
    
    if(floorFlag)
    {
        mod_relay_on(RELAY_FLOOR,RELAY_DELAY_TIME); 
    }
    else
    {
        mod_relay_off(RELAY_FLOOR,0);  
    }
    
    if(linkFlag)
    {
        uint16_t linkDelayTime = 0;
        linkDelayTime = (uint16_t)ptSysPara->record.linkRelayDelayTime * 600;
        if(0 == linkDelayTime)
        {
            linkDelayTime = RELAY_DELAY_TIME + RELAY_DELAY_TIME ;
        }
        mod_relay_on(RELAY_ASSIST,linkDelayTime);
    }
    else
    {
        mod_relay_off(RELAY_ASSIST,0);
    }
}

void app_relayOut_scanTask(void)
{    
    SysPara_t *ptSysPara;
    ptSysPara = controler_getSysParaPt();        
     //以下为输出逻辑判断
    mod_relay_scanTask(GetSysTickMillisecond());
    app_relayOut_clean();
    if(CLEAN_IDLE == cleanStatus)
    {
        if((app_relayOutput_enable()) && (!outsideLimitFlag))
        {
            ptSysPara->floorRelayFlag = app_output_floorLogic(); 
        }
        else
        {
            ptSysPara->floorRelayFlag = false;
        }
        
        if(OW_STATUS_RUN == ptSysPara->owStatus)
        {
            ptSysPara->floorRelayFlag = false;
        }
        
        app_relay_ltp();       
    }
    app_relayOut_type();   
}
//-----------------------APP.ThreeFanOut.C--END---------------------------------


