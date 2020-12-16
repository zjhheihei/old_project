/*******************************************************************************
*
*	模块名称 : APP三速风机模块
*	文件名称 : APP.ThreeFanOut.c
*	版    本 : V1.0
*	说    明 : 1、
*                  2、
*                 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/


#include ".\APP_Configure.h"

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************三速风机模块参数**************************************
static APP_3FanParameter_Def  APP_3Fan_Para;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************传出三速风机模块参数***********************************
APP_3FanParameter_Def *APP_3Fan_PutOutParameter(void)
{
      return &APP_3Fan_Para;
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************传出三速风机模块参数***********************************
void APP_3Fan_PutInParameter(APP_3FanParameter_Def *_In_me)
{
     APP_3Fan_Para.FanSpeedStatus = _In_me->FanSpeedStatus;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************检查输出是否改变**************************************
BOOLEAN APP_ThreeFan_ParaCheck(void)
{
     static FanWorkSpeed_Def   KeepStatus;
     APP_DisplayParameter_Def  *APP_3FanDis_Para;
     APP_StorageParameter_Def  *APP_3FanSto_Para;
     APP_RTC_Parameter_Def     *APP_3FanRTC_Para;
     
     APP_3FanDis_Para = APP_Display_PutOutParameter();
     APP_3FanSto_Para = APP_Storage_PutOutParameter();
     APP_3FanRTC_Para = APP_RTC_PutOutPara();
     
     if(TwoSpeed == APP_3FanSto_Para->FanGearsSelect)
     {
           if(SpeedMid == APP_3FanSto_Para->FanHandWorkSpeed)
           {
                  APP_3FanSto_Para->FanHandWorkSpeed = SpeedHigh;   
                  APP_Storage_PutInPara();
           }
     }
     
     if(DisAutoMode == APP_3FanDis_Para->DisMainStatus.RunStatus)                      //自动状态
     {
          if((APP_3FanRTC_Para->RTC.Hour >= APP_3FanSto_Para->DayProPara.StartHour) &&
             (APP_3FanRTC_Para->RTC.Hour < APP_3FanSto_Para->DayProPara.StopHour))
          {
                APP_3Fan_Para.FanSpeedStatus = APP_3FanSto_Para->FanHandWorkSpeed;    //开风机时段内跟随手动风     
          }
          else
          {
                APP_3Fan_Para.FanSpeedStatus = SpeedNo;                               //其他时候关闭风
          }
     }
     else if(DisHandMode == APP_3FanDis_Para->DisMainStatus.RunStatus)                //手动风根据设定风速
     {
          APP_3Fan_Para.FanSpeedStatus = APP_3FanSto_Para->FanHandWorkSpeed;  
     }
     else
     {
          APP_3Fan_Para.FanSpeedStatus = SpeedNo;                                     //其他模式关闭输出
     }
     
     if(KeepStatus != APP_3Fan_Para.FanSpeedStatus)                                   
     {
            KeepStatus = APP_3Fan_Para.FanSpeedStatus;
            return BN_TRUE;
     }
     
     return BN_FALSE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************状态改变后进入修改输出**********************************
void APP_ThreeFan_Out(void)
{
     switch(APP_3Fan_Para.FanSpeedStatus)
     {
            case SpeedNo:
            {
                 Mod_RealyLogicControl(RelayNone);
                 break;
            }
            case SpeedLow:
            {
                 Mod_RealyLogicControl(RelayOne);
                 break;
            }
            case SpeedMid:
            {
                 Mod_RealyLogicControl(RelayThree);
                 break;
            }
            case SpeedHigh:
            {
                 Mod_RealyLogicControl(RelayTwo); 
                 break;
            }
     }
     
     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************滤网计时扫描任务*************************************
void APP_StrainerCheck_ScanTask(void)
{
     static DelayTickClk_Def          CheckTime = {Type_Minute,0,0};
     static APP_StorageParameter_Def  *APP_3FanSto_Para;
   
     DelayTickClockRun(&CheckTime);
         
     if(DelayTickClockFinished(&CheckTime))
     {
        ReloadDealyTickClock(&CheckTime,10);
        APP_3FanSto_Para = APP_Storage_PutOutParameter();          
        if(StatusChangStrainer & APP_3FanSto_Para->DeviceStatusBits)
        {
                  
        }
        else
        {
            if(SpeedNo != APP_3Fan_Para.FanSpeedStatus)
            {
                APP_3FanSto_Para->CleanPara.RunCleanMinite += 10;                            
                APP_Storage_PutInPara();
            }
                  
            if(APP_3FanSto_Para->CleanPara.RunCleanMinite >=\
              (APP_3FanSto_Para->CleanPara.SetCleanDay*1440))
            {
                APP_3FanSto_Para->DeviceStatusBits |= StatusChangStrainer;//更换滤网
            }                    
       }           
    }
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************3速风机扫描任务**************************************
void APP_3FanOut_ScanTask(void)
{  
     Mod_RelayPWMOutScanTask();
     
     switch(APP_3Fan_Para.FanTask)
     {
            case FanCheck:
            {
                 if(APP_ThreeFan_ParaCheck())
                 {
                     APP_3Fan_Para.FanTask = FanOut; 
                 }
                 break;
            }
            case FanOut:
            {
                 APP_ThreeFan_Out();
                 APP_3Fan_Para.FanTask = FanCheck; 
                 break;
            }
            default:
            {
                 APP_3Fan_Para.FanTask = FanCheck;
                 break;
            }
              
     }
     APP_StrainerCheck_ScanTask();
}
//------------------------------E N D-------------------------------------------



//-----------------------APP.ThreeFanOut.C--END---------------------------------


