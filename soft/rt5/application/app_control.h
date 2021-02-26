//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_CONTROL_H
#define _APP_CONTROL_H
//------------------------------E N D-------------------------------------------
#include "..\.\application\app_cfg.h"
#include "..\.\application\app_preheating.h"


#define STEP_TEMP                  5
#define MIN_SET_TEMP               50


#define MAX_MAX_SET_TEMP           900
#define MIN_MAX_SET_TEMP           50
#define MAX_MIN_SET_TEMP           900
#define MIN_MIN_SET_TEMP           50


#define MAX_DIF_TEMP               100
#define MIN_DIF_TEMP               5


#define MAX_LTP_TEMP               100
#define MIN_LTP_TEMP               45


#define MAX_ADJ_TEMP               100
#define MIN_ADJ_TEMP               -95



#define MAX_LINK_RELAY_DELAY       5
#define MIN_LINK_RELAY_DELAY       0



#define LIMIT_TEMP_PROTECT_DIF     30


#define MAX_LIF_TEMP               600
#define MIN_LIF_TEMP               160


#define MAX_COMFORTABLE_SET_TEMP   350
#define MIN_COMFORTABLE_SET_TEMP   50


#define MAX_SAVE_SET_TEMP          350
#define MIN_SAVE_SET_TEMP          50


#define MAX_LAST_TIME              240
#define MIN_LAST_TIME              5



#define MAX_WEEK_PRG               PROGRAM_7 
#define MIN_WEEK_PRG               PROGRAM_OFF 


#define MAX_RELAY_OUT_TYPE         RELAY_OUT_TYPE_MAIN_P_ASSIST_N_OFF 
#define MIN_RELAY_OUT_TYPE         RELAY_OUT_TYPE_MAIN_P_ASSIST_P  


#define FAC_OW_ENABLE              true
#define MAX_OW_CHECK_TIME          30
#define MIN_OW_CHECK_TIME          2
#define FAC_OW_CHECK_TIME          15
#define MAX_OW_DOWN_TEMP           40
#define MIN_OW_DOWN_TEMP           20
#define FAC_OW_DOWN_TEMP           30
#define MAX_OW_BACK_TIME           60
#define MIN_OW_BACK_TIME           10
#define FAC_OW_BACK_TIME           30
#define FAC_PREHEAT_ENABLE         false
#define FAC_CLEAN_CFG              false

#define FAC_TIME_CONTROL_CFG       false
#define FAC_DEMO_CFG               false





#define FAC_RELAY_OUT_TYPE_CFG     RELAY_SAME_DIRECTION




#define MIN_QUICK_TEMP             5
#define FAC_QUICK_TEMP             220

#define  ms_200                    200
#define  ms_500                    500
#define  ms_1000                   1000
#define  ms_1500                   1500
#define  ms_2000                   2000
#define  ms_3000                   3000
#define  ms_5000                   5000
#define  ms_7000                   7000
#define  ms_10000                  10000
#define  ms_12000                  12000
#define  ms_15000                  15000
#define  ms_20000                  20000
#define  ms_30000                  30000
#define  ms_50000                  50000
#define  ms_55000                  55000
#define  ms_60000                  60000

   




typedef enum
{
    SYS_STATUS_POWER_OFF = ((uint8_t)0x00), 
    SYS_STATUS_RUN_MODE,
    SYS_STATUS_HIGH_SETTING,
    SYS_STATUS_SHOW_VERSIONS,
    SYS_STATUS_SHOW_ALL, 
    SYS_STATUS_SMARTLINK,
    SYS_STATUS_RTC_SET,
    SYS_STATUS_PRG_SET,
    SYS_STATUS_AI_ADJ_SET,
}systemRunStatus_t;

typedef enum
{
    HIGH_SETTING_NTC_ADJ = ((uint8_t)0x00),
    HIGH_SETTING_NTC_MODE_SECELT,
    HIGH_SETTING_MAX_SET_TEMP,
    HIGH_SETTING_MIN_SET_TEMP,
    HIGH_SETTING_DIF,
    HIGH_SETTING_POF,
    HIGH_SETTING_LIF,
    HIGH_SETTING_LTP,
    HIGH_SETTING_PRG,
    HIGH_SETTING_RELAY_OUT_TYPE,
    HIGH_SETTING_LINK_DELAY_TIME,
    HIGH_SETTING_WINDOW_CHECK_SWITCH,
    HIGH_SETTING_WINDOW_CHECK_TIME_CYC,
    HIGH_SETTING_WINDOW_CHECK_DOWN_TEMP,
    HIGH_SETTING_WINDOW_CHECK_BACK_TIME,
    HIGH_SETTING_WINDOW_PREHEAT_SWITCH,
    HIGH_SETTING_NTC_TYPE_SECELT,  
    HIGH_SETTING_SAVE_SET_TEMP,
    HIGH_SETTING_FAC,    
    HIGH_SETTING_MAX_INDEX,       
}highSetMenu_t;

typedef enum
{  
    FAC_IDLE = 0x00,
    FAC_START,
    FAC_BUSY,
    FAC_FINISH,
}FACStatus_t;

typedef struct
{
    bool               disAdjTempFlag:1;
    bool               blinkFlag:1;
    highSetMenu_t      menu; 
    FACStatus_t        facStatus; 
}highSet_t;

typedef enum
{
    WORK_MODE_SUN  = 0x00,
    WORK_MODE_MOON = 0x01,
}workMode_t;

typedef enum
{
    TEMP_CONTROL_MODE_MANUAL    = 0x00 ,
    TEMP_CONTROL_MODE_TIME             ,
    TEMP_CONTROL_MODE_TEMPORARY        ,
}tempControlMode_t;

typedef enum
{
    WIFI_LINK_IDLE    = ((uint8_t)0x00),
    WIFI_LINK_DELAY,
    WIFI_LINK_BUSY,                      
    WIFI_LINK_FAIL,
    WIFI_LINK_SUCCESS,
}linkStatus;                               

typedef struct
{
    linkStatus           linkStatus;   
    uint8_t              linkDis;
    uint8_t              linkType;
    bool                 onLine;
}wifi_t;

typedef enum
{
    SENSOR_IN = ((uint8_t)0x00),
    SENSOR_OUT,
    SENSOR_ALL,      
}sensorMode_t;

typedef enum
{
    B3380_10K = ((uint8_t)0x00),
    B3950_5K,     
}sensorType_t;

typedef enum
{
    KEY_STATUS_UNLOCK  = 0x00,
    KEY_STATUS_LOCKED ,
    KEY_STATUS_BLINK  ,
}lockStatus_t;

typedef enum
{
    TIMMING_STATUS_IDLE = 0x00,
    TIMMING_STATUS_SET,
    TIMMING_STATUS_RUN,
}timmingStatus_t;

typedef enum
{
    TIMMING_SET_DDR_OFF = 0x00,
    TIMMING_SET_DDR_ON,
}timmingSetDdr_t;

typedef struct
{//超时
    timmingStatus_t      status;
    timmingSetDdr_t      setDdr;   
    uint8_t              halfHour;
    bool                 disFlag;
}timming_t;

typedef enum
{
    TIME_CON_STATUS_IDLE = 0x00,
    TIME_CON_STATUS_RUN,
    TIME_CON_STATUS_REST,
}timeContorlStatus_t;

typedef struct
{//超时
    timeContorlStatus_t status; 
    uint8_t             runMinute;  
    bool                disTimeControlFlag;
}timeContorl_t;


typedef struct
{//超时
    bool      flag; 
    uint16_t  delayTime;                    
}timeOutEvent_t;

typedef enum
{//超时
    RELAY_OUT_TYPE_MAIN_P_ASSIST_P = 0X00,
    RELAY_OUT_TYPE_MAIN_P_ASSIST_N,
    RELAY_OUT_TYPE_MAIN_N_ASSIST_N,
    RELAY_OUT_TYPE_MAIN_P_ASSIST_N_OFF,                
}relayOutType_t;

typedef enum
{       
    PROGRAM_5 = ((uint8_t)0x00),   
    PROGRAM_6,
    PROGRAM_7,         
}weekProgramCfg_t;


typedef union
{
    uint16_t hourMinute;
    struct
    {
        uint8_t hour   ;      
        uint8_t minute ;    
        int16_t temp   ;
    };   
}interval_t;

typedef struct
{//超时
    bool      switch_flag; 
    uint8_t   check_time;
    int16_t   down_temp;
    uint8_t   back_time;
}window_t;

typedef struct
{//超时
    uint8_t   menu; 
    bool      enbaleBlinkFlag;    
}rtcSet_t;

typedef struct
{//超时
    uint8_t   menu; 
    bool      enbaleBlinkFlag;    
}prgSet_t;

typedef enum
{    
    AI_ADJ_MENU_T = 0,  
    AI_ADJ_MENU_R,      
}aiAdjSetMenu_t;

typedef struct
{    
    int16_t                  t_max;  
    int16_t                  r_max;
}aiAdjSetPara_t;

typedef struct
{
    systemRunStatus_t      sysRunStatus          ;       
    lockStatus_t           lockStatus            ;       
    uint8_t                linkRelayDelayTime    ;    
    bool                   powerOnOffFlag:1      ;
    bool                   autoGotoFac:1         ;    
    int16_t                manSetTemp            ;
    int16_t                inAdjTemp             ;
    int16_t                outAdjTemp            ;                 
    int16_t                maxSetTemp            ;
    int16_t                minSetTemp            ;
    int16_t                diffTemp              ;      
    int16_t                lifSetTemp            ; 
    int16_t                ltpSetTemp            ;
    int16_t                saveSetTemp           ;
    sensorMode_t           sensorMode            ; 
    sensorType_t           sensorType            ;
    relayOutType_t         relayOutType          ;
    weekProgramCfg_t       weekPrg               ;
    interval_t             interval[6]           ;    
    window_t               window                ;
    tempControlMode_t      tempControlMode       ;
    preheatPara_t          preheat               ; 
    aiAdjSetPara_t         aiAdjSetPara          ;
}RecordPara_t; 

typedef struct
{                         
    int16_t              runMeasTemp;            //运行测量温度
    int16_t              runSetTemp;             //运行设定温度
    int16_t              runLifTemp;             //运行测量外限温度    
    bool                 floorRelayFlag:1;       //地暖输出             
    bool                 dispUpdataEvent:1;      //定时刷新
    bool                 blinkEvent:1;           //500ms秒闪动
    bool                 blink_1000ms_flag:1;    
    bool                 blink_RTC_TEMP_flag:1;    
    bool                 systemUpdataEvent:1; 
    bool                 fast_set_blink_flag:1;
    highSet_t            highSet;
    rtcSet_t             rtcSet;
    prgSet_t             prgSet;
    wifi_t               wifi;                                      
    uint8_t              systemError;    
    uint8_t              autoGotoFacTime;
    aiAdjSetMenu_t       aiAdjSetMenu;
    RecordPara_t         record;                   
}SysPara_t;






//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//function
//名称  : controler_getSysParaPt
//功能  : 传出系统参数指针
//input : 无             
//output: SysPara_t类型指针首地址----------------------------->>传出
SysPara_t * controler_getSysParaPt(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//function
//名称  : controler_getSysParaPt
//功能  : 传出存储参数指针
//input : 无             
//output: RecordPara_t类型指针首地址----------------------------->>传出
RecordPara_t * controler_getSysParaRecordPt(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//task
//名称  : app_control_scanTask
//功能  : 控制台运行扫描任务，所有参数的计算和刷新
//input : 无             
//output: 无
void app_control_scanTask(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    DELAY_CONTROLER_COMMON = ((uint8_t)0x00),  
    DELAY_AUTO_GOTO_FAC,
    DELAY_HIGH_SET,
    DELAY_TIMMING_BLINK_RTC_TEMP,
    DELAY_LOCK_BLNK_STATUS_LAST_TIME,
    DELAY_TIME_CONTROL_TIME,
    DELAY_FAC,
    DELAY_HIGH_SET_BLINK_STOP,
    DELAY_TIME_RTC_SET,
    DELAY_TIME_PRG_SET,
    DELAY_FAST_SET_BLINK_TIME,
    DELAY_TIMER_MAX
}delayTime_t;
//function
//名称  : app_control_timeOut_get
//功能  : 判断是否超时
//input :              _in_type<<----------------------------->>传入
//output:true超时       bool类型 ----------------------------->>传出
bool app_control_timeOut_get(delayTime_t type);
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//function
//名称  : app_control_timeOut_set
//功能  : 判断是否超时
//input :              _in_type<<----------------------------->>传入
//                  _in_delayMs<<----------------------------->>传入
//output:无
//void app_control_timeOut_set(delayTime_t _in_type,uint16_t _in_delayMs);
void app_control_timeOut_set(delayTime_t type,uint16_t milliscond);
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//init
//名称  : APP_control_init
//功能  : 控制台初始化
//input : 无
//output: 无
void app_control_init(void);
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//function
//名称  : app_con_gotoFac
//功能  : 系统参数恢复出厂设置
//input : 无
//output: 无
void app_con_gotoFac(void);
//------------------------------E N D-------------------------------------------

void app_con_tempLegalProtect(int16_t max,int16_t min,int16_t *_out_setTemp);

#if defined(__TUYA716__) || defined(__TUYA716_5G__)
int16_t app_con_getSoftAdjTemp(void);
#endif

#endif