/*******************************************************************************
*
*	ģ������ : SD3078������ʱģ��
*	�ļ����� : ExMain.c
*	��    �� : V1.0
*	˵    �� : ��ʾSD3078��д�ͳ�ʼ����ʹ�á�
*                  ���ģ���32kHz��������ñ���߼������ǲ�����
                   ʵ��77�����ܳ嵽3V���������Ϊ32.7KHz��������
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-9  zhoujh  ��ʽ����
*	Copyright (C), 2018-2022,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun)  
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
    ITF_Clock_Configure();                  //����MCUʱ��HSI_16M
    ITF_Systick_Configure();                //����ϵͳ����1ms
    Mod_SD3078_Configure();
    
    while(i<60000)
    {
        i++;
          
    }
    App_SD3078WriteDate();
    while(1)
    {   
       SystickTimeRun();                    //ϵͳʱ������
       HSICalibration_Task();               //HSI��������
       
       if(++i>60000)
       {
        i=0;
        App_SD3078ReadDate();  
       }
       
       
    }
}
//------------------------------E N D-------------------------------------------


//-----------------------ExMain.c--END------------------------------------------