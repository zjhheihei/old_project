/*******************************************************************************
*
*	ģ������ : ����TIM1_3ͬ��PWM���
*	�ļ����� : ExMain.c
*	��    �� : V1.0
*	˵    �� : �������Ƽ̵�������
*                  3�������ֱ����3���̵���������һ�������ر����м̵�����
*                  ��E51.33BӲ������֤ͨ����
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-9  zhoujh  ��ʽ����
*	Copyright (C), 2018-2022,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun)  
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
    ITF_Clock_Configure();                  //����MCUʱ��HSI_16M
    ITF_Systick_Configure();                //����ϵͳ����1ms
    
    while(1)
    {   
       SystickTimeRun();                        //ϵͳʱ������
       HSICalibration_Task();                   //HSI��������
       Mod_RelayPWMOutScanTask();               //PWMɨ������
       Mod_Key_ScanTask();                          //����ɨ������
       
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