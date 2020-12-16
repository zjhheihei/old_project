
/*******************************************************************************
*
*	模块名称 : ADC按键模块
*	文件名称 : MOD_ADC_NTC.c
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
#include ".\Mod_ADC_NTC.h"

//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//传感器3380温度值表 
//-----------------------------------------------------------------------------
const INT16U B3380_10k_Table[140]={    //10k 上拉电阻
                                      3568,3546,3523,3499,3475,3449,3424,3397,3370,3342,    //-20~-11
                                      3314,3285,3255,3225,3194,3162,3130,3098,3064,3031,    //-10~-1,递减表
                                      2996,2962,2927,2891,2855,2818,2781,2744,2706,2669,    //0~9
                                      2630,2592,2553,2515,2476,2437,2398,2359,2319,2280,    //10~19
                                      2241,2202,2163,2125,2086,2048,2009,1971,1933,1896,    //20~29
                                      1859,1822,1785,1749,1714,1678,1643,1607,1575,1541,    //30~39
                                      1508,1475,1443,1411,1379,1349,1319,1289,1259,1231,    //40~49
                                      1203,1175,1148,1122,1096,1070,1045,1021,997,973,      //50~59
                                      950,927,905,883,862,842,822,802,783,764,              //60~69
                                      746,728,710,693,677,661,645,629,614,599,              //70~79
                                      585,571,558,544,531,518,506,495,483,471,              //80~89
                                      460,449,439,428,418,409,399,389,381,372,              //90~99
                                      363,354,346,339,331,323,316,308,301,295,              //100~109
                                      288,282,275,269,263,257,251,246,241,236,              //110~119          
                                  };
//++++++++++++++++++++++++++++++++end+++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++++
//搜索3380温度表,
//入口：NTC类型,ADC值，
//出口：温度值,有符号数
//------------------------------------------------------------------------------
INT16S Mod_SearchNTCTable(INT16U Adc_AvgValue)
{
    const INT16U *pResistorTable;
    INT16U  table_high = 0, table_low = 0;
    INT16S  ret_tempera = 0;            //返回温度
    INT16S  remain = 0;                 //小数部分
    INT16U  DivRm;
    //INT16U Adc1_Get_Value_temp;  
    pResistorTable=(const INT16U*)B3380_10k_Table;
    //Adc1_Get_Value_temp = Adc_AvgValue;

    if(Adc_AvgValue>pResistorTable[0])          //查表
    {
        Adc_AvgValue=pResistorTable[0];
    }
    if(Adc_AvgValue<pResistorTable[139])
    {
       Adc_AvgValue=pResistorTable[139];
    }
    
    while(ret_tempera<139)       
    {
        if(Adc_AvgValue>=pResistorTable[ret_tempera])
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
        remain     = (Adc_AvgValue-table_low)*10/(table_high-table_low);
        DivRm      = ((Adc_AvgValue-table_low)*10)%(table_high-table_low);
        if(DivRm>((table_high-table_low)/2))
        {
            remain+=1;
        }
    }
    ret_tempera = (ret_tempera-20)*10;
    ret_tempera-=remain;           //小数部分
    return(ret_tempera);
}
//++++++++++++++++++++++++++++end++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++start++++++++++++++++++++++++++++++++++++++++++++
//ADC扫描任务,
//入口：
//出口：
//-----------------------------------------------------------------------------
static Mod_AdcTaskSatus_Def  AdcTaskStatus;
static Mod_AdcEvent_Def      AdcEvent;
INT16U Adc_GetAvgValue ;

void ADC_ScanTask(void)
{
  static INT16U Adc_Count  = 0;
  static INT16U Adc_TotalValue = 0;
  static INT16U Adc_OnceValue  = 0;
  
  switch(AdcTaskStatus)
   {
        case Adc_Init:                    //事件：扫描硬件初始化
        {
            BSP_ADC_HardwareConfigure();  //开启ADC硬件初始化
            AdcTaskStatus = Adc_Channal; //选择ADC转换通道
            break;
        }  
        case Adc_Channal:                //事件：选择ADC转换通道
        {
            BSP_ADC_ChannalConfigure();   //选择ADC转换通道
            AdcTaskStatus = Adc_Convert;     //开启ADC转换开关
            break;
        }  
        
        case Adc_Convert:                 //事件：开启ADC转换开关
        {
            BSP_ADC_Convert();          //开启ADC转换开关
            AdcTaskStatus = Adc_Wait ;   //判断ADC转换完成
            break;
        }
        
        case Adc_Wait:                  //事件：判断ADC转换完成
        {
              if(BSP_ADC_Finish())      //如果完成adc转换
              {
                  Adc_OnceValue = BSP_AdcOnceValue();    //将AD值给adc_once
                  Adc_TotalValue += Adc_OnceValue;       //计算采用16次值
                  if(++Adc_Count == 16)                  //判断是否执行16次
                  {
                      Adc_GetAvgValue = Adc_TotalValue/16 ;//求16次平均值
                      AdcEvent = Finish ;                 //发出完成标志事件
                      Adc_Count  = 0    ;                 //清除计数标志
                      Adc_OnceValue  = 0 ;
                      Adc_TotalValue = 0 ;
                     
                  }
                  else
                  {
                      AdcTaskStatus = Adc_Convert;      //16次没完成，继续转换
                  }
              }
        }
   }
   
}
INT16U Mod_Adc_GetTemp(void)  //
{
    return Adc_GetAvgValue;
}

BOOLEAN Mod_Adc_FinishEvent(void)
{
     if(Finish == AdcEvent)
     {
        AdcEvent = None;
        return BN_TRUE;
     }
     return BN_FALSE;
}





//-----------------------MOD_ADC_NTC.c--END------------------------------------