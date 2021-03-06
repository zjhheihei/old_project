/*******************************************************************************
*
*	模块名称 : 主模块
*	文件名称 : Main.c
*	版    本 : V1.0
*	说    明 : 扫描所有任务
*       
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-25  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************包含所有头文件***************************************
#include ".\APP_Configure.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************主任务*******************************************
void main(void)  
{
    ITF_IWDG_Configure();     //独立看门狗配置,硬件看门狗仍需配置
    ITF_Clock_Configure();    //主频系统时钟配置
    ITF_Systick_Configure();  //系统脉动时钟 
    ITF_LCD_Configure();      //LCD配置
    Mod_SD3078_Configure();   //时钟芯片配置
    APP_Sto_Configure();      //内置EEPROM配置
    
    while(1)
    {
       ITF_IWDG_FeedDog();        //喂狗512mS,main任务一轮含2400波特率通讯,一轮约16mS
        SystickTimeRun();          //脉动1mS
        HSICalibration_Task();     //TIM2捕捉时钟芯片频率，获取主时钟频率修正波特率
        APP_Backlight_ScanTask();  //背光扫描任务
        APP_RTC_ScanTask();        //RTC扫描任务
        APP_Key_ScanTask();        //按键扫描任务
        APP_Display_ScanTask();    //显示扫描任务
        APP_Temp_ScanTask();       //温度扫描任务
        APP_Sto_ScanTask();        //存储扫描任务
        APP_3FanOut_ScanTask();    //3速风机扫描任务
        APP_Rs485_ScanTask();      //485通信任务   
    }
}
//------------------------------E N D-------------------------------------------



//----------------------------Main.C--END---------------------------------------