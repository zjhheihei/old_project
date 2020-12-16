//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_KEYBOARD_H
#define _MOD_KEYBOARD_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\Depend\BSP_Keyboard.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************按键状态类型****************************************
typedef enum
{
    KeyNonePush       = 0x00,
    KeyPushDown       = 0x01,
    KeyPushUp         = 0x02,
    KeyContinueDown   = 0x04,
}KeyRetValue_Def;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************按键状态类型****************************************
typedef enum
{  
    MaskOnce  = 0x01,
    ReCapture = 0X02,
}KeyOperation_Def;
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Event
//名称: 按键屏蔽重读获取事件组合函数
//功能: 可以执行屏蔽一次按键、重读一个按键一次和获取按键事件
//入口: 无

//出口:有按键状态发生改变返回BN_TURE,其他情况返回BN_FALSE  
BOOLEAN Mod_Key_GetEvent(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 按键屏蔽重读获取事件组合函数
//功能: 可以执行屏蔽一次按键、重读一个按键一次和获取按键事件
//入口: 
//想要执行的操作          _In_KeyOperation  <<------------------------------传入
//                     屏蔽    MackOnce
//                     重读    ReCapture
//
//具体按键值              _In_KeyInBit      <<------------------------------传入
//
//出口: 操作获取按键事件且有按键发生返回BN_TURE,其他情况返回BN_FALSE
  
BOOLEAN Mod_Key_MaskReCaptureOperation(KeyOperation_Def _In_KeyOperation,INT16U _In_KeyInBit);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: 按键扫描函数
//功能: 其他模块可以调用
//入口: 无
//出口: 无
void Mod_Key_ScanTask(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 按键值操作
//功能: 处理扫描函数得到的按键值，软件滤波功能，并对其状态做出判断，其他组件可调用
//入口: 、和。
//      按键值           ： _IN_KeyInBit      <<-------------------------传入
//      首次长按延时时间 ： _IN_KeyFistDelay  <<-------------------------传入
//      长按每次进入时间 ： _IN_KeyConDelay   <<-------------------------传入
//
//出口: KeyRetValue_Def类型的按键状态

KeyRetValue_Def Key_ValueOperation(INT16U _IN_KeyInBits,INT16U _IN_KeyFistDelay,INT16U _IN_KeyConDelay);
//------------------------------E N D-------------------------------------------
#endif


//-----------------------Mod_Keyboard.h--END------------------------------------