/*******************************************************************************
*
*	模块名称 : 继电器PWM模块
*	文件名称 : BSP_RelayPWM.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_RelayPWM
                   3、TM1的3路PWM口输出控制3路继电器，仅作硬件配置
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-27  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*            
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************继电器PWMIO口操作宏定义，采用位操作方式************************* 
//#define RELAY1_PIN_ODR   PG_ODR_ODR4
//#define RELAY1_PIN_DDR   PG_DDR_DDR4
//#define RELAY1_PIN_CR1   PG_CR1_C14
//#define RELAY1_PIN_CR2   PG_CR2_C24
//  
//#define RELAY2_PIN_ODR   PG_ODR_ODR5
//#define RELAY2_PIN_DDR   PG_DDR_DDR5
//#define RELAY2_PIN_CR1   PG_CR1_C15
//#define RELAY2_PIN_CR2   PG_CR2_C25


#define RELAY1_PIN_ODR   PG_ODR_ODR5
#define RELAY1_PIN_DDR   PG_DDR_DDR5
#define RELAY1_PIN_CR1   PG_CR1_C15
#define RELAY1_PIN_CR2   PG_CR2_C25
  
#define RELAY2_PIN_ODR   PG_ODR_ODR4
#define RELAY2_PIN_DDR   PG_DDR_DDR4
#define RELAY2_PIN_CR1   PG_CR1_C14
#define RELAY2_PIN_CR2   PG_CR2_C24


#define RELAY3_PIN_ODR   PG_ODR_ODR6
#define RELAY3_PIN_DDR   PG_DDR_DDR6
#define RELAY3_PIN_CR1   PG_CR1_C16
#define RELAY3_PIN_CR2   PG_CR2_C26  

#define RELAY4_PIN_ODR   PG_ODR_ODR7
#define RELAY4_PIN_DDR   PG_DDR_DDR7
#define RELAY4_PIN_CR1   PG_CR1_C17
#define RELAY4_PIN_CR2   PG_CR2_C27 
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************TIM1配置，及其3路PWM口输出IO口配置推挽。***********************
void bsp_relay_configure(void)
{   
    RELAY1_PIN_DDR = 1;
    RELAY1_PIN_CR1 = 1;
    RELAY1_PIN_CR2 = 0;
    RELAY1_PIN_ODR = 0;
    
    RELAY2_PIN_DDR = 1;
    RELAY2_PIN_CR1 = 1;
    RELAY2_PIN_CR2 = 0;
    RELAY2_PIN_ODR = 0;
      
    RELAY3_PIN_DDR = 1;
    RELAY3_PIN_CR1 = 1;
    RELAY3_PIN_CR2 = 0;
    RELAY3_PIN_ODR = 0;
    
    RELAY4_PIN_DDR = 1;
    RELAY4_PIN_CR1 = 1;
    RELAY4_PIN_CR2 = 0;  
    RELAY4_PIN_ODR = 0;
    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************3路继电器口输出*************************************

void bsp_relay1_on(void)
{
    RELAY1_PIN_ODR = 1;
}

void bsp_relay1_off(void)
{
    RELAY1_PIN_ODR = 0;
}

void bsp_relay2_on(void)
{
    RELAY2_PIN_ODR = 1;
}

void bsp_relay2_off(void)
{
    RELAY2_PIN_ODR = 0;
}

void bsp_relay3_on(void)
{
    RELAY3_PIN_ODR = 1;
}

void bsp_relay3_off(void)
{
    RELAY3_PIN_ODR = 0;
}

void bsp_relay4_on(void)
{
    RELAY1_PIN_ODR = 1;
}

void bsp_relay4_off(void)
{
    RELAY1_PIN_ODR = 0;
}

void bsp_relay5_on(void)
{
    //RELAY1_PIN_ODR = 1;
}

void bsp_relay5_off(void)
{
    //RELAY1_PIN_ODR = 0;
}

void bsp_relay6_on(void)
{
    //RELAY1_PIN_ODR = 1;
}

void bsp_relay6_off(void)
{
    //RELAY1_PIN_ODR = 0;
}
//------------------------------E N D-------------------------------------------



//-----------------------BSP_RelayPWM.c--END------------------------------------
