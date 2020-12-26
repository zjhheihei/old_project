#include ".\APP_Cfg.h"

typedef enum
{
    PREHEAT_STATUS_IDLE = 0X00,   
    PREHEAT_STATUS_WORK,
}preheatStatus_t;

typedef struct
{
    preheatStatus_t  status;      //预加热状态
    uint16_t         setTime;     //预加热提前时间
    uint16_t         nextPrgTime; //下个时段时间
    uint16_t         nextPrgTemp; //下个时段设定温度
    uint16_t         startTime;   //预加热启动时间
    uint16_t         runTime;
    uint8_t          runInt;      //记录运行时段
    bool             finishFlag;
}preheatRun_t;

static preheatRun_t preheatRun;

bool app_preheat_getStatus(void)
{
    if(PREHEAT_STATUS_WORK == preheatRun.status)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void app_preheat_HexTimeToMinute(uint16_t *_out_minute) 
{
    uint16_t hourMinute = 0;
    hourMinute = *_out_minute;
    uint16_t ten,units;
    ten = (((hourMinute >> 8) & 0x00ff)*60) ;
    units = hourMinute & 0x00ff;
    *_out_minute = ten + units; 
}

uint16_t app_preheat_diffMinute(uint16_t timeBig,uint16_t timeSmall)
{
     static uint16_t big = 0;
     static uint16_t small = 0;
     uint16_t retDifTime;
     
     big =  (timeBig >> 8)*60 + (timeBig &0x00ff);
     small = (timeSmall >> 8)*60 + (timeSmall &0x00ff);
     if(big >= small)
     {
         retDifTime = big - small;
     }
     else
     {
         retDifTime = 0;
     }
     return retDifTime;
}

uint16_t getPreheatTime(uint16_t addTime)
{
    uint16_t retTime = 0;
    uint8_t ten,units;
    ten = addTime / 7;
    units = addTime % 7;
    retTime = ten;
    if(4 <= units)
    {
        retTime += 1;
    }
    return retTime;
}

void app_preheat_idle(uint8_t runInt)
{    
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();               
//    preheatRun.setTime = getPreheatTime(ptSysPara->record.preheat.addTime[runInt]); 
//    switch(runInt)
//    {
//        case 0: 
//        case 1: 
//        case 2: 
//        case 3: 
//        case 4: 
//        case 6:
//        {
//            preheatRun.nextPrgTime = ptSysPara->record.interval[runInt+1].hourMinute;
//            preheatRun.nextPrgTemp = ptSysPara->record.interval[runInt+1].temp;
//            break;
//        }
//        case 5:
//        {
//            preheatRun.nextPrgTime = ptSysPara->record.interval[0].hourMinute;
//            preheatRun.nextPrgTemp = ptSysPara->record.interval[0].temp;
//            break;
//        }         
//        case 7:
//        {
//            preheatRun.nextPrgTime = ptSysPara->record.interval[6].hourMinute;
//            preheatRun.nextPrgTemp = ptSysPara->record.interval[6].temp;
//            break;   
//        }                                                              
//    }
//    
//    uint16_t diffTime = 0;
//    diffTime = app_preheat_diffMinute(preheatRun.nextPrgTime,ptSysPara->rtc.hourMinute);
//    if(diffTime <= preheatRun.setTime)
//    {     
//        int16_t diffTemp = 0;
//        diffTemp = preheatRun.nextPrgTemp - ptSysPara->runMeasTemp;
//        if(diffTemp >= ptSysPara->record.diffTemp)
//        {
//            preheatRun.status = PREHEAT_STATUS_WORK;
//            if(!preheatRun.finishFlag)
//            {//本时段预加热没有开启过，刷新时间，有预加热过，以第一次完整的完成过程为基准
//              //后续反复开启加热不记录时间，但仍然开启预加热
//                preheatRun.startTime = ptSysPara->rtc.hourMinute;
//                preheatRun.runTime = 0;    
//            }                             
//        }        
//    }           
}

void app_preheat_work(void)
{   
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();
//    int16_t diffTemp = 0;  
//    diffTemp = preheatRun.nextPrgTemp - ptSysPara->runMeasTemp;
//    if(!preheatRun.finishFlag)
//    {//未完成预加热，记录加热时间
//        preheatRun.runTime = app_preheat_diffMinute\
//              (ptSysPara->rtc.hourMinute,preheatRun.startTime);          
//    }      
//    if(diffTemp <= - ptSysPara->record.diffTemp)
//    {//加热完成                
//        preheatRun.finishFlag = true; 
//        preheatRun.status = PREHEAT_STATUS_IDLE;
//    }   
}

bool app_preheat_enable(void)
{
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
    //else if(TEMP_CONTROL_MODE_MANUAL == ptSysPara->record.tempControlMode)
//    {
//        return false;
//    }
    else if(ptSysPara->systemError)
    {
        return false;
    }
//    else if(!ptSysPara->record.preheat.switchflag)
//    {
//        return false;
//    }
    else
    {
        return true;
    }
}

void app_preheat_scanTask(void)
{   
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();
//    if(preheatRun.runInt != app_interval_getInterval())
//    {//时段改变        
//        if(preheatRun.runTime) //有运行过
//        {
//            if(preheatRun.finishFlag)
//            {//预加热完成
//                ptSysPara->record.preheat.addTime[preheatRun.runInt] -= preheatRun.setTime;
//                ptSysPara->record.preheat.addTime[preheatRun.runInt] += preheatRun.runTime;
//            }
//            else
//            {//没有完成预加热
//                if(preheatRun.runTime < (preheatRun.setTime - 1)) //运行时间
//                {//没有完成一次预加热时间，可能是中途开启，不记录时间
//                    
//                }
//                else
//                {//完成预加热时间，没有达到预定的温度，预加热时间需要延长 
//                     ptSysPara->record.preheat.addTime[preheatRun.runInt] += 35;//7次增加5分钟
//                     if(420 <= ptSysPara->record.preheat.addTime[preheatRun.runInt])
//                     {
//                         ptSysPara->record.preheat.addTime[preheatRun.runInt] = 420;
//                     }
//                }
//            }
//        }
//        preheatRun.runInt = app_interval_getInterval(); 
//        preheatRun.status = PREHEAT_STATUS_IDLE;
//        preheatRun.finishFlag = false;;    
//    }
//    else
//    {
//        if(app_preheat_enable())
//        {
//            if(PREHEAT_STATUS_IDLE == preheatRun.status)
//            {
//                app_preheat_idle(preheatRun.runInt);
//            }
//            else if(PREHEAT_STATUS_WORK == preheatRun.status)
//            {
//                app_preheat_work();
//            }            
//        }
//        else
//        {
//            preheatRun.status = PREHEAT_STATUS_IDLE;
//            preheatRun.finishFlag = false;;    
//        }
//    }            
}