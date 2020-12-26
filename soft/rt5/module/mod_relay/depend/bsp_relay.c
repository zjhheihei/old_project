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

#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

#define __RELAY_1_ENBALE__
#define __RELAY_2_ENBALE__
//#define __RELAY_3_ENBALE__
//#define __RELAY_4_ENBALE__
//#define __RELAY_5_ENBALE__
//#define __RELAY_6_ENBALE__
#define __PWM_PIN_ENABLE__


#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
#ifdef __RELAY_1_ENBALE__
#define RELAY1_PIN_ODR   PG_ODR_ODR4
#define RELAY1_PIN_DDR   PG_DDR_DDR4
#define RELAY1_PIN_CR1   PG_CR1_C14
#define RELAY1_PIN_CR2   PG_CR2_C24
#define relay1Init()     RELAY1_PIN_DDR = 1;RELAY1_PIN_CR1  =1; RELAY1_PIN_CR2 = 0
#define relay1On()       RELAY1_PIN_ODR = 1
#define relay1Off()      RELAY1_PIN_ODR = 0
#endif  

#ifdef __RELAY_2_ENBALE__
#define RELAY2_PIN_ODR   PG_ODR_ODR5
#define RELAY2_PIN_DDR   PG_DDR_DDR5
#define RELAY2_PIN_CR1   PG_CR1_C15
#define RELAY2_PIN_CR2   PG_CR2_C25
#define relay2Init()     RELAY2_PIN_DDR = 1;RELAY2_PIN_CR1 =1; RELAY2_PIN_CR2 = 0
#define relay2On()       RELAY2_PIN_ODR = 1
#define relay2Off()      RELAY2_PIN_ODR = 0
#endif

#ifdef __RELAY_3_ENBALE__
#define RELAY3_PIN_ODR   PG_ODR_ODR6
#define RELAY3_PIN_DDR   PG_DDR_DDR6
#define RELAY3_PIN_CR1   PG_CR1_C16
#define RELAY3_PIN_CR2   PG_CR2_C26  
#define realy3Init()     RELAY3_PIN_DDR = 1;RELAY3_PIN_CR1 =1; RELAY3_PIN_CR2 = 0
#define relay3On()       RELAY3_PIN_ODR = 1
#define relay3Off()      RELAY3_PIN_ODR = 0
#endif


#ifdef __RELAY_4_ENBALE__
#define RELAY4_PIN_ODR   PG_ODR_ODR7
#define RELAY4_PIN_DDR   PG_DDR_DDR7
#define RELAY4_PIN_CR1   PG_CR1_C17
#define RELAY4_PIN_CR2   PG_CR2_C27
#define realy4Init()     RELAY4_PIN_DDR = 1;RELAY4_PIN_CR1 =1; RELAY4_PIN_CR2 = 0
#define relay4On()       RELAY4_PIN_ODR = 1
#define relay4Off()      RELAY4_PIN_ODR = 0
#endif


#ifdef __RELAY_5_ENBALE__
#define RELAY5_PIN_ODR   PG_ODR_ODR7
#define RELAY5_PIN_DDR   PG_DDR_DDR7
#define RELAY5_PIN_CR1   PG_CR1_C17
#define RELAY5_PIN_CR2   PG_CR2_C27
#define relay5Init()     RELAY5_PIN_DDR = 1;RELAY5_PIN_CR1 =1; RELAY5_PIN_CR2 = 0
#define relay5On()       RELAY5_PIN_ODR = 1
#define relay5Off()      RELAY5_PIN_ODR = 0
#endif


#ifdef __RELAY_6_ENBALE__
#define RELAY6_PIN_ODR   PG_ODR_ODR7
#define RELAY6_PIN_DDR   PG_DDR_DDR7
#define RELAY6_PIN_CR1   PG_CR1_C17
#define RELAY6_PIN_CR2   PG_CR2_C27
#define relay6Init()     RELAY6_PIN_DDR = 1;RELAY6_PIN_CR1 =1; RELAY6_PIN_CR2 = 0
#define relay6On()       RELAY6_PIN_ODR = 1
#define relay6Off()      RELAY6_PIN_ODR = 0
#endif


#ifdef __PWM_PIN_ENABLE__
#define PWM_PIN_ODR   PB_ODR_ODR2
#define PWM_PIN_DDR   PB_DDR_DDR2
#define PWM_PIN_CR1   PB_CR1_C12
#define PWM_PIN_CR2   PB_CR2_C22
#define pwmInit()     PWM_PIN_DDR = 1;PWM_PIN_CR1 =1; PWM_PIN_CR2 = 0; PWM_PIN_ODR = 1;
#endif

////////////////////////////////////////////////////////////////////////////////
#elif defined(__HDSC32L136__)
#include "gpio.h"

#ifdef __RELAY_1_ENBALE__
#define RELAY_1_PORT       GpioPortC
#define RELAY_1_PIN        GpioPin10
void relay1Init(void)
{
    stc_gpio_config_t relayCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    relayCfg.enDir = GpioDirOut;
    relayCfg.enDrv = GpioDrvH;
    relayCfg.enPuPd = GpioNoPuPd;
    relayCfg.enOD = GpioOdDisable;
    relayCfg.enCtrlMode = GpioAHB;
    Gpio_ClrIO(RELAY_1_PORT,RELAY_1_PIN); 
    Gpio_Init(RELAY_1_PORT,RELAY_1_PIN,&relayCfg);
    Gpio_ClrIO(RELAY_1_PORT,RELAY_1_PIN); 
}

void relay1On(void)
{
    Gpio_SetIO(RELAY_1_PORT,RELAY_1_PIN);
}

void relay1Off(void)
{
    Gpio_ClrIO(RELAY_1_PORT,RELAY_1_PIN); 
}
#endif

#ifdef __RELAY_2_ENBALE__
#define RELAY_2_PORT       GpioPortC
#define RELAY_2_PIN        GpioPin11
void relay2Init(void)
{
    stc_gpio_config_t relayCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    relayCfg.enDir = GpioDirOut;
    relayCfg.enDrv = GpioDrvH;
    relayCfg.enPuPd = GpioNoPuPd;
    relayCfg.enOD = GpioOdDisable;
    relayCfg.enCtrlMode = GpioAHB;
    Gpio_ClrIO(RELAY_2_PORT,RELAY_2_PIN); 
    Gpio_Init(RELAY_2_PORT,RELAY_2_PIN,&relayCfg);
    Gpio_ClrIO(RELAY_2_PORT,RELAY_2_PIN); 
}

void relay2On(void)
{
    Gpio_SetIO(RELAY_2_PORT,RELAY_2_PIN);
}

void relay2Off(void)
{
    Gpio_ClrIO(RELAY_2_PORT,RELAY_2_PIN); 
}
#endif


#ifdef __RELAY_3_ENBALE__
#define RELAY_3_PORT       GpioPortC
#define RELAY_3_PIN        GpioPin12
void relay1Init(void)
{
    stc_gpio_config_t relayCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    relayCfg.enDir = GpioDirOut;
    relayCfg.enDrv = GpioDrvH;
    relayCfg.enPuPd = GpioNoPuPd;
    relayCfg.enOD = GpioOdDisable;
    relayCfg.enCtrlMode = GpioAHB;
    Gpio_ClrIO(RELAY_3_PORT,RELAY_3_PIN); 
    Gpio_Init(RELAY_3_PORT,RELAY_3_PIN,&relayCfg);
    Gpio_ClrIO(RELAY_3_PORT,RELAY_3_PIN); 
}

void relay3On(void)
{
    Gpio_SetIO(RELAY_3_PORT,RELAY_3_PIN);
}

void relay3Off(void)
{
    Gpio_ClrIO(RELAY_3_PORT,RELAY_3_PIN); 
}
#endif


#ifdef __RELAY_4_ENBALE__
#define RELAY_4_PORT       GpioPortD
#define RELAY_4_PIN        GpioPin2
void relay1Init(void)
{
    stc_gpio_config_t relayCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    relayCfg.enDir = GpioDirOut;
    relayCfg.enDrv = GpioDrvH;
    relayCfg.enPuPd = GpioNoPuPd;
    relayCfg.enOD = GpioOdDisable;
    relayCfg.enCtrlMode = GpioAHB;
    Gpio_ClrIO(RELAY_4_PORT,RELAY_4_PIN); 
    Gpio_Init(RELAY_4_PORT,RELAY_4_PIN,&relayCfg);
    Gpio_ClrIO(RELAY_4_PORT,RELAY_4_PIN); 
}

void relay4On(void)
{
    Gpio_SetIO(RELAY_4_PORT,RELAY_4_PIN);
}

void relay4Off(void)
{
    Gpio_ClrIO(RELAY_4_PORT,RELAY_4_PIN); 
}
#endif

#ifdef __PWM_PIN_ENABLE__
#define PWM_PORT       GpioPortA
#define PWM_PIN        GpioPin15
void pwmInit(void)
{
    stc_gpio_config_t relayCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    relayCfg.enDir = GpioDirOut;
    relayCfg.enDrv = GpioDrvH;
    relayCfg.enPuPd = GpioNoPuPd;
    relayCfg.enOD = GpioOdDisable;
    relayCfg.enCtrlMode = GpioAHB;  
    Gpio_Init(PWM_PORT,PWM_PIN,&relayCfg);
    Gpio_SetIO(PWM_PORT,PWM_PIN);
}
#endif
#endif




void bsp_relay_configure(void)
{   
    #ifdef __PWM_PIN_ENABLE__
    pwmInit();
    #endif
    
    #ifdef __RELAY_1_ENBALE__
    relay1Init();
    relay1Off();
    #endif
    
    
    #ifdef __RELAY_2_ENBALE__
    relay2Init();
    relay2Off();
    #endif
      
    
    #ifdef __RELAY_3_ENBALE__
    realy3Init();
    relay3Off();
    #endif
    
    
    #ifdef __RELAY_4_ENBALE__
    realy4Init();
    relay4Off();
    #endif
    
    
    #ifdef __RELAY_5_ENBALE__
    realy5Init();
    relay5Off();
    #endif
    
    #ifdef __RELAY_6_ENBALE__
    realy6Init();
    relay6Off();
    #endif
}


void bsp_relay1_on(void)
{
    #ifdef __RELAY_1_ENBALE__
    relay1On();
    #endif
}

void bsp_relay1_off(void)
{
    #ifdef __RELAY_1_ENBALE__
    relay1Off();
    #endif
}

void bsp_relay2_on(void)
{
    #ifdef __RELAY_2_ENBALE__
    relay2On();
    #endif
}

void bsp_relay2_off(void)
{
    #ifdef __RELAY_2_ENBALE__
    relay2Off();
    #endif
}

void bsp_relay3_on(void)
{
    #ifdef __RELAY_3_ENBALE__
    relay3On();
    #endif
}

void bsp_relay3_off(void)
{
    #ifdef __RELAY_3_ENBALE__
    relay3Off();
    #endif
}

void bsp_relay4_on(void)
{
    #ifdef __RELAY_4_ENBALE__
    relay4On();
    #endif
}

void bsp_relay4_off(void)
{
    #ifdef __RELAY_4_ENBALE__
    relay4Off();
    #endif
}

void bsp_relay5_on(void)
{
    #ifdef __RELAY_5_ENBALE__
    relay5On();
    #endif
}

void bsp_relay5_off(void)
{
    #ifdef __RELAY_5_ENBALE__
    relay5Off();
    #endif
}

void bsp_relay6_on(void)
{
    #ifdef __RELAY_6_ENBALE__
    relay6On();
    #endif
}

void bsp_relay6_off(void)
{
    #ifdef __RELAY_6_ENBALE__
    relay6Off();
    #endif
}

