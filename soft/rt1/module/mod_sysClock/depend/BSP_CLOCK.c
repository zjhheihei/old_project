#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

BOOLEAN LSE_Error;
INT32U  HSIFrequency;

#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"


static void TIM2_Configure(void);

void BSP_Clock_Configure(void)
{
    static volatile INT32U i;
    
    CLK_ICKCR_bit.LSION = 1;
    
    CLK_ICKCR_bit.HSION = 1;
    while(0 == CLK_ICKCR_bit.HSIRDY);
    if(0x01 != CLK_SCSR)
    {
        CLK_SWCR_bit.SWEN = 1;
        CLK_SWR = 0x01;                 //HSI
        while(0x01 != CLK_SCSR);
    }
    CLK_CKDIVR_bit.CKM = 0;         //Presacler =1
    CLK_SWCR_bit.SWEN = 0;
    
    CLK_ECKCR_bit.LSEBYP = 0;
    CLK_ECKCR_bit.LSEON = 1;
    LSE_Error = BN_TRUE;
    
    for(i=0;i<50000;i++)
    {
        if(0 != CLK_ECKCR_bit.LSERDY)
        {
            LSE_Error = BN_FALSE;
            break;
        }
    }
    
    TIM2_Configure();
    HSIFrequency=16000000;  //上电默认16 MHZ
}

static void TIM2_Configure(void)
{
    CLK_PCKENR1_bit.PCKEN16 = 1;   //Beep;
    CLK_PCKENR1_bit.PCKEN10 = 1;   //TIM2

    CLK_CBEEPR_bit.CLKBEEPSEL0 = 0;
    CLK_CBEEPR_bit.CLKBEEPSEL1 = 1;  //LSE clock used as BEEP clock source
    BEEP_CSR1_bit.MSR = 1;           //LSE Connect to TIM2 CH1
    
    TIM2_CR1 = 0x00;
    TIM2_CR2 = 0x00;
    TIM2_SMCR = 0x00;
    TIM2_ETR = 0x00;
    TIM2_DER = 0x00;
    TIM2_IER = 0x02;   //CC1IE Enabled
    TIM2_EGR = 0x00;
    TIM2_CCMR1 = 0x7D; //0111 11 01 
    TIM2_CCMR2 = 0x00;
    TIM2_CCER1 = 0x01;   //0000 00 01 CC1P--Rising Edge，CC1E--Capture Enable 

    TIM2_CNTRH = 0x00;
    TIM2_CNTRL = 0x00;
    TIM2_PSCR  = 0x03;
    TIM2_ARRH  = 0xFF;
    TIM2_ARRL  = 0xFF;
    TIM2_CCR1H = 0x00;
    TIM2_CCR1L = 0x00;
    TIM2_CCR2H = 0x00;
    TIM2_CCR2L = 0x00;
    TIM2_BKR   = 0x00;
    TIM2_OISR  = 0x00;
    
    TIM2_CR1_bit.CEN = 1;
    
    //__enable_interrupt();  
}
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

volatile BOOLEAN allowReadOSW = BN_FALSE;
volatile INT32U onceOutputWidth;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIM2 CCI1F interrupt,244us 4096 count~~1s
//------------------------------------------------------------------------------
#pragma vector=TIM2_CAPCOM_CC1IF_vector
__interrupt void TIM2_CC1IF_ISR(void)
{
    static INT16U count_onceOutput = 0;
    static INT32U widthAdd;
    static INT16U KeepCCR0;
    INT16U ReadCCR0;
    
    if(TIM2_SR1_bit.CC1IF)
    {
        ReadCCR0 = TIM2_CCR1H;
        ReadCCR0 = ReadCCR0<<8;
        ReadCCR0 |= TIM2_CCR1L;
        if(TIM2_SR2_bit.CC1OF)
        {
            TIM2_SR2_bit.CC1OF = 0;
        }
        else
        {
            count_onceOutput++;
            widthAdd += (ReadCCR0-KeepCCR0);
            if(count_onceOutput & 0x1000)  //4096
            {
                if(BN_FALSE == allowReadOSW)
                {
                    allowReadOSW = BN_TRUE;
                    onceOutputWidth = widthAdd;
                }  
                widthAdd = 0;
                count_onceOutput = 0;
            }
        }
        KeepCCR0=ReadCCR0;
    }
}


void bsp_hsi_adj_scanTask(void)
{
    static INT8U  TheRefOsc_Filter = 0;
    static INT8U  cnt_sysFeq;
    static INT32U add_sysFeq;
    INT32U readWidth;   
    if(allowReadOSW)
    {
        readWidth = onceOutputWidth;
        allowReadOSW = BN_FALSE;
        
        if(TheRefOsc_Filter <2)
        {
            TheRefOsc_Filter ++;
        }
        else
        {
            add_sysFeq += readWidth;
            cnt_sysFeq++;
            if(cnt_sysFeq & 0x08)  
            {
                if((add_sysFeq >= 12800000) && (add_sysFeq <= 19200000))
                {
                    HSIFrequency = add_sysFeq;
                }
                else
                {//频率校准误差20%的时候,认为校准的值是错值,HSI复位
                    HSIFrequency = 16000000;
                }
                add_sysFeq = 0;
                cnt_sysFeq = 0;
            }
        }
    }
}

#elif defined(__HDSC32L136__)
#include "sysctrl.h"
#include "gpio.h"
#include "flash.h"
#include "stdint.h"

void BSP_Clock_Configure(void)
{
    stc_sysctrl_clk_config_t stcCfg;
    Flash_WaitCycle(FlashWaitCycle0);   
    ///< 时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);    
    ///< 选择内部RCH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    ///< HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    ///< PCLK 为HCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    ///< 系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
    
    
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    
    ///< 加载目标频率的RCH的TRIM值
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
    ///< 使能RCH（默认打开，此处可不需要再次打开）
    //Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    ///< 时钟切换到RCH
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    ///< 关闭RCL时钟
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);
    HSIFrequency = Sysctrl_GetHClkFreq();
}


//uint32_t bsp_sysClock_getRunFreq(void)
//{
//    return Sysctrl_GetHClkFreq();
//}


void bsp_hsi_adj_scanTask(void)
{
    
}
#endif
