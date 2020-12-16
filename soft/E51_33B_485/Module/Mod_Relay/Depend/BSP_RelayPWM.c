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
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************继电器PWMIO口操作宏定义，采用位操作方式*************************  
#define Relay1_Pin_DDR   PD_DDR_DDR2
#define Relay1_Pin_CR1   PD_CR1_C12
#define Relay1_Pin_CR2   PD_CR2_C22
  
#define Relay2_Pin_DDR   PD_DDR_DDR4
#define Relay2_Pin_CR1   PD_CR1_C14
#define Relay2_Pin_CR2   PD_CR2_C24
    
#define Relay3_Pin_DDR   PD_DDR_DDR5
#define Relay3_Pin_CR1   PD_CR1_C15
#define Relay3_Pin_CR2   PD_CR2_C25  
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************TIM1配置3路PWM*****************************************  
void BSP_TIM1_Configure(void)
{
    CLK_PCKENR2_bit.PCKEN21=1;   //使能TIM1时钟   
    
    TIM1_PSCRH = 0;
    TIM1_PSCRL = 0;        //不分频2MHz
    
    TIM1_ARRH = 0X01;
    TIM1_ARRL = 0X89;        //16MHz/(399+1) = 40KHz   
    
    TIM1_CR1 |= 0x80;    //使能ARP,边沿对齐，向上计数
    TIM1_EGR |= 0x02;    //更新TIM1，使PSC有效
    TIM1_EGR |= 0x20;    //重新初始化TIM1
       
    TIM1_CCR1H = 0X00;
    TIM1_CCR1L = 0X00;       //占空比0%
    TIM1_CCMR1 = 0X68;      //配置TIM1_CH1为PWM1模式输出
    
    TIM1_CCR2H = 0X00;
    TIM1_CCR2L = 0X00;       //占空比0%
    TIM1_CCMR2 = 0X68;      //配置TIM1_CH2为PWM1模式输出*/
    
    TIM1_CCR3H = 0X00;
    TIM1_CCR3L = 0X00;       //占空比0%
    TIM1_CCMR3 = 0X68;      //配置TIM1_CH3为PWM1模式输出
   
    /*TIM1_CCER1 |= 0x01;   //Enable TIM1_CH1 channel
    TIM1_CCER1 |= 0x10;   //Enable TIM1_CH2 channel
    TIM1_CCER2 |= 0x01;  //使能TIM1_CH3通道*/
    
    TIM1_BKR |= 0x80;    //
    TIM1_CR1 |= 0x01;    //使能TIM1 TIM1_CR1_CEN
      
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************TIM1配置，及其3路PWM口输出IO口配置推挽。***********************
void BSP_RelayAndPWM_Configure(void)
{   
    Relay1_Pin_DDR = 1;
    Relay1_Pin_CR1 = 1;
    Relay1_Pin_CR2 = 0;
    
    Relay2_Pin_DDR = 1;
    Relay2_Pin_CR1 = 1;
    Relay2_Pin_CR2 = 0;
    
    Relay3_Pin_DDR = 1;
    Relay3_Pin_CR1 = 1;
    Relay3_Pin_CR2 = 0;   
    
    BSP_TIM1_Configure();    
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************设置3路PWM口100%输出************************************
void BSP_RelayPWM_Set_100Percent_Out(void)
{   
     TIM1_CCR1H = 0X01;
     TIM1_CCR1L = 0X90; 
     
     TIM1_CCR2H = 0X01;
     TIM1_CCR2L = 0X90; 
     
     TIM1_CCR3H = 0X01;
     TIM1_CCR3L = 0X90;       
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************设置3路PWM口60%输出*************************************
void BSP_RelayPWM_Set_60Percent_Out(void)
{
     TIM1_CCR1H = 0X00;
     TIM1_CCR1L = 0Xf0; 
     
     TIM1_CCR2H = 0X00;
     TIM1_CCR2L = 0Xf0; 
     
     TIM1_CCR3H = 0X00;
     TIM1_CCR3L = 0Xf0;       
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************3路继电器口输出*************************************
void BSP_Relay1_OutStatus(BOOLEAN _In_NewStatus)
{
     if(BN_TRUE == _In_NewStatus)
     {
          TIM1_CCER1_CC1E = 1;
     }
     else if(BN_FALSE == _In_NewStatus)
     {
          TIM1_CCER1_CC1E = 0;
     }
}

void BSP_Relay2_OutStatus(BOOLEAN _In_NewStatus)
{
     if(BN_TRUE == _In_NewStatus)
     {
          TIM1_CCER1_CC2E = 1;
     }
     else if(BN_FALSE == _In_NewStatus)
     {
          TIM1_CCER1_CC2E = 0;
     }
}

void BSP_Relay3_OutStatus(BOOLEAN _In_NewStatus)
{
     if(BN_TRUE == _In_NewStatus)
     {
          TIM1_CCER2_CC3E = 1;
     }
     else if(BN_FALSE == _In_NewStatus)
     {
          TIM1_CCER2_CC3E = 0;
     }
}
//------------------------------E N D-------------------------------------------



//-----------------------BSP_RelayPWM.c--END------------------------------------
