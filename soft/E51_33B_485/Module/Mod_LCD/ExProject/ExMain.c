/*******************************************************************************
*
*	ģ������ : �ӻ�Modbus��ʾ
*	�ļ����� : ExMain.c
*	��    �� : V1.0
*	˵    �� : ��ʾMod_Modbus�Ľӿں���ʹ�ã�Ҫʹ��Mod����APP����Ҫ����
*                  �ı����ͺ�����
*                  ��������1402��Ӳ���ϲ���ͨ����
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-18  zhoujh  ��ʽ����
*	Copyright (C), 2018-2022,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun) 
*       MCU      ��STM8L052R8
*******************************************************************************/



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************������ͷ�ļ�***************************************
#include "..\..\Mod_Keyboard\Src\Mod_Keyboard.h"
#include "..\..\Mod_LCD\Src\Mod_LCD.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\..\Mod_Backlight\Src\Mod_Backlight.h"
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_SD3078\Src\Mod_SD3078.h"
//------------------------------E N D-------------------------------------------



#define KeyMenu        KeyAloneBit1
#define KeyAdd         KeyAloneBit2
#define KeyPower       KeyAloneBit0
#define KeyDEC         KeyAloneBit3

typedef enum
{
    ParameterSet  = 0x00,
    TimeSet       = 0x01,
    ShutDown      = 0x02,
    Hand          = 0x03,
    Auto          = 0x04,
}DisTaskStatus_Def;


typedef struct
{
    DisTaskStatus_Def           TaskStatus;     
    //DisTimeSetStatus_Def        TimeSetStatus;
    //DisParameterSetStatus_Def   ParameterSetStatus;
    //BOOLEAN                     TimeSetFlashEvent;
}App_ParameterDis_Def;

App_ParameterDis_Def App_ParameterDis;
void App_LCD_ScanTask(void)
{
      static MilliscondTickClk_Def LCD_RefreshTime;
      static MilliscondTickClk_Def App_LCD_FanSpeedRefreshTime;
      MillisecondTickClockRun(&LCD_RefreshTime);
      MillisecondTickClockRun(&App_LCD_FanSpeedRefreshTime);
        
      if(GetMillisecondTickClockFninshed(&LCD_RefreshTime))
      {
                     
            ReloadMillisecondTickClock(&LCD_RefreshTime,500);
            Mod_LCD_RefreshGroupTask();          
      }
       
      if(GetMillisecondTickClockFninshed(&App_LCD_FanSpeedRefreshTime))
      {
            ReloadMillisecondTickClock(&App_LCD_FanSpeedRefreshTime,150);
           // if((Hand == App_ParameterDis.TaskStatus)||(Auto == App_ParameterDis.TaskStatus))//�Զ����ֶ�״̬ˢ����Ҷ
            {
                  Mod_LCD_RefreshFanSpeedTask();
            }
      }
}
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************������ͷ�ļ�***************************************
void main(void)
{
    static DelayTickClk_Def LCDFalshTime={Type_Second,0,0};
    static INT32U i; 
    static INT8U Buff[4];
    ITF_Clock_Configure();
    ITF_Systick_Configure();
    ITF_Backlight_Configure();
    ITF_Backlight_Enable();
    Mod_SD3078_Configure();
    
    
    ITF_LCD_Configure();
    Mod_LCD_SetAll(BN_TRUE);
    Mod_LCD_RefreshGroupTask();
    while(i<100000)
    {
        i++;
    }
    i = 0;
    
    //Mod_LCD_SetAll(BN_FALSE);
    //Mod_LCD_RefreshGroupTask();
    
    while(1)
    {
          SystickTimeRun(); 
          DelayTickClockRun(&LCDFalshTime);
          Buff[0] =  i;
          Buff[1] =  i;
          Buff[2] =  i;
          Buff[3] =  i;
          
          Mod_LCD_UpDateDigtal(Buff,DisDigtal);
          
          if(DelayTickClockFinished(&LCDFalshTime))
          {
                 ReloadDealyTickClock(&LCDFalshTime,1);
                 i++;
                 if(i>9)i=0;
                 Mod_LCD_RefreshGroupTask(); 
          }
    }
}
//------------------------------E N D-------------------------------------------


//End++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++