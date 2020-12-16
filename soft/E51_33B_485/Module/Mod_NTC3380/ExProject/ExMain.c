/*******************************************************************************
*
*	ģ������ : 12λADCת���¶�
*	�ļ����� : Ex_Main.c
*	��    �� : V1.0
*	˵    �� : 1��ȡ�¶�,View�˵�ѡ��Live watch ���Temp��
*                 ��ϵ�����鿴Tempֵ�Ƿ���ȷ��
*                 �Ѿ���E51.33BӲ������֤ͨ����
* 
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-08-02  jinzh ��ʽ����
*	Copyright (C), 2015-2020,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun) 
*       MCU      ��STM8L052R8
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_NTC3380\Src\Mod_NTC3380.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
//------------------------------E N D-------------------------------------------
 static INT16S Temp = 0;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    //ITF_Clock_Configure();
    ITF_Systick_Configure();
    while(1)
    {
        SystickTimeRun();
        //HSICalibration_Task();      
        Temp = Mod_NTC3380_GetTemperature(InsideSerson);
        Mod_NTC3380_ScanTask();
        
    }
    
}
//------------------------------E N D-------------------------------------------

//--------------------------Backlight.c--END------------------------------------