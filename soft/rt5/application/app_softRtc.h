//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_SOFTRTC_H
#define _APP_SOFTRTC_H
//------------------------------E N D-------------------------------------------

#define WEEK_SUNDAY    0
#define WEEK_SATURDAY  6

#define SET_MENU_MINUTE  0 
#define SET_MENU_HOUR    1 
#define SET_MENU_WEEK    2 

#define MAX_SECOND       59
#define MIN_SECOND       0
#define FAC_SECOND       0
#define MAX_MINUTE       59
#define MIN_MINUTE       0
#define FAC_MINUTE       0
#define MAX_HOUR         23
#define MIN_HOUR         0
#define FAC_HOUR         12
#define MAX_WEEK         6
#define MIN_WEEK         0
#define FAC_WEEK         1





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
typedef union
{
    struct
    {
        uint8_t hour;      
        uint8_t minute;        
        uint8_t week;
        uint8_t second;
    };
    uint16_t hm;   
    uint16_t ws;
    bool  write_flag;
}realTime_t;
//func
//名称: app_real_time_getTime
//功能: 获取实时时钟指针
//入口: 无
//出口: realTime_t指针
realTime_t *app_real_time_getTime(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: app_softRtc_scanTask
//功能: 软时钟运行,main定时调用
//入口: 无
//出口: 无
void app_rtc_task(void);
//------------------------------E N D-------------------------------------------

void app_real_time_write_rtc(void);
#endif



//--------------------------APP.ThreeFanOut.h--END--------------------------------------