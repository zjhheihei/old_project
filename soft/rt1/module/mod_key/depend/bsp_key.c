
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


#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

#define __KEY_1_ENBALE__
#define __KEY_2_ENBALE__
#define __KEY_3_ENBALE__
#define __KEY_4_ENBALE__
#define __KEY_5_ENBALE__
//#define __KEY_6_ENBALE__
//#define __KEY_7_ENBALE__
//#define __KEY_8_ENBALE__


#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
#ifdef __KEY_1_ENBALE__
#define READ_KEY1      PA_IDR_IDR3
#define KEY1_PIN_DDR   PA_DDR_DDR3
#define KEY1_PIN_CR1   PA_CR1_C13
#define KEY1_PIN_CR2   PA_CR2_C23 
#define key1Init()     KEY1_PIN_DDR = 0;  KEY1_PIN_CR1 = 0; KEY1_PIN_CR2 = 0
 
#endif

#ifdef __KEY_2_ENBALE__
#define READ_KEY2      PA_IDR_IDR2
#define KEY2_PIN_DDR   PA_DDR_DDR2
#define KEY2_PIN_CR1   PA_CR1_C12
#define KEY2_PIN_CR2   PA_CR2_C22
#define key2Init()     KEY2_PIN_DDR = 0;  KEY2_PIN_CR1 = 0; KEY2_PIN_CR2 = 0 
#endif

#ifdef __KEY_3_ENBALE__
#define READ_KEY3      PE_IDR_IDR7
#define KEY3_PIN_DDR   PE_DDR_DDR7
#define KEY3_PIN_CR1   PE_CR1_C17
#define KEY3_PIN_CR2   PE_CR2_C27
#define key3Init()     KEY3_PIN_DDR = 0;  KEY3_PIN_CR1 = 0; KEY3_PIN_CR2 = 0 
#endif

#ifdef __KEY_4_ENBALE__
#define READ_KEY4      PE_IDR_IDR6
#define KEY4_PIN_DDR   PE_DDR_DDR6
#define KEY4_PIN_CR1   PE_CR1_C16
#define KEY4_PIN_CR2   PE_CR2_C26
#define key4Init()     KEY4_PIN_DDR = 0;  KEY4_PIN_CR1 = 0; KEY4_PIN_CR2 = 0 
#endif

#ifdef __KEY_5_ENBALE__
#define READ_KEY5      PC_IDR_IDR7
#define KEY5_PIN_DDR   PC_DDR_DDR7
#define KEY5_PIN_CR1   PC_CR1_C17
#define KEY5_PIN_CR2   PC_CR2_C27
#define key5Init()     KEY5_PIN_DDR = 0;  KEY5_PIN_CR1 = 0; KEY5_PIN_CR2 = 0 
#endif

#ifdef __KEY_6_ENBALE__
#define READ_KEY6      PF_IDR_IDR7
#define KEY6_PIN_DDR   PF_DDR_DDR7
#define KEY6_PIN_CR1   PF_CR1_C17
#define KEY6_PIN_CR2   PF_CR2_C27
#define key6Init()     KEY6_PIN_DDR = 0;  KEY6_PIN_CR1 = 0; KEY6_PIN_CR2 = 0
#endif

#ifdef __KEY_7_ENBALE__
#define READ_KEY7      PF_IDR_IDR7
#define KEY7_PIN_DDR   PF_DDR_DDR7
#define KEY7_PIN_CR1   PF_CR1_C17
#define KEY7_PIN_CR2   PF_CR2_C27
#define key7Init()     KEY7_PIN_DDR = 0;  KEY7_PIN_CR1 = 0; KEY7_PIN_CR2 = 0
#endif

#ifdef __KEY_8_ENBALE__
#define READ_KEY8      PF_IDR_IDR7
#define KEY8_PIN_DDR   PF_DDR_DDR7
#define KEY8_PIN_CR1   PF_CR1_C17
#define KEY8_PIN_CR2   PF_CR2_C27
#define key8Init()     KEY8_PIN_DDR = 0;  KEY8_PIN_CR1 = 0; KEY8_PIN_CR2 = 0
#endif


#elif defined(__HDSC32L136__)
////////////////////////////////////华大////////////////////////////////////////
#include "gpio.h"
#include "stdint.h"
#include <stdbool.h>

#ifdef __KEY_1_ENBALE__
#define KEY1_PORT   GpioPortD
#define KEY1_PIN    GpioPin4
void key1Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY1_PORT,KEY1_PIN,&keyGpioCfg);    
}
#define READ_KEY1  Gpio_GetInputIO(KEY1_PORT,KEY1_PIN)
#endif

#ifdef __KEY_2_ENBALE__
#define KEY2_PORT   GpioPortD
#define KEY2_PIN    GpioPin5
void key2Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY2_PORT,KEY2_PIN,&keyGpioCfg);    
}
#define READ_KEY2  Gpio_GetInputIO(KEY2_PORT,KEY2_PIN) 
#endif

#ifdef __KEY_3_ENBALE__
#define KEY3_PORT   GpioPortA
#define KEY3_PIN    GpioPin2
void key3Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY3_PORT,KEY3_PIN,&keyGpioCfg);    
}
#define READ_KEY3  Gpio_GetInputIO(KEY3_PORT,KEY3_PIN) 
#endif

#ifdef __KEY_4_ENBALE__
#define KEY4_PORT   GpioPortA
#define KEY4_PIN    GpioPin1
void key4Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY4_PORT,KEY4_PIN,&keyGpioCfg);    
}
#define READ_KEY4  Gpio_GetInputIO(KEY4_PORT,KEY4_PIN) 
#endif

#ifdef __KEY_5_ENBALE__
#define KEY5_PORT   GpioPortA
#define KEY5_PIN    GpioPin0
void key5Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY5_PORT,KEY5_PIN,&keyGpioCfg);    
}
#define READ_KEY5  Gpio_GetInputIO(KEY5_PORT,KEY5_PIN) 
#endif


#ifdef __KEY_6_ENBALE__
#define KEY6_PORT   GpioPortD
#define KEY6_PIN    GpioPin4
void key6Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY6_PORT,KEY6_PIN,&keyGpioCfg);    
}
#define READ_KEY6  Gpio_GetInputIO(KEY6_PORT,KEY6_PIN) 
#endif

#ifdef __KEY_7_ENBALE__
#define KEY7_PORT   GpioPortD
#define KEY7_PIN    GpioPin4
void key7Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY7_PORT,KEY7_PIN,&keyGpioCfg);    
}
#define READ_KEY7  Gpio_GetInputIO(KEY7_PORT,KEY7_PIN) 
#endif


#ifdef __KEY_8_ENBALE__
#define KEY8_PORT   GpioPortD
#define KEY8_PIN    GpioPin4
void key8Init(void)
{   
    stc_gpio_config_t keyGpioCfg;   
    keyGpioCfg.enDir = GpioDirIn;
    keyGpioCfg.enDrv = GpioDrvL;
    keyGpioCfg.enPuPd = GpioPu;
    keyGpioCfg.enOD = GpioOdDisable;     
    Gpio_Init(KEY8_PORT,KEY8_PIN,&keyGpioCfg);    
}
#define READ_KEY8  Gpio_GetInputIO(KEY8_PORT,KEY8_PIN) 
#endif

#endif




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


void bsp_keyIo_configure(void)
{
    #if defined(__KEY_1_ENBALE__)     
    key1Init();    
    #endif  
    
    #if defined(__KEY_2_ENBALE__)  
    key2Init();
    #endif  
    
    #if defined(__KEY_3_ENBALE__)  
    key3Init();
    #endif  
        
    #if defined(__KEY_4_ENBALE__)  
    key4Init();
    #endif  
     
    
    #if defined(__KEY_5_ENBALE__)  
    key5Init();
    #endif  
    
    #if defined(__KEY_6_ENBALE__)  
    key6Init();
    #endif 
    
    #if defined(__KEY_7_ENBALE__)  
    key7Init();
    #endif 
      
    #if defined(__KEY_8_ENBALE__)  
    key8Init();
    #endif 
}


void bsp_key_cofigure(INT16U *_out_keyGroup)
{
    bsp_keyIo_configure(); 
    _out_keyGroup[0] = KeyAloneBit1 | KeyAloneBit3;
    _out_keyGroup[1] = KeyAloneBit1 | KeyAloneBit4;
    _out_keyGroup[2] = KeyAloneBit0 | KeyAloneBit4;
    _out_keyGroup[3] = 0;  
}


BOOLEAN bsp_getKey(INT8U _in_keyNum) 
{   
    BOOLEAN keyFlag = BN_FALSE;   
    switch(_in_keyNum)
    {      
        case 0:
        {
            #ifdef __KEY_1_ENBALE__
            if(0 == READ_KEY1)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        }
        case 1:
        {
            #ifdef __KEY_2_ENBALE__
            if(0 == READ_KEY2)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        }  
        case 2:
        {
            #ifdef __KEY_3_ENBALE__
            if(0 == READ_KEY3)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        } 
        case 3:
        {
            #ifdef __KEY_4_ENBALE__
            if(0 == READ_KEY4)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        } 
        case 4:
        {
            #ifdef __KEY_5_ENBALE__
            if(0 == READ_KEY5)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        } 
        case 5:
        {
            #ifdef __KEY_6_ENBALE__
            if(0 == READ_KEY6)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        } 
        case 6:
        {
            #ifdef __KEY_7_ENBALE__
            if(0 == READ_KEY7)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        } 
        case 7:
        {
            #ifdef __KEY_8_ENBALE__
            if(0 == READ_KEY8)
            {
                keyFlag = BN_TRUE;
            }
            #endif
            break; 
        } 
        default:break;
    }   
    return keyFlag;
}







