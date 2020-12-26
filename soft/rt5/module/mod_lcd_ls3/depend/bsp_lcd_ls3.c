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
   CLK_CRTCR |= 0x04;   //RTC采用LSE,不分频   
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



//-----------------------——--BSP_LCD.c--END-------------------------------------
