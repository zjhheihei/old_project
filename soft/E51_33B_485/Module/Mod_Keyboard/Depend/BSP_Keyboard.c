
/*******************************************************************************
*
*	模块名称 : BSP按键模块
*	文件名称 : BSP_Keyboard.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_KeyBoard
                   3、仅做硬件配置和组合键配置
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-6  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*            
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************独立按键和组合键宏定义配置，采用16位方式的低8位*******************
#define  KeyAloneBit0     0x0001    
#define  KeyAloneBit1     0x0002  
#define  KeyAloneBit2     0x0004  
#define  KeyAloneBit3     0x0008  
#define  KeyAloneBit4     0x0010
#define  KeyAloneBit5     0x0020  
#define  KeyAloneBit6     0x0040  
#define  KeyAloneBit7     0x0080  
#define  KeyGroupBit0     0x0100   //组合按键
#define  KeyGroupBit1     0x0200   //组合按键
#define  KeyGroupBit2     0x0400   //组合按键
#define  KeyGroupBit3     0x0800   //组合按键
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************按键IO口操作宏定义，采用位操作方式*****************************
#define KEY0_Pin_IDR   PG_IDR_IDR5     
#define KEY0_Pin_DDR   PG_DDR_DDR5
#define KEY0_Pin_CR1   PG_CR1_C15
#define KEY0_Pin_CR2   PG_CR2_C25

#define KEY1_Pin_IDR   PE_IDR_IDR1     
#define KEY1_Pin_DDR   PE_DDR_DDR1
#define KEY1_Pin_CR1   PE_CR1_C11
#define KEY1_Pin_CR2   PE_CR2_C21

#define KEY2_Pin_IDR   PE_IDR_IDR0     
#define KEY2_Pin_DDR   PE_DDR_DDR0
#define KEY2_Pin_CR1   PE_CR1_C10
#define KEY2_Pin_CR2   PE_CR2_C20

#define KEY3_Pin_IDR   PG_IDR_IDR6     
#define KEY3_Pin_DDR   PG_DDR_DDR6
#define KEY3_Pin_CR1   PG_CR1_C16
#define KEY3_Pin_CR2   PG_CR2_C26
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******组合键设置函数，KeyGroupAndAloneMap[4]存储配置好的组合键键值************
static INT16U BSP_KeyGroupAndAloneMap[4];
void BSP_SetKeyGroupArray(INT8U KeyGroupNumber,INT16U GroupBits)
{
    if(KeyGroupNumber<4)
    {
        BSP_KeyGroupAndAloneMap[KeyGroupNumber]=GroupBits;
    }
    else
    {
        while(1);
    }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******按键硬件配置为悬浮输入，并启用两组组合键，并返回组合键数组指针**********
INT16U *BSP_KeyHardWares_Configure(void)
{
    KEY0_Pin_DDR = 0;
    KEY0_Pin_CR1 = 1;
    KEY0_Pin_CR2 = 0;
   
    KEY1_Pin_DDR = 0;
    KEY1_Pin_CR1 = 1;
    KEY1_Pin_CR2 = 0;
   
    KEY2_Pin_DDR = 0;
    KEY2_Pin_CR1 = 1;
    KEY2_Pin_CR2 = 0;
    
    KEY3_Pin_DDR = 0;
    KEY3_Pin_CR1 = 1;
    KEY3_Pin_CR2 = 0;
    
    BSP_SetKeyGroupArray(0,KeyAloneBit1|KeyAloneBit2); //菜单和+调整时间
    BSP_SetKeyGroupArray(1,0); //菜单和-清理滤网
    BSP_SetKeyGroupArray(2,0);                         //未启用输入0
    BSP_SetKeyGroupArray(3,0);                         //未启用输入0
    
    return (BSP_KeyGroupAndAloneMap);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************获取按键0*****************************************
BOOLEAN BSP_GetKey0(void) 
{
  
   if(KEY0_Pin_IDR)  
   {
        return BN_FALSE;
   }
   return BN_TRUE;
   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************获取按键1*****************************************
BOOLEAN BSP_GetKey1(void) 
{

   if(KEY1_Pin_IDR)  
   {
        return BN_FALSE;
   }
   return BN_TRUE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************获取按键2*****************************************
BOOLEAN BSP_GetKey2(void) 
{

   if(KEY2_Pin_IDR)  
   {
        return BN_FALSE;
   }
   return BN_TRUE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************获取按键3*****************************************
BOOLEAN BSP_GetKey3(void) 
{
   if(KEY3_Pin_IDR)  
   {
        return BN_FALSE;
   }
   return BN_TRUE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************获取按键4，未启用返回假**********************************
BOOLEAN BSP_GetKey4(void) 
{
    return BN_FALSE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************获取按键5，未启用返回假**********************************
BOOLEAN BSP_GetKey5(void) 
{
    return BN_FALSE;
}
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************获取按键6，未启用返回假**********************************
BOOLEAN BSP_GetKey6(void) 
{
    return BN_FALSE;
}
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************获取按键7，未启用返回假**********************************
BOOLEAN BSP_GetKey7(void) 
{
    return BN_FALSE;
}
//------------------------------E N D-------------------------------------------


//-----------------------BSP_Keyboard.c--END------------------------------------
