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
#define FAC_SYSTEM_UPDATA_EVENT    false 
#define FAC_TIMMING_TIME_FALG      false 
#define FAC_HIGH_SETTING_MENU      HIGH_SETTING_NTC_ADJ
#define FAC_WIFI_LINK_STATUS       WIFI_LINK_IDLE
#define FAC_WIFI_DIS_CNT           0 
#define FAC_WIFI_LINK_TYPE         SMART_CONFIG
#define FAC_WIFI_ON_LINE           false
#define FAC_SYSTEM_ERROR           NONE_ERROR
#define FAC_TIMMING_STATUS         TIMMING_STATUS_IDLE
#define FAC_TIMMING_SET_DDR        TIMMING_SET_DDR_OFF
#define FAC_TIMMING_HALF_HOUR      0
#define FAC_TIME_CONTROL_STATUS    TIME_CON_STATUS_IDLE
#define FAC_TIME_CONTROL_RUN_MINUTE 0
#define FAC_QUICK_SET_CFG          WORK_MODE_SUN  
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
#else
#define FAC_OUTPUT_TIME_CONTROL_FLAG         false
#define FAC_OUTPUT_OPEN_LAST_TIME   5
#define FAC_OUTPUT_CLOSE_LAST_TIME  5
#define FAC_SERSON_CFG   SENSOR_IN
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
    .highSet.disAdjTempFlag     = false,
    .highSet.menu               = FAC_HIGH_SETTING_MENU,          
    .highSet.facStatus          = FAC_IDLE,       
    .wifi.linkStatus            = FAC_WIFI_LINK_STATUS,
    .wifi.linkDis               = FAC_WIFI_DIS_CNT,
    .wifi.linkType              = FAC_WIFI_LINK_TYPE,
    .wifi.onLine                = FAC_WIFI_ON_LINE,      
    .systemError                = FAC_SYSTEM_ERROR,
    .timming.status             = FAC_TIMMING_STATUS,
    .timming.setDdr             = FAC_TIMMING_SET_DDR,
    .timming.halfHour           = FAC_TIMMING_HALF_HOUR,
    .timming.disFlag            = FAC_TIMMING_TIME_FALG,
    .timeContorl.status         = FAC_TIME_CONTROL_STATUS,
    .timeContorl.runMinute      = FAC_TIME_CONTROL_RUN_MINUTE,
    .workMode                   = FAC_QUICK_SET_CFG, 
    .tempControlMode            = FAC_TIME_CON_CFG,  
    .autoGotoFacTime            = 0,
    
    .record.sysRunStatus        = FAC_RUN_SYS_STATUS,
    .record.lockStatus          = FAC_LOCK_STATUS,   
    .record.linkRelayDelayTime  = FAC_LINK_RELAY_DELAY,  
    .record.powerOnOffFlag      = FAC_POF_CFG,  
    .record.outputTimeControFlag = FAC_OUTPUT_TIME_CONTROL_FLAG,
    .record.autoGotoFac         = false,
    .record.manSetTemp          = FAC_SET_TEMP,     
    .record.inAdjTemp           = FAC_ADJ_TEMP,     
    .record.outAdjTemp          = FAC_ADJ_TEMP,   
    .record.maxSetTemp          = FAC_MAX_SET_TEMP, 
    .record.minSetTemp          = FAC_MIN_SET_TEMP,     
    .record.diffTemp            = FAC_DIF_TEMP,
    .record.sunSetTemp          = FAC_COMFORTABLE_SET_TEMP,
    .record.moonSetTemp         = FAC_SAVE_SET_TEMP,
    .record.outputOpenLastTime  = FAC_OUTPUT_OPEN_LAST_TIME,
    .record.outputCloseLastTime = FAC_OUTPUT_CLOSE_LAST_TIME,
    .record.lifSetTemp          = FAC_LIF_TEMP,
    .record.ltpSetTemp          = FAC_LTP_TEMP,
    .record.sensorMode          = FAC_SERSON_CFG,
    .record.sensorType          = FAC_3380_10K,    
    .record.relayOutType        = FAC_RELAY_OUT_TYPE,
    .record.weekPrg             = FAC_WEEK_PRG,
    .record.interval[INT0].hour       = FAC_INTERVAL0_HOUR,
    .record.interval[INT0].minute     = FAC_INTERVAL0_MINUTE,
    .record.interval[INT0].temp       = FAC_INTERVAL0_TEMP,
    
    .record.interval[INT1].hour       = FAC_INTERVAL1_HOUR,
    .record.interval[INT1].minute     = FAC_INTERVAL1_MINUTE,
    .record.interval[INT1].temp       = FAC_INTERVAL1_TEMP,
    
    .record.interval[INT2].hour       = FAC_INTERVAL2_HOUR,
    .record.interval[INT2].minute     = FAC_INTERVAL2_MINUTE,
    .record.interval[INT2].temp       = FAC_INTERVAL2_TEMP,
    
    .record.interval[INT3].hour       = FAC_INTERVAL3_HOUR,
    .record.interval[INT3].minute     = FAC_INTERVAL3_MINUTE,
    .record.interval[INT3].temp       = FAC_INTERVAL3_TEMP,

    .record.interval[INT4].hour       = FAC_INTERVAL4_HOUR,
    .record.interval[INT4].minute     = FAC_INTERVAL4_MINUTE,
    .record.interval[INT4].temp       = FAC_INTERVAL4_TEMP,
    
    .record.interval[INT5].hour       = FAC_INTERVAL5_HOUR,
    .record.interval[INT5].minute     = FAC_INTERVAL5_MINUTE,
    .record.interval[INT5].temp       = FAC_INTERVAL5_TEMP,
    
    .record.interval[INT6].hour       = FAC_INTERVAL6_HOUR,
    .record.interval[INT6].minute     = FAC_INTERVAL6_MINUTE,
    .record.interval[INT6].temp       = FAC_INTERVAL6_TEMP,
    
    .record.interval[INT7].hour       = FAC_INTERVAL7_HOUR,
    .record.interval[INT7].minute     = FAC_INTERVAL7_MINUTE,
    .record.interval[INT7].temp       = FAC_INTERVAL7_TEMP,
};



void app_con_gotoFac(void)
{  
    if(SYS_STATUS_TIMING_SET < s_sysPara.record.sysRunStatus)
    {
        s_sysPara.record.sysRunStatus         = SYS_STATUS_POWER_OFF;
    }   
    //s_sysPara.record.lockStatus         = KEY_STATUS_UNLOCK,
    s_sysPara.workMode                         = FAC_QUICK_SET_CFG;    
    s_sysPara.tempControlMode                  =   FAC_TIME_CON_CFG;   
    s_sysPara.record.linkRelayDelayTime        = FAC_LINK_RELAY_DELAY;   
    //s_sysPara.record.autoGotoFac               = true;  
    s_sysPara.record.powerOnOffFlag            = FAC_POF_CFG;  
    s_sysPara.record.outputTimeControFlag      = FAC_OUTPUT_TIME_CONTROL_FLAG;
    //s_sysPara.record.timeControlFlag     = FAC_TIME_CONTROL_CFG;    
    s_sysPara.record.manSetTemp                = FAC_SET_TEMP;               
    //s_sysPara.record.inAdjTemp           = FAC_ADJ_TEMP,  
    //s_sysPara.record.outAdjTemp          = FAC_ADJ_TEMP,
    s_sysPara.record.maxSetTemp                = FAC_MAX_SET_TEMP; 
    s_sysPara.record.minSetTemp                = FAC_MIN_SET_TEMP; 
    s_sysPara.record.diffTemp                  = FAC_DIF_TEMP;
    s_sysPara.record.sunSetTemp                = FAC_COMFORTABLE_SET_TEMP;
    s_sysPara.record.moonSetTemp               = FAC_SAVE_SET_TEMP;
    s_sysPara.record.outputOpenLastTime        = FAC_OUTPUT_OPEN_LAST_TIME;
    s_sysPara.record.outputCloseLastTime       = FAC_OUTPUT_CLOSE_LAST_TIME;
    s_sysPara.record.lifSetTemp                = FAC_LIF_TEMP;
    s_sysPara.record.ltpSetTemp                = FAC_LTP_TEMP;
    s_sysPara.record.sensorMode                = FAC_SERSON_CFG;   
    s_sysPara.record.sensorType                = FAC_3380_10K;
    s_sysPara.record.relayOutType              = FAC_RELAY_OUT_TYPE;
    s_sysPara.record.weekPrg                   = FAC_WEEK_PRG;
    s_sysPara.record.interval[INT0].hour       = FAC_INTERVAL0_HOUR;
    s_sysPara.record.interval[INT0].minute     = FAC_INTERVAL0_MINUTE;
    s_sysPara.record.interval[INT0].temp       = FAC_INTERVAL0_TEMP;
    
    s_sysPara.record.interval[INT1].hour       = FAC_INTERVAL1_HOUR;
    s_sysPara.record.interval[INT1].minute     = FAC_INTERVAL1_MINUTE;
    s_sysPara.record.interval[INT1].temp       = FAC_INTERVAL1_TEMP;
    
    s_sysPara.record.interval[INT2].hour       = FAC_INTERVAL2_HOUR;
    s_sysPara.record.interval[INT2].minute     = FAC_INTERVAL2_MINUTE;
    s_sysPara.record.interval[INT2].temp       = FAC_INTERVAL2_TEMP;
    
    s_sysPara.record.interval[INT3].hour       = FAC_INTERVAL3_HOUR;
    s_sysPara.record.interval[INT3].minute     = FAC_INTERVAL3_MINUTE;
    s_sysPara.record.interval[INT3].temp       = FAC_INTERVAL3_TEMP;
    
    s_sysPara.record.interval[INT4].hour       = FAC_INTERVAL4_HOUR;
    s_sysPara.record.interval[INT4].minute     = FAC_INTERVAL4_MINUTE;
    s_sysPara.record.interval[INT4].temp       = FAC_INTERVAL4_TEMP;
    
    s_sysPara.record.interval[INT5].hour       = FAC_INTERVAL5_HOUR;
    s_sysPara.record.interval[INT5].minute     = FAC_INTERVAL5_MINUTE;
    s_sysPara.record.interval[INT5].temp       = FAC_INTERVAL5_TEMP;
    
    s_sysPara.record.interval[INT6].hour       = FAC_INTERVAL6_HOUR;
    s_sysPara.record.interval[INT6].minute     = FAC_INTERVAL6_MINUTE;
    s_sysPara.record.interval[INT6].temp       = FAC_INTERVAL6_TEMP;
    
    s_sysPara.record.interval[INT7].hour       = FAC_INTERVAL7_HOUR;
    s_sysPara.record.interval[INT7].minute     = FAC_INTERVAL7_MINUTE;
    s_sysPara.record.interval[INT7].temp       = FAC_INTERVAL7_TEMP;    
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

#if defined(__TUYA716__) || defined(__TUYA716_5G__)
typedef struct
{ 
    uint8_t                  step;
    uint16_t                 runTime;
    uint8_t                  addCyc;
    int16_t                  runTemp;
}softAdjTempPara_t;

static softAdjTempPara_t softAdjTempPara; 


#define THREE_MINUTES        18

#define MAX_RUN_ADJ_TEMP     60

int16_t app_con_getSoftAdjTemp(void)
{
    return softAdjTempPara.runTemp;
}

void app_control_measTempAdj(void)
{
    static uint16_t lastTick = 0;          
    uint16_t passTick = GetSysTickMillisecond() - lastTick;
    if(ms_10000 <= passTick)
    {
        lastTick = GetSysTickMillisecond();
        if(MAX_RUN_ADJ_TEMP > softAdjTempPara.runTemp)
        {
            if(++softAdjTempPara.addCyc >= THREE_MINUTES)
            {
                softAdjTempPara.addCyc = 0;
                softAdjTempPara.runTemp += 1;
            }
        }  
        else
        {
            softAdjTempPara.addCyc = 0;
        }
    }   
}
#endif



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
    #if defined(__TUYA716__) || defined(__TUYA716_5G__)
    app_control_measTempAdj();
    if(SENSOR_OUT != s_sysPara.record.sensorMode)
    {//单外置不减温度
        s_sysPara.runMeasTemp -= softAdjTempPara.runTemp;
    }   
    #endif    
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
    switch(s_sysPara.tempControlMode)
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
    app_con_tempLegalProtect(s_sysPara.record.maxSetTemp,s_sysPara.record.minSetTemp,&s_sysPara.runSetTemp);
    app_con_tempLegalProtect(s_sysPara.record.maxSetTemp,s_sysPara.record.minSetTemp,&s_sysPara.record.sunSetTemp);
    app_con_tempLegalProtect(s_sysPara.record.maxSetTemp,s_sysPara.record.minSetTemp,&s_sysPara.record.moonSetTemp);      
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
            
            //if(app_control_timeOut_get(DELAY_TIMMING))
            //{
                if(TIMMING_STATUS_RUN == s_sysPara.timming.status)
                {
                    if(app_control_timeOut_get(DELAY_TIMMING))
                    {
                        app_control_timeOut_set(DELAY_TIMMING,ms_5000);
                        if(s_sysPara.timming.disFlag)
                        {
                            s_sysPara.timming.disFlag = false;
                        }
                        else
                        {
                            s_sysPara.timming.disFlag = true;
                        }
                    }                   
                }
                else
                {
                    s_sysPara.timming.disFlag = false;
                }
            //}
            if(TIME_CON_STATUS_IDLE != s_sysPara.timeContorl.status)
            {
                if(app_control_timeOut_get(DELAY_TIME_CONTROL_TIME))
                {
                    if(s_sysPara.timeContorl.disTimeControlFlag)
                    {
                        s_sysPara.timeContorl.disTimeControlFlag = false;
                        //#ifdef NDEBUG
                        app_control_timeOut_set(DELAY_TIME_CONTROL_TIME,ms_50000); 
                        //#else
                        //app_control_timeOut_set(DELAY_TIME_CONTROL_TIME,ms_10000); 
                        //#endif
                    }
                    else 
                    {
                        s_sysPara.timeContorl.disTimeControlFlag = true;
                        app_control_timeOut_set(DELAY_TIME_CONTROL_TIME,ms_5000); 
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
        case SYS_STATUS_TIMING_SET:
        {
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {
                if(s_sysPara.timming.halfHour)
                {
                    s_sysPara.timming.status = TIMMING_STATUS_RUN;
                    app_control_timeOut_set(DELAY_TIMMING,ms_5000);
                }
                
                if(TIMMING_SET_DDR_OFF == s_sysPara.timming.setDdr)
                {
                    s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;                   
                }
                else
                {
                    s_sysPara.record.sysRunStatus = SYS_STATUS_POWER_OFF;
                    s_sysPara.timming.disFlag = true;
                }
            }
            
            if(app_control_timeOut_get(DELAY_TIMMING))
            {
                if(TIMMING_STATUS_SET == s_sysPara.timming.status)
                {
                    s_sysPara.timming.status = TIMMING_STATUS_IDLE;
                }
            }
            break;
        }
        default:break;
    }
    
//    if(app_control_timeOut_get(DELAY_BILINK_5SECOND))
//    {
//        app_control_timeOut_set(DELAY_BILINK_5SECOND,ms_5000);
//        if(s_sysPara.blink_5second_flag)
//        {
//            s_sysPara.blink_5second_flag = false;
//        }
//        else
//        {
//            s_sysPara.blink_5second_flag = true;
//        }
//    }
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
    
    if(TIMMING_STATUS_IDLE == s_sysPara.timming.status)
    {
        if(s_sysPara.timming.disFlag)
        {
            s_sysPara.timming.disFlag = false;
        }
    }
}

void app_control_scanTask(void)
{
    app_control_scan_measTemp();      //更新测量温度
    APP_control_update_system();      //状态更新
    APP_control_timeOutEvent_scan();  //超时时间扫描
    app_control_updata_500ms_Blink(); //闪动事件发送   
    system_status_check();
}






