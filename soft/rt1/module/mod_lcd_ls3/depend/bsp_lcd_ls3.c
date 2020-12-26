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

///////////////////////////////////ST平台///////////////////////////////////////
#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
#define SEG00_SEG07_REG  LCD_PM0 
#define SET_SEG_00    0X01
#define SET_SEG_01    0X02
#define SET_SEG_02    0X04
#define SET_SEG_03    0X08
#define SET_SEG_04    0X10
#define SET_SEG_05    0X20
#define SET_SEG_06    0X40
#define SET_SEG_07    0X80
#define SEG08_SEG15_REG  LCD_PM1
#define SET_SEG_08    0X01
#define SET_SEG_09    0X02
#define SET_SEG_10    0X04
#define SET_SEG_11    0X08
#define SET_SEG_12    0X10
#define SET_SEG_13    0X20
#define SET_SEG_14    0X40
#define SET_SEG_15    0X80

#define SEG16_SEG23_REG  LCD_PM2
#define SET_SEG_16    0X01
#define SET_SEG_17    0X02
#define SET_SEG_18    0X04
#define SET_SEG_19    0X08
#define SET_SEG_20    0X10
#define SET_SEG_21    0X20
#define SET_SEG_22    0X40
#define SET_SEG_23    0X80

#define SEG24_SEG31_REG  LCD_PM3
#define SET_SEG_24    0X01
#define SET_SEG_25    0X02
#define SET_SEG_26    0X04
#define SET_SEG_27    0X08
#define SET_SEG_28    0X10
#define SET_SEG_29    0X20
#define SET_SEG_30    0X40
#define SET_SEG_31    0X80

#define SEG32_SEG39_REG  LCD_PM4
#define SET_SEG_32       0X01
#define SET_SEG_33       0X02
#define SET_SEG_34       0X04
#define SET_SEG_35       0X08
#define SET_SEG_36       0X10
#define SET_SEG_37       0X20
#define SET_SEG_38       0X40
#define SET_SEG_39       0X80


#define LCD_VCC_2_6_V_HIGH_DRIVE      0X10
#define LCD_VCC_2_7_V_HIGH_DRIVE      0X12
#define LCD_VCC_2_8_V_HIGH_DRIVE      0X14
#define LCD_VCC_3_0_V_HIGH_DRIVE      0X16
#define LCD_VCC_3_1_V_HIGH_DRIVE      0X18
#define LCD_VCC_3_2_V_HIGH_DRIVE      0X1A
#define LCD_VCC_3_4_V_HIGH_DRIVE      0X1C
#define LCD_VCC_3_5_V_HIGH_DRIVE      0X1E

#define LCD_VCC_2_6_V_LOW_DRIVE      0X00
#define LCD_VCC_2_7_V_LOW_DRIVE      0X02
#define LCD_VCC_2_8_V_LOW_DRIVE      0X04
#define LCD_VCC_3_0_V_LOW_DRIVE      0X06
#define LCD_VCC_3_1_V_LOW_DRIVE      0X08
#define LCD_VCC_3_2_V_LOW_DRIVE      0X0A
#define LCD_VCC_3_4_V_LOW_DRIVE      0X0C
#define LCD_VCC_3_5_V_LOW_DRIVE      0X0E

void bsp_lcd_segConfigure(void)
{   
    SEG00_SEG07_REG = SET_SEG_01 | SET_SEG_02 | SET_SEG_03 | SET_SEG_04 |\
      SET_SEG_05 | SET_SEG_06 | SET_SEG_07;
    SEG08_SEG15_REG = SET_SEG_08 | SET_SEG_09 | SET_SEG_10 | SET_SEG_11 | SET_SEG_15; 
    SEG16_SEG23_REG = SET_SEG_16 | SET_SEG_18 | SET_SEG_19 | SET_SEG_20 | SET_SEG_21;                  
    SEG32_SEG39_REG = SET_SEG_36 | SET_SEG_37 | SET_SEG_38 | SET_SEG_39; 
}

void bsp_lcd_configure(void)
{ 
   CLK_PCKENR2_bit.PCKEN23 = 1;
   CLK_CRTCR |= 0x04;   //RTC采用内部LSI,不分频
   LCD_FRQ   |= 0x00;   //- Prescaler = 4 - Divider = 16  
   LCD_CR1   |= 0x06;   //0000 0110   1/4Duty 1/3bias  
   LCD_CR2   |= 0xe0;    //3.4V  
   LCD_CR2   |= LCD_VCC_2_8_V_LOW_DRIVE;
   
   
   LCD_CR3   |= 0x01;   //死区事件一个相位周期 
   bsp_lcd_segConfigure();   
   LCD_CR3   |= 0x40;     
}

void bsp_lcd_refresh(INT8U *ptBuf,INT8U len)
{
    INT8U i = 0;
    volatile INT8U *ptLcdReg;
    INT8U *ptLcdDataBuf;
    ptLcdDataBuf = ptBuf;
    ptLcdReg = &LCD_RAM0;
    if(len <= 22)
    {
        for(i = 0;i < len;i++)
        {
            *ptLcdReg = ptLcdDataBuf[i];
            ptLcdReg++;
        } 
    }    
}
///////////////////////////////////华大平台/////////////////////////////////////
#elif defined(__HDSC32L136__)


