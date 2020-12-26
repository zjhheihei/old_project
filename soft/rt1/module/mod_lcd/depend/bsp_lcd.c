/*********************************************************************************************************
*
*	模块名称 : 液晶模块
*	文件名称 : BSP_LCD.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_LCD调用
*                  3、做硬件配置
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-11-12  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.2      
*********************************************************************************************************/



#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"


#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
void bsp_lcd_segConfigure(void)
{   
    LCD_PM0 = 0xFE;    //S1  - S8
    LCD_PM1 = 0xFF;    //S8  - S15
    LCD_PM2 = 0x1F;    //S16 - S20
    //LCD_PM4 = 0xF0;    //S36 - S39
}

void bsp_refresh_lcd(uint8_t *ptbuf,uint8_t len)
{
    INT8U i = 0;
    volatile INT8U *p;    
    p = &MUC_RAM_ADDRESS;
    if(len < 22)
    {
       for(i = 0;i < len;i++)
       {
         *p = ptbuf[i];
          p++;
       }  
    }    
}

void bsp_lcd_configure(void)
{ 
   CLK_PCKENR2_bit.PCKEN23 = 1;
   CLK_CRTCR |= 0x04;   //RTC采用内部LSI,不分频
   LCD_FRQ   |= 0x00;   //- Prescaler = 4 - Divider = 16  
   LCD_CR1   |= 0x06;   //0000 0110   1/4Duty 1/3bias
   //LCD_CR2   |= 0xe6;   //3.0V 
   LCD_CR2   |= 0xfe;    //3.4V 
   LCD_CR3   |= 0x01;   //死区事件一个相位周期 
   bsp_lcd_segConfigure();   
   LCD_CR3   |= 0x40;     
}

///////////////////////////////////////////////////////////////////////////////
#elif defined(__HDSC32L136__)
#include "lcd.h"
#include "lpm.h"
#include "gpio.h"
#include "stdint.h"
#include <stdbool.h>
#define COM_POART      GpioPortA 
#define COM0_PIN       GpioPin9  
#define COM1_PIN       GpioPin10
#define COM2_PIN       GpioPin11 
#define COM3_PIN       GpioPin12


#define SEG00_POART     GpioPortA
#define SEG00_PIN       GpioPin8

#define SEG01_04_POART    GpioPortC
#define SEG_01_PIN        GpioPin9
#define SEG_02_PIN        GpioPin8
#define SEG_03_PIN        GpioPin7
#define SEG_04_PIN        GpioPin6

 
#define SEG_05_13_POART   GpioPortB
#define SEG_05_PIN        GpioPin15
#define SEG_06_PIN        GpioPin14
#define SEG_07_PIN        GpioPin13
#define SEG_08_PIN        GpioPin12
#define SEG_09_PIN        GpioPin11
#define SEG_10_PIN        GpioPin10
#define SEG_11_PIN        GpioPin2
#define SEG_12_PIN        GpioPin1
#define SEG_13_PIN        GpioPin0

#define SEG_14_15_POART   GpioPortC
#define SEG_14_PIN        GpioPin5
#define SEG_15_PIN        GpioPin4

#define SEG_16_18_POART   GpioPortA
#define SEG_16_PIN        GpioPin7
#define SEG_17_PIN        GpioPin6
#define SEG_18_PIN        GpioPin5

#define SEG_00_ON    0x0001
#define SEG_01_ON    0x0002
#define SEG_02_ON    0x0004
#define SEG_03_ON    0x0008
#define SEG_04_ON    0x0010
#define SEG_05_ON    0x0020
#define SEG_06_ON    0x0040
#define SEG_07_ON    0x0080
#define SEG_08_ON    0x0100
#define SEG_09_ON    0x0200
#define SEG_10_ON    0x0400
#define SEG_11_ON    0x0800
#define SEG_12_ON    0x1000
#define SEG_13_ON    0x2000
#define SEG_14_ON    0x4000
#define SEG_15_ON    0x8000
#define SEG_16_ON    0x10000
#define SEG_17_ON    0x20000
#define SEG_18_ON    0x40000
#define SEG_19_ON    0x80000
#define SEG_20_ON    0x100000
#define SEG_21_ON    0x200000
#define SEG_22_ON    0x400000
#define SEG_23_ON    0x800000
#define SEG_24_ON    0x1000000
#define SEG_25_ON    0x2000000
#define SEG_26_ON    0x4000000
#define SEG_27_ON    0x8000000
#define SEG_28_ON    0x10000000
#define SEG_29_ON    0x20000000
#define SEG_30_ON    0x40000000
#define SEG_31_ON    0x80000000

#define COM0_ON    0x100
#define COM1_ON    0x200
#define COM2_ON    0x400
#define COM3_ON    0x800
#define MUX_ON     0X1000

#define SEG0_SEG31   0
#define SEG32_SEG39  1





void bsp_lcd_segAndComAnalogConfigure(void)
{   
    Gpio_SetAnalogMode(COM_POART, COM0_PIN );   //COM0~COM3 
    Gpio_SetAnalogMode(COM_POART, COM1_PIN);
    Gpio_SetAnalogMode(COM_POART, COM2_PIN );   //COM0~COM3 
    Gpio_SetAnalogMode(COM_POART, COM3_PIN);
    
    Gpio_SetAnalogMode(SEG00_POART, SEG00_PIN);      //SEG0
    Gpio_SetAnalogMode(SEG01_04_POART, SEG_01_PIN);  //SEG1
    Gpio_SetAnalogMode(SEG01_04_POART, SEG_02_PIN);  //SEG2
    Gpio_SetAnalogMode(SEG01_04_POART, SEG_03_PIN);  //SEG3
    Gpio_SetAnalogMode(SEG01_04_POART, SEG_04_PIN);  //SEG4
    
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_05_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_06_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_07_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_08_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_09_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_10_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_11_PIN); 
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_12_PIN);  
    Gpio_SetAnalogMode(SEG_05_13_POART, SEG_13_PIN);  
    
    Gpio_SetAnalogMode(SEG_14_15_POART, SEG_14_PIN); 
    Gpio_SetAnalogMode(SEG_14_15_POART, SEG_15_PIN);  
    
    Gpio_SetAnalogMode(SEG_16_18_POART, SEG_16_PIN); 
    Gpio_SetAnalogMode(SEG_16_18_POART, SEG_17_PIN);  
    Gpio_SetAnalogMode(SEG_16_18_POART, SEG_18_PIN); 
    
    
    uint32_t segSelect = 0;
    segSelect = ~(SEG_00_ON |SEG_01_ON | SEG_02_ON | SEG_03_ON |\
                  SEG_04_ON |SEG_05_ON | SEG_06_ON | SEG_07_ON |\
                  SEG_08_ON |SEG_09_ON | SEG_10_ON | SEG_11_ON |\
                  SEG_12_ON |SEG_13_ON | SEG_14_ON | SEG_15_ON |\
                  SEG_16_ON |SEG_17_ON | SEG_18_ON);//低选通,取反
    LCD_SetSegCom(SEG0_SEG31,segSelect); 
    segSelect = 0;
    segSelect = ~(COM0_ON|COM1_ON|COM2_ON|COM3_ON);//低选通,取反
    LCD_SetSegCom(SEG32_SEG39,segSelect); 
}

#endif

//-----------------------——--BSP_LCD.c--END-------------------------------------
