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

uint16_t diffMinute(uint16_t timeBig,uint16_t timeSmall)
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

static void idle_status(uint8_t runInt)
{    
    SysPara_t *para;  
    para = controler_getSysParaPt();               
    preheatRun.setTime = getPreheatTime(para->record.preheat.addTime[runInt]); 
    switch(runInt)
    {
        case 0: 
        case 1: 
        case 2:        
        case 4:
        {
            preheatRun.nextPrgTime = para->record.interval[runInt + 1].hourMinute;
            preheatRun.nextPrgTemp = para->record.interval[runInt + 1].temp;
            break;
        }
        case 3:
        {
            preheatRun.nextPrgTime = para->record.interval[0].hourMinute;
            preheatRun.nextPrgTemp = para->record.interval[0].temp;
            break;
        }         
        case 5:
        {
            preheatRun.nextPrgTime = para->record.interval[4].hourMinute;
            preheatRun.nextPrgTemp = para->record.interval[4].temp;
            break;   
        }                                                              
    }
    realTime_t *realTime;
    realTime = app_real_time_getTime();
    uint16_t diffTime = 0;
    diffTime = diffMinute(preheatRun.nextPrgTime,realTime->hm);
    if(diffTime <= preheatRun.setTime)
    {     
        int16_t diffTemp = 0;
        diffTemp = preheatRun.nextPrgTemp - para->runMeasTemp;
        if(diffTemp >= para->record.diffTemp)
        {
            preheatRun.status = PREHEAT_STATUS_WORK;
            if(!preheatRun.finishFlag)
            {//本时段预加热没有开启过，刷新时间，有预加热过，以第一次完整的完成过程为基准
              //后续反复开启加热不记录时间，但仍然开启预加热
                preheatRun.startTime = realTime->hm;
                preheatRun.runTime = 0;    
            }                             
        }        
    }           
}

static void work_status(void)
{   
    SysPara_t *para;  
    para = controler_getSysParaPt();
    int16_t diffTemp = 0;  
    diffTemp = preheatRun.nextPrgTemp - para->runMeasTemp;
    if(!preheatRun.finishFlag)
    {//未完成预加热，记录加热时间
        realTime_t *realTime;
        realTime = app_real_time_getTime();
        preheatRun.runTime = diffMinute(realTime->hm,preheatRun.startTime);          
    }      
    if(diffTemp <= -para->record.diffTemp)
    {//加热完成                
        preheatRun.finishFlag = true; 
        preheatRun.status = PREHEAT_STATUS_IDLE;
    }   
}

static bool enable_check(void)
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
    else if(TEMP_CONTROL_MODE_MANUAL == ptSysPara->record.tempControlMode)
    {
        return false;
    }
    else if(ptSysPara->systemError)
    {
        return false;
    }
    else if(!ptSysPara->record.preheat.switchflag)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void app_preheat_task(void)
{   
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    if(preheatRun.runInt != app_inter_runInter())
    {//时段改变        
        if(preheatRun.runTime) //有运行过
        {
            if(preheatRun.finishFlag)
            {//预加热完成
                ptSysPara->record.preheat.addTime[preheatRun.runInt] -= preheatRun.setTime;
                ptSysPara->record.preheat.addTime[preheatRun.runInt] += preheatRun.runTime;
            }
            else
            {//没有完成预加热
                if(preheatRun.runTime < (preheatRun.setTime - 1)) //运行时间
                {//没有完成一次预加热时间，可能是中途开启，不记录时间
                    
                }
                else
                {//完成预加热时间，没有达到预定的温度，预加热时间需要延长 
                     ptSysPara->record.preheat.addTime[preheatRun.runInt] += 35;//7次增加5分钟
                     if(420 <= ptSysPara->record.preheat.addTime[preheatRun.runInt])
                     {
                         ptSysPara->record.preheat.addTime[preheatRun.runInt] = 420;
                     }
                }
            }
        }
        preheatRun.runInt     = app_inter_runInter(); 
        preheatRun.status     = PREHEAT_STATUS_IDLE;
        preheatRun.finishFlag = false;;    
    }
    else
    {
        if(enable_check())
        {
            if(PREHEAT_STATUS_IDLE == preheatRun.status)
            {
                idle_status(preheatRun.runInt);
            }
            else if(PREHEAT_STATUS_WORK == preheatRun.status)
            {
                work_status();
            }            
        }
        else
        {
            preheatRun.status = PREHEAT_STATUS_IDLE;
            preheatRun.finishFlag = false;;    
        }
    }            
}