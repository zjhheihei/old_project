
/*******************************************************************************
*
*	模块名称 : Mod_ADC模块
*	文件名称 : MOD_NTC3380.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、 
                   3、 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-12  jinzh 正式发布
*	Copyright (C), 2015-2020,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.2      
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_NTC3380.h"
//------------------------------E N D-------------------------------------------

typedef enum
{
    Adc_Configure          = 0x00,   //ADC初始化
    Adc_ChannalSelect      = 0x01,   //通道选择
    Adc_Start              = 0x02,   //开启ADC
    Adc_Wait               = 0x03,   
}Mod_AdcTaskSatus_Def;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************函数使用的内部变量*************************************
static Mod_AdcTaskSatus_Def Mod_AdcTaskSatus;
static INT16S InSideTemperature  = 0;
static INT16S OutSideTemperature = 0;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************3380_____-20℃-119℃*************************************
const INT16U B3380_10k_Table[140]=
{    //10k 上拉电阻
3568,3546,3523,3499,3475,3449,3424,3397,3370,3342,    //-20~-11
3314,3285,3255,3225,3194,3162,3130,3098,3064,3031,    //-10~-1,递减表
2996,2962,2927,2891,2855,2818,2781,2744,2706,2669,    //0~9
2630,2592,2553,2515,2476,2437,2398,2359,2319,2280,    //10~19
2241,2202,2163,2125,2086,2048,2009,1971,1933,1896,    //20~29
1859,1822,1785,1749,1714,1678,1643,1607,1575,1541,    //30~39
1508,1475,1443,1411,1379,1349,1319,1289,1259,1231,    //40~49
1203,1175,1148,1122,1096,1070,1045,1021, 997, 973,    //50~59
 950, 927, 905, 883, 862, 842, 822, 802, 783, 764,    //60~69
 746, 728, 710, 693, 677, 661, 645, 629, 614, 599,    //70~79
 585, 571, 558, 544, 531, 518, 506, 495, 483, 471,    //80~89
 460, 449, 439, 428, 418, 409, 399, 389, 381, 372,    //90~99
 363, 354, 346, 339, 331, 323, 316, 308, 301, 295,    //100~109
 288, 282, 275, 269, 263, 257, 251, 246, 241, 236,    //110~119          
};
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************ADC对温度值查表**************************************
INT16S Mod_SearchNTCTable(INT16U _In_Adc_AvgValue)
{
    const INT16U *pResistorTable;
    INT16U  table_high = 0, table_low = 0;
    INT16S  ret_tempera = 0;            //返回温度
    INT16S  remain = 0;                 //小数部分
    INT16U  DivRm;
    //INT16U Adc1_Get_Value_temp;  
    pResistorTable=(const INT16U*)B3380_10k_Table;
    //Adc1_Get_Value_temp = Adc_AvgValue;

    if(_In_Adc_AvgValue>pResistorTable[0])          //查表
    {
        _In_Adc_AvgValue=pResistorTable[0];
    }
    if(_In_Adc_AvgValue<pResistorTable[139])
    {
       _In_Adc_AvgValue=pResistorTable[139];
    }
    
    while(ret_tempera<139)       
    {
        if(_In_Adc_AvgValue>=pResistorTable[ret_tempera])
        {
            break;
        }
        else
        {
            ret_tempera++;
        }
    }
    if(ret_tempera == 0)
    {
        remain = 0;
    }
    else
    {
        table_high = pResistorTable[ret_tempera-1];
        table_low  = pResistorTable[ret_tempera];
        remain     = (_In_Adc_AvgValue-table_low)*10/(table_high-table_low);
        DivRm      = ((_In_Adc_AvgValue-table_low)*10)%(table_high-table_low);
        if(DivRm>((table_high-table_low)/2))        
        {
            remain+=1;
        }
    }
    ret_tempera = (ret_tempera-20)*10;
    ret_tempera-=remain;           //小数部分
    return(ret_tempera);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************返回温度值****************************************
INT16S Mod_NTC3380_GetTemperature(Mod_SersonSelect_Def _In_SersonSelect)
{
     if(InsideSerson == _In_SersonSelect)
     {
            if(0x7fff != InSideTemperature)
            {
                  if(InSideTemperature > 999)
                  {
                        InSideTemperature = 999;
                  }
                  
                  if(InSideTemperature <-99)
                  {
                        InSideTemperature = -99;
                  } 
            }
            
            return InSideTemperature;
     }
     else 
     {
            if(0x7fff != InSideTemperature)
            {
                  if(OutSideTemperature > 999)
                  {
                      OutSideTemperature = 999;
                  }
                  
                  if(OutSideTemperature <-99)
                  {
                      OutSideTemperature = -99;
                  }
            }
            
            return OutSideTemperature;
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************返回温度值****************************************
void Mod_NTC3380_ScanTask(void)
{
     static INT8U   ChannelCount     = InsideNumber;
            INT16U  ADC1_OnceDate    = 0;
            INT16U  ADC1_Remainder   = 0;
     static INT16U  ADC1_16thDate    = 0;
     static INT8U   ADC1_SampleTimes = 0;
     static INT32U  ADC1_TimOut = 0;
     
     switch(Mod_AdcTaskSatus)
     {
            case Adc_Configure:
            {
                 BSP_ADC_Configure();
                 Mod_AdcTaskSatus = Adc_ChannalSelect;
                 break;
            }
            
            case Adc_ChannalSelect:
            {   
                 if(InsideNumber == ChannelCount)
                 {
                       BSP_ADC_ChannalSelect(ADC_Channel_Inside); 
                 }
                 else if(OutsideNumber == ChannelCount)
                 {
                       BSP_ADC_ChannalSelect(ADC_Channel_Outside); 
                 }
                 else
                 {
                       ChannelCount = InsideNumber ;
                       BSP_ADC_ChannalSelect(ADC_Channel_Inside); 
                 }                     
                                                           
                 Mod_AdcTaskSatus = Adc_Start;
                 break;
            }
            
            case Adc_Start:
            {
                 BSP_ADC12_Start();
                 ADC1_SampleTimes++;
                 Mod_AdcTaskSatus = Adc_Wait;
                 break;
            }
            
            case Adc_Wait:
            {
                 if(BSP_ADC1_SampleOnceFinish())
                 {       
                        ADC1_TimOut = 0;
                        if(ADC1_SampleTimes>16)
                        {
                              ADC1_SampleTimes = 0;
                              Mod_AdcTaskSatus = Adc_ChannalSelect;             //完成16次采用退回通道选择
                              
                              ADC1_Remainder = ADC1_16thDate&0x000f;  //获取小数 
                              ADC1_16thDate = ADC1_16thDate>>4;
                              //ADC1_16thDate = 0;
                              if(ADC1_Remainder>7)
                              {
                                   ADC1_16thDate += 1; 
                              }                         
                              if((ADC1_16thDate>4050)||(ADC1_16thDate<30))
                              {
                                    if(InsideNumber == ChannelCount)          
                                    {
                                          InSideTemperature  = 0x7fff;
                                    }
                                    else if(OutsideNumber == ChannelCount)
                                    {
                                          OutSideTemperature = 0x7fff;
                                    }
                              }
                              else
                              {
                                    if(InsideNumber == ChannelCount)          
                                    {
                                          InSideTemperature = Mod_SearchNTCTable(ADC1_16thDate);
                                    }
                                    else if(OutsideNumber == ChannelCount)
                                    {
                                          OutSideTemperature = Mod_SearchNTCTable(ADC1_16thDate);
                                    }
                              }
                              ADC1_16thDate = 0;
                              
                              ChannelCount++;
                 
                              if(ChannelCount > MaxAdcChannleCount)
                              {
                                   ChannelCount = InsideNumber;
                              }
                        }
                        else
                        {
                              ADC1_OnceDate  = BSP_ADC1_GetValue();
                              ADC1_16thDate += ADC1_OnceDate;
                              Mod_AdcTaskSatus = Adc_Start;
                        }
                 }
                 if(++ADC1_TimOut>1000) //内部判定超时，可以根据总任务时间，再进行调整
                 {
                      ADC1_TimOut = 0;
                      Mod_AdcTaskSatus = Adc_Configure;
                      ADC1_SampleTimes = 0;
                      ADC1_16thDate = 0;
                      ChannelCount = InsideNumber;
                 }
                 break;
            }
     }
}
//------------------------------E N D-------------------------------------------


//-----------------------MOD_NTC3380.c--END------------------------------------