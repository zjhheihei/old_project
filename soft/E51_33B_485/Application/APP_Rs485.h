//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_RS485_H
#define _APP_RS485_H
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 传出RS485通信状态
//功能: 显示模块调用显示通讯图标
//入口: 无
//出口: BN_TRUE代表正在通讯
BOOLEAN APP_Rs485_PutOutStatus(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: 按键扫描模块
//功能: 用于扫描按键，主函定时数调用。
//入口: 无
//出口: 无
void APP_Rs485_ScanTask(void);
//------------------------------E N D-------------------------------------------


#endif



//--------------------------APP.RTC.h--END--------------------------------------