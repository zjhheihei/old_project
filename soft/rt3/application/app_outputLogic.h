#ifndef _APP_OUTPUTLOGIC_H
#define _APP_OUTPUTLOGIC_H

typedef enum
{
     OUTPUT_NULL = ((uint8_t)0x00),
     OUTPUT_OPEN,
     OUTPUT_CLOSE,
}output_t;

typedef enum
{
     DDR_COLD = ((uint8_t)0x00),
     DDR_WARM,   
}outputDDR_t;


typedef enum
{
    SET_WIND_SPEED_AI = 0x00,
    SET_WIND_SPEED_LOW,
    SET_WIND_SPEED_MID,
    SET_WIND_SPEED_HIGH,   
}setWindSpeed_t;
#define MAX_SET_WIND_SPEED SET_WIND_SPEED_HIGH  
#define MIN_SET_WIND_SPEED SET_WIND_SPEED_AI

typedef enum
{
    RUN_WIND_SPEED_CLOLSE= ((uint8_t)0x00),
    RUN_WIND_SPEED_LOW,
    RUN_WIND_SPEED_MID,
    RUN_WIND_SPEED_HIGH,
}runWindSpeed_t;
runWindSpeed_t app_out_getWindSpeed(void);


////++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
////func  
////名称: app_output_floorLogic
////功能: 地板逻辑
////入口: 无                         <<-------------------------- 传入     
////出口: true 输出开启   false 输出关闭  
//bool app_out_fanOut(void);
////------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func  
//名称: app_output_radiationRun
//功能: 辐射输出
//入口: 无                         <<-------------------------- 传入     
//出口: true 输出开启   false 输出关闭  
bool app_output_radiationRun(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func  
//名称: app_output_set_radiation_off
//功能: 设置强制关闭输出
//入口: 无                          
//出口: 无
void app_output_set_radiation_off(void);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func  
//名称: app_outputLogic_scanTask
//功能: 逻辑运算扫描任务：获取运算的参数
//入口: 无  
//出口: 无  
void app_outputLogic_scanTask(void);
//------------------------------E N D-------------------------------------------

#endif



//--------------------------APP.ThreeFanOut.h--END--------------------------------------