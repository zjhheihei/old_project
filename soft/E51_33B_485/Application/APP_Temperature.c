/*******************************************************************************
*
*	模块名称 : 温度模块
*	文件名称 : APP.Temperature.c
*	版    本 : V1.0
*	说    明 : 1、传出校温后的温度
*                  2、温度扫描任务
                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//------------------------------E N D-------------------------------------------

#include ".\APP_Configure.h"


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************温度变量******************************************
static INT16S APP_Temp_InsideTemp; 
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************传出温度******************************************
INT16S APP_Temp_GetTemperature(void)
{
       return APP_Temp_InsideTemp;
}
//------------------------------E N D-------------------------------------------

          

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************温度扫描******************************************
void APP_Temp_ScanTask(void)
{
     INT16S Temp = 0;
     static APP_StorageParameter_Def *APP_StoPara;
     
     Mod_NTC3380_ScanTask();     
     APP_StoPara = APP_Storage_PutOutParameter();
     
     Temp = Mod_NTC3380_GetTemperature(InsideSerson);
     if(0x7fff != Temp)
     {
          APP_Temp_InsideTemp = Temp + APP_StoPara->CalibrateTemperature;
          if(APP_Temp_InsideTemp > 999)
          {
                APP_Temp_InsideTemp = 999; 
          }
     }
     else
     {
          APP_Temp_InsideTemp = 0x7fff;
     }
}
//------------------------------E N D-------------------------------------------


//-----------------------APP_Temperature.c--END------------------------------------


