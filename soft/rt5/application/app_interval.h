//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_INTERVAL_H
#define _APP_INTERVAL_H
//------------------------------E N D-------------------------------------------

#define WEEK_PRG_MENU_WROK_INT0_HM       0
#define WEEK_PRG_MENU_WROK_INT0_TEMP     1
#define WEEK_PRG_MENU_WROK_INT1_HM       2
#define WEEK_PRG_MENU_WROK_INT1_TEMP     3
#define WEEK_PRG_MENU_WROK_INT2_HM       4
#define WEEK_PRG_MENU_WROK_INT2_TEMP     5
#define WEEK_PRG_MENU_WROK_INT3_HM       6
#define WEEK_PRG_MENU_WROK_INT3_TEMP     7
#define WEEK_PRG_MENU_REST_INT0_HM       8
#define WEEK_PRG_MENU_REST_INT0_TEMP     9
#define WEEK_PRG_MENU_REST_INT1_HM       10
#define WEEK_PRG_MENU_REST_INT1_TEMP     11

#define WEEK_7             0
#define WEEK_1             1
#define WEEK_2             2
#define WEEK_3             3
#define WEEK_4             4
#define WEEK_5             5
#define WEEK_6             6



#define FAC_WORK_INT0_HM           0x0600
#define FAC_WORK_INT0_TEMP         200

#define FAC_WORK_INT1_HM           0x0800
#define FAC_WORK_INT1_TEMP         150

#define FAC_WORK_INT2_HM           0x1100 //17:00
#define FAC_WORK_INT2_TEMP         150

#define FAC_WORK_INT3_HM           0x161E //22:30
#define FAC_WORK_INT3_TEMP         170

#define FAC_REST_INT0_HM           0x0800 //08:00
#define FAC_REST_INT0_TEMP         220

#define FAC_REST_INT1_HM           0x1700 //22:30
#define FAC_REST_INT1_TEMP         150
 

#define WORK_INT0     0
#define WORK_INT1     1
#define WORK_INT2     2
#define WORK_INT3     3
#define REST_INT0     4
#define REST_INT1     5




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
void app_interval_task(void);
//------------------------------E N D-------------------------------------------

#endif



//----------------------APP.TimeInterval.h--END---------------------------------