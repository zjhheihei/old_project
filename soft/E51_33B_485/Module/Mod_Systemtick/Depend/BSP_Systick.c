//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static volatile INT8U SystickCount;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//获取系统计数
//-----------------------------------------------------------------------------
INT8U Pull_SystickCount(void)
{
    INT8U Read_Int8u;
    
    Read_Int8u=SystickCount;
    return(Read_Int8u);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIM5
//16M SYSCLK
//-----------------------------------------------------------------------------
#define Poied    (16000-1)
//1ms的配置

//-----------------------------------------------------------------------------
void Systick_Configure(void)
{
    CLK_PCKENR3_bit.PCKEN31=1;   //Enable Tim5 Peripheral clock
    TIM5_CR1=0x80;
    TIM5_PSCR_bit.PSC=0;
    TIM5_ARRH=(INT8U)(Poied>>8);
    TIM5_ARRL=(INT8U)(Poied&0x00FF);
    TIM5_IER_bit.UIE=1;
    TIM5_CR1_bit.CEN=1;
    __enable_interrupt();
   
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIM5 update interrupt
//-----------------------------------------------------------------------------
#pragma vector=TIM5_OVR_UIF_vector
 __interrupt void TIM5_ISR(void)
 {
     if(TIM5_SR1_bit.UIF)
     {
         TIM5_SR1_bit.UIF=0;
         SystickCount++;
     }
     
 }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++