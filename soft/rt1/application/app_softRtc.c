/*******************************************************************************
*
*	模块名称 : 输出控制
*	文件名称 : OutputControl.c
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
    REAL_TIME_STATUS_INIT = ((uint8_t)0x00), 
    REAL_TIME_STATUS_CHECK_NET,
    REAL_TIME_STATUS_CHECK_NET_RTC_VAILD,
    REAL_TIME_STATUS_RUN,   
}realTimeRunStatus_t;

realTimeRunStatus_t realTimeRunStatus;
static realTime_t realTime = 
{
    .hour   = 8,
    .minute = 0,
    .week   = 4,
    .second = 0,
};

void app_softRtc_handle(void)
{
    static uint16_t secondCyc = 0;
           uint16_t passSecond = 0;
    passSecond = GetSysTickMillisecond() - secondCyc;   
    if(ms_1000 <= passSecond)
    {
        secondCyc = GetSysTickMillisecond();
        if(++realTime.second >= 60)
        {
            realTime.second = 0;
            if(++realTime.minute >= 60)
            {
                realTime.minute = 0;
                if(++realTime.hour >= 24)
                {
                    realTime.hour = 0;
                    if(++realTime.week >= 7)
                    {
                        realTime.week = 0;
                    }
                }
            }
        }
    }
}

#define HALF_A_HOUR       30
void app_getNetRtc(realTime_t *_out_rtc)
{
    static uint16_t getNetRtcCyc = 0;
    static uint8_t  halfHourCnt = 0;
    uint16_t passSecond = 0;
    passSecond = GetSysTickMillisecond() - getNetRtcCyc;  
    if(ms_60000 <= passSecond)
    {
        getNetRtcCyc = GetSysTickMillisecond();
        halfHourCnt++;
        if(++halfHourCnt >= HALF_A_HOUR)
        {
            if(WIFI_CONN_CLOUD == app_pullTuyaWifiStateFormThisModule())
            {
                net_rtcTime_Def netRtc;
                netRtc = app_pullNetRtcTimeFromThisModule();
                if(true == netRtc.netRTC_isValid)
                { 
                    realTime.second = netRtc.netRTC_second;
                    realTime.hour = netRtc.netRTC_hour;
                    realTime.minute = netRtc.netRTC_minute;
                    realTime.week = netRtc.netRTC_week;               
                }
            }
        }       
    }
}

void app_softRtc_sacnTask(void)
{
    static uint16_t softRtcTimeOut = 0;
   switch(realTimeRunStatus)
   {
      case REAL_TIME_STATUS_INIT:
      {
          if(WIFI_CONN_CLOUD == app_pullTuyaWifiStateFormThisModule())
          {
              realTimeRunStatus = REAL_TIME_STATUS_CHECK_NET;
              softRtcTimeOut = GetSysTickMillisecond();
          }
          break;
      }
      case REAL_TIME_STATUS_CHECK_NET:
      {
          net_rtcTime_Def netRtc;
          netRtc = app_pullNetRtcTimeFromThisModule();
          if(true == netRtc.netRTC_isValid)
          {
              realTime.second = netRtc.netRTC_second;
              realTime.hour = netRtc.netRTC_hour;
              realTime.minute = netRtc.netRTC_minute;
              realTime.week = netRtc.netRTC_week;
              realTimeRunStatus = REAL_TIME_STATUS_CHECK_NET_RTC_VAILD;
          }
          else
          {
              realTimeRunStatus = REAL_TIME_STATUS_INIT;             
          }
          softRtcTimeOut = GetSysTickMillisecond();
          break;
      }
      case REAL_TIME_STATUS_CHECK_NET_RTC_VAILD:
      {
          net_rtcTime_Def netRtc;
          netRtc = app_pullNetRtcTimeFromThisModule();
          if(true == netRtc.netRTC_isValid)
          {
              if((realTime.hour != netRtc.netRTC_hour) || (realTime.minute != netRtc.netRTC_hour))
              {//网络时钟开始运作
                  realTimeRunStatus = REAL_TIME_STATUS_RUN;
              }
          }
          else
          {
              uint16_t passTime = 0;
              passTime = GetSysTickMillisecond() - softRtcTimeOut;
              if(ms_60000 <= passTime)
              {
                  realTimeRunStatus = REAL_TIME_STATUS_RUN;
              }
          }
          break;
      }
      case REAL_TIME_STATUS_RUN:
      {
          app_softRtc_handle();
          app_getNetRtc(&realTime);
          break;
      }
   }
}


bool app_real_time_getVaildStatus(void)
{
    if(REAL_TIME_STATUS_RUN == realTimeRunStatus)
    {
        return true;
    }
    return false;
}

realTime_t *app_real_time_getTime(void)
{
    return &realTime;
}
    
