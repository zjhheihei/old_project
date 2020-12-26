
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
#include ".\depend\bsp_ntc3380.h"
#include ".\mod_ntc3380.h"
//------------------------------E N D-------------------------------------------

typedef enum
{
    ADC_STATUS_INIT        = 0x00,   //ADC初始化
    ADC_STATUS_RUN,
}adcStatus_t;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************函数使用的内部变量*************************************
static adcStatus_t adcStatus = ADC_STATUS_INIT;


static  INT16S meastTemp[MAX_ADC_CHANNEL] = {220,220};

static INT8U outSideSensorType = SENSOR_TYPE_3380_10K;


void mod_ntc3380_outsideSensorType(INT8U type)
{
    if(SENSOR_TYPE_3950_05K >= type)
    {
        outSideSensorType = type;
    }
}



#define MAX_MEAS_ADC_DAT 4050
#define MIN_MEAS_ADC_DAT 30

BOOLEAN measAdcRightCheck(INT16U measAdcDat)
{
    if((MAX_MEAS_ADC_DAT < measAdcDat) || (MIN_MEAS_ADC_DAT > measAdcDat))
    {
        return BN_FALSE;
    }
    return BN_TRUE;
}

const INT16U B3380_10k_Table[110]=
{    //10k 上拉电阻
//3568,3546,3523,3499,3475,3449,3424,3397,3370,3342,    //-20~-11
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
 //363, 354, 346, 339, 331, 323, 316, 308, 301, 295,    //100~109
 //288, 282, 275, 269, 263, 257, 251, 246, 241, 236,    //110~119          
};

//const INT16U B3950_10k_Table[110]=
//{    //10k 上拉电阻
////3714 ,3693 ,3672 ,3649 ,3626 ,3602 ,3578 ,3552 ,3525 ,3498 ,//-20~-11
//3469 ,3440 ,3409 ,3378 ,3346 ,3313 ,3279 ,3245 ,3209 ,3173 ,//-10~-1,递减表
//3136 ,3098 ,3059 ,3020 ,2979 ,2939 ,2898 ,2856 ,2813 ,2770 ,//0~9
//2726 ,2682 ,2648 ,2593 ,2548 ,2503 ,2458 ,2412 ,2366 ,2321 ,//10~19
//2275 ,2229 ,2184 ,2138 ,2093 ,2048 ,2003 ,1959 ,1915 ,1880 ,//20~29
//1828 ,1785 ,1743 ,1700 ,1659 ,1618 ,1578 ,1538 ,1500 ,1461 ,//30~39
//1423 ,1387 ,1350 ,1315 ,1280 ,1245 ,1212 ,1179 ,1147 ,1115 ,//40~49
//1085 ,1055 ,1026 ,997  ,969  ,942  ,915  ,889  ,864  ,840  ,//50~59
//816  ,793  ,770  ,748  ,727  ,706  ,686  ,666  ,647  ,629  ,//60~69
//610  ,593  ,576  ,559  ,544  ,528  ,513  ,498  ,484  ,471  ,//70~79
//457  ,444  ,432  ,419  ,408  ,396  ,385  ,374  ,364  ,354  ,//80~89 
//344  ,334  ,325  ,316  ,307  ,299  ,291  ,283  ,275  ,268  ,//90~99 
////260  ,253  ,246  ,240  ,234  ,227  ,221  ,216  ,210  ,204  ,//100~109 
////199  ,194  ,189  ,184  ,179  ,175  ,170  ,166  ,162  ,157  ,//110~119               
//};

const INT16U B3950_5k_Table[110]=
{
3049 ,3004 ,2958 ,2912 ,2865 ,2817 ,2768 ,2719 ,2669 ,2619, //-10~-1,递减表
2568 ,2517 ,2466 ,2415 ,2364 ,2312 ,2261 ,2210 ,2158 ,2108, //0~9
2057 ,2007 ,1957 ,1907 ,1858 ,1810 ,1762 ,1715 ,1669 ,1623, //10~19
1578 ,1534 ,1491 ,1448 ,1406 ,1365 ,1325 ,1286 ,1248 ,1211, //20~29
1174 ,1139 ,1104 ,1070 ,1037 ,1005 ,974  ,944  ,915  ,886,  //30~39
858  ,831  ,805  ,780  ,755  ,731  ,708  ,686  ,664  ,643,  //40~49
623  ,603  ,584  ,566  ,548  ,531  ,514  ,498  ,483  ,467,  //50~59
453  ,439  ,425  ,412  ,399  ,387  ,375  ,364  ,352  ,342,  //60~69
331  ,321  ,312  ,302  ,293  ,284  ,276  ,268  ,260  ,252,  //70~79
245  ,237  ,231  ,224  ,217  ,211  ,205  ,199  ,193  ,188,  //80~89 
183  ,178  ,173  ,168  ,163  ,159  ,154  ,150  ,146  ,142,  //90~99 
};


INT16S Mod_SearchNTCTable(INT16U _In_Adc_AvgValue,INT8U tableType)
{
    const INT16U *pResistorTable;
    INT16U  table_high = 0, table_low = 0;
    INT16S  ret_tempera = 0;            //返回温度
    INT16S  remain = 0;                 //小数部分
    INT16U  DivRm;
    if(SENSOR_TYPE_3950_05K == tableType)
    {
        pResistorTable=(const INT16U*)B3950_5k_Table;
    }
//    else if(SENSOR_TYPE_3950_10K == tableType)
//    {
//        pResistorTable=(const INT16U*)B3950_10k_Table;
//    }
    else
    {
        pResistorTable=(const INT16U*)B3380_10k_Table;
    }
    
    if(_In_Adc_AvgValue>pResistorTable[0])          //查表
    {       
        ret_tempera = -95;
    }
    else if(_In_Adc_AvgValue<pResistorTable[109])
    {
       ret_tempera = 995;
    }
    else
    {
        while(ret_tempera<109)       
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
        ret_tempera = (ret_tempera-10)*10;
        ret_tempera-=remain;           //小数部分
    }
    return(ret_tempera);
}

INT8U  ntcError;
INT8U mod_ntc3380_error(void)
{
    return ntcError;
}



INT16S mod_ntc3380_getMeasTemp(INT8U sersonType)
{
    INT16S Temperature;
    if(OUTSIDE_SERSON >= sersonType)
    {
        Temperature = meastTemp[sersonType];
    }
    return Temperature;
}




void mod_ntc3380_scanTask(void)
{  
    switch(adcStatus)
    {
        case ADC_STATUS_INIT:
        {
            bsp_ntc3380_configure();
            
            adcStatus = ADC_STATUS_RUN;
            break;
        }
        case ADC_STATUS_RUN:
        {
            static INT16U getAdcBuf[2];
            if(bsp_ntc3380_scanTask(getAdcBuf))
            {
                if(measAdcRightCheck(getAdcBuf[INSIDE_SERSON]))
                {
                    ntcError &= ~INSIDE_ERROR;
                    meastTemp[INSIDE_SERSON] = Mod_SearchNTCTable(getAdcBuf[INSIDE_SERSON],SENSOR_TYPE_3380_10K);
                }
                else
                {
                    ntcError |= INSIDE_ERROR;
                    meastTemp[INSIDE_SERSON] = 999;
                }
                
                if(measAdcRightCheck(getAdcBuf[OUTSIDE_SERSON]))
                {
                    ntcError &= ~OUTSIDE_ERROR;
                    meastTemp[OUTSIDE_SERSON] = Mod_SearchNTCTable(getAdcBuf[OUTSIDE_SERSON],outSideSensorType);
                }
                else
                {
                    ntcError |= OUTSIDE_ERROR;
                    meastTemp[OUTSIDE_SERSON] = 999;
                }
            }
            break;
        }
        default:break;
    }
}



