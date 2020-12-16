//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_BACKLIGHT_H
#define _APP_BACKLIGHT_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************����ģ��ʹ�õı���**************************************
typedef enum
{
    BL_PowUp   = 0x00,
    BL_On      = 0x01,
    BL_WaitOFF = 0x02,
    BL_OFF     = 0x03,
}BacklightStatus_Def; 

typedef struct
{
     BacklightStatus_Def   BacklightStatus;
}APP_BacklightParameter_Def;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//����: ��������ģ������׵�ַ
//����: ��������Ҫ���������ģ��ʹ��
//���: ��

//����:  APP_BacklightParameter_Defָ��
APP_BacklightParameter_Def *APP_Backlight_PutOutParameter(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//����: ѹ�뱳��ģ�����
//����: ��Ӧ����ģ���޸ı���ģ�����
//���: �ṹ��ָ�� *_In_me <<-----------------------------------����

//����: ��
void APP_Backlight_PutInParameter(APP_BacklightParameter_Def *_In_me);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//����: ����ɨ��ģ��
//����: ����ˢ�±��⣬������ʱ�����á�
//���: ��
//����: ��
void APP_Backlight_ScanTask(void);
//------------------------------E N D-------------------------------------------



#endif



//--------------------------APP.RTC.h--END--------------------------------------