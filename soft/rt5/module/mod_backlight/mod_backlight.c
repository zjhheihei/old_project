/*******************************************************************************
*
*	模块名称 : Mod背光模块
*	文件名称 : Mod_Backlight.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不主动访问其他模块。
                   2、映射BSP中的文件供其他函数调用
	           
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-07-06  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_backlight.h"

//------------------------------E N D-------------------------------------------
static INT16U      ledTimes        = 0;

typedef enum
{
    BACK_LIGHT_INIT   =(INT8U)0x00, 
    BACK_LIGHT_RUN    =(INT8U)0x01,
}backLight_status_t;


void mod_backLight_on(INT16U _in_secondLedOn)
{
    if(_in_secondLedOn != 0)
    {
        ledTimes = _in_secondLedOn;
    }
    bsp_backlight_on();     
}

void mod_backLight_task(INT16U _in_msTick)
{  
    static backLight_status_t emStatus = BACK_LIGHT_INIT;
    switch(emStatus)
    {
        case BACK_LIGHT_INIT:
        {
            bsp_backlight_configure();
            emStatus = BACK_LIGHT_RUN;
            break;
        }
        case BACK_LIGHT_RUN:
        {
            static INT16U lastTick = 0;
            if(_in_msTick - lastTick >= 1000)
            {
                lastTick = _in_msTick;               
                if(ledTimes != 0)
                {
                    if(ledTimes > 0)
                    {                        
                        ledTimes--;
                        if(ledTimes == 0)
                        {
                            bsp_backlight_off();                            
                        }
                    }
                    else
                    {
                        ledTimes = 0;
                        bsp_backlight_off();                       
                    }
                } 
            }
            break;
        }
        default:break;
    }
}



//-----------------------Mod_Backlight.c--END------------------------------------



