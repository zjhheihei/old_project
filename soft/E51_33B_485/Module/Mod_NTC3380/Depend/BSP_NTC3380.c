
/*******************************************************************************
*	模块名称 : ADC模块
*	文件名称 : BSP_NTC3380.c
*	版    本 : V1.0
*	说    明 : 1、配置ADC硬件和软件
*                  2、采集采用中断方式
*                  3、采集完成触发事件和发出ADC值 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-08-02  jinzh  正式发布
*	Copyright (C), 2015-2020,   menredGroup    
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************内部变量定义***************************************
#include "DataTypedef.h"
#include "iostm8l052r8.h"
#include "intrinsics.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************内部变量定义***************************************
static BOOLEAN ADC_SampleOnceFinishEvent = BN_FALSE;
static INT16U  ADC_SampleOnceDate = 0;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************PC4口宏定义****************************************
#define AD_PIN_ODR3   PC_ODR_ODR3   
#define AD_PIN_IDR3   PC_IDR_IDR3  
#define AD_PIN_DDR3   PC_DDR_DDR3
#define AD_PIN_CR13   PC_CR1_C13    
#define AD_PIN_CR23   PC_CR2_C23    
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************ADC_IO口配置为浮空输入**********************************
void BSP_ADC_Hardware_Configure(void)
{
    PC_DDR_DDR3 = 0 ;     
    PC_CR1_C13 = 0 ;     
    PC_CR2_C23 = 0 ;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************ADC软件配置****************************************
void BSP_ADC_Software_Configure(void)
{
     CLK_PCKENR2_PCKEN20 = 1;
     ADC1_TRIGR1_VREFINTON = 1;
     ADC1_TRIGR1_TSON      = 1; 
     ADC1_CR1_ADON = 1;
     ADC1_CR2_PRESC = 0;
     ADC1_SQR1_DMAOFF = 1;   
     ADC1_CR3_SMTP2   = 0x03;
     //ADC1_SQR4_CHSEL_S4 = 1;
     ADC1_CR1_EOCIE   = 1;
   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************ADC初始化配置****************************************
void BSP_ADC_Configure(void)
{
     BSP_ADC_Hardware_Configure();
     BSP_ADC_Software_Configure();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************ADC通道选择****************************************
#define ADC_Channel_Inside     5
#define ADC_Channel_Outside    6
void BSP_ADC_ChannalSelect(INT8U _In_Channel)
{
     //ADC1_CR3 &= 0xe0;
     //ADC1_CR3_CHSEL = _In_Channel;
     //ADC1_CR3 |= _In_Channel; 
     if(ADC_Channel_Inside == _In_Channel)
     {
           ADC1_SQR4_CHSEL_S4 = 1;
           ADC1_SQR4_CHSEL_S5 = 0;
     }
     else if(ADC_Channel_Inside == _In_Channel)
     {
           ADC1_SQR4_CHSEL_S5 = 1;
           ADC1_SQR4_CHSEL_S4 = 0;
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************ADC开启转换****************************************
void BSP_ADC12_Start(void)
{
    ADC1_CR1_START = 1;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************读取单次转换完成事件***********************************
BOOLEAN BSP_ADC1_SampleOnceFinish(void)
{
    if(ADC_SampleOnceFinishEvent)
    {
         ADC_SampleOnceFinishEvent = BN_FALSE;
         return BN_TRUE;
    }     
    return BN_FALSE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************读取单次转换完成事件***********************************
INT16U BSP_ADC1_GetValue(void)
{ 
    INT16U ADC1_Value = 0;
    ADC1_Value = ADC_SampleOnceDate;
    return ADC1_Value;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************中断处理函数***************************************
void BSP_ADC1_ISR(void)
{
    ADC_SampleOnceDate = 0;
    ADC_SampleOnceDate = ADC1_DRH;
    ADC_SampleOnceDate <<= 8;
    ADC_SampleOnceDate |= ADC1_DRL;
    ADC_SampleOnceFinishEvent = BN_TRUE;  
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************************中断********************************************
#pragma vector = ADC_EOC_vector

__interrupt void ADC_ISR(void)
{
      if(ADC1_SR_EOC)
      {
           ADC1_SR_EOC = 0;
           BSP_ADC1_ISR(); 
      }
}
//------------------------------E N D-------------------------------------------