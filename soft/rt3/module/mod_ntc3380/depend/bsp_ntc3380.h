#ifndef _BSP_NTC3380_H
#define _BSP_NTC3380_H


#ifndef  __Snail_DataRetype__
#include ".\SnailDataTypedef.h"
#endif
#include "DeviceSelect.h"


#define MAX_ADC_CHANNEL        2
#define InsideNumber           1
#define OutsideNumber          2


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//名称:  bsp_ntc3380_configure
//功能: 配置ADC硬件和软件 
//入口: 无
//出口: 无
void bsp_ntc3380_configure(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称:  bsp_ntc3380_getAdc
//功能:  获取ADC的值
//入口:  无
//出口:  INT16U类型ADC的值
BOOLEAN bsp_ntc3380_scanTask(INT16U *_out_adc);
//------------------------------E N D-------------------------------------------
#endif

//-----------------------BSP_NTC3380.h--END------------------------------------