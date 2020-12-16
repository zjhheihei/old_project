//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_NTC3380_H
#define _MOD_NTC3380_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\Depend\BSP_NTC3380.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************获取内外传感器温度值************************************   
typedef enum
{
    InsideSerson  = 0x00,
    OutsideSerson = 0x01,
}Mod_SersonSelect_Def;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称:  返回温度值
//功能:  
//入口:  _In_SersonSelect   <<-------------------------传入
//       E51.33B只有内置传感器InsideSerson
//
//出口:  返回INT16S值，开短路返回0x7fff;
INT16S Mod_NTC3380_GetTemperature(Mod_SersonSelect_Def _In_SersonSelect);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称:  温度扫描模块
//功能:  开启ADC采集、温度转换和采集超时保护。
//入口:  无
//出口:  无
void Mod_NTC3380_ScanTask(void);
//------------------------------E N D-------------------------------------------


#endif
//-----------------------Mod_ADC_NTC.h--END-----------------------------------
