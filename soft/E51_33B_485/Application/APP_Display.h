//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_DISPLAY_H
#define _APP_DISPLAY_H
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************��ʾ����״̬****************************************
typedef enum               //������ģʽ
{
     DisSoftStartMode = 0x00,
     DisAutoMode      = 0x01,
     DisHandMode      = 0x02,
     DisShutDownMode  = 0x03,
     DisRTCSetMode    = 0x04,
     DisParaSetMode   = 0x05,
     DisDayProMode    = 0x06,         
}DisMainStatus_Def;



typedef struct                 //��ģʽ�¼�����
{
    BOOLEAN  SecondEvent ;      //��������
    INT8U    TempRtcEvent;      //�¶Ⱥ�ʱ������
    BOOLEAN  TimeSetEvent;      //ʱ���趨����
    INT16U   FanLeafTime ;      //��Ҷʱ��
    BOOLEAN  OnceTrigger;       //���δ���ˢ��
    BOOLEAN  AutoNoFanFlag;     //�Զ��޷��־
}DisMainFalsh_Def;



typedef struct                  
{
    DisMainStatus_Def  RunStatus;
    DisMainFalsh_Def   FalshFlag;
}DisMainPara_Def;

//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************������״̬****************************************
typedef enum               
{
    DisALL      = 0x00,
    DisPV       = 0x01,
    DisWait     = 0x02,
    LeaveMode   = 0x03,
}DisSoftStartStatus_Def;
//------------------------------E N D-------------------------------------------

typedef struct
{
    DisSoftStartStatus_Def  RunStatus;
    
}DisSoftStartPara_Def;



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************ʱ������ģ������************************************
typedef enum
{
    InToRTCSetMode      = 0x00,
    SetMinite           = 0x01,
    SetHour             = 0x02,
    LeaveRTCSetMode     = 0x03,
    LeavedRTCSetMode    = 0x04,
}DisRTCSetStatus_Def;


typedef struct
{
    BOOLEAN  MiniteHour;
    BOOLEAN  Leave;
    BOOLEAN  SetTrigger;
    BOOLEAN  SelectTrigger;
}DisRTCFalshFlag_Def;


typedef struct
{
    DisRTCSetStatus_Def  SetStatus;
    DisRTCFalshFlag_Def  FalshFlag;
}DisRTCSetPara_Def;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************���̿�����***************************************
typedef enum
{
    SetStartHour    = 0x00,
    SetStopHour     = 0x01,
    LeaveDayProMode = 0x02,
}DisDayProSetStatus_Def;


typedef struct
{
    DisDayProSetStatus_Def  SetStatus;

}DisDayProSetPara_Def;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************�߼�����ģ������*************************************
typedef enum
{
    CurrentTempDis      = 0x00,
    AdjTempDis          = 0x01,
    FanGearsSetDis      = 0x02, //��������趨
    CleanTimeSetDis     = 0x03, //���ʱ��
    BackLightSetDis     = 0x04, //��������
    SlaveAddressDis     = 0x05, //�ӻ���ַ
    BaudrateDis         = 0x06, //������
    ParityDis           = 0x07,
    StopBitsDis         = 0x08,
    DayProSelectDis     = 0x09,
    FacWaitDis          = 0x0a,
    FacFinishDis        = 0x0b,   
}DisParaSetStatus_Def; 


typedef enum
{
    FinishFristStep      = 0x00,
    FinishSecondStep     = 0x01,
    FinishThirdStep      = 0x02, //��������趨
 
}FacFinishStep_Def; 


typedef struct
{
    FacFinishStep_Def   FacFinishStep;
    BOOLEAN             GoBackToCurrentTempFlag;
}DisParaFalshFlag_Def;


typedef struct
{
    DisParaSetStatus_Def  SetStatus;
    DisParaFalshFlag_Def  FalshFlag;
}DisParaSetPara_Def;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************��ʾģ��ʹ�õı���**************************************
typedef struct
{
    DisMainPara_Def             DisMainStatus;
    DisRTCSetPara_Def           DisRTCSetPara;
    DisParaSetPara_Def          DisParaSetPara;
    DisDayProSetPara_Def        DisDayProSetPara;
    DisSoftStartPara_Def        DisSoftStartPara;
}APP_DisplayParameter_Def;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//����: ������ʾģ������׵�ַ
//����: ������ģ����Ҫ��ʾ������ģ��ʹ��
//���: ��

//����:  APP_DisplayParameter_Defָ��
APP_DisplayParameter_Def *APP_Display_PutOutParameter(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//����: ѹ����ʾģ�����
//����: ��Ӧ����ģ���޸���ʾģ�����
//���: �ṹ��ָ�� *_In_me <<-----------------------------------����

//����: ��
void APP_Display_PutInPara(APP_DisplayParameter_Def *_In_me);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//����: ��ʾɨ��ģ��
//����: ����ˢ����ʾ��������ʱ�����á�
//���: ��
//����: ��
void APP_Display_ScanTask(void);
//------------------------------E N D-------------------------------------------


#endif



//-----------------------Mod_LCD.h--END------------------------------------
