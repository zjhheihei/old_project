//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_BACKLIGHT_H
#define _MOD_BACKLIGHT_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\Depend\BSP_Backlight.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////汇总///////////////////////////////////////////
//映射BSP库中的函数，功能与BSP库中的一致，下方有个各个函数详解，用IDE搜索功能定位
///*背光配置 */   #define ITF_Backlight_Configure        BSP_Backlight_Configure
///*背光打开 */   #define ITF_Backlight_Enable           BSP_Backlight_Enable
///*背光关闭 */   #define ITF_Backlight_Disable          BSP_BSP_Backlight_Enable
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//名称: 背光配置, 
//功能: 映射BSP库中的文件，主要用于初始化
//入口: 无
//
//出口: 无
#define ITF_Backlight_Configure            BSP_Backlight_Configure
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//名称: 开背光
//功能: 映射BSP库中的文件，其他模块可调用
//入口: 无
//
//出口: 无
#define ITF_Backlight_Enable               BSP_Backlight_Enable
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//名称: 关背光
//功能: 映射BSP库中的文件，其他模块可调用
//入口: 无
//
//出口: 无
#define ITF_Backlight_Disable               BSP_Backlight_Disable
//------------------------------E N D-------------------------------------------

#endif
//-----------------------Mod_Backlight.h--END-----------------------------------
