/*******************************************************************************
*
*	模块名称 : APP时钟模块
*	文件名称 : APP.RTC.c
*	版    本 : V1.0
*	说    明 : 1、传出压入RTC数据，压入时直接修改写使能
*                  2、定时读RTC功能
                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/

#include ".\APP_Configure.h"


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************模块使用的变量**************************************
static APP_RTC_Parameter_Def APP_RTC_Para;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************压入RTC参数*****************************************
void APP_RTC_PutInPara(APP_RTC_Parameter_Def *_In_me)
{
     APP_RTC_Para.RTC.Second     = _In_me->RTC.Second;
     APP_RTC_Para.RTC.Minite     = _In_me->RTC.Minite;
     APP_RTC_Para.RTC.Hour       = _In_me->RTC.Hour;
     APP_RTC_Para.RTC.Month      = _In_me->RTC.Month;
     APP_RTC_Para.RTC.Week       = _In_me->RTC.Week;
     APP_RTC_Para.RTC.Day        = _In_me->RTC.Day;
     APP_RTC_Para.RTC.Year       = _In_me->RTC.Year; 
     APP_RTC_Para.RTCWriteEnable = _In_me->RTCWriteEnable;
}
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************传出RTC参数首地址*************************************
APP_RTC_Parameter_Def *APP_RTC_PutOutPara(void)
{
      return &APP_RTC_Para;   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************RTC扫描函数****************************************
void APP_RTC_ScanTask(void)
{
     static DelayTickClk_Def   App_ReadRTCTime ={Type_Second,0,0};
     APP_DisplayParameter_Def  *APP_RTCDis_Para;
     
     DelayTickClockRun(&App_ReadRTCTime);
     
     
     if(APP_RTC_Para.RTCWriteEnable)
     {       
             Mod_SD3078_WriteRTC(&APP_RTC_Para.RTC);
             APP_RTC_Para.RTCWriteEnable = BN_FALSE;
     }
     
     
     if(DelayTickClockFinished(&App_ReadRTCTime))
     {
            ReloadDealyTickClock(&App_ReadRTCTime,20);
            APP_RTCDis_Para = APP_Display_PutOutParameter();
            if(DisRTCSetMode != APP_RTCDis_Para->DisMainStatus.RunStatus)
            {
                    Mod_SD3078_ReadRTC(&APP_RTC_Para.RTC);
            }
     }
}
//------------------------------E N D-------------------------------------------



//--------------------------APP.RTC.C--END--------------------------------------


