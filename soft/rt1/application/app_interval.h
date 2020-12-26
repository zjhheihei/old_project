//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_INTERVAL_H
#define _APP_INTERVAL_H
//------------------------------E N D-------------------------------------------

#define WEEK_PRG_MENU_MAIN_INT0     0
#define WEEK_PRG_MENU_MAIN_INT1     1
#define WEEK_PRG_MENU_MAIN_INT2     2
#define WEEK_PRG_MENU_MAIN_INT3     3
#define WEEK_PRG_MENU_MAIN_INT4     4
#define WEEK_PRG_MENU_MAIN_INT5     5
#define WEEK_PRG_MENU_MAIN_INT6     6
#define WEEK_PRG_MENU_MAIN_INT7     7

#define WEEK_PRG_MENU_SON_TIME  0
#define WEEK_PRG_MENU_SON_TEMP  1

#define WEEK_PRG_SET_NONE  0
#define WEEK_PRG_SET_MENU  1
#define WEEK_PRG_SET_PARA  2

#define DIS_WEEK_12345             7
#define DIS_WEEK_123456            8
#define DIS_WEEK_1234567           9
#define DIS_WEEK_67                10
#define DIS_WEEK_7                 11


#define FAC_INTERVAL0_HOUR_MINUTE  0x0600
#define FAC_INTERVAL0_HOUR         6
#define FAC_INTERVAL0_MINUTE       0
#define FAC_INTERVAL0_TEMP         200

#define FAC_INTERVAL1_HOUR_MINUTE  0x0800
#define FAC_INTERVAL1_HOUR         8
#define FAC_INTERVAL1_MINUTE       0
#define FAC_INTERVAL1_TEMP         150

#define FAC_INTERVAL2_HOUR_MINUTE  0x0b1E //11:30
#define FAC_INTERVAL2_HOUR         11 
#define FAC_INTERVAL2_MINUTE       30 
#define FAC_INTERVAL2_TEMP         150

#define FAC_INTERVAL3_HOUR_MINUTE  0x0c00 //12:00
#define FAC_INTERVAL3_HOUR         12 
#define FAC_INTERVAL3_MINUTE       0 
#define FAC_INTERVAL3_TEMP         170

#define FAC_INTERVAL4_HOUR_MINUTE  0x1100 //17:00
#define FAC_INTERVAL4_HOUR         17 //17:00
#define FAC_INTERVAL4_MINUTE       0  //17:00
#define FAC_INTERVAL4_TEMP         220

#define FAC_INTERVAL5_HOUR_MINUTE  0x161E //22:30
#define FAC_INTERVAL5_HOUR         22
#define FAC_INTERVAL5_MINUTE       30 
#define FAC_INTERVAL5_TEMP         150

#define FAC_INTERVAL6_HOUR_MINUTE  0x0800
#define FAC_INTERVAL6_HOUR         8
#define FAC_INTERVAL6_MINUTE       0
#define FAC_INTERVAL6_TEMP         200

#define FAC_INTERVAL7_HOUR_MINUTE  0x1700 //23:00
#define FAC_INTERVAL7_HOUR         23 
#define FAC_INTERVAL7_MINUTE       0 
#define FAC_INTERVAL7_TEMP         150  

#define INT0     0
#define INT1     1
#define INT2     2
#define INT3     3
#define INT4     4
#define INT5     5
#define INT6     6
#define INT7     7



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: app_interval_getInterval
//功能: 获取当前运行时段
//入口: 无
//出口: 0 1 2 3 4 5 工作日的6个时段  6 7休息日的两个时段
uint8_t app_inter_runInter(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: app_interval_scanTask
//功能: 时段计算扫描函数，定时调用
//入口: 无
//出口: 无
void app_interval_scanTask(void);
//------------------------------E N D-------------------------------------------

#endif



//----------------------APP.TimeInterval.h--END---------------------------------