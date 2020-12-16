//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_BACKLIGHT_H
#define _APP_BACKLIGHT_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************背光模块使用的变量**************************************
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
//名称: 传出北港模块参数首地址
//功能: 给其他需要背光参数的模块使用
//入口: 无

//出口:  APP_BacklightParameter_Def指针
APP_BacklightParameter_Def *APP_Backlight_PutOutParameter(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 压入背光模块参数
//功能: 供应其他模块修改背光模块参数
//入口: 结构体指针 *_In_me <<-----------------------------------传入

//出口: 无
void APP_Backlight_PutInParameter(APP_BacklightParameter_Def *_In_me);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: 背光扫描模块
//功能: 用于刷新背光，主函定时数调用。
//入口: 无
//出口: 无
void APP_Backlight_ScanTask(void);
//------------------------------E N D-------------------------------------------



#endif



//--------------------------APP.RTC.h--END--------------------------------------