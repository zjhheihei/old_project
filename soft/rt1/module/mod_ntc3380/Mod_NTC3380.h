//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_NTC3380_H
#define _MOD_NTC3380_H
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#define INSIDE_SERSON        0x00
#define OUTSIDE_SERSON       0x01
//Function
//名称:  返回温度值
//功能:  
//入口:  sersonType   <<----------------------------------------------传入
//       仅支持 00 和 01 内置和外置，非00和01 返回00
//
//出口:  返回温度
INT16S mod_ntc3380_getMeasTemp(INT8U sersonType);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称:  温度扫描模块
//功能:  开启ADC采集、温度转换和采集超时保护。
//入口:  无
//出口:  无
void mod_ntc3380_scanTask(void);
//------------------------------E N D-------------------------------------------

#define NONE_ERROR           0x00
#define INSIDE_ERROR         0x01
#define OUTSIDE_ERROR        0x02
#define ALL_ERROR            0x03
//Task
//名称:  返回传感器错误
//功能:  
//入口:  无
//出口:  0x00 无错误， INSIDE_ERROR 内置传感器错误  
//                    OUTSIDE_ERROR 外置传感器错误  
//                        ALL_ERROR 两个传感器都错误
INT8U mod_ntc3380_error(void);



#define SENSOR_TYPE_3380_10K   00
#define SENSOR_TYPE_3950_05K   01
//#define SENSOR_TYPE_3950_10K   02
void mod_ntc3380_outsideSensorType(INT8U type);
#endif
//-----------------------Mod_ADC_NTC.h--END-----------------------------------

