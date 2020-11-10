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

#define HALF_A_HOUR   1800
static uint16_t getNetRtcCnt = 360;
void app_softRtc_getNetTime(rtc_t *_out_rtc)
{
    static uint16_t getNetRtcBlinkTime = 1795;
    if(++getNetRtcBlinkTime  >= HALF_A_HOUR)
    {   
        if(getNetRtcCnt > 0)
        {
            getNetRtcCnt--; 
        }       
        getNetRtcBlinkTime = getNetRtcCnt * 5;
        net_rtcTime_Def netRtc;
        netRtc = app_pullNetRtcTimeFromThisModule();
        if(true == netRtc.netRTC_isValid)
        {
             _out_rtc->second = 0;
             _out_rtc->minute = netRtc.netRTC_minute;
             _out_rtc->hour = netRtc.netRTC_hour;
             _out_rtc->week = netRtc.netRTC_week;
        }
        else
        {
             getNetRtcBlinkTime = 1795;
        }
    }         
}

void app_softRtc_scanTask(void)
{
    static uint16_t lastRtcTick = 0;
           uint16_t diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastRtcTick;
    if(ms_1000 <= diffTick)
    {
        SysPara_t *ptSysPara;
        ptSysPara = controler_getSysParaPt();
        lastRtcTick = GetSysTickMillisecond();
               
        if(SYS_STATUS_RTCSET != ptSysPara->record.sysRunStatus)
        {
            if(WIFI_CONN_CLOUD == app_pullTuyaWifiStateFormThisModule())
            {
                app_softRtc_getNetTime(&ptSysPara->rtc);
            }
            else
            {
                getNetRtcCnt = 360;
            }
            
            if(++ptSysPara->rtc.second > MAX_SECOND)
            {
                ptSysPara->rtc.second = MIN_SECOND;
                if(++ptSysPara->rtc.minute > MAX_MINUTE)
                {
                    ptSysPara->rtc.minute = MIN_MINUTE;
                    if(++ptSysPara->rtc.hour > MAX_HOUR)
                    {
                        ptSysPara->rtc.hour = MIN_HOUR;
                        if(++ptSysPara->rtc.week > MAX_WEEK)
                        {
                            ptSysPara->rtc.week = MIN_WEEK; 
                        }
                    }
                }
            }
        }
        else
        {
            ptSysPara->rtc.second = MIN_SECOND;
        }
    }
}
    
//-----------------------APP.ThreeFanOut.C--END---------------------------------


