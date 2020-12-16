//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//-------------------------------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN LSE_Error;
INT32U  HSIFrequency;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void TIM2_Configure(void);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//16M HSI
//-------------------------------------------------------------------------------------------------
void Clock_Configure(void)
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
    
    CLK_ECKCR_bit.LSEBYP = 1;
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
static void TIM2_Configure(void)
{
    //CLK_ECKCR_bit.LSEON = 1;
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
    TIM2_CCMR1 = 0x01; //0000 00 01 
    TIM2_CCMR2 = 0x00;
    TIM2_CCER1 = 0x01;   //0000 00 01 CC1P--Rising Edge，CC1E--Capture Enable 

    TIM2_CNTRH = 0x00;
    TIM2_CNTRL = 0x00;
    TIM2_PSCR  = 0x00;
    TIM2_ARRH  = 0xFF;
    TIM2_ARRL  = 0xFF;
    TIM2_CCR1H = 0x00;
    TIM2_CCR1L = 0x00;
    TIM2_CCR2H = 0x00;
    TIM2_CCR2L = 0x00;
    TIM2_BKR   = 0x00;
    TIM2_OISR  = 0x00;

    TIM2_CR1_bit.CEN = 1;

}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
volatile INT16U OSCWidth[32];
volatile INT8U OSCWidthIndex;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TIM2 CCI1F interrupt
//-------------------------------------------------------------------------------------------------
#pragma vector=TIM2_CAPCOM_CC1IF_vector
 __interrupt void TIM2_CC1IF_ISR(void)
{
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
            KeepCCR0=ReadCCR0;
        }
        else
        {
            if(OSCWidthIndex<32)
            {
                OSCWidth[OSCWidthIndex]=ReadCCR0-KeepCCR0;
                OSCWidthIndex++;
            }
            KeepCCR0=ReadCCR0;      
        } 
        TIM2_SR1_bit.CC1IF = 0;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define REFOSCWH_MAX   620
#define REFOSCWH_MIN   360
//-------------------------------------------------------------------------------------------------
//校准主程序
//-------------------------------------------------------------------------------------------------
void INTOSCCalibration(void)
{
    static BOOLEAN ThisIsFirstRefOsc = BN_TRUE;
    static INT16U CalibrationCnt = 0;
    static INT32U CalibrationData = 0;
    static INT16U ThisRefOscMaxWidth = REFOSCWH_MIN;
    static INT16U ThisRefOscMinWidth = REFOSCWH_MAX;
    static INT8U RefisFailedCnt = 0;
    
    INT8U Read_OSCWidthIndex;
    
    __disable_interrupt();
    Read_OSCWidthIndex=OSCWidthIndex;
    __enable_interrupt();
    
    if(Read_OSCWidthIndex == 32)
    {
        while(Read_OSCWidthIndex)
        {
            if(ThisIsFirstRefOsc)
            {
                CalibrationCnt++;
                if(CalibrationCnt>(32-1))
                {
                    if(OSCWidth[32-Read_OSCWidthIndex] > ThisRefOscMaxWidth)
                    {
                        ThisRefOscMaxWidth = OSCWidth[32-Read_OSCWidthIndex];
                    }
                    if(OSCWidth[32-Read_OSCWidthIndex] < ThisRefOscMinWidth)
                    {
                        ThisRefOscMinWidth = OSCWidth[32-Read_OSCWidthIndex];
                    }
                    if(ThisRefOscMaxWidth < REFOSCWH_MAX)
                    {
                        ThisRefOscMaxWidth = ThisRefOscMaxWidth+5;
                    }
                    else
                    {
                        ThisRefOscMaxWidth = REFOSCWH_MAX;
                    }
                    if(ThisRefOscMinWidth > REFOSCWH_MIN)
                    {
                        ThisRefOscMinWidth = ThisRefOscMinWidth-5;
                    }
                    else
                    {
                        ThisRefOscMinWidth = REFOSCWH_MIN;
                    }
                    CalibrationCnt=0;
                    CalibrationData=0;
                    ThisIsFirstRefOsc = BN_FALSE;
                }
            }
            else
            {
                if(OSCWidth[32-Read_OSCWidthIndex]>(ThisRefOscMaxWidth))
                {
                    RefisFailedCnt++;
                    if(RefisFailedCnt>15)
                    {
                         
                        CalibrationCnt = 0;
                        RefisFailedCnt = 0;
                        ThisIsFirstRefOsc = BN_TRUE; //连续16个数据不对，重新生成滤波标准
                    }
                }
                else if(OSCWidth[32-Read_OSCWidthIndex]<(ThisRefOscMinWidth))
                {
                    RefisFailedCnt++;
                    if(RefisFailedCnt>15)
                    {
                        CalibrationCnt = 0;
                        RefisFailedCnt = 0;
                        ThisIsFirstRefOsc = BN_TRUE; //连续16个数据不对，重新生成滤波标准
                    }
                }
                else
                {
                    RefisFailedCnt = 0;
                    CalibrationData=CalibrationData+OSCWidth[32-Read_OSCWidthIndex];
                    CalibrationCnt++;
                    if(CalibrationCnt>(32768-1))
                    {
                        ThisRefOscMaxWidth=CalibrationData/32768+5;
                        ThisRefOscMinWidth=CalibrationData/32768-5;
                        HSIFrequency = CalibrationData;
                        //WriteInternalOSCFrequency(CalibrationData);  //输出校准后的频率值
                        CalibrationCnt=0;
                        CalibrationData=0;
                    }
                }  
            }
            Read_OSCWidthIndex--;
        }
        __disable_interrupt();;
        OSCWidthIndex=0;
        __enable_interrupt();;
    }
    else if(Read_OSCWidthIndex<32)
    {
    }
    else 
    {//溢出，清零
        __disable_interrupt();;
        OSCWidthIndex=0;
        __enable_interrupt();;  
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++