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
}realTime_t;


realTime_t *app_real_time_getTime(void);
bool app_real_time_getVaildStatus(void);
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: app_softRtc_scanTask
//功能: 软时钟运行,main定时调用
//入口: 无
//出口: 无
void app_softRtc_sacnTask(void);
//------------------------------E N D-------------------------------------------
#endif



//--------------------------APP.ThreeFanOut.h--END--------------------------------------