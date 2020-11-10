//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_CONTROL_H
#define _APP_CONTROL_H
//------------------------------E N D-------------------------------------------
#include "..\.\application\app_cfg.h"
#include "..\.\application\app_preheating.h"


#define STEP_TEMP                  5

#define FAC_SET_TEMP               220
#define MIN_SET_TEMP               50


#define MAX_MAX_SET_TEMP           900
#define MIN_MAX_SET_TEMP           50
#define FAC_MAX_SET_TEMP           350

#define MAX_DIF_TEMP               100
#define MIN_DIF_TEMP               5
#define FAC_DIF_TEMP               10

#define MAX_ADJ_TEMP               100
#define MIN_ADJ_TEMP               -95
#define FAC_ADJ_TEMP               0

#define MAX_LOW_PROTECT_TEMP       100
#define MIN_LOW_PROTECT_TEMP       45
#define FAC_LOW_PROTECT_TEMP       45

#define MAX_LINK_RELAY_DELAY       5
#define MIN_LINK_RELAY_DELAY       0
#define FAC_LINK_RELAY_DELAY       1


#define LIMIT_TEMP_PROTECT_DIF     30


#define MAX_LIF_TEMP               600
#define MIN_LIF_TEMP               160
#define FAC_LIF_TEMP               350


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
#define FAC_POF_CFG                false
#define FAC_DEMO_CFG               false
#define FAC_PRG_CFG                PROGRAM_OFF
#define FAC_RELAY_OUT_TYPE_CFG     RELAY_SAME_DIRECTION
#define FAC_TEMP_CONTROL_CFG       TEMP_CONTROL_MODE_MANUAL      

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
#define  ms_30000                  30000
#define  ms_60000                  60000

#define BOX_RUN_MODE_COLD          0x0000
#define BOX_RUN_MODE_WARM          0x0001


#if defined(__TUYA7X3__)
#define FAC_SERSON_CFG   SENSOR_IN 
#elif defined(__TUYA716__)
#define FAC_SERSON_CFG   SENSOR_ALL
#elif defined(__TUYA716_5G__)
#define FAC_SERSON_CFG   SENSOR_ALL
#elif defined(__TUYA7X3_5G__)
#define FAC_SERSON_CFG   SENSOR_IN
#endif
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************系统状态******************************************
typedef enum
{
    SYS_STATUS_POWER_OFF = ((uint8_t)0x00), 
    SYS_STATUS_RUN_MODE,
    SYS_STATUS_HIGH_SETTING,
    SYS_STATUS_SHOW_VERSIONS,
    SYS_STATUS_SHOW_ALL, 
    SYS_STATUS_SMARTLINK,
    SYS_STATUS_PROGRAM,
    SYS_STATUS_RTCSET,
    SYS_STATUS_AISET,
}systemRunStatus_t;

typedef enum
{
    HIGH_SETTING_NTC_ADJ= ((uint8_t)0x00),
    HIGH_SETTING_OUTPUT_TPYE,
    HIGH_SETTING_LINK_DELAY,
    HIGH_SETTING_SERSON_TYPE,
    HIGH_SETTING_LIF,   
    HIGH_SETTING_DIF,
    HIGH_SETTING_LTP, 
    HIGH_SETTING_MAX_SET, 
    HIGH_SETTING_WEEK_PRG,
    HIGH_SETTING_QUICK,
    HIGH_SETTING_POWUP_OFF,
    HIGH_SETTING_DESCALE,
    HIGH_SETTING_DEMO, 
    HIGH_SETTING_OW_ENABNE,
    HIGH_SETTING_OW_CHECK_TIME,
    HIGH_SETTING_OW_CHECK_TEMP,
    HIGH_SETTING_OW_BACK_TIME,
    HIGH_SETTING_PREHEAT_ENABLE,
    HIGH_SETTING_SENSOR_TYEP,
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
    highSetMenu_t      menu; 
    FACStatus_t        facStatus; 
}highSet_t;
//****************************高级设置******************************************
//------------------------------E N D-------------------------------------------
typedef enum
{
    TEMP_CONTROL_MODE_MANUAL = ((uint8_t)0x00), 
    TEMP_CONTROL_MODE_TIME,
    TEMP_CONTROL_MODE_TEMPORARY,         
}tempControlMode_t;

typedef enum
{
    PROGRAM_OFF = ((INT8U)0x00),         
    PROGRAM_5,   
    PROGRAM_6,
    PROGRAM_7,         
}weekPrg_t; 


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

typedef union
{
    uint16_t hourMinute;
    struct
    {
        uint8_t hour;//帧长度，不包括前导码
        uint8_t minute;//协议类型       
        int16_t temp;
    };   
}interval_t;

typedef union
{
    uint16_t hourMinute;
    struct
    {
        uint8_t hour;
        uint8_t minute;    
        uint8_t week;
        uint8_t second;
    };   
}rtc_t;




  

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************传感器、控温模式、锁金和485******************************
typedef enum
{
    SENSOR_IN = ((uint8_t)0x00),
    SENSOR_OUT,
    SENSOR_ALL,      
}senCfg_t;

typedef enum
{
    SENSOR_3380_10K = ((uint8_t)0x00),
    SENSOR_3950_5K,
    //SENSOR_3950_10K,      
}senType_t;

typedef enum
{
    NONE_LOCK = 0x00,
    KEY_LOCK,
    //REMOTE_LOCK,
}lockStatus_t;

;

typedef struct
{//超时
    bool      flag; 
    uint16_t  delayTime;                    
}timeOutEvent_t;

typedef enum
{
    RELAY_SAME_DIRECTION=((uint8_t)0x00),//主辅同向
    ASSIST_RELAY_REVERSE,               //主辅反向
    RELAY_ALL_REVERSE,                 //主辅同时反向
    MAIN_RELAY_REVERSE,
}outputType_t;
//*********************传感器、控温模式、锁金和485******************************
//------------------------------E N D-------------------------------------------



typedef struct
{
    bool      enableFlag;
    uint8_t   checkTime;
    int16_t   downTemp;
    uint8_t   backTime;
}owPara_t;

typedef enum
{
    OW_STATUS_INIT    = ((uint8_t)0x00),
    OW_STATUS_DELAY,
    OW_STATUS_CHECK,
    OW_STATUS_RUN,
}owStatus_t;

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

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************系统参数******************************************
typedef struct
{
    systemRunStatus_t      sysRunStatus;       
    lockStatus_t           lockStatus;
    tempControlMode_t      tempControlMode;
    uint8_t                linkRelayDelayTime;             
    bool                   powerOnOffFlag:1;
    bool                   cleanFlag:1;
    bool                   demoFlag:1;
    int16_t                ltpTemp;          
    int16_t                manSetTemp;
    int16_t                inAdjTemp;
    int16_t                outAdjTemp;                 
    int16_t                maxSetTemp;
    int16_t                diffTemp;
    int16_t                quickTemp;
    int16_t                lifTemp;
    senCfg_t               sersonType;
    interval_t             interval[8];
    weekPrg_t              weekPrg;
    outputType_t           outputType;
    owPara_t               owPara;
    preheatPara_t          preheat; 
    senType_t              senMode;
    aiAdjSetPara_t         aiAdjSetPara;
}RecordPara_t; 

typedef struct
{                         
    int16_t       runMeasTemp;      
    int16_t       runSetTemp; 
    int16_t       runLifTemp;
    bool          disKeepFlag:1;
    bool          floorRelayFlag:1;                   
    bool          dispUpdataEvent:1;
    bool          blink_5second_flag:1;
    bool          blinkEvent:1;
    bool          systemUpdataEvent:1;  
    highSet_t     highSet;
    wifi_t        wifi;
    rtc_t         rtc;
    uint8_t       rtcSetMenu; 
    uint8_t       weekPrgMainMenu;
    uint8_t       weekPrgSonMenu;                                      
    uint8_t       systemError;
    owStatus_t    owStatus;
    aiAdjSetMenu_t aiAdjSetMenu;
    RecordPara_t  record;                   
}SysPara_t;
//*****************************系统参数******************************************
//------------------------------E N D-------------------------------------------




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
    DELAY_DIS_KEEP,
    DELAY_BILINK_5SECOND,
    DELAY_FAC,
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
void app_con_gotoFac(bool allFlag);
//------------------------------E N D-------------------------------------------



#if defined(__TUYA716__) || defined(__TUYA716_5G__)
int16_t app_con_getSoftAdjTemp(void);
#endif

#endif