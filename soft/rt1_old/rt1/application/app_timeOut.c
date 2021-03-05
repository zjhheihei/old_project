#include ".\app_cfg.h"



static timeOut_t timeOut[TIME_OUT_MAX];

bool app_timeOut_getStatus(timeOutType_t type)
{
    if(timeOut[type].flag)
    {
        timeOut[type].flag = false;
        return true;
    }
    return false;
}

uint16_t app_timeOut_getTime(timeOutType_t type)
{
    return timeOut[type].time;
}

void app_timeOut_setTime(timeOutType_t type,uint16_t time)
{
    timeOut[type].flag = false;
    timeOut[type].time = time;       
}

bool app_timeOut_fixCyc(uint16_t *_out_lastTick,uint16_t delayMsCyc)
{
    uint16_t diffTick = 0;
    diffTick = GetSysTickMillisecond() - *_out_lastTick;
    if(diffTick >= delayMsCyc)
    {
        *_out_lastTick = GetSysTickMillisecond();
        return true;
    }
    return false;
}

uint16_t app_timeOut_passTime(uint16_t lastTick)
{
    uint16_t passTime = 0;
    passTime = GetSysTickMillisecond() - lastTick;
    return passTime;
}

bool app_timeOut_blink500ms(void)
{
    static uint16_t blinkTick = 0;
    static bool     blinkFlag = false;
    if(app_timeOut_fixCyc(&blinkTick,ms_500))
    {
        if(blinkFlag)
        {
            blinkFlag = false;
        }
        else
        {
            blinkFlag = true;
        }
    }
    return blinkFlag;
}

void app_timeOut_uadata(void)
{
//    sysPara_t *ptSys;  
//    ptSys = controler_getSysParaPt();
//    switch(ptSys->runSysStatus)
//    {               
//        case SYS_STATUS_POWER_OFF:                 break;
//        case SYS_STATUS_RUN_MODE:                  
//        {
//            if(app_timeOut_getStatus(TIME_OUT_RUN_MODE))
//            {             
//                if(ptSys->disForRunMode_disTemp)
//                {
//                    ptSys->disForRunMode_disTemp = false;
//                }
//                else
//                {
//                    ptSys->disForRunMode_disTemp = true;
//                }   
//                app_timeOut_setTime(TIME_OUT_RUN_MODE,ms_4000);
//            }
//            break;
//        }
//        case SYS_STATUS_HIGH_SETTING:              
//        {
//            if(app_timeOut_getStatus(TIME_OUT_HIGH_SET))
//            {
//                if(HIGH_SETTING_NTC_ADJ == ptSys->highSet.menu)
//                {
//                    ptSys->highSet.adjStatus = ADJ_MESA_TEMP;                    
//                }
//                else if(HIGH_SETTING_FAC == ptSys->highSet.menu)
//                {
//                    if(FAC_FINISH > ptSys->highSet.facStatus)
//                    {
//                        ptSys->highSet.facStatus++;
//                        app_timeOut_setTime(TIME_OUT_HIGH_SET,ms_500);
//                    }
//                }               
//            }
//            break;     
//        }
//        case SYS_STATUS_SHOW_VERSIONS:             
//        {
//            if(app_timeOut_getStatus(TIME_OUT_COMMON))
//            {
//                if(ptSys->record.powFlag)
//                {
//                    ptSys->runSysStatus = SYS_STATUS_RUN_MODE;
//                }
//                else
//                {
//                    ptSys->runSysStatus = SYS_STATUS_POWER_OFF;
//                }            
//            }            
//            break;   
//        }
//        case SYS_STATUS_SHOW_ALL:                  
//        {
//           if(app_timeOut_getStatus(TIME_OUT_COMMON))
//           {
//               ptSys->runSysStatus = SYS_STATUS_SHOW_VERSIONS;
//               app_timeOut_setTime(TIME_OUT_COMMON,ms_1000);
//           }                       
//           break;
//        }  
//       case SYS_STATUS_RTC_SET:                   
//       {
//          if(app_timeOut_getStatus(TIME_OUT_COMMON))
//          {
//              ptSys->runSysStatus = SYS_STATUS_RUN_MODE;              
//          }    
//          
//          if(app_timeOut_getStatus(TIME_OUT_RTC_SET))
//          {
//              ptSys->disForRtcSet_disBlink = true;
//          }
//          break;
//       }
//       case SYS_STATUS_PRG_SET:                   break;       
//       default:break;
//    }  
}


void app_timeOut_init(void)
{
    app_timeOut_setTime(TIME_OUT_RUN_MODE,ms_4000);
}

void app_timeOut_scanTask(void)
{   
    static bool     initFlag = true;
    static uint16_t lastSysTick = 0;               
           uint16_t diffTick = 0;          
    diffTick = app_timeOut_passTime(lastSysTick);
    if(initFlag)
    {
        initFlag = false;
        app_timeOut_init();
    }
    if(diffTick)
    {
        lastSysTick = GetSysTickMillisecond();
        uint8_t i = 0;
        for(;i< TIME_OUT_MAX;i++)
        {                       
            if(timeOut[i].time>0)
            {   
                if(timeOut[i].time >= diffTick)
                {
                    timeOut[i].time -= diffTick;
                }
                else
                {
                    timeOut[i].time = 0;
                }
             
                if(0 == timeOut[i].time)
                {
                    timeOut[i].flag = true;                    
                }
            }
        }
    }  
    app_timeOut_uadata();
}