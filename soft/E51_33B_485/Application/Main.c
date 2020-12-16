/*******************************************************************************
*
*	ģ������ : ��ģ��
*	�ļ����� : Main.c
*	��    �� : V1.0
*	˵    �� : ɨ����������
*       
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2018-08-25  zhoujh  ��ʽ����
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************��������ͷ�ļ�***************************************
#include ".\APP_Configure.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************������*******************************************
void main(void)  
{
    ITF_IWDG_Configure();     //�������Ź�����,Ӳ�����Ź���������
    ITF_Clock_Configure();    //��Ƶϵͳʱ������
    ITF_Systick_Configure();  //ϵͳ����ʱ�� 
    ITF_LCD_Configure();      //LCD����
    Mod_SD3078_Configure();   //ʱ��оƬ����
    APP_Sto_Configure();      //����EEPROM����
    
    while(1)
    {
       ITF_IWDG_FeedDog();        //ι��512mS,main����һ�ֺ�2400������ͨѶ,һ��Լ16mS
        SystickTimeRun();          //����1mS
        HSICalibration_Task();     //TIM2��׽ʱ��оƬƵ�ʣ���ȡ��ʱ��Ƶ������������
        APP_Backlight_ScanTask();  //����ɨ������
        APP_RTC_ScanTask();        //RTCɨ������
        APP_Key_ScanTask();        //����ɨ������
        APP_Display_ScanTask();    //��ʾɨ������
        APP_Temp_ScanTask();       //�¶�ɨ������
        APP_Sto_ScanTask();        //�洢ɨ������
        APP_3FanOut_ScanTask();    //3�ٷ��ɨ������
        APP_Rs485_ScanTask();      //485ͨ������   
    }
}
//------------------------------E N D-------------------------------------------



//----------------------------Main.C--END---------------------------------------