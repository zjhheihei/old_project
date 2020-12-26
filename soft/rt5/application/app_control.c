/*******************************************************************************
*
*	模块名称 : APP控制台
*	文件名称 : APP.control.c
*	版    本 : V1.0
*	说    明 : 1、处理逻辑
*                  
*                 
*	修改记录 :
*		版本号  日期        作者         说明
*		V1.0   2019-08-27  zhoujh&Linzl  正式发布
*	Copyright (C), 2015-2020,  menredGroup
*
*******************************************************************************/
#include ".\APP_Cfg.h"
/******************************************************************************/ 
static timeOutEvent_t timeOutEvent[DELAY_TIMER_MAX];
preheatPara_t preheatPara;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++

#define FAC_RUN_MEAS_TEMP          220
#define FAC_RUN_SET_TEMP           220
#define FAC_RUN_LIF_TEMP           220
#define FAC_FLOOR_RELAY            false
#define FAC_DISUPDATA_EVENT        false
#define FAC_BLINK_EVENT            false
#define FAC_BLINK_RTC_TEMP         false
#define FAC_SYSTEM_UPDATA_EVENT    false 

#define FAC_HIGH_SETTING_ADJ_FLAG  false
#define FAC_HIGH_SETTING_MENU      HIGH_SETTING_NTC_ADJ
#define FAC_HIGH_SETTING_BLINK_FALG false

#define FAC_HIGH_SETTING_RTC_SET_MUNU   0 
#define FAC_HIGH_SETTING_RTC_SET_BLINK   true
#define FAC_HIGH_SETTING_PRG_SET_MUNU   0 
#define FAC_HIGH_SETTING_PRG_SET_BLINK   true

#define FAC_WIFI_LINK_STATUS       WIFI_LINK_IDLE
#define FAC_WIFI_DIS_CNT           0 
#define FAC_WIFI_LINK_TYPE         SMART_CONFIG
#define FAC_WIFI_ON_LINE           false
#define FAC_SYSTEM_ERROR           NONE_ERROR

 
#define FAC_TIME_CON_CFG           TEMP_CONTROL_MODE_MANUAL 

#define FAC_RUN_SYS_STATUS         SYS_STATUS_POWER_OFF
#define FAC_LOCK_STATUS            KEY_STATUS_UNLOCK
#define FAC_LINK_RELAY_DELAY       1
#define FAC_POF_CFG                false


#ifdef __OUT_SIDE_CONTROL__
#define FAC_OUTPUT_TIME_CONTROL_FLAG         true
#define FAC_OUTPUT_OPEN_LAST_TIME   90
#define FAC_OUTPUT_CLOSE_LAST_TIME  10
#define FAC_SERSON_CFG   SENSOR_ALL
#define FAC_T_MAX                   30
#define FAC_R_MAX                   25
#else
#define FAC_OUTPUT_TIME_CONTROL_FLAG         false
#define FAC_OUTPUT_OPEN_LAST_TIME   5
#define FAC_OUTPUT_CLOSE_LAST_TIME  5
#define FAC_SERSON_CFG   SENSOR_IN
#ifdef __WIFI_CONTROL__
#define FAC_T_MAX                   10
#ifdef __RT5_36i__
#define FAC_R_MAX                   15
#else
#define FAC_R_MAX                   10
#endif
#else
#define FAC_T_MAX                   0
#define FAC_R_MAX                   0
#endif
#endif


#define FAC_SET_TEMP               220
#define FAC_ADJ_TEMP               0
#define FAC_MAX_SET_TEMP           350
#define FAC_MIN_SET_TEMP           100
#define FAC_DIF_TEMP               10
#define FAC_COMFORTABLE_SET_TEMP   220
#define FAC_SAVE_SET_TEMP          160


#define FAC_LIF_TEMP               420
#define FAC_LTP_TEMP               MIN_LTP_TEMP
#define FAC_3380_10K               B3380_10K
#define FAC_WEEK_PRG               PROGRAM_7
#define FAC_RELAY_OUT_TYPE         MIN_RELAY_OUT_TYPE

#define FAC_WINDOW_SWITCH          true
#define FAC_WINDOW_CHECK_TIME      15
#define FAC_WINDOW_DOWN_TEMP       30
#define FAC_WINDOW_BACK_TIME       30
#define FAC_PREHEAT                false
#define FAC_TEMP_CON_MODE          TEMP_CONTROL_MODE_MANUAL




SysPara_t s_sysPara =
{    
    .runMeasTemp                = FAC_RUN_MEAS_TEMP,
    .runSetTemp                 = FAC_RUN_SET_TEMP,
    .runLifTemp                 = FAC_RUN_LIF_TEMP,
    .floorRelayFlag             = FAC_FLOOR_RELAY,
    .dispUpdataEvent            = FAC_DISUPDATA_EVENT,
    .blinkEvent                 = FAC_BLINK_EVENT,   
    .blink_1000ms_flag          = FAC_BLINK_EVENT,
    .systemUpdataEvent          = FAC_SYSTEM_UPDATA_EVENT, 
    .blink_RTC_TEMP_flag        = FAC_BLINK_RTC_TEMP,
    .highSet.disAdjTempFlag     = FAC_HIGH_SETTING_ADJ_FLAG,
    .highSet.blinkFlag          = FAC_HIGH_SETTING_BLINK_FALG,
    .highSet.menu               = FAC_HIGH_SETTING_MENU,          
    .highSet.facStatus          = FAC_IDLE,  
    .rtcSet.menu                = FAC_HIGH_SETTING_RTC_SET_MUNU,
    .rtcSet.enbaleBlinkFlag     = FAC_HIGH_SETTING_RTC_SET_BLINK,
    .prgSet.menu                = FAC_HIGH_SETTING_PRG_SET_MUNU,
    .prgSet.enbaleBlinkFlag     = FAC_HIGH_SETTING_PRG_SET_BLINK,   
    .wifi.linkStatus            = FAC_WIFI_LINK_STATUS,
    .wifi.linkDis               = FAC_WIFI_DIS_CNT,
    .wifi.linkType              = FAC_WIFI_LINK_TYPE,
    .wifi.onLine                = FAC_WIFI_ON_LINE,      
    .systemError                = FAC_SYSTEM_ERROR, 
    .autoGotoFacTime            = 0,
    
    .record.sysRunStatus        = FAC_RUN_SYS_STATUS,
    .record.lockStatus          = FAC_LOCK_STATUS,   
    .record.linkRelayDelayTime  = FAC_LINK_RELAY_DELAY,  
    .record.powerOnOffFlag      = FAC_POF_CFG,      
    .record.autoGotoFac         = false,
    .record.manSetTemp          = FAC_SET_TEMP,     
    .record.inAdjTemp           = FAC_ADJ_TEMP,     
    .record.outAdjTemp          = FAC_ADJ_TEMP,   
    .record.maxSetTemp          = FAC_MAX_SET_TEMP, 
    .record.minSetTemp          = FAC_MIN_SET_TEMP,     
    .record.diffTemp            = FAC_DIF_TEMP,
    .record.lifSetTemp          = FAC_LIF_TEMP,
    .record.ltpSetTemp          = FAC_LTP_TEMP,
    .record.sensorMode          = FAC_SERSON_CFG,
    .record.sensorType          = FAC_3380_10K,    
    .record.relayOutType        = FAC_RELAY_OUT_TYPE,
    .record.weekPrg             = FAC_WEEK_PRG,
    .aiAdjSetMenu               = AI_ADJ_MENU_T,
    .record.interval[WORK_INT0].hourMinute = FAC_WORK_INT0_HM,
    .record.interval[WORK_INT0].temp       = FAC_WORK_INT0_TEMP,
    .record.interval[WORK_INT1].hourMinute = FAC_WORK_INT1_HM,
    .record.interval[WORK_INT1].temp       = FAC_WORK_INT1_TEMP,
    .record.interval[WORK_INT2].hourMinute = FAC_WORK_INT2_HM,
    .record.interval[WORK_INT2].temp       = FAC_WORK_INT2_TEMP,
    .record.interval[WORK_INT3].hourMinute = FAC_WORK_INT3_HM,
    .record.interval[WORK_INT3].temp       = FAC_WORK_INT3_TEMP,
    .record.interval[REST_INT0].hourMinute = FAC_REST_INT0_HM,
    .record.interval[REST_INT0].temp       = FAC_REST_INT0_TEMP,
    .record.interval[REST_INT1].hourMinute = FAC_REST_INT1_HM,
    .record.interval[REST_INT1].temp       = FAC_REST_INT1_TEMP,
    
    .record.window.switch_flag = FAC_WINDOW_SWITCH,
    .record.window.check_time  = FAC_WINDOW_CHECK_TIME,
    .record.window.down_temp   = FAC_WINDOW_DOWN_TEMP,
    .record.window.back_time   = FAC_WINDOW_BACK_TIME,
    .record.tempControlMode    = FAC_TEMP_CON_MODE,
    .record.preheat            = FAC_PREHEAT,
    .record.aiAdjSetPara.t_max = FAC_T_MAX,
    .record.aiAdjSetPara.r_max = FAC_R_MAX,
};



void app_con_gotoFac(void)
{  
    if(SYS_STATUS_PRG_SET < s_sysPara.record.sysRunStatus)
    {
        s_sysPara.record.sysRunStatus         = SYS_STATUS_POWER_OFF;
    }   
    //s_sysPara.record.sysRunStatus        = FAC_RUN_SYS_STATUS   ;
    //s_sysPara.record.lockStatus          = FAC_LOCK_STATUS      ;   
    s_sysPara.record.linkRelayDelayTime  = FAC_LINK_RELAY_DELAY ;  
    s_sysPara.record.powerOnOffFlag      = FAC_POF_CFG          ;      
    //s_sysPara.record.autoGotoFac         = false                ;
    s_sysPara.record.manSetTemp          = FAC_SET_TEMP         ;     
    //s_sysPara.record.inAdjTemp           = FAC_ADJ_TEMP         ;     
    //s_sysPara.record.outAdjTemp          = FAC_ADJ_TEMP         ;   
    s_sysPara.record.maxSetTemp          = FAC_MAX_SET_TEMP     ;
    s_sysPara.record.minSetTemp          = FAC_MIN_SET_TEMP     ;    
    s_sysPara.record.diffTemp            = FAC_DIF_TEMP         ;
    s_sysPara.record.lifSetTemp          = FAC_LIF_TEMP         ;
    s_sysPara.record.ltpSetTemp          = FAC_LTP_TEMP         ;
    s_sysPara.record.sensorMode          = FAC_SERSON_CFG       ;
    s_sysPara.record.sensorType          = FAC_3380_10K         ;   
    s_sysPara.record.relayOutType        = FAC_RELAY_OUT_TYPE   ;
    s_sysPara.record.weekPrg             = FAC_WEEK_PRG         ;
    
    s_sysPara.record.interval[WORK_INT0].hourMinute = FAC_WORK_INT0_HM;
    s_sysPara.record.interval[WORK_INT0].temp       = FAC_WORK_INT0_TEMP;
    s_sysPara.record.interval[WORK_INT1].hourMinute = FAC_WORK_INT1_HM;
    s_sysPara.record.interval[WORK_INT1].temp       = FAC_WORK_INT1_TEMP;
    s_sysPara.record.interval[WORK_INT2].hourMinute = FAC_WORK_INT2_HM;
    s_sysPara.record.interval[WORK_INT2].temp       = FAC_WORK_INT2_TEMP;
    s_sysPara.record.interval[WORK_INT3].hourMinute = FAC_WORK_INT3_HM;
    s_sysPara.record.interval[WORK_INT3].temp       = FAC_WORK_INT3_TEMP;
    s_sysPara.record.interval[REST_INT0].hourMinute = FAC_REST_INT0_HM;
    s_sysPara.record.interval[REST_INT0].temp       = FAC_REST_INT0_TEMP;
    s_sysPara.record.interval[REST_INT1].hourMinute = FAC_REST_INT1_HM;
    s_sysPara.record.interval[REST_INT1].temp       = FAC_REST_INT1_TEMP;
    
    s_sysPara.record.window.switch_flag = FAC_WINDOW_SWITCH;
    s_sysPara.record.window.check_time  = FAC_WINDOW_CHECK_TIME;
    s_sysPara.record.window.down_temp   = FAC_WINDOW_DOWN_TEMP;
    s_sysPara.record.window.back_time   = FAC_WINDOW_BACK_TIME;
    s_sysPara.record.tempControlMode    = FAC_TEMP_CON_MODE;
    s_sysPara.record.preheat.switchflag = FAC_PREHEAT;
    s_sysPara.record.preheat.addTime[0] = 420;
    s_sysPara.record.preheat.addTime[1] = 420;
    s_sysPara.record.preheat.addTime[2] = 420;
    s_sysPara.record.preheat.addTime[3] = 420;
    s_sysPara.record.preheat.addTime[4] = 420;
    s_sysPara.record.preheat.addTime[5] = 420;
}

SysPara_t * controler_getSysParaPt(void)
{
    return &s_sysPara;
}

RecordPara_t * controler_getSysParaRecordPt(void)
{
    return &s_sysPara.record;
}
//***********************系统参数和存储参数接口*********************************
//------------------------------E N D-------------------------------------------







void app_control_init(void)
{    
    if(s_sysPara.record.powerOnOffFlag)
    {
        s_sysPara.record.sysRunStatus = SYS_STATUS_POWER_OFF; 
        s_sysPara.record.lockStatus = KEY_STATUS_UNLOCK;
    }
    else
    {
        if((SYS_STATUS_POWER_OFF == s_sysPara.record.sysRunStatus) || (SYS_STATUS_HIGH_SETTING == s_sysPara.record.sysRunStatus))
        {
            s_sysPara.record.sysRunStatus = SYS_STATUS_POWER_OFF; 
            s_sysPara.record.lockStatus = KEY_STATUS_UNLOCK;
        }
        else
        {
            s_sysPara.record.sysRunStatus = SYS_STATUS_SHOW_ALL;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);   
        }       
    } 
    if(KEY_STATUS_BLINK == s_sysPara.record.lockStatus)
    {
        s_sysPara.record.lockStatus = KEY_STATUS_LOCKED;
    }
    app_control_timeOut_set(DELAY_TIMMING_BLINK_RTC_TEMP,ms_5000);
}



void measTempCorrect(int16_t *_out_Temp)
{
    static int16_t LastMeasureTemp = 220;
    int16_t CurrentMeasureTemp = 0;
    int16_t ErrTemp = 0;
    int16_t IntegerTemp = 0;
    int16_t DecimalTemp = 0;
    CurrentMeasureTemp = *_out_Temp;
    
    if(CurrentMeasureTemp > LastMeasureTemp)
    {
        ErrTemp = CurrentMeasureTemp - LastMeasureTemp;         
        DecimalTemp = ErrTemp % 10;
        IntegerTemp = ErrTemp - DecimalTemp;
        
        if(4 <= DecimalTemp)
        {
            if(9 <= DecimalTemp)
            {
                LastMeasureTemp = LastMeasureTemp + IntegerTemp + 10;  
            }
            else
            {
                LastMeasureTemp = LastMeasureTemp + IntegerTemp + 5;   
            }
        }
        else
        {
            LastMeasureTemp =  LastMeasureTemp + IntegerTemp;
        }         
    }
    else if(CurrentMeasureTemp < LastMeasureTemp)
    {
        ErrTemp = LastMeasureTemp - CurrentMeasureTemp;
        DecimalTemp = ErrTemp % 10;
        IntegerTemp = ErrTemp - DecimalTemp;
      
        if(4 <= DecimalTemp)
        {
            if(9 <= DecimalTemp)
            {
                LastMeasureTemp = LastMeasureTemp - IntegerTemp - 10;  
            }
            else
            {
                LastMeasureTemp = LastMeasureTemp - IntegerTemp - 5;   
            }
        }
        else
        {
            LastMeasureTemp =  LastMeasureTemp - IntegerTemp;
        }                                       
    }    
    *_out_Temp = LastMeasureTemp;
}

void measTempCorrect_out(int16_t *_out_Temp)
{
    static int16_t LastMeasureTemp = 220;
    int16_t CurrentMeasureTemp = 0;
    int16_t ErrTemp = 0;
    int16_t IntegerTemp = 0;
    int16_t DecimalTemp = 0;
    CurrentMeasureTemp = *_out_Temp;
    
    if(CurrentMeasureTemp > LastMeasureTemp)
    {
        ErrTemp = CurrentMeasureTemp - LastMeasureTemp;         
        DecimalTemp = ErrTemp % 10;
        IntegerTemp = ErrTemp - DecimalTemp;
        
        if(4 <= DecimalTemp)
        {
            if(9 <= DecimalTemp)
            {
                LastMeasureTemp = LastMeasureTemp + IntegerTemp + 10;  
            }
            else
            {
                LastMeasureTemp = LastMeasureTemp + IntegerTemp + 5;   
            }
        }
        else
        {
            LastMeasureTemp =  LastMeasureTemp + IntegerTemp;
        }         
    }
    else if(CurrentMeasureTemp < LastMeasureTemp)
    {
        ErrTemp = LastMeasureTemp - CurrentMeasureTemp;
        DecimalTemp = ErrTemp % 10;
        IntegerTemp = ErrTemp - DecimalTemp;
      
        if(4 <= DecimalTemp)
        {
            if(9 <= DecimalTemp)
            {
                LastMeasureTemp = LastMeasureTemp - IntegerTemp - 10;  
            }
            else
            {
                LastMeasureTemp = LastMeasureTemp - IntegerTemp - 5;   
            }
        }
        else
        {
            LastMeasureTemp =  LastMeasureTemp - IntegerTemp;
        }                                       
    }    
    *_out_Temp = LastMeasureTemp;
}

typedef struct
{ 
    uint8_t                  t_cyc;
    uint8_t                  r_cyc;
    int16_t                  t_runTemp;
    int16_t                  r_runTemp;
}aiAdjRunPara_t;

static aiAdjRunPara_t aiAdjRunPara; 


#define THREE_MINUTES          3  
#define MAX_RUN_T_ADJ_TEMP     30

int16_t get_aiAdjRunTemp(void)
{
    int16_t retAdjTemp = 0;
    retAdjTemp = aiAdjRunPara.t_runTemp + aiAdjRunPara.r_runTemp;
    return retAdjTemp; 
}

void aiAdjTemp_handle(void)
{
    static uint16_t adj_tick = 0;    
    uint16_t passTick = GetSysTickMillisecond() - adj_tick;
    if(ms_60000 <= passTick)
    {
        adj_tick = GetSysTickMillisecond();       
        if(s_sysPara.record.aiAdjSetPara.t_max > aiAdjRunPara.t_runTemp)
        {      
            aiAdjRunPara.t_cyc++;
            if(THREE_MINUTES <= aiAdjRunPara.t_cyc)
            {
                aiAdjRunPara.t_cyc = 0;
                aiAdjRunPara.t_runTemp += 1;
            }
        }  
        else
        {           
            aiAdjRunPara.t_cyc = 0;
        }
        aiAdjRunPara.r_cyc++;
        if(THREE_MINUTES <= aiAdjRunPara.r_cyc)
        {
            aiAdjRunPara.r_cyc = 0;
            if(s_sysPara.floorRelayFlag)
            {
                if(s_sysPara.record.aiAdjSetPara.r_max > aiAdjRunPara.r_runTemp)
                {                   
                    aiAdjRunPara.r_runTemp += 1;
                }
            }
            else
            {
                if(0 < aiAdjRunPara.r_runTemp)
                {                   
                    aiAdjRunPara.r_runTemp -= 1;
                }
            }
        }                              
    } 
    if(s_sysPara.record.aiAdjSetPara.t_max < aiAdjRunPara.t_runTemp)
    {
        aiAdjRunPara.t_runTemp = s_sysPara.record.aiAdjSetPara.t_max;
    }
    if(s_sysPara.record.aiAdjSetPara.r_max < aiAdjRunPara.r_runTemp)
    {
        aiAdjRunPara.r_runTemp = s_sysPara.record.aiAdjSetPara.r_max;
    }
}



void app_control_updata_measTemp(void)
{   
    int16_t iTemp,oTemp;
    uint8_t error = NONE_ERROR;
    
    iTemp = mod_ntc3380_getMeasTemp(INSIDE_SERSON);
    oTemp = mod_ntc3380_getMeasTemp(OUTSIDE_SERSON);
    error = mod_ntc3380_error();   
    switch(s_sysPara.record.sensorMode)
    {
        case SENSOR_IN:
        {
            if(INSIDE_ERROR & error)
            {
                s_sysPara.systemError = INSIDE_ERROR;
                s_sysPara.runMeasTemp = 995;
            }
            else
            {
                s_sysPara.runMeasTemp = iTemp + s_sysPara.record.inAdjTemp;
                s_sysPara.systemError = NONE_ERROR;          
            }
            s_sysPara.runLifTemp = 995;
            break;
        }
        case SENSOR_OUT:
        {
            if(OUTSIDE_ERROR & error)
            {
                s_sysPara.systemError = OUTSIDE_ERROR;
                s_sysPara.runMeasTemp = 995;                
            }
            else
            {
                s_sysPara.runMeasTemp = oTemp + s_sysPara.record.outAdjTemp;
                s_sysPara.systemError = NONE_ERROR;                  
            } 
            s_sysPara.runLifTemp = s_sysPara.runMeasTemp;
            break;
        }
        case SENSOR_ALL:
        {
            if(ALL_ERROR == error)
            {
                s_sysPara.runMeasTemp = 995;
                s_sysPara.runLifTemp = 995;
                s_sysPara.runLifTemp = 995;
            }
            else if(INSIDE_ERROR & error)
            {                                       
                s_sysPara.runMeasTemp = 995;
                s_sysPara.runLifTemp = oTemp + s_sysPara.record.outAdjTemp;              
            } 
            else if(OUTSIDE_ERROR & error)
            {
                s_sysPara.runMeasTemp = iTemp + s_sysPara.record.inAdjTemp;
                s_sysPara.runLifTemp = 995;
            }
            else if(NONE_ERROR == error)
            {
                s_sysPara.runMeasTemp = iTemp + s_sysPara.record.inAdjTemp;
                s_sysPara.runLifTemp = oTemp + s_sysPara.record.outAdjTemp;
            }
            s_sysPara.systemError = error;          
        }
    }
    if(s_sysPara.runMeasTemp > 995)
    {
        s_sysPara.runMeasTemp = 995;
    }
    if(s_sysPara.runMeasTemp < -95)
    {
        s_sysPara.runMeasTemp = -95;
    }
    
    aiAdjTemp_handle();
    if(SENSOR_OUT != s_sysPara.record.sensorMode)
    {//单外置不减温度
        s_sysPara.runMeasTemp -= get_aiAdjRunTemp();
    }   
    measTempCorrect(&s_sysPara.runMeasTemp);
    measTempCorrect_out(&s_sysPara.runLifTemp);
}

void app_control_scan_measTemp(void)
{
    static uint16_t lastSystemMsTime = 0;     
    static uint16_t  period = 0;
    period = GetSysTickMillisecond() - lastSystemMsTime;
    if(period >= 50)
    {       
        lastSystemMsTime = GetSysTickMillisecond(); 
        mod_ntc3380_scanTask();
    }
}

void app_con_tempLegalProtect(int16_t max,int16_t min,int16_t *_out_setTemp)
{
    int16_t checkTemp = 0;
    checkTemp = *_out_setTemp;
    if(checkTemp > max)
    {
        checkTemp = max;
    }
    else if(checkTemp < min)
    {
        checkTemp = min;
    }
    *_out_setTemp = checkTemp;
}

void runSetTemp_handle(void)
{   
    switch(s_sysPara.record.tempControlMode)
    {
        case TEMP_CONTROL_MODE_MANUAL:
        {
            s_sysPara.runSetTemp = s_sysPara.record.manSetTemp; 
            break;
        }
        case TEMP_CONTROL_MODE_TIME:
        {
            s_sysPara.runSetTemp = s_sysPara.record.interval[app_inter_runInter()].temp;
            break;
        }
        default:break;
    } 
    if(TEMP_CONTROL_MODE_MANUAL == s_sysPara.record.tempControlMode) 
    {
        s_sysPara.runSetTemp = s_sysPara.record.manSetTemp;
    }
    else
    {
        uint8_t interval = app_inter_runInter();
        if(app_preheat_getStatus())
        {
            if(3 == interval)
            {
                s_sysPara.runSetTemp = s_sysPara.record.interval[0].temp;  
            }
            else if(5 == interval)
            {
                s_sysPara.runSetTemp = s_sysPara.record.interval[4].temp;  
            }
            else
            {
                s_sysPara.runSetTemp = s_sysPara.record.interval[interval + 1].temp;  
            }
            s_sysPara.record.tempControlMode = TEMP_CONTROL_MODE_TIME;               
        }
        else
        {
            if(TEMP_CONTROL_MODE_TIME == s_sysPara.record.tempControlMode) 
            {                    
                s_sysPara.runSetTemp = s_sysPara.record.interval[interval].temp;  
            }
        }
    }
    app_con_tempLegalProtect(s_sysPara.record.maxSetTemp,s_sysPara.record.minSetTemp,&s_sysPara.runSetTemp);
    //app_con_tempLegalProtect(s_sysPara.record.maxSetTemp,s_sysPara.record.minSetTemp,&s_sysPara.record.sunSetTemp);
    //app_con_tempLegalProtect(s_sysPara.record.maxSetTemp,s_sysPara.record.minSetTemp,&s_sysPara.record.moonSetTemp);      
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************500mS闪动和系统参数更新*********************************
void app_control_updata_500ms_Blink(void)
{
    static uint16_t lastSystemMsTime = 0;     
    static uint16_t period = 0;
    static uint8_t  period_1000ms = 0;
    period = GetSysTickMillisecond() - lastSystemMsTime;
    if(period >= 500)
    {       
        lastSystemMsTime = GetSysTickMillisecond(); 
        s_sysPara.dispUpdataEvent = true;
        s_sysPara.systemUpdataEvent = true;             
        if(++s_sysPara.wifi.linkDis > 9)
        {
            s_sysPara.wifi.linkDis = 0;
        }
        if(++period_1000ms > 1)
        {
            period_1000ms = 0;
            if(s_sysPara.blink_1000ms_flag)
            {
                s_sysPara.blink_1000ms_flag = false;
            }
            else
            {
                s_sysPara.blink_1000ms_flag = true;
            }          
        }
        
        if(s_sysPara.blinkEvent)
        {
            s_sysPara.blinkEvent = false;
        }
        else
        {
            s_sysPara.blinkEvent = true;
        }
    }
}

void APP_control_update_system(void)
{
    static  systemRunStatus_t lastRunStatus = SYS_STATUS_POWER_OFF;
    if(s_sysPara.systemUpdataEvent)
    {//500ms刷新一次
        s_sysPara.systemUpdataEvent = false;
        app_control_updata_measTemp();   //测量温度
        runSetTemp_handle();       //设定温度                
    }
    else
    {
        if(lastRunStatus != s_sysPara.record.sysRunStatus)
        {
            lastRunStatus = s_sysPara.record.sysRunStatus;
            s_sysPara.dispUpdataEvent = true;
            s_sysPara.systemUpdataEvent = true;        
        }
    }
}
//***************************系统和闪动*****************************************
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************超时时间扫描****************************************
bool app_control_timeOut_get(delayTime_t type)
{
    if(timeOutEvent[type].flag)
    {
        timeOutEvent[type].flag = false;
        return true;
    }
    return false;
}

void app_control_timeOut_set(delayTime_t type,uint16_t milliscond)
{
    timeOutEvent[type].flag = false;
    timeOutEvent[type].delayTime = milliscond;       
}

void app_control_timeOut_highSetHandle(void)
{  
    if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
    {
        if(HIGH_SETTING_NTC_ADJ != s_sysPara.highSet.menu)
        {
            s_sysPara.record.sysRunStatus = SYS_STATUS_POWER_OFF;
        }       
    }
    
    if(app_control_timeOut_get(DELAY_AUTO_GOTO_FAC))
    {
      
        if((!s_sysPara.record.autoGotoFac) && (HIGH_SETTING_NTC_ADJ == s_sysPara.highSet.menu))
        {
            if(++s_sysPara.autoGotoFacTime >= 10)
            {
                s_sysPara.record.autoGotoFac = true;
                app_con_gotoFac();
            }
            app_control_timeOut_set(DELAY_AUTO_GOTO_FAC,ms_60000);
        }
    }
    
    if(app_control_timeOut_get(DELAY_HIGH_SET))
    {
        if(HIGH_SETTING_NTC_ADJ == s_sysPara.highSet.menu)
        {
            s_sysPara.highSet.disAdjTempFlag = false;
        }
        else if(HIGH_SETTING_FAC == s_sysPara.highSet.menu)
        {
            if(FAC_FINISH > s_sysPara.highSet.facStatus)
            {
                s_sysPara.highSet.facStatus++;
                app_control_timeOut_set(DELAY_HIGH_SET,ms_500);
            }
        }
    }
    if(app_control_timeOut_get(DELAY_HIGH_SET_BLINK_STOP))
    {
        s_sysPara.highSet.blinkFlag = true;
    }
}

void app_control_timeOut_wifiLinkHandle(void)
{
    static uint16_t checkOnLineTime = 0;
    static uint16_t checkOffLineTime = 0;
    if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
    {     
        if(WIFI_LINK_BUSY == s_sysPara.wifi.linkStatus)
        {
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);
            s_sysPara.wifi.linkStatus = WIFI_LINK_FAIL;
        }
        else if(WIFI_LINK_DELAY == s_sysPara.wifi.linkStatus)
        {
            s_sysPara.wifi.linkStatus = WIFI_LINK_BUSY;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);
        }
        else
        {
            s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;
        }
    }
    
    if(WIFI_LINK_DELAY == s_sysPara.wifi.linkStatus)
    {
        checkOnLineTime = 0;
        if(WIFI_CONN_CLOUD != app_pullTuyaWifiStateFormThisModule())
        {
            if(++checkOffLineTime > 10)
            {
                s_sysPara.wifi.linkStatus = WIFI_LINK_BUSY ;
                app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);
                checkOffLineTime = 0;
            }
//            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_2000);
//           // s_sysPara.wifi.linkStatus = WIFI_LINK_BUSY;
//            //app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);
        }
        else
        {
            checkOffLineTime = 0;
        }
    }
    else 
    {
        checkOffLineTime = 0;
        if((WIFI_LINK_BUSY == s_sysPara.wifi.linkStatus) || (WIFI_LINK_FAIL == s_sysPara.wifi.linkStatus)) 
        {
            if(++checkOnLineTime > 10)
            {
                if(WIFI_CONN_CLOUD == app_pullTuyaWifiStateFormThisModule())
                {
                    app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_3000);
                    s_sysPara.wifi.linkStatus = WIFI_LINK_SUCCESS;
                    s_sysPara.wifi.onLine = true;
                }
            }
        }
    }
}

void app_control_timeOutHandle(void)
{
    switch(s_sysPara.record.sysRunStatus)
    {
        case SYS_STATUS_POWER_OFF:break;       
        case SYS_STATUS_RUN_MODE:
        {
            if(app_control_timeOut_get(DELAY_LOCK_BLNK_STATUS_LAST_TIME))
            {
                if(KEY_STATUS_BLINK == s_sysPara.record.lockStatus)
                {
                    s_sysPara.record.lockStatus = KEY_STATUS_LOCKED;  
                }
            }    
            if(app_control_timeOut_get(DELAY_TIMMING_BLINK_RTC_TEMP))
            {
                app_control_timeOut_set(DELAY_TIMMING_BLINK_RTC_TEMP,ms_5000);
                {
                    if(s_sysPara.blink_RTC_TEMP_flag)
                    {
                        s_sysPara.blink_RTC_TEMP_flag = false;
                    }
                    else
                    {
                        s_sysPara.blink_RTC_TEMP_flag = true;
                    }                   
                }
            }    
            break;
        }
        case SYS_STATUS_HIGH_SETTING:
        {
            app_control_timeOut_highSetHandle();
            break;
        }
        case SYS_STATUS_SHOW_VERSIONS:
        {         
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {        
                s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;               
            }
            break;
        }
        case SYS_STATUS_SHOW_ALL:
        {
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {
                app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);
                s_sysPara.record.sysRunStatus = SYS_STATUS_SHOW_VERSIONS;
            }
            break;
        }       
        case SYS_STATUS_SMARTLINK:
        {
            app_control_timeOut_wifiLinkHandle();
            break;
        }     
        case SYS_STATUS_RTC_SET:
        {
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {                
                s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;
                app_real_time_write_rtc();
            }
            if(app_control_timeOut_get(DELAY_TIME_RTC_SET))
            {
                s_sysPara.rtcSet.enbaleBlinkFlag = true;
            }
            break;
        }
        case SYS_STATUS_PRG_SET:
        {
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {                
                s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;
            }
            if(app_control_timeOut_get(DELAY_TIME_PRG_SET))
            {
                s_sysPara.prgSet.enbaleBlinkFlag = true;
            }
            break;
        }
        case SYS_STATUS_AI_ADJ_SET:
        {
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {                
                s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;
            }
            break;
        }
        default:break;
    }
}

void APP_control_timeOutEvent_scan(void)
{   
    unsigned char i;
    static uint16_t lastSystemMsTime = 0;     
           uint16_t CurrentSystemMsTime = 0;
           uint16_t DiffMsTime = 0;          
    CurrentSystemMsTime = GetSysTickMillisecond();
    if(CurrentSystemMsTime >= lastSystemMsTime)
    {
        DiffMsTime = CurrentSystemMsTime - lastSystemMsTime;
    }
    else
    {
        DiffMsTime = CurrentSystemMsTime + 65535 - lastSystemMsTime;
    }
    
    if(0 != DiffMsTime)
    {
        lastSystemMsTime = CurrentSystemMsTime;
        for(i=0;i<DELAY_TIMER_MAX;i++)
        {                       
            if(timeOutEvent[i].delayTime>0)
            {   
                if(timeOutEvent[i].delayTime >= DiffMsTime)
                {
                    timeOutEvent[i].delayTime -= DiffMsTime;
                }
                else
                {
                    timeOutEvent[i].delayTime = 0;
                }
             
                if(timeOutEvent[i].delayTime==0)
                {
                    timeOutEvent[i].flag = true;                    
                }
            }
        }
    }
    app_control_timeOutHandle(); 
}

void system_status_check(void)
{
    if(SYS_STATUS_POWER_OFF == s_sysPara.record.sysRunStatus)
    {
        if(KEY_STATUS_UNLOCK != s_sysPara.record.lockStatus)
        {
            s_sysPara.record.lockStatus = KEY_STATUS_UNLOCK;
        }
    }
    
//    if(TIMMING_STATUS_IDLE == s_sysPara.timming.status)
//    {
//        if(s_sysPara.timming.disFlag)
//        {
//            s_sysPara.timming.disFlag = false;
//        }
//    }
}

void app_control_scanTask(void)
{
    app_control_scan_measTemp();      //更新测量温度
    APP_control_update_system();      //状态更新
    APP_control_timeOutEvent_scan();  //超时时间扫描
    app_control_updata_500ms_Blink(); //闪动事件发送   
    system_status_check();
}






