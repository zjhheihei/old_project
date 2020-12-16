/******************************************************************************
*
*	ģ������ : �����ϵͳʱ��������ʾģ��
*	�ļ����� : ExMain.c
*	��    �� : V1.0
*	˵    �� : �ϵ粻����ʱ�ӣ�����Ĭ�ϵ��ڲ�16/8=2M������
                   ������1�������,��֤����ģ�顣
                   ��BELL3Ӳ������֤ͨ����
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-6  zhoujh  ��ʽ����
*	Copyright (C), 2018-2022,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4��EWSTM8-2104-Autorun��     
*******************************************************************************/



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_Backlight\Src\Mod_Backlight.h"
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//�궨��
#define Delay1second   20000 
#define Opend          0xff
#define Closed         0x00
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    
    static unsigned long BacklightDelayTime = 0;  //������ʱʱ��
    static unsigned char BacklightStatus    = 0;  //����״̬
    ITF_Backlight_Configure();                     //����Ӳ������
    
    while(1)
    {
        if(++BacklightDelayTime>Delay1second)     //��ʱʱ�䵽
        {
             BacklightDelayTime = 0;              //������ʱʱ��
             if(Closed == BacklightStatus)        //�ж�״̬���ر��Ⲣ��¼״̬
             {
                  ITF_Backlight_Enable();
                  BacklightStatus = Opend;
             }
             else if(Opend == BacklightStatus)
             {
                  ITF_Backlight_Disable();
                  BacklightStatus = Closed;
                  
                  
             }
        }
       
    }    
   
}
//------------------------------E N D-------------------------------------------

//--------------------------Backlight.c--END------------------------------------
