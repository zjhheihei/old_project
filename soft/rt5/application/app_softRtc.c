#include ".\APP_Cfg.h"



typedef enum
{
    REAL_TIME_MAIN_STATUS_INIT = ((uint8_t)0x00), 
    REAL_TIME_MAIN_STATUS_RUN,
    REAL_TIME_MAIN_STATUS_WRITE_LOCAL,
}realTimeMainStatus_t;

#define MINUTE_30            90
#define MINUTE_1             3
static  bool                 rtcWriteFlag     = false;
static  uint16_t             writeDelayTime   = 0;
static  uint8_t              getNetRtcCyc_fix = MINUTE_1;
static  realTimeMainStatus_t mainStatus       = REAL_TIME_MAIN_STATUS_INIT;
static realTime_t realTime = 
{
    .hour   = 8,
    .minute = 0,
    .week   = 4,
    .second = 0,
};

static uint8_t local_rtc[7];

void read_loacl_rtc(void)
{
    mod_real_time_read();
    realTime.second = local_rtc[0];                   
    realTime.minute = local_rtc[1];
    realTime.hour   = local_rtc[2];
    realTime.week   = local_rtc[3];
}

void write_loacl_rtc(void)
{
    local_rtc[0] = realTime.second;
    local_rtc[1] = realTime.minute;
    local_rtc[2] = realTime.hour;
    local_rtc[3] = realTime.week;
    mod_real_time_write();
}

void write_delay(void)
{
    rtcWriteFlag = true;
    writeDelayTime = GetSysTickMillisecond();
}

static bool read_enable(void)
{
    SysPara_t *para;  
    para =  controler_getSysParaPt();
    if(SYS_STATUS_RUN_MODE == para->record.sysRunStatus)
    {
        return true;
    }
    return false;
}




void app_rtc_task(void)
{
   static uint16_t getLocalRtcCyc = 0;
   static uint8_t  getNetRtcCyc = 3;  
          uint16_t passTime = 0;
   passTime = GetSysTickMillisecond() - getLocalRtcCyc;        
   switch(mainStatus)
   {
      case REAL_TIME_MAIN_STATUS_INIT:
      {         
          mod_real_time_configure(&local_rtc[0]);
          write_loacl_rtc();
          mainStatus = REAL_TIME_MAIN_STATUS_RUN;                            
          break;
      }
      case REAL_TIME_MAIN_STATUS_RUN:
      {
          if(ms_20000 < passTime)
          {
              getLocalRtcCyc = GetSysTickMillisecond();
              read_loacl_rtc();
              if(WIFI_CONN_CLOUD == app_pullTuyaWifiStateFormThisModule())
              {
                  if(++getNetRtcCyc >= getNetRtcCyc_fix)
                  {
                      getNetRtcCyc = 0;                      
                      if((read_enable()) && !rtcWriteFlag) 
                      {                                                    
                          net_rtcTime_Def netRtc;
                          netRtc = app_pullNetRtcTimeFromThisModule();
                          if(true == netRtc.netRTC_isValid)
                          {
                              if((0x04 == netRtc.netRTC_week) && (0x08 == netRtc.netRTC_hour) && (0x00 == netRtc.netRTC_minute))
                              {
                                  getNetRtcCyc_fix = 1;
                              }
                              else
                              {                                                            
                                  if(getNetRtcCyc_fix < MINUTE_30)
                                  {
                                      getNetRtcCyc_fix ++;
                                  }
                                  realTime.second = netRtc.netRTC_second;
                                  realTime.hour   = netRtc.netRTC_hour;
                                  realTime.minute = netRtc.netRTC_minute;
                                  realTime.week = netRtc.netRTC_week; 
                                  write_loacl_rtc();
                                  write_delay();
                              }
                          }                     
                      }                      
                  }
              }
          }          
          break;
      }     
   }
   
   if(rtcWriteFlag)
   {
      uint16_t delayTime = 0;
      delayTime = GetSysTickMillisecond() - writeDelayTime; 
      getLocalRtcCyc = GetSysTickMillisecond();
      if(ms_2000 < delayTime)
      {
          rtcWriteFlag = false;
          write_loacl_rtc();
      }
   }
}

realTime_t *app_real_time_getTime(void)
{
    return &realTime;
}
  
void app_real_time_write_rtc(void)
{
    rtcWriteFlag = true;
    writeDelayTime = GetSysTickMillisecond();
}


