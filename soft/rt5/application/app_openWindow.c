#include ".\APP_Cfg.h"

typedef enum
{
    OPEN_WINDOW_STATUS_INIT = ((uint8_t)0x00),
    OPEN_WINDOW_STATUS_DELAY,
    OPEN_WINDOW_STATUS_IDLE,     
    OPEN_WINDOW_STATUS_RUN,               
}openWindowStatus_t;

static int16_t  maxTempGroup[30];
static uint8_t  minuteCnt = 0;
static uint16_t openWindowRunTime = 0;
static openWindowStatus_t openWindowStatus = OPEN_WINDOW_STATUS_INIT;



bool app_openWinow_run(void)
{
    if(OPEN_WINDOW_STATUS_RUN == openWindowStatus)
    {
        return true;
    }
    return false;
}

void app_openWindow_clr(void)
{
    openWindowStatus = OPEN_WINDOW_STATUS_INIT;
    openWindowRunTime = 0;     
}

void openWindow_check(void)
{   
    static int16_t  runStoMaxTemp = -99;
    static int16_t  runMinuteMaxTemp = -99;   
    static uint8_t checkTime = 0;
    if(++checkTime >= 6)
    {//3s
        checkTime = 0;     
        SysPara_t *para;  
        para = controler_getSysParaPt();       
        if(0 == minuteCnt)
        {
            uint8_t i = 0;
            runStoMaxTemp = -99;
            for(;i < para->record.window.check_time;i++)
            {
                if(runStoMaxTemp < maxTempGroup[i])
                {
                    runStoMaxTemp = maxTempGroup[i];
                }
            }         
            runMinuteMaxTemp = -99; 
        } 
        
        int16_t diffTemp = runMinuteMaxTemp - para->runMeasTemp; 
        if(para->record.window.down_temp <= diffTemp)
        {
            openWindowStatus = OPEN_WINDOW_STATUS_RUN;
            return;
        }
        else
        {
            if(0 > diffTemp)
            {
                runMinuteMaxTemp = para->runMeasTemp;
            }
        }            
        diffTemp = runStoMaxTemp - para->runMeasTemp;
        if(para->record.window.down_temp <= diffTemp)
        {
            openWindowStatus = OPEN_WINDOW_STATUS_RUN;
            return;
        }
        minuteCnt++;
        
        #if defined(__DEBUG__)
        if(2 == minuteCnt)
        #else
        if(20 == minuteCnt)
        #endif
        {
            int16_t buf[30];
            uint8_t i = 0;
            for(;i < 30;i++)
            {
                buf[i] = maxTempGroup[i];
            }
            for(i = 0;i < 29;i++)
            {
                maxTempGroup[i + 1] = buf[i];  
            }                    
            maxTempGroup[0] = runMinuteMaxTemp;
            minuteCnt = 0;
        } 
    }
}

bool openWindow_enable(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    if(para->systemError)
    {
        return false;
    }
    if(!para->record.window.switch_flag)
    {
        return false;
    }
    if(SYS_STATUS_POWER_OFF == para->record.sysRunStatus)
    {
        return false;
    }    
    return true;
}

#define OW_RUN_DELAY_TIME  20
void app_openWindow_task(void)
{   
    static uint8_t  owRunDelayTime = 0;
    static uint16_t lastTick = 0;
           uint16_t diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastTick;
    if(diffTick > 500)
    {       
        lastTick = GetSysTickMillisecond();       
        SysPara_t *para;  
        para = controler_getSysParaPt();
        if(openWindow_enable())              
        {
            switch(openWindowStatus)
            {
                case OPEN_WINDOW_STATUS_INIT:
                {
                    uint8_t i = 0;
                    for(;i < 30;i++)
                    {
                        maxTempGroup[i] = -99;
                    } 
                    minuteCnt = 0;
                    openWindowRunTime = 0;  
                    openWindowStatus = OPEN_WINDOW_STATUS_DELAY;
                    break;
                } 
                case OPEN_WINDOW_STATUS_DELAY:
                {
                    if(++owRunDelayTime >= OW_RUN_DELAY_TIME)
                    {    
                        owRunDelayTime = 0;
                        openWindowStatus = OPEN_WINDOW_STATUS_IDLE;
                    }
                    break;
                }
                case OPEN_WINDOW_STATUS_IDLE:
                {
                    openWindow_check();
                    break;
                }
                case OPEN_WINDOW_STATUS_RUN:
                {
                    openWindowRunTime++;
                    #if defined(__DEBUG__)
                    if(openWindowRunTime >= para->record.window.back_time*10)
                    #else
                    if(openWindowRunTime >= para->record.window.back_time*120)
                    #endif       
                    {
                        openWindowStatus = OPEN_WINDOW_STATUS_INIT;
                    }
                    break;
                }
            }
        }                                  
        else
        {
            openWindowStatus = OPEN_WINDOW_STATUS_INIT;
            openWindowRunTime = 0;          
        }      
    }        
}