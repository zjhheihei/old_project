//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_LCD_H
#define _BSP_LCD_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef  Data_Retyped
#include ".\DataTypedef.h"
#endif
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "iostm8l052r8.h"
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************LCD编程小时0-7宏定义************************************
#define Dis_0Clock_Ram           LCD_RAM2_S016
#define Dis_1Clock_Ram           LCD_RAM1_S009 
#define Dis_2Clock_Ram           LCD_RAM1_S010
#define Dis_3Clock_Ram           LCD_RAM4_S110
#define Dis_4Clock_Ram           LCD_RAM8_S210
#define Dis_5Clock_Ram           LCD_RAM11_S310
#define Dis_6Clock_Ram           LCD_RAM11_S311
#define Dis_7Clock_Ram           LCD_RAM8_S211

//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************LCD编程小时8-15宏定义************************************
#define Clock8_15                1
#define Dis_8Clock_Ram           LCD_RAM4_S111
#define Dis_9Clock_Ram           LCD_RAM1_S011
#define Dis_10Clock_Ram          LCD_RAM0_S007
#define Dis_11Clock_Ram          LCD_RAM4_S107
#define Dis_12Clock_Ram          LCD_RAM7_S207
#define Dis_13Clock_Ram          LCD_RAM11_S307
#define Dis_14Clock_Ram          LCD_RAM11_S306
#define Dis_15Clock_Ram          LCD_RAM7_S206
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************LCD编程小时16-23宏定义**********************************
#define Dis_16Clock_Ram          LCD_RAM4_S106
#define Dis_17Clock_Ram          LCD_RAM0_S006
#define Dis_18Clock_Ram          LCD_RAM0_S005 
#define Dis_19Clock_Ram          LCD_RAM4_S105
#define Dis_20Clock_Ram          LCD_RAM7_S205
#define Dis_21Clock_Ram          LCD_RAM11_S305
#define Dis_22Clock_Ram          LCD_RAM13_S321
#define Dis_23Clock_Ram          LCD_RAM9_S221
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_24Clock_Ram          LCD_RAM6_S121
#define Dis_Hour_Ram             LCD_RAM5_S116
//*************************小数、秒针和℃***************************************
#define Dis_Point_Ram            LCD_RAM9_S216
#define Dis_Col_Ram              LCD_RAM12_S316
#define Dis_TempSymbol_P_Ram     LCD_RAM21_S336

//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_AutoWork_Ram          LCD_RAM12_S315
#define Dis_HandWork_Ram          LCD_RAM12_S313
#define Dis_Clean_Ram             LCD_RAM21_S338
#define Dis_SumClock_Ram          LCD_RAM11_S309
#define Dis_Outside_Ram           LCD_RAM8_S209
#define Dis_RoomTemp_Ram          LCD_RAM4_S109
#define Dis_Communication_Ram     LCD_RAM19_S239
#define Dis_RH_Ram                LCD_RAM17_S139
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_FanLow_Ram           LCD_RAM15_S039
#define Dis_FanMid_Ram           LCD_RAM2_S020
#define Dis_FanHigh_Ram          LCD_RAM2_S021
#define Dis_FanSpeed_A_Ram       LCD_RAM9_S220
#define Dis_FanSpeed_B_Ram       LCD_RAM13_S320
#define Dis_FanSpeed_C_Ram       LCD_RAM6_S120
#define Dis_FanRoom_Ram          LCD_RAM21_S339

//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_Dig1_G_Ram           LCD_RAM5_S113
#define Dis_Dig1_F_Ram           LCD_RAM8_S212
#define Dis_Dig1_E_Ram           LCD_RAM5_S112
#define Dis_Dig1_D_Ram           LCD_RAM1_S012
#define Dis_Dig1_C_Ram           LCD_RAM1_S013
#define Dis_Dig1_B_Ram           LCD_RAM8_S213
#define Dis_Dig1_A_Ram           LCD_RAM12_S312

//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_Dig2_G_Ram           LCD_RAM5_S115
#define Dis_Dig2_F_Ram           LCD_RAM8_S214
#define Dis_Dig2_E_Ram           LCD_RAM5_S114
#define Dis_Dig2_D_Ram           LCD_RAM1_S014
#define Dis_Dig2_C_Ram           LCD_RAM1_S015
#define Dis_Dig2_B_Ram           LCD_RAM8_S215
#define Dis_Dig2_A_Ram           LCD_RAM12_S314

//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_Dig3_G_Ram           LCD_RAM17_S136
#define Dis_Dig3_F_Ram           LCD_RAM9_S217
#define Dis_Dig3_E_Ram           LCD_RAM5_S117
#define Dis_Dig3_D_Ram           LCD_RAM2_S017
#define Dis_Dig3_C_Ram           LCD_RAM15_S036
#define Dis_Dig3_B_Ram           LCD_RAM19_S236
#define Dis_Dig3_A_Ram           LCD_RAM12_S317

//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************LCD编程小时24和小时标识宏定义*******************************
#define Dis_Dig4_G_Ram           LCD_RAM17_S138
#define Dis_Dig4_F_Ram           LCD_RAM19_S237
#define Dis_Dig4_E_Ram           LCD_RAM17_S137
#define Dis_Dig4_D_Ram           LCD_RAM15_S037
#define Dis_Dig4_C_Ram           LCD_RAM15_S038
#define Dis_Dig4_B_Ram           LCD_RAM19_S238
#define Dis_Dig4_A_Ram           LCD_RAM21_S337

//------------------------------E N D-------------------------------------------


void BSP_LCD_DisClrAll(void);
void BSP_LCD_DisSetAll(void);
void BSP_LCD_Configure(void);

#endif




//-----------------------BSP_Keyboard.h--END------------------------------------
