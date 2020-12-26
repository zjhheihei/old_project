 
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
#include <stdbool.h>
#include <stdint.h>
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------





void bsp_real_time_lock(void)
{
    RTC_ISR1_INIT = 0;
    RTC_WPR=0x55;//使能写保护
    RTC_WPR=0x55;
}

bool bsp_real_time_unLock(void)
{
    uint16_t errTime = 0;
    RTC_WPR=0xCA;
    RTC_WPR=0x53;   
    RTC_ISR1_INIT = 1;          
    while(!RTC_ISR1_INITF)
    {
        if(++errTime > 255)
        {           
            return false; 
        }
    }
    return true;
}

bool bsp_real_time_configure(uint8_t *rtcBuf)
{
    CLK_PCKENR2_PCKEN22 = 1; 
    CLK_CRTCR |= 0x10;   //RTC采用LSE,不分频
    if(bsp_real_time_unLock())
    {      
        RTC_TR1    = rtcBuf[0];           
        RTC_TR2    = rtcBuf[1];       
        RTC_TR3    = rtcBuf[2];        
        RTC_DR1    = rtcBuf[4];       
        rtcBuf[3] <<= 5;
        RTC_DR2    =  0x1f & rtcBuf[5];
        RTC_DR2 |= rtcBuf[3];
        RTC_DR3    = rtcBuf[6];
        RTC_CR1_FMT = 0;                
        //RTC_APRER = 0x7F;         
        //RTC_SPRERL = 0x00;
        //RTC_SPRERL = 0xFF;        
        bsp_real_time_lock();
        return true;
    }
    return false;  
}

bool bsp_real_time_read_enable(void)
{
    if(RTC_ISR1_RSF) 
    {       
        return true;
    }
    return false;
}




//-----------------------BSP_Keyboard.c--END------------------------------------
