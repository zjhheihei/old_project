//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_KEYBOARD_H
#define _BSP_KEYBOARD_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef  Data_Retyped
#include ".\DataTypedef.h"
#endif

#ifndef  _MOD_SYSTEMTICK_H
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#endif

//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************组合键宏定义配置，采用16为方式的高8位****************************
#define  KeyGroupBit0     0x0100   //组合按键
#define  KeyGroupBit1     0x0200   //组合按键
#define  KeyGroupBit2     0x0400   //组合按键
#define  KeyGroupBit3     0x0800   //组合按键
#define  NOCONTINUE       0xffff   //无组合连击
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************独立按键宏定义配置，采用16位方式的低8位**************************
#define  KeyNull          0x0000
#define  KeyAloneBit0     0x0001    
#define  KeyAloneBit1     0x0002  
#define  KeyAloneBit2     0x0004  
#define  KeyAloneBit3     0x0008  
#define  KeyAloneBit4     0x0010
#define  KeyAloneBit5     0x0020  
#define  KeyAloneBit6     0x0040  
#define  KeyAloneBit7     0x0080  
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//名称: 获取按键端口
//功能: 读取IO状态
//入口: 无

//出口: BN_TRUE代表有键按下

BOOLEAN BSP_GetKey0(void);
BOOLEAN BSP_GetKey1(void); 
BOOLEAN BSP_GetKey2(void); 
BOOLEAN BSP_GetKey3(void);
BOOLEAN BSP_GetKey4(void);
BOOLEAN BSP_GetKey5(void);
BOOLEAN BSP_GetKey6(void);       
BOOLEAN BSP_GetKey7(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//名称: 按键和组合键配置
//功能: 配置按键IO口和组合键，组合键采用指针返回，规避mod和bsp函数之间出现相互
//      调用
//入口: 无

//出口: INT16U* 的指针，该指针是存储组合键配置的首地址

INT16U* BSP_KeyHardWares_Configure(void);
//------------------------------E N D-------------------------------------------
#endif

//-----------------------BSP_Keyboard.h--END------------------------------------
