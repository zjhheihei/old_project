//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
#include ".\data_typedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static volatile INT8U SystickCount;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//获取系统计数
//-----------------------------------------------------------------------------
#define __TIM4UsedToSystemTick__
//#define __TIM5UsedToSystemTick__
INT8U Pull_SystickCount(void)
{
    INT8U Read_Int8u;
    
    Read_Int8u=SystickCount;
    return(Read_Int8u);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIM5
//12M SYSCLK
//-----------------------------------------------------------------------------
#define Poied    (16000-1)
//1ms的配置

//-----------------------------------------------------------------------------
void Systick_Configure(void)
{
    #if defined(__TIM4UsedToSystemTick__)
    CLK_PCKENR1_bit.PCKEN12 = 1; 
    
    TIM4_CNTR = 0;
    //TIM4_CR1 = 0x00;            //Auto_Reload Enable
    TIM4_PSCR = 7;           //160000000 / 2(7) = 125000
    TIM4_EGR_UG = 1;            //Soft Event Triger
    TIM4_ARR = 125;
    TIM4_IER_bit.UIE = 1;
    TIM4_CR1_bit.CEN = 1;
    #elif defined (__TIM5UsedToSystemTick__)
    CLK_PCKENR3_bit.PCKEN31=1;  
    TIM5_CR1=0x80;
    TIM5_PSCR_bit.PSC=0;
    TIM5_ARRH=(INT8U)(Poied>>8);
    TIM5_ARRL=(INT8U)(Poied&0x00FF);
    TIM5_IER_bit.UIE=1;
    TIM5_CR1_bit.CEN=1;   
    #endif 
    __enable_interrupt();
   
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIM5 update interrupt
//-----------------------------------------------------------------------------
#if defined(__TIM4UsedToSystemTick__)
#pragma vector = TIM4_UIF_vector
 __interrupt void TIM4_ISR(void)
{
    if(TIM4_SR1_bit.UIF)
    {
        TIM4_SR1_bit.UIF = 0;
        SystickCount++;
    }
}
#elif defined(__TIM5UsedToSystemTick__)
#pragma vector=TIM5_OVR_UIF_vector
 __interrupt void TIM5_ISR(void)
 {
     if(TIM5_SR1_bit.UIF)
     {
         TIM5_SR1_bit.UIF=0;
         SystickCount++;
     }
     
 }
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++