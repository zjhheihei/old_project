
/*******************************************************************************
*
*	模块名称 : BSP背光模块
*	文件名称 : BSP_Backlight.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_Backlight调用
                   3、硬件配置。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-2  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*             
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//宏定义背光IO管脚寄存器宏定义


#define Backlight_Pin_ODR   PC_ODR_ODR6   
#define Backlight_Pin_DDR   PC_DDR_DDR6
#define Backlight_Pin_CR1   PC_CR1_C16
#define Backlight_Pin_CR2   PC_CR2_C26


//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//配置PG2口为推挽,初始化为低
void BSP_Backlight_Configure(void)
{
    Backlight_Pin_DDR = 1;
    Backlight_Pin_CR1 = 1; 
    Backlight_Pin_CR2 = 0;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//开背光
void BSP_Backlight_Enable(void)
{
    Backlight_Pin_ODR = 1;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//关背光
void BSP_Backlight_Disable(void)
{
    Backlight_Pin_ODR = 0;
}
//------------------------------E N D-------------------------------------------


//-----------------------BSP_Backlight.c--END-----------------------------------
