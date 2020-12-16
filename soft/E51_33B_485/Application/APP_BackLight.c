/*******************************************************************************
*
*	模块名称 : APP背光模块
*	文件名称 : APP.Backlight.c
*	版    本 : V1.0
*	说    明 : 1、传出压入背光数据
*                  2、背光扫描
                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/

#include ".\APP_Configure.h"


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************模块使用的变量**************************************
static APP_BacklightParameter_Def APP_BL_Para;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************传出背光参数指针*************************************
APP_BacklightParameter_Def *APP_Backlight_PutOutParameter(void)
{
      return &APP_BL_Para;
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************压入背光参数指针*************************************
void APP_Backlight_PutInParameter(APP_BacklightParameter_Def *_In_me)
{
      APP_BL_Para.BacklightStatus = _In_me->BacklightStatus;
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************背光扫描******************************************
void APP_Backlight_ScanTask(void)
{
     APP_StorageParameter_Def *APP_BLSto_Para;
     static DelayTickClk_Def    BLDelayTime= {Type_Second,0,0}; 
     
     DelayTickClockRun(&BLDelayTime);
     APP_BLSto_Para = APP_Storage_PutOutParameter();
     
     switch(APP_BL_Para.BacklightStatus)
     {
            case BL_PowUp:
            {
                 ITF_Backlight_Configure();
                 APP_BL_Para.BacklightStatus = BL_On;
                 break;
            }
            case BL_On:
            {
                 ReloadDealyTickClock(&BLDelayTime,APP_BLSto_Para->BacklightTimeWorkTime*60);
                 APP_BL_Para.BacklightStatus = BL_WaitOFF;
                 ITF_Backlight_Enable();
                 break;
            }
            case BL_WaitOFF:
            {
                 if(APP_BLSto_Para->BacklightTimeWorkTime <= 60)
                 {
                       if(DelayTickClockFinished(&BLDelayTime))
                       {
                              ITF_Backlight_Disable();
                              APP_BL_Para.BacklightStatus = BL_OFF;
                       }
                 }
                 break;
            }
            default:
            {
                 break;
            }
     }
}
//------------------------------E N D-------------------------------------------


//------------------------APP.Backlight.C--END----------------------------------


