/******************************************************************************
*
*	模块名称 : 背光和系统时钟联动演示模块
*	文件名称 : ExMain.c
*	版    本 : V1.0
*	说    明 : 上电不配置时钟，采用默认的内部16/8=2M的评率
                   背光做1秒的闪动,验证背光模块。
                   在BELL3硬件上验证通过。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-6  zhoujh  正式发布
*	Copyright (C), 2018-2022,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.10.4（EWSTM8-2104-Autorun）     
*******************************************************************************/



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_Backlight\Src\Mod_Backlight.h"
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//宏定义
#define Delay1second   20000 
#define Opend          0xff
#define Closed         0x00
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    
    static unsigned long BacklightDelayTime = 0;  //定义延时时间
    static unsigned char BacklightStatus    = 0;  //背光状态
    ITF_Backlight_Configure();                     //背光硬件配置
    
    while(1)
    {
        if(++BacklightDelayTime>Delay1second)     //延时时间到
        {
             BacklightDelayTime = 0;              //清零延时时间
             if(Closed == BacklightStatus)        //判断状态开关背光并记录状态
             {
                  ITF_Backlight_Enable();
                  BacklightStatus = Opend;
             }
             else if(Opend == BacklightStatus)
             {
                  ITF_Backlight_Disable();
                  BacklightStatus = Closed;
                  
                  
             }
        }
       
    }    
   
}
//------------------------------E N D-------------------------------------------

//--------------------------Backlight.c--END------------------------------------
