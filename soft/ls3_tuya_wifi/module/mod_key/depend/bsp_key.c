
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
#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
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
#define KEY0_PIN_IDR   PF_IDR_IDR6
#define KEY0_PIN_DDR   PF_DDR_DDR6
#define KEY0_PIN_CR1   PF_CR1_C16
#define KEY0_PIN_CR2   PF_CR2_C26

#define KEY1_PIN_IDR   PF_IDR_IDR5
#define KEY1_PIN_DDR   PF_DDR_DDR5
#define KEY1_PIN_CR1   PF_CR1_C15
#define KEY1_PIN_CR2   PF_CR2_C25

#define KEY2_PIN_IDR   PE_IDR_IDR6
#define KEY2_PIN_DDR   PE_DDR_DDR6
#define KEY2_PIN_CR1   PE_CR1_C16
#define KEY2_PIN_CR2   PE_CR2_C26

#define KEY3_PIN_IDR   PE_IDR_IDR7
#define KEY3_PIN_DDR   PE_DDR_DDR7
#define KEY3_PIN_CR1   PE_CR1_C17
#define KEY3_PIN_CR2   PE_CR2_C27

#define KEY4_PIN_IDR   PC_IDR_IDR7
#define KEY4_PIN_DDR   PC_DDR_DDR7
#define KEY4_PIN_CR1   PC_CR1_C17
#define KEY4_PIN_CR2   PC_CR2_C27

#define KEY5_PIN_IDR   PF_IDR_IDR7
#define KEY5_PIN_DDR   PF_DDR_DDR7
#define KEY5_PIN_CR1   PF_CR1_C17
#define KEY5_PIN_CR2   PF_CR2_C27
//------------------------------E N D-------------------------------------------

void bsp_key_configure(void)
{
    KEY0_PIN_DDR = 0;  
    KEY0_PIN_CR1 = 0;  
    KEY0_PIN_CR2 = 0;
    
    KEY1_PIN_DDR = 0;  
    KEY1_PIN_CR1 = 0;  
    KEY1_PIN_CR2 = 0;
    
    KEY2_PIN_DDR = 0;  
    KEY2_PIN_CR1 = 0;  
    KEY2_PIN_CR2 = 0;
    
    KEY3_PIN_DDR = 0;  
    KEY3_PIN_CR1 = 0;  
    KEY3_PIN_CR2 = 0;
    
    KEY4_PIN_DDR = 0;  
    KEY4_PIN_CR1 = 0;  
    KEY4_PIN_CR2 = 0;
    
    KEY5_PIN_DDR = 0;  
    KEY5_PIN_CR1 = 0;  
    KEY5_PIN_CR2 = 0;   
}

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******组合键设置函数，KeyGroupAndAloneMap[4]存储配置好的组合键键值************
static INT16U BSP_KeyGroupAndAloneMap[4];
void BSP_SetKeyGroupArray(INT8U KeyGroupNumber,INT16U GroupBits)
{    
    if(KeyGroupNumber < 4)
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
//******按键硬件配置为上拉输入，并启用两组组合键，并返回组合键数组指针**********
INT16U *bsp_key_init(void)
{   
    bsp_key_configure(); 
    BSP_SetKeyGroupArray(0,KeyAloneBit1|KeyAloneBit3); //+ -童锁
    BSP_SetKeyGroupArray(1,KeyAloneBit0|KeyAloneBit2); //Smartlink                        //菜单和-清理滤网
    BSP_SetKeyGroupArray(2,KeyAloneBit1|KeyAloneBit4); //AI设置                          
    BSP_SetKeyGroupArray(3,0);                         //未启用输入0   
    return (BSP_KeyGroupAndAloneMap);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************获取按键0*****************************************
BOOLEAN bsp_getKey(INT8U _in_keyNum) 
{   
    BOOLEAN keyFlag = BN_FALSE;   
    switch(_in_keyNum)
    {
        case 0:if(0 == KEY0_PIN_IDR)keyFlag = BN_TRUE;break; 
        case 1:if(0 == KEY1_PIN_IDR)keyFlag = BN_TRUE;break;
        case 2:if(0 == KEY2_PIN_IDR)keyFlag = BN_TRUE;break;
        case 3:if(0 == KEY3_PIN_IDR)keyFlag = BN_TRUE;break;
        case 4:if(0 == KEY4_PIN_IDR)keyFlag = BN_TRUE;break;
        case 5:if(0 == KEY5_PIN_IDR)keyFlag = BN_TRUE;break;
        //case 6:if(0 == KEY6_PIN_IDR)keyFlag = BN_TRUE;break;
        //case 7:if(0 == KEY7_PIN_IDR)keyFlag = BN_TRUE;break;
        default:break;
    }   
    return keyFlag;
}
//------------------------------E N D-------------------------------------------





//-----------------------BSP_Keyboard.c--END------------------------------------
