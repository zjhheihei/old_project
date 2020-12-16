
/*******************************************************************************
*
*	模块名称 : 独立看门狗
*	文件名称 : BSP_IWDG.c
*	版    本 : V1.0
*	说    明 : 1、配置和喂狗函数
*                  
*                    
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-08-23  jinzh 正式发布
*	Copyright (C), 2015-2020,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.2      
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "DataTypedef.h"
#include "iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************配置看门狗512mS超时时间**********************************
void BSP_IWDG_Configure(void)
{
     IWDG_KR  = 0xcc;   //启动IWDG
     IWDG_KR  = 0x55;   //写入解锁
     
     /*16ms_E51.BB最小超时时间
     IWDG_PR  = 0x01;   //4分频率
     IWDG_RLR = 75;     //写入75 
     */
     /*实际采用512mS超时*/
     IWDG_PR  = 0x06;
     IWDG_RLR = 75;
     
     IWDG_KR  = 0xaa;   //锁定并刷新数据
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************************喂狗********************************************
void BSP_IWDG_FeedDog(void)
{
     IWDG_KR  = 0xaa;   //锁定并刷新数据 4mS喂狗
}
//------------------------------E N D-------------------------------------------