
/*******************************************************************************
*
*	模块名称 : BSP背光模块
*	文件名称 : BSP_Backlight.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_Backlight调用
                   3、硬件配置。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-2  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*             
*******************************************************************************/


#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
#define BACKLIGHT_PIN_ODR   PC_ODR_ODR4           
#define BACKLIGHT_PIN_DDR   PC_DDR_DDR4
#define BACKLIGHT_PIN_CR1   PC_CR1_C14
#define BACKLIGHT_PIN_CR2   PC_CR2_C24

#define backlightInit()     BACKLIGHT_PIN_DDR = 1;BACKLIGHT_PIN_CR1 = 1;BACKLIGHT_PIN_CR2 = 0
#define backlightOn()       BACKLIGHT_PIN_ODR = 1
#define backlightOff()      BACKLIGHT_PIN_ODR = 0;
#elif defined(__HDSC32L136__)
#include "gpio.h"
#include <stdbool.h>

#define BACKLIGHT_PORT   GpioPortC
#define LED_PORT_PIN     GpioPin13

void backlightInit(void)
{
    stc_gpio_config_t backlightCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    backlightCfg.enDir = GpioDirOut;
    backlightCfg.enDrv = GpioDrvH;
    backlightCfg.enPuPd = GpioNoPuPd;
    backlightCfg.enOD = GpioOdDisable;
    backlightCfg.enCtrlMode = GpioAHB;
    Gpio_ClrIO(BACKLIGHT_PORT,LED_PORT_PIN); 
    Gpio_Init(BACKLIGHT_PORT,LED_PORT_PIN,&backlightCfg);
    Gpio_ClrIO(BACKLIGHT_PORT,LED_PORT_PIN);  
}

void backlightOn(void)
{
    Gpio_SetIO(BACKLIGHT_PORT,LED_PORT_PIN);
}

void backlightOff(void)
{
    Gpio_ClrIO(BACKLIGHT_PORT,LED_PORT_PIN); 
} 
#endif




void bsp_backlight_configure(void)
{
    backlightInit();  
}
void bsp_backlight_on(void)
{
    backlightOn();
}
void bsp_backlight_off(void)
{
    backlightOff();
}

