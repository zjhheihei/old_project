//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_DISPLAY_H
#define _APP_DISPLAY_H
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************显示运行状态****************************************
typedef enum               //主运行模式
{
     DisSoftStartMode = 0x00,
     DisAutoMode      = 0x01,
     DisHandMode      = 0x02,
     DisShutDownMode  = 0x03,
     DisRTCSetMode    = 0x04,
     DisParaSetMode   = 0x05,
     DisDayProMode    = 0x06,         
}DisMainStatus_Def;



typedef struct                 //主模式事件变量
{
    BOOLEAN  SecondEvent ;      //秒针闪动
    INT8U    TempRtcEvent;      //温度和时钟轮流
    BOOLEAN  TimeSetEvent;      //时钟设定闪动
    INT16U   FanLeafTime ;      //扇叶时间
    BOOLEAN  OnceTrigger;       //单次触发刷新
    BOOLEAN  AutoNoFanFlag;     //自动无风标志
}DisMainFalsh_Def;



typedef struct                  
{
    DisMainStatus_Def  RunStatus;
    DisMainFalsh_Def   FalshFlag;
}DisMainPara_Def;

//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************软启动状态****************************************
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
//**************************时钟设置模块类型************************************
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
//***************************天编程块类型***************************************
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
//*************************高级设置模块类型*************************************
typedef enum
{
    CurrentTempDis      = 0x00,
    AdjTempDis          = 0x01,
    FanGearsSetDis      = 0x02, //风机档数设定
    CleanTimeSetDis     = 0x03, //清洁时间
    BackLightSetDis     = 0x04, //背光设置
    SlaveAddressDis     = 0x05, //从机地址
    BaudrateDis         = 0x06, //波特率
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
    FinishThirdStep      = 0x02, //风机档数设定
 
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
//***********************显示模块使用的变量**************************************
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
//名称: 传出显示模块参数首地址
//功能: 给其他模块需要显示参数的模块使用
//入口: 无

//出口:  APP_DisplayParameter_Def指针
APP_DisplayParameter_Def *APP_Display_PutOutParameter(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 压入显示模块参数
//功能: 供应其他模块修改显示模块参数
//入口: 结构体指针 *_In_me <<-----------------------------------传入

//出口: 无
void APP_Display_PutInPara(APP_DisplayParameter_Def *_In_me);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: 显示扫描模块
//功能: 用于刷新显示，主函定时数调用。
//入口: 无
//出口: 无
void APP_Display_ScanTask(void);
//------------------------------E N D-------------------------------------------


#endif



//-----------------------Mod_LCD.h--END------------------------------------
