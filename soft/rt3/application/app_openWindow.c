#include ".\APP_Cfg.h"


static int16_t  maxTempGroup[30];
static uint8_t  minuteCnt = 0;
static uint16_t openWindowRunTime = 0;

//void app_openWindow_check(void)
//{   
//    static int16_t  runStoMaxTemp = -99;
//    static int16_t  runMinuteMaxTemp = -99;   
//    static uint8_t checkTime = 0;
//    if(++checkTime >= 6)
//    {
//        checkTime = 0;     
//        SysPara_t *ptSysPara;  
//        ptSysPara = controler_getSysParaPt();       
//        if(0 == minuteCnt)
//        {
//            uint8_t i = 0;
//            runStoMaxTemp = -99;
//            for(;i < ptSysPara->record.owPara.checkTime;i++)
//            {
//                if(runStoMaxTemp < maxTempGroup[i])
//                {
//                    runStoMaxTemp = maxTempGroup[i];
//                }
//            }         
//            runMinuteMaxTemp = -99; 
//        } 
//        
//        int16_t diffTemp = runMinuteMaxTemp - ptSysPara->runMeasTemp; 
//        if(ptSysPara->record.owPara.downTemp <= diffTemp)
//        {
//            ptSysPara->owStatus = OW_STATUS_RUN;
//            return;
//        }
//        else
//        {
//            if(0 > diffTemp)
//            {
//                runMinuteMaxTemp = ptSysPara->runMeasTemp;
//            }
//        }            
//        diffTemp = runStoMaxTemp - ptSysPara->runMeasTemp;
//        if(ptSysPara->record.owPara.downTemp <= diffTemp)
//        {
//            ptSysPara->owStatus = OW_STATUS_RUN;
//            return;
//        }
//        minuteCnt++;
//        
//        #if defined(__DEBUG__)
//        if(2 == minuteCnt)
//        #else
//        if(20 == minuteCnt)
//        #endif
//        {
//            int16_t buf[30];
//            uint8_t i = 0;
//            for(;i < 30;i++)
//            {
//                buf[i] = maxTempGroup[i];
//            }
//            for(i = 0;i < 29;i++)
//            {
//                maxTempGroup[i + 1] = buf[i];  
//            }                    
//            maxTempGroup[0] = runMinuteMaxTemp;
//            minuteCnt = 0;
//        } 
//    }
//}


#define OW_RUN_DELAY_TIME  60
void app_openWindow_scanTask(void)
{   
//    static uint8_t  owRunDelayTime = 0;
//    static uint16_t lastTick = 0;
//           uint16_t diffTick = 0;
//    diffTick = GetSysTickMillisecond() - lastTick;
//    if(diffTick > 500)
//    {       
//        lastTick = GetSysTickMillisecond();       
//        SysPara_t *ptSysPara;  
//        ptSysPara = controler_getSysParaPt();
//        if((0 == ptSysPara->systemError) && (ptSysPara->record.owPara.enableFlag) &&\
//          (SYS_STATUS_POWER_OFF != ptSysPara->record.sysRunStatus))              
//        {
//            switch(ptSysPara->owStatus)
//            {
//                case OW_STATUS_INIT:
//                {
//                    uint8_t i = 0;
//                    for(;i < 30;i++)
//                    {
//                        maxTempGroup[i] = -99;
//                    } 
//                    minuteCnt = 0;
//                    openWindowRunTime = 0;  
//                    ptSysPara->owStatus = OW_STATUS_DELAY;
//                    break;
//                } 
//                case OW_STATUS_DELAY:
//                {
//                    if(++owRunDelayTime >= OW_RUN_DELAY_TIME)
//                    {    
//                        owRunDelayTime = 0;
//                        ptSysPara->owStatus = OW_STATUS_CHECK;
//                    }
//                    break;
//                }
//                case OW_STATUS_CHECK:
//                {
//                    app_openWindow_check();
//                    break;
//                }
//                case OW_STATUS_RUN:
//                {
//                    openWindowRunTime++;
//                    #if defined(__DEBUG__)
//                    if(openWindowRunTime >= ptSysPara->record.owPara.backTime*10)
//                    #else
//                    if(openWindowRunTime >= ptSysPara->record.owPara.backTime*120)
//                    #endif       
//                    {
//                        ptSysPara->owStatus = OW_STATUS_INIT;
//                    }
//                    break;
//                }
//            }
//        }                                  
//        else
//        {
//            ptSysPara->owStatus = OW_STATUS_INIT;
//            owRunDelayTime = 0;          
//        }      
//    }        
}