#ifndef _APP_OUTPUTLOGIC_H
#define _APP_OUTPUTLOGIC_H



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func  
//名称: app_output_floorLogic
//功能: 地板逻辑
//入口: 无                         <<-------------------------- 传入     
//出口: true 输出开启   false 输出关闭  
bool app_output_floorLogic(void);
bool app_output_ltpLogic(bool lastLtpFlag);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func  
//名称: app_outputLogic_scanTask
//功能: 逻辑运算扫描任务：获取运算的参数
//入口: 无  
//出口: 无  
void app_outputLogic_scanTask(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func  
//名称: app_logic_runAtMakeWarmMode
//功能: 获取制热模式
//入口: 无  
//出口: 无  
bool app_logic_runAtMakeWarmMode(void);
//------------------------------E N D-------------------------------------------

#endif



//--------------------------APP.ThreeFanOut.h--END--------------------------------------