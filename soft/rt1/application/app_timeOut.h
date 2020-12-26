//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_TIMEOUT_H
#define _APP_TIMEOUT_H
//------------------------------E N D-------------------------------------------
#define ms_50     50
#define ms_200    200
#define ms_500    500
#define ms_1000   1000
#define ms_1500   1500
#define ms_2000   2000
#define ms_3000   3000
#define ms_4000   4000
#define ms_5000   5000
#define ms_20000  20000
typedef struct
{
    bool      flag; 
    uint16_t  time;                    
}timeOut_t;

typedef enum
{
    TIME_OUT_COMMON = ((uint8_t)0x00), //公共定时器，用于各类模块退出
    TIME_OUT_HIGH_SET,                 //高级设置的校温和恢复出厂设置使用
    TIME_OUT_READ_RTC,                 //实时时钟读取使用
    TIME_OUT_PARA_SET,                 //设置界面
    TIME_OUT_RUN_MODE,
    TIME_OUT_RTC_SET,
    TIME_OUT_MAX,
}timeOutType_t;




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func
//名称: app_timeOut_getStatus
//功能: 是否超时判断
//入口:                     超时类型type<<---------------------传入 
//出口: true:超时  false:未超时
bool app_timeOut_getStatus(timeOutType_t type);
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func
//名称: app_timeOut_setTime
//功能: 设置超时时间
//入口:                   超时类型type<<----------------------传入
//                        超时时间time<<----------------------传入 
//出口: 无
void app_timeOut_setTime(timeOutType_t type,uint16_t time);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: app_timeOut_scanTask
//功能: 超时扫描任务
//入口: 无
//出口: 无
void app_timeOut_scanTask(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task
//名称: app_timeOut_fixCyc
//功能: 固定时间判断
//入口: *_out_lastTick --------------------->>传出
//         delayMsCyc  <<---------------------传入
//出口: 无
bool app_timeOut_fixCyc(uint16_t *_out_lastTick,uint16_t delayMsCyc);
//------------------------------E N D-------------------------------------------
uint16_t app_timeOut_passTime(uint16_t lastTick);
bool app_timeOut_blink500ms(void);
#endif



//--------------------------APP.RTC.h--END--------------------------------------