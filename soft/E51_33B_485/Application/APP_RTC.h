//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_RTC_H
#define _APP_RTC_H
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************RTC模块使用的变量类型**********************************
typedef struct
{
    Mod_SD3078RTCParameter_Def   RTC;
    BOOLEAN                      RTCWriteEnable; 
}APP_RTC_Parameter_Def;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 传出RTC模块参数首地址
//功能: 给其他模块需要RTC参数的模块使用
//入口: 无

//出口:  APP_RTC_Parameter_Def 结构体指针
APP_RTC_Parameter_Def *APP_RTC_PutOutPara(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 压入RTC模块参数
//功能: 供应其他模块修改RTC模块参数
//入口: 结构体指针 *_In_me <<-----------------------------------传入

//出口:  无
void APP_RTC_PutInPara(APP_RTC_Parameter_Def *_In_me);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: RTC扫描模块
//功能: 用于读写RTC，主函定时数调用。
//入口: 无
//出口: 无
void APP_RTC_ScanTask(void);
//------------------------------E N D-------------------------------------------


#endif



//--------------------------APP.RTC.h--END--------------------------------------