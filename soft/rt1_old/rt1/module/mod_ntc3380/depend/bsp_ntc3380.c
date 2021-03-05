
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


#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"


static void average_16times(INT16U *_out_dat)
{
    INT16U mainDat = 0;
    mainDat = *_out_dat;
    INT16U point = 0;
    point = mainDat & 0x000f;
    mainDat >>= 4;
    if(7 < point)
    {
        mainDat += 1;
    }
    *_out_dat = mainDat;
}




#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
static BOOLEAN ADC_SampleOnceFinishEvent = BN_FALSE;
static INT16U  ADC_SampleOnceDate = 0;



#define __CHANNEL_1_ENABLE__
#define __CHANNEL_2_ENABLE__
#if defined (__CHANNEL_1_ENABLE__)
#define CHANNEL_1_DDR2    PF_DDR_DDR1
#define CHANNEL_1_CR12    PF_CR1_C11
#define CHANNEL_1_CR22    PF_CR2_C21
#define channel_1_io_init() CHANNEL_1_DDR2 = 0; CHANNEL_1_CR12 = 0; CHANNEL_1_CR22 = 0
#endif

#if defined (__CHANNEL_2_ENABLE__)
#define CHANNEL_2_DDR2    PF_DDR_DDR0
#define CHANNEL_2_CR12    PF_CR1_C10
#define CHANNEL_2_CR22    PF_CR2_C20
#define channel_2_io_init() CHANNEL_2_DDR2 = 0; CHANNEL_2_CR12 = 0; CHANNEL_2_CR22 = 0
#endif




void io_init(void)
{
    #if defined (__CHANNEL_1_ENABLE__)
    channel_1_io_init();
    #endif
    #if defined (__CHANNEL_2_ENABLE__)
    channel_2_io_init();
    #endif
}


void soft_init(void)
{
     CLK_PCKENR2_PCKEN20 = 1;
     ADC1_TRIGR1_VREFINTON = 1;
     ADC1_TRIGR1_TSON      = 1; 
     ADC1_CR1_ADON = 1;
     ADC1_CR2_PRESC = 0;
     ADC1_SQR1_DMAOFF = 1;   
     ADC1_CR3_SMTP2   = 0x03;
     ADC1_CR1_EOCIE   = 1;  
}


#define ADC_CHANNLE_INSIDE      0
#define ADC_CHANNLE_OUTSIDE     1
void bsp_ntc3380_channleSelect(INT8U channel)
{
     if(ADC_CHANNLE_INSIDE == channel)
     {           
          ADC1_SQR1_CHSEL_S25 = 0; 
           ADC1_SQR1_CHSEL_S24 = 1;    
     }
     else if(ADC_CHANNLE_OUTSIDE == channel)
     {       
           ADC1_SQR1_CHSEL_S24 = 0;
           ADC1_SQR1_CHSEL_S25 = 1;                        
     }     
}

void bsp_ntc3380_start(void)
{
    ADC1_CR1_START = 1;
}

void bsp_ntc3380_configure(void)
{
     io_init();
     soft_init();
     bsp_ntc3380_channleSelect(ADC_CHANNLE_INSIDE); 
     bsp_ntc3380_start();
}

BOOLEAN bsp_ntc3380_finsih(void)
{
    if(ADC_SampleOnceFinishEvent)
    {
         ADC_SampleOnceFinishEvent = BN_FALSE;
         return BN_TRUE;
    }     
    return BN_FALSE;
}


void bsp_ntc3380_getAdc(INT16U *_out_adc)
{    
    *_out_adc = ADC_SampleOnceDate;  
}





BOOLEAN bsp_ntc3380_scanTask(INT16U *_out_adc)
{
           BOOLEAN finishTwiceFlag = BN_FALSE;
    static INT8U   channelFinishCnt = 0;
    static INT8U   SampleFinishCnt = 0;
    static INT16U  stoBuf[2];
    static INT16U  mainSample = 0;
    if(bsp_ntc3380_finsih())
    {       
        mainSample += ADC_SampleOnceDate;
        SampleFinishCnt++;
        if(SampleFinishCnt >= 16)
        {
            average_16times(&mainSample);
            stoBuf[channelFinishCnt] = mainSample; 
            SampleFinishCnt = 0;
            mainSample = 0;
            channelFinishCnt++;
            if(channelFinishCnt >= 2)
            {
                channelFinishCnt = 0; 
                finishTwiceFlag = BN_TRUE;
                _out_adc[0] = stoBuf[0];
                _out_adc[1] = stoBuf[1];
                stoBuf[0] = 0;
                stoBuf[1] = 0;
            }
            bsp_ntc3380_channleSelect(channelFinishCnt);
            
        }
        bsp_ntc3380_start();
    }
    return finishTwiceFlag;
}


void BSP_ADC1_ISR(void)
{
    ADC_SampleOnceDate = 0;
    ADC_SampleOnceDate = ADC1_DRH;
    ADC_SampleOnceDate <<= 8;
    ADC_SampleOnceDate |= ADC1_DRL;
    ADC_SampleOnceFinishEvent = BN_TRUE;  
}

#pragma vector = ADC_EOC_vector

__interrupt void ADC_ISR(void)
{
      if(ADC1_SR_EOC)
      {
           ADC1_SR_EOC = 0;
           BSP_ADC1_ISR(); 
      }
}

////////////////////////////////////////////////////////////////////////////////
#elif defined(__HDSC32L136__)
#include "adc.h"
#include "gpio.h"
#include "bgr.h"

static stc_adc_irq_t stcAdcIrqFlag;

#define MAX_ADC_CHANNEL   4

#define __CHANNEL_1_ENABLE__
#define __CHANNEL_2_ENABLE__


#if defined (__CHANNEL_1_ENABLE__)
#define CHANNEL_1_PORT    GpioPortC
#define CHANNEL_1_PIN     GpioPin0
#endif

#if defined (__CHANNEL_2_ENABLE__)
#define CHANNEL_2_PORT    GpioPortC
#define CHANNEL_2_PIN     GpioPin1
#endif

#define SCAN_CHANNLE_1    AdcExInputCH10
#define SCAN_CHANNLE_2    AdcExInputCH11
#define SCAN_CHANNLE_3    AdcExInputCH10
#define SCAN_CHANNLE_4    AdcExInputCH11

static INT16U adcResultBuf[MAX_ADC_CHANNEL];


void bsp_ntc3380_irqCallback(void)
{    
    INT8U i = 0;
    for(;i < MAX_ADC_CHANNEL;i++)
    {
        Adc_GetJqrResult(&adcResultBuf[i], i);
    }
    stcAdcIrqFlag.bAdcJQRIrq = TRUE;
}

void bsp_ntc3380_start(void)
{
     Adc_JQR_Start();
}

void bsp_ntc3380_configure(void)
{
    stc_adc_cfg_t              stcAdcCfg;
    stc_adc_irq_t              stcAdcIrq;
    stc_adc_irq_calbakfn_pt_t  stcAdcIrqCalbaks;
    stc_gpio_config_t          stcAdcAN11Port;
    stc_gpio_config_t          stcAdcAN12Port;
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
    DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
    DDL_ZERO_STRUCT(stcAdcIrqFlag);  
    DDL_ZERO_STRUCT(stcAdcAN11Port);
    DDL_ZERO_STRUCT(stcAdcAN12Port);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Gpio_SetAnalogMode(CHANNEL_1_PORT, CHANNEL_1_PIN);        
    Gpio_SetAnalogMode(CHANNEL_2_PORT, CHANNEL_2_PIN);        
    if (Ok != Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE))
    {
        return;
    }
    
    
    Adc_Enable();
    //Bgr_BgrEnable();
    M0P_BGR->CR_f.BGR_EN = TRUE;
    uint8_t i = 0;
    for(;i< 200;i++)
    {
        ;
    }
    //delay100us(1);
    stcAdcCfg.enAdcOpMode       = AdcSCanMode;         //扫描模式
    stcAdcCfg.enAdcClkDiv       = AdcClkSysTDiv1;
    stcAdcCfg.enAdcSampTimeSel  = AdcSampTime8Clk;
    stcAdcCfg.enAdcRefVolSel    = RefVolSelAVDD;
    stcAdcCfg.bAdcInBufEn       = FALSE;

    Adc_Init(&stcAdcCfg);
    
    Adc_ConfigJqrChannel(JQRCH0MUX, SCAN_CHANNLE_1);
    Adc_ConfigJqrChannel(JQRCH1MUX, SCAN_CHANNLE_2);
    Adc_ConfigJqrChannel(JQRCH2MUX, SCAN_CHANNLE_3);   
    Adc_ConfigJqrChannel(JQRCH3MUX, SCAN_CHANNLE_4);   
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    Adc_EnableIrq();
    
    stcAdcIrq.bAdcJQRIrq = TRUE;
    stcAdcIrqCalbaks.pfnAdcJQRIrq = bsp_ntc3380_irqCallback;   
    Adc_ConfigIrq(&stcAdcIrq, &stcAdcIrqCalbaks);   
    Adc_ConfigJqrMode(&stcAdcCfg, MAX_ADC_CHANNEL, FALSE);  
    bsp_ntc3380_start();
}



BOOLEAN bsp_ntc3380_finish(void)
{
    if(TRUE == stcAdcIrqFlag.bAdcJQRIrq)
    {
        stcAdcIrqFlag.bAdcJQRIrq = FALSE;
        return BN_TRUE;
    }
    return BN_FALSE;
}

void bsp_ntc3380_getAdc(INT16U *_out_Adc)
{
    uint8_t i = 0;
    for(;i < MAX_ADC_CHANNEL;i++)
    {
        _out_Adc[i] = adcResultBuf[i];
    }
}

void bsp_ntc3380_channleSelect(INT8U channel)
{
    ;//华大通道采用扫描方式无需选择
    //为了和ST兼容所以做此空函数
}

BOOLEAN bsp_ntc3380_scanTask(INT16U *_out_adc)
{
    BOOLEAN finishTwiceFlag = BN_FALSE;
    static INT8U mainSampleCnt = 0;    
    static INT16U inMainMeasAdc = 0;
    static INT16U outMainMeasAdc = 0;
    if(bsp_ntc3380_finish())
    {
        INT16U adcBuf[4];
        bsp_ntc3380_getAdc(&adcBuf[0]);
        inMainMeasAdc += adcBuf[0];
        inMainMeasAdc += adcBuf[2];
        outMainMeasAdc += adcBuf[1];
        outMainMeasAdc += adcBuf[3];
        mainSampleCnt++;
        if(7 < mainSampleCnt)
        {
            mainSampleCnt = 0;
            average_16times(&inMainMeasAdc);
            average_16times(&outMainMeasAdc);
            _out_adc[0] = inMainMeasAdc;
            _out_adc[1] = outMainMeasAdc;
            inMainMeasAdc = 0;
            outMainMeasAdc = 0;
            finishTwiceFlag  = BN_TRUE;
        }
        bsp_ntc3380_start();
    }
    return finishTwiceFlag;
}
#endif




//------------------------------E N D-------------------------------------------