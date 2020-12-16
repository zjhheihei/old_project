/*******************************************************************************
*
*	模块名称 : 12位ADC转换温度
*	文件名称 : Ex_Main.c
*	版    本 : V1.0
*	说    明 : 1、取温度,View菜单选择Live watch 添加Temp，
*                 配合电阻箱查看Temp值是否正确。
*                 已经在E51.33B硬件上验证通过。
* 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-08-02  jinzh 正式发布
*	Copyright (C), 2015-2020,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun) 
*       MCU      ：STM8L052R8
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_NTC3380\Src\Mod_NTC3380.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
//------------------------------E N D-------------------------------------------
 static INT16S Temp = 0;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    //ITF_Clock_Configure();
    ITF_Systick_Configure();
    while(1)
    {
        SystickTimeRun();
        //HSICalibration_Task();      
        Temp = Mod_NTC3380_GetTemperature(InsideSerson);
        Mod_NTC3380_ScanTask();
        
    }
    
}
//------------------------------E N D-------------------------------------------

//--------------------------Backlight.c--END------------------------------------