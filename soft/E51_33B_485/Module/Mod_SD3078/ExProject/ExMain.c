/*******************************************************************************
*
*	模块名称 : SD3078程序延时模块
*	文件名称 : ExMain.c
*	版    本 : V1.0
*	说    明 : 演示SD3078读写和初始配置使用。
*                  充电模块和32kHz输出用万用表和逻辑分析仪测量。
                   实测77分钟能冲到3V，输出方波为32.7KHz（有误差）。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-9  zhoujh  正式发布
*	Copyright (C), 2018-2022,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun)  
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_SD3078\Src\Mod_SD3078.h"
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
//------------------------------E N D-------------------------------------------




 

Mod_SD3078RTCParameter_Def Mod_SD3078RTCParameter; 


void App_SD3078WriteDate(void)
{
     Mod_SD3078RTCParameter.Second = 0x30;
     Mod_SD3078RTCParameter.Minite = 0x30;
     Mod_SD3078RTCParameter.Hour   = 0x01;
     Mod_SD3078RTCParameter.Day    = 0x01;
     Mod_SD3078RTCParameter.Week   = 0x01;
     Mod_SD3078RTCParameter.Month  = 0x09;
     Mod_SD3078RTCParameter.Year   = 0x99;
     Mod_SD3078_WriteRTC(&Mod_SD3078RTCParameter);
}



void App_SD3078ReadDate(void)
{
     Mod_SD3078_ReadRTC(&Mod_SD3078RTCParameter);
}


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    INT32U i = 0;
    ITF_Clock_Configure();                  //配置MCU时钟HSI_16M
    ITF_Systick_Configure();                //配置系统脉动1ms
    Mod_SD3078_Configure();
    
    while(i<60000)
    {
        i++;
          
    }
    App_SD3078WriteDate();
    while(1)
    {   
       SystickTimeRun();                    //系统时钟任务
       HSICalibration_Task();               //HSI修正任务
       
       if(++i>60000)
       {
        i=0;
        App_SD3078ReadDate();  
       }
       
       
    }
}
//------------------------------E N D-------------------------------------------


//-----------------------ExMain.c--END------------------------------------------