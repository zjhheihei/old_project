/*******************************************************************************
*
*	模块名称 : 测试TIM1_3同到PWM输出
*	文件名称 : ExMain.c
*	版    本 : V1.0
*	说    明 : 按键控制继电器开关
*                  3个按键分别控制3个继电器，另外一个按键关闭所有继电器。
*                  在E51.33B硬件上验证通过。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-9  zhoujh  正式发布
*	Copyright (C), 2018-2022,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun)  
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_Relay\Src\Mod_RelayPWM.h"
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\..\Mod_Keyboard\Src\Mod_Keyboard.h"
//------------------------------E N D-------------------------------------------


#define KeyMenu        KeyAloneBit1
#define KeyAdd         KeyAloneBit2
#define KeyPower       KeyAloneBit0
#define KeyDEC         KeyAloneBit3


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    ITF_Clock_Configure();                  //配置MCU时钟HSI_16M
    ITF_Systick_Configure();                //配置系统脉动1ms
    
    while(1)
    {   
       SystickTimeRun();                        //系统时钟任务
       HSICalibration_Task();                   //HSI修正任务
       Mod_RelayPWMOutScanTask();               //PWM扫描任务
       Mod_Key_ScanTask();                          //按键扫描任务
       
       if(Mod_Key_MackReCaptureEventOperation(GetEvent,KeyNull))                                  
        {
              if((KeyPushDown)&Key_ValueOperation(KeyMenu,0xffff,0xffff))
              {
                  Mod_RealyLogicControl(RelayOne);
              }
              
              if((KeyPushDown)&Key_ValueOperation(KeyAdd,0xffff,0xffff))
              {
                  Mod_RealyLogicControl(RelayTwo);
              }
              
              if((KeyPushDown)&Key_ValueOperation(KeyDEC,0xffff,0xffff))
              {
                  Mod_RealyLogicControl(RelayThree);
              } 
              
              if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
              {
                  Mod_RealyLogicControl(RelayNone);
              }
        }
    }
}
//------------------------------E N D-------------------------------------------


//-----------------------ExMain.c--END------------------------------------------