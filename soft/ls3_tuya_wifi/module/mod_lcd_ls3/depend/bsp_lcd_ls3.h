//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_LCD_LS3_H
#define _BSP_LCD_LS3_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef  __Snail_DataRetype__
#include ".\SnailDataTypedef.h"
#endif

#include "iostm8l052r8.h"
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#define MUC_RAM_ADDRESS    LCD_RAM0

//*************************LS3WIFI字符显示**************************************
//------------------------------E N D-------------------------------------------


//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//名称:  液晶模块初始化
//功能:  
//入口:  无
//出口:  无
void bsp_lcd_configure(void);
//------------------------------E N D-------------------------------------------
//void bsp_lcd_buf(INT8U *buf);

#endif




//-----------------------BSP_Keyboard.h--END------------------------------------