#include "lcd.h"
#include "lpm.h"
#include "gpio.h"
#include "stdint.h"
#include <stdbool.h>

#define MAX_LCD_RAM    4

#define COM_POART      GpioPortA 
#define COM0_PIN       GpioPin9  
#define COM1_PIN       GpioPin10
#define COM2_PIN       GpioPin11 
#define COM3_PIN       GpioPin12


#define SEG00_POART       GpioPortA
#define SEG00_PIN         GpioPin8

#define SEG_01_04_POART   GpioPortC
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

#define SEG_16_19_POART   GpioPortA
#define SEG_16_PIN        GpioPin7
#define SEG_17_PIN        GpioPin6
#define SEG_18_PIN        GpioPin5
#define SEG_19_PIN        GpioPin4
#define SEG_20_PIN        GpioPin3

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

#define COM0_ON      0x100
#define COM1_ON      0x200
#define COM2_ON      0x400
#define COM3_ON      0x800
#define MUX_ON       0X1000

#define SEG0_SEG31   0
#define SEG32_SEG39  1

void bsp_lcd_segAndComAnalogConfigure(void)
{   
    Gpio_SetAnalogMode(COM_POART, COM0_PIN );   //COM0~COM3 
    Gpio_SetAnalogMode(COM_POART, COM1_PIN);
    Gpio_SetAnalogMode(COM_POART, COM2_PIN );   //COM0~COM3 
    Gpio_SetAnalogMode(COM_POART, COM3_PIN);
    
    Gpio_SetAnalogMode(SEG00_POART, SEG00_PIN);      //SEG0
    Gpio_SetAnalogMode(SEG_01_04_POART, SEG_01_PIN);  //SEG1
    Gpio_SetAnalogMode(SEG_01_04_POART, SEG_02_PIN);  //SEG2
    Gpio_SetAnalogMode(SEG_01_04_POART, SEG_03_PIN);  //SEG3
    Gpio_SetAnalogMode(SEG_01_04_POART, SEG_04_PIN);  //SEG4
    
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
    
    Gpio_SetAnalogMode(SEG_16_19_POART, SEG_16_PIN); 
    Gpio_SetAnalogMode(SEG_16_19_POART, SEG_17_PIN);  
    Gpio_SetAnalogMode(SEG_16_19_POART, SEG_18_PIN); 
    Gpio_SetAnalogMode(SEG_16_19_POART, SEG_19_PIN); 
    Gpio_SetAnalogMode(SEG_16_19_POART, SEG_20_PIN);  
    
    
    uint32_t segSelect = 0;
    segSelect = ~(SEG_00_ON |SEG_01_ON | SEG_02_ON | SEG_03_ON |\
                  SEG_04_ON |SEG_05_ON | SEG_06_ON | SEG_07_ON |\
                  SEG_08_ON |SEG_09_ON | SEG_10_ON | SEG_11_ON |\
                  SEG_12_ON |SEG_13_ON | SEG_14_ON | SEG_15_ON |\
                  SEG_16_ON |SEG_17_ON | SEG_18_ON | SEG_19_ON |\
                  SEG_20_ON);//低选通,取反
    LCD_SetSegCom(SEG0_SEG31,segSelect); 
    segSelect = 0;
    segSelect = ~(COM0_ON|COM1_ON|COM2_ON|COM3_ON);//低选通,取反
    LCD_SetSegCom(SEG32_SEG39,segSelect); 
}


void bsp_lcd_configure(void)
{ 
    stc_lcd_config_t lcdCfg;   
    DDL_ZERO_STRUCT(lcdCfg);    
    Sysctrl_ClkSourceEnable(SysctrlClkRCL,TRUE);
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLcd,TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    bsp_lcd_segAndComAnalogConfigure();
    lcdCfg.enDispMode = LcdMode1;
    lcdCfg.enBias = LcdBias3;
    lcdCfg.enBiasSrc = LcdInRes_High;//内部电阻方式
    lcdCfg.enClk = LcdRCL;
    lcdCfg.enDuty = LcdDuty4;
    lcdCfg.enCpClk = LcdClk16k;
    lcdCfg.enScanClk = LcdClk256hz;
    M0P_LCD->CR0_f.CONTRAST = 3;
    LCD_Init(&lcdCfg);
    LCD_EnFunc(LcdEn,TRUE);    
}

INT32U megre32bit(uint8_t *pt)
{
    INT32U retDat = 0;
    retDat |= (INT32U)pt[0];
    retDat <<= 8;
    retDat |= (INT32U)pt[1];
    retDat <<= 8;
    retDat |= (INT32U)pt[2];
    retDat <<= 8;
    retDat |= (INT32U)pt[3];
    return retDat;
}

void bsp_lcd_refresh(INT8U *ptBuf,INT8U len)
{
    INT8U *pt;
    pt = ptBuf;
    
    INT8U i = 0;
    INT8U length = len / 4;
    for(;i < length;i++)
    {
        if(8 > i)
        {
            INT32U dat = 0;
            dat = megre32bit(&ptBuf[i * 4]);
            LCD_WriteRam0_7Int32(i,dat);
            pt++;
        }       
    }
}
#endif


//-----------------------——--BSP_LCD.c--END-------------------------------------
