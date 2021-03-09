/*******************************************************************************
*
*	模块名称 : 输出控制
*	文件名称 : app_timming.c
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


typedef enum
{
    TIME_CON_STATUS_IDLE = 0x00,
    TIME_CON_STATUS_RUN,
    TIME_CON_STATUS_REST,
}timeContorlStatus_t;

typedef struct
{//超时
    timeContorlStatus_t status; 
    uint8_t             runMinute;  
    bool                disTimeControlFlag;
    uint8_t             *ptSetTime;        //设定的运行和停歇时间，设定和停歇地址必须相邻且运行在前
}timeContorl_t;

static timeContorl_t timeContorl;


bool app_timeControl_restStatus(void)
{
    if(TIME_CON_STATUS_REST == timeContorl.status)
    {
        return true;
    } 
    return false;
}

void app_timeControl_reset(void)
{
    timeContorl.status = TIME_CON_STATUS_IDLE;
}

void app_timeControl_configure(uint8_t *ptSetTime)
{
    timeContorl.ptSetTime = ptSetTime;
}

void app_timeControl_handle(bool timeControlEnable)
{
    static uint16_t timeConOneMinuteCyc = 0;       
    if(timeControlEnable && app_radiationLogic_get_status())
    {      
       switch(timeContorl.status)
       {
          case TIME_CON_STATUS_IDLE:
          {            
              timeContorl.runMinute = 0;
              timeContorl.status = TIME_CON_STATUS_RUN;
              timeConOneMinuteCyc = GetSysTickMillisecond();                                   
              break;
          }         
          default:
          {
              uint16_t passTick = 0;
              passTick = GetSysTickMillisecond() - timeConOneMinuteCyc;
              if(ms_60000 <= passTick)
              {
                  timeConOneMinuteCyc = GetSysTickMillisecond();
                  timeContorl.runMinute++; 
                  uint8_t *ptLastTime = timeContorl.ptSetTime;
                  ptLastTime += ((uint8_t)timeContorl.status - 1);                  
                  if(timeContorl.runMinute >= *ptLastTime)
                  {
                      timeContorl.runMinute = 0;
                      if(TIME_CON_STATUS_REST == timeContorl.status)
                      {
                          timeContorl.status = TIME_CON_STATUS_RUN;
                      }
                      else
                      {
                          timeContorl.status = TIME_CON_STATUS_REST;                            
                      }
                  }
              }
              break;
          }
       }       
    }
    else
    {
        timeContorl.status = TIME_CON_STATUS_IDLE;               
    } 
    if(TIME_CON_STATUS_REST == timeContorl.status)
    {
        app_radiationLogic_set_off();
    }
}

void app_timeControl_scanTask(void)
{
    static bool initFlag = true;
    SysPara_t *ptSys;
    ptSys = controler_getSysParaPt(); 
    if(initFlag)
    {
        initFlag = false;
        app_timeControl_configure(&ptSys->record.outputOpenLastTime);
        timeContorl.status = TIME_CON_STATUS_RUN;
    }
    else
    {
        app_timeControl_handle(ptSys->record.outputTimeControFlag);
    }
}
