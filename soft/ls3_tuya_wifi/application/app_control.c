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
SysPara_t s_sysPara =
{    
    .runMeasTemp         = 220,
    .runSetTemp          = 220,
    .runLifTemp          = 220,
    .disKeepFlag         = false,
    .floorRelayFlag      = false,//地暖阀门输出    
    .dispUpdataEvent     = false,
    .blink_5second_flag  = false,
    .blinkEvent          = false,        
    .systemUpdataEvent   = false,   
    .highSet.menu = HIGH_SETTING_NTC_ADJ,          
    .highSet.facStatus = FAC_IDLE,       
    .wifi.linkStatus = WIFI_LINK_IDLE,
    .wifi.linkDis = 0,
    .wifi.linkType = SMART_CONFIG,
    .wifi.onLine = false,   
    .rtc.hour = 8,
    .rtc.minute = 0,
    .rtc.week = 4,
    .rtcSetMenu = SET_MENU_MINUTE,
    .weekPrgMainMenu = WEEK_PRG_MENU_MAIN_INT0,
    .weekPrgSonMenu = WEEK_PRG_MENU_SON_TIME,       
    .systemError            = NONE_ERROR,
    .owStatus               = OW_STATUS_INIT,
    .aiAdjSetMenu              = AI_ADJ_MENU_T,         
    .record.sysRunStatus       = SYS_STATUS_POWER_OFF,
    .record.lockStatus         = NONE_LOCK,
    .record.tempControlMode    = FAC_TEMP_CONTROL_CFG,    
    .record.linkRelayDelayTime = FAC_LINK_RELAY_DELAY,
    
    .record.powerOnOffFlag     = FAC_POF_CFG,
    .record.cleanFlag          = FAC_CLEAN_CFG,
    .record.demoFlag           = FAC_DEMO_CFG,
     
    .record.ltpTemp = FAC_LOW_PROTECT_TEMP,
    .record.manSetTemp = 200,               
    .record.inAdjTemp = FAC_ADJ_TEMP,  
    .record.outAdjTemp = FAC_ADJ_TEMP,
    .record.maxSetTemp = 350,        
    .record.diffTemp = FAC_DIF_TEMP,
    .record.quickTemp = FAC_QUICK_TEMP,
    .record.lifTemp   = FAC_LIF_TEMP,
    .record.sersonType = FAC_SERSON_CFG,
    .record.weekPrg = PROGRAM_OFF,
    .record.outputType = RELAY_SAME_DIRECTION,
    .record.owPara.enableFlag = FAC_OW_ENABLE,
    .record.owPara.checkTime = FAC_OW_CHECK_TIME,
    .record.owPara.downTemp = FAC_OW_DOWN_TEMP,
    .record.owPara.backTime = FAC_OW_BACK_TIME,
    .record.preheat.switchflag = FAC_PREHEAT_ENABLE,
    .record.preheat.addTime[0] = 420,  
    .record.preheat.addTime[1] = 420,
    .record.preheat.addTime[2] = 420,
    .record.preheat.addTime[3] = 420,
    .record.preheat.addTime[4] = 420,
    .record.preheat.addTime[5] = 420,
    .record.preheat.addTime[6] = 420,
    .record.preheat.addTime[7] = 420,
    
    
    .record.interval[INT0].hourMinute = FAC_INTERVAL0_HOUR_MINUTE,
    .record.interval[INT0].temp = FAC_INTERVAL0_TEMP,
    .record.interval[INT1].hourMinute = FAC_INTERVAL1_HOUR_MINUTE,
    .record.interval[INT1].temp = FAC_INTERVAL1_TEMP,
    .record.interval[INT2].hourMinute = FAC_INTERVAL2_HOUR_MINUTE,
    .record.interval[INT2].temp = FAC_INTERVAL2_TEMP,
    .record.interval[INT3].hourMinute = FAC_INTERVAL3_HOUR_MINUTE,
    .record.interval[INT3].temp = FAC_INTERVAL3_TEMP,
    .record.interval[INT4].hourMinute = FAC_INTERVAL4_HOUR_MINUTE,
    .record.interval[INT4].temp = FAC_INTERVAL4_TEMP,
    .record.interval[INT5].hourMinute = FAC_INTERVAL5_HOUR_MINUTE,
    .record.interval[INT5].temp = FAC_INTERVAL5_TEMP,
    .record.interval[INT6].hourMinute = FAC_INTERVAL6_HOUR_MINUTE,
    .record.interval[INT6].temp = FAC_INTERVAL6_TEMP,
    .record.interval[INT7].hourMinute = FAC_INTERVAL7_HOUR_MINUTE,
    .record.interval[INT7].temp = FAC_INTERVAL7_TEMP,   
    #if defined(__TUYA716__) || defined(__TUYA716_5G__)
    .record.aiAdjSetPara.t_max  = 30,
    .record.aiAdjSetPara.r_max  = 25,   
    #else
    .record.aiAdjSetPara.t_max  = 15,
    .record.aiAdjSetPara.r_max  = 15,   
    #endif
};



void app_con_gotoFac(bool allFlag)
{  
    s_sysPara.record.tempControlMode    = FAC_TEMP_CONTROL_CFG;    
    s_sysPara.record.linkRelayDelayTime = FAC_LINK_RELAY_DELAY;   
    s_sysPara.record.powerOnOffFlag     = FAC_POF_CFG;
    s_sysPara.record.cleanFlag          = FAC_CLEAN_CFG;
    s_sysPara.record.demoFlag           = FAC_DEMO_CFG;   
    s_sysPara.record.ltpTemp            = FAC_LOW_PROTECT_TEMP;
    s_sysPara.record.manSetTemp         = FAC_SET_TEMP;               
    s_sysPara.record.maxSetTemp         = FAC_MAX_SET_TEMP;        
    s_sysPara.record.diffTemp           = FAC_DIF_TEMP;
    s_sysPara.record.quickTemp          = FAC_QUICK_TEMP;
    s_sysPara.record.lifTemp            = FAC_LIF_TEMP;
    s_sysPara.record.sersonType         = FAC_SERSON_CFG;
    s_sysPara.record.weekPrg            = FAC_PRG_CFG;
    s_sysPara.record.outputType         = FAC_RELAY_OUT_TYPE_CFG;   
    s_sysPara.record.interval[INT0].hourMinute = FAC_INTERVAL0_HOUR_MINUTE;
    s_sysPara.record.interval[INT0].temp       = FAC_INTERVAL0_TEMP;
    s_sysPara.record.interval[INT1].hourMinute = FAC_INTERVAL1_HOUR_MINUTE;
    s_sysPara.record.interval[INT1].temp       = FAC_INTERVAL1_TEMP;
    s_sysPara.record.interval[INT2].hourMinute = FAC_INTERVAL2_HOUR_MINUTE;
    s_sysPara.record.interval[INT2].temp       = FAC_INTERVAL2_TEMP;
    s_sysPara.record.interval[INT3].hourMinute = FAC_INTERVAL3_HOUR_MINUTE;
    s_sysPara.record.interval[INT3].temp       = FAC_INTERVAL3_TEMP;
    s_sysPara.record.interval[INT4].hourMinute = FAC_INTERVAL4_HOUR_MINUTE;
    s_sysPara.record.interval[INT4].temp       = FAC_INTERVAL4_TEMP;
    s_sysPara.record.interval[INT5].hourMinute = FAC_INTERVAL5_HOUR_MINUTE;
    s_sysPara.record.interval[INT5].temp       = FAC_INTERVAL5_TEMP;
    s_sysPara.record.interval[INT6].hourMinute = FAC_INTERVAL6_HOUR_MINUTE;
    s_sysPara.record.interval[INT6].temp       = FAC_INTERVAL6_TEMP;
    s_sysPara.record.interval[INT7].hourMinute = FAC_INTERVAL7_HOUR_MINUTE;
    s_sysPara.record.interval[INT7].temp       = FAC_INTERVAL7_TEMP;   
    s_sysPara.record.owPara.enableFlag         = FAC_OW_ENABLE;
    s_sysPara.record.owPara.checkTime          = FAC_OW_CHECK_TIME;
    s_sysPara.record.owPara.downTemp           = FAC_OW_DOWN_TEMP;
    s_sysPara.record.owPara.backTime           = FAC_OW_BACK_TIME;
    s_sysPara.record.preheat.switchflag        = FAC_PREHEAT_ENABLE;
    uint8_t i = 0;
    for(;i < 8;i++)
    {
        if(420 <= s_sysPara.record.preheat.addTime[i])
        {
            s_sysPara.record.preheat.addTime[i] = 420;
        }
    }
    
    if(allFlag)
    {
        s_sysPara.record.inAdjTemp  = FAC_ADJ_TEMP;  
        s_sysPara.record.outAdjTemp = FAC_ADJ_TEMP;
        for(;i < 8;i++)
        {
            s_sysPara.record.preheat.addTime[i] = 420;
        }      
    }
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




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************控制台初始化***************************************
void app_control_init(void)
{    
    if(s_sysPara.record.powerOnOffFlag)
    {
        s_sysPara.record.sysRunStatus = SYS_STATUS_POWER_OFF;    
    }
    else
    {
        if((SYS_STATUS_POWER_OFF != s_sysPara.record.sysRunStatus) &&\
          (SYS_STATUS_HIGH_SETTING != s_sysPara.record.sysRunStatus))
        {
            s_sysPara.record.sysRunStatus = SYS_STATUS_SHOW_ALL;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);
            if(PROGRAM_OFF == s_sysPara.record.weekPrg)
            {
                s_sysPara.record.tempControlMode = TEMP_CONTROL_MODE_MANUAL;
            }
            else
            {
                if(TEMP_CONTROL_MODE_MANUAL != s_sysPara.record.tempControlMode)
                {
                    s_sysPara.record.tempControlMode = TEMP_CONTROL_MODE_TIME;                                      
                }
            }
        }
        else
        {
            s_sysPara.record.sysRunStatus=SYS_STATUS_POWER_OFF;
        }     
    } 
    app_control_timeOut_set(DELAY_BILINK_5SECOND,ms_5000);
    mod_ntc3380_outsideSensorType(s_sysPara.record.senMode);
}
//***************************控制台初始化***************************************
//------------------------------E N D-------------------------------------------






//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************测量温度和设定温度************************************
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




//#if defined(__TUYA716__) || defined(__TUYA716_5G__)
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

void app_control_measTempAdj(void)
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
//#endif



void app_control_updata_measTemp(void)
{   
    int16_t iTemp,oTemp;
    uint8_t error = NONE_ERROR;
    
    iTemp = mod_ntc3380_getMeasTemp(INSIDE_SERSON);
    oTemp = mod_ntc3380_getMeasTemp(OUTSIDE_SERSON);
    error = mod_ntc3380_error();   
    switch(s_sysPara.record.sersonType)
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
            break;
        }
        case SENSOR_ALL:
        {
            if(ALL_ERROR & error)
            {
                s_sysPara.runMeasTemp = 995;
                s_sysPara.runLifTemp = 995;
                
            }
            else if(INSIDE_ERROR & error)
            {                                       
                s_sysPara.runMeasTemp = 995;
                s_sysPara.runLifTemp = oTemp;
            } 
            else if(OUTSIDE_ERROR & error)
            {
                s_sysPara.runMeasTemp = iTemp + s_sysPara.record.inAdjTemp;
                s_sysPara.runLifTemp = 995;
            }
            else if(NONE_ERROR == error)
            {
                s_sysPara.runMeasTemp = iTemp + s_sysPara.record.inAdjTemp;
                s_sysPara.runLifTemp = oTemp;
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
    //#if defined(__TUYA716__) || defined(__TUYA716_5G__)
    app_control_measTempAdj();
    if(SENSOR_OUT != s_sysPara.record.sersonType)
    {//单外置不减温度
        s_sysPara.runMeasTemp -= get_aiAdjRunTemp();
    }   
    //#endif
    measTempCorrect(&s_sysPara.runMeasTemp);
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

void app_control_updata_setTemp(void)
{
    if(SYS_STATUS_POWER_OFF == s_sysPara.record.sysRunStatus)
    {
        if(MIN_LOW_PROTECT_TEMP < s_sysPara.record.ltpTemp)
        {
            //s_sysPara.runSetTemp = s_sysPara.record.ltpTemp;
        }
    }
    else
    {
        if(TEMP_CONTROL_MODE_MANUAL == s_sysPara.record.tempControlMode) 
        {
            s_sysPara.runSetTemp = s_sysPara.record.manSetTemp;
        }
        else
        {
            uint8_t interval = app_interval_getInterval();;
            if(app_preheat_getStatus())
            {
                if(5 == interval)
                {
                    s_sysPara.runSetTemp = s_sysPara.record.interval[0].temp;  
                }
                else if(7 == interval)
                {
                    s_sysPara.runSetTemp = s_sysPara.record.interval[6].temp;  
                }
                else
                {
                    s_sysPara.runSetTemp = s_sysPara.record.interval[interval+1].temp;  
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
                
        if(s_sysPara.runSetTemp > s_sysPara.record.maxSetTemp)
        {
            s_sysPara.runSetTemp = s_sysPara.record.maxSetTemp;
            s_sysPara.record.manSetTemp = s_sysPara.record.maxSetTemp;
        }
        
        if(s_sysPara.record.quickTemp > s_sysPara.record.maxSetTemp)
        {
            s_sysPara.record.quickTemp = s_sysPara.record.maxSetTemp;
        }
    }    
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************500mS闪动和系统参数更新*********************************
void app_control_updata_500ms_Blink(void)
{
    static uint16_t lastSystemMsTime = 0;     
    static uint16_t period = 0;
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
    static systemRunStatus_t lastRunStatus = SYS_STATUS_POWER_OFF;
    if(s_sysPara.systemUpdataEvent)
    {//500ms刷新一次
        s_sysPara.systemUpdataEvent = false;
        app_control_updata_measTemp();   //测量温度
        app_control_updata_setTemp();       //设定温度                
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
    if(app_control_timeOut_get(DELAY_DIS_KEEP))
    {
        s_sysPara.disKeepFlag = false;
    }
    if(app_control_timeOut_get(DELAY_FAC))
    {
        if(FAC_FINISH > s_sysPara.highSet.facStatus)
        {
            s_sysPara.highSet.facStatus++;
            app_control_timeOut_set(DELAY_FAC,ms_500);
        }
    }    
    if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
    {
        s_sysPara.record.sysRunStatus = SYS_STATUS_POWER_OFF;
    }
}

void app_control_timeOut_wifiLinkHandle(void)
{
    if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
    {     
        if(WIFI_LINK_BUSY == s_sysPara.wifi.linkStatus)
        {
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);
            s_sysPara.wifi.linkStatus = WIFI_LINK_FAIL;
        }
        else
        {
            s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;
        }
    }
    
    if(WIFI_LINK_DELAY == s_sysPara.wifi.linkStatus)
    {
        if(WIFI_CONN_CLOUD != app_pullTuyaWifiStateFormThisModule())
        {
            s_sysPara.wifi.linkStatus = WIFI_LINK_BUSY ;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);
        }
    }
    
    if(WIFI_LINK_BUSY == s_sysPara.wifi.linkStatus) 
    {
        if(WIFI_CONN_CLOUD == app_pullTuyaWifiStateFormThisModule())
        {
             app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_3000);
             s_sysPara.wifi.linkStatus = WIFI_LINK_SUCCESS;
             s_sysPara.wifi.onLine = true;
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
            //app_control_timeOut_runModeHandle();
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
                app_control_timeOut_set(DELAY_DIS_KEEP,ms_5000);
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
        case SYS_STATUS_PROGRAM:        
        case SYS_STATUS_RTCSET:
        {
            if(app_control_timeOut_get(DELAY_DIS_KEEP))
            {
                s_sysPara.disKeepFlag = false;
            }
            
            if(app_control_timeOut_get(DELAY_CONTROLER_COMMON))
            {              
                s_sysPara.record.sysRunStatus = SYS_STATUS_RUN_MODE;
            }             
            break;
        }
        default:break;
    }
    
    if(app_control_timeOut_get(DELAY_BILINK_5SECOND))
    {
        app_control_timeOut_set(DELAY_BILINK_5SECOND,ms_5000);
        if(s_sysPara.blink_5second_flag)
        {
            s_sysPara.blink_5second_flag = false;
        }
        else
        {
            s_sysPara.blink_5second_flag = true;
        }
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
        DiffMsTime = CurrentSystemMsTime +65535 - lastSystemMsTime;
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
//**************************超时时间扫描****************************************
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************超时时间扫描****************************************
void app_control_scanTask(void)
{
    app_control_scan_measTemp();//更新测量温度
    APP_control_update_system();//状态更新
    APP_control_timeOutEvent_scan();//超时时间扫描
    app_control_updata_500ms_Blink();//闪动事件发送
    
    //SysPara_t *ptSysPara;  
    //ptSysPara =  controler_getSysParaPt( );
    
    
    /*INT8U regCloud_addr;
    INT16U regCloud_dealis;
    if(app_pullControlDataFromThis(&regCloud_addr,&regCloud_dealis))
    {
        if(regCloudAddr_power == regCloud_addr)
        {
            if(regCloudAddr_power_off == regCloud_dealis)
            {
                ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;
            }
            else
            {
                ptSysPara->record.sysRunStatus = SYS_STATUS_RUN_MODE;
            }
        }
    }
    
    if(app_pullAllDataUpdateOnceStatus())  //推送所有消息
    {
        app_pushControlDataFromDevice_update(regCloudAddr_power,0x0001);
        app_pushControlDataFromDevice_update(regCloudAddr_ctlMode,0);
        app_pushControlDataFromDevice_update(regCloudAddr_setTemperature,250);
        app_pushControlDataFromDevice_update(regCloudAddr_measureTemperature,300);
        app_pushControlDataFromDevice_update(regCloudAddr_heatingStatus,1);
        app_pushControlDataFromDevice_update(regCloudAddr_notFrozen,1);
        app_pushControlDataFromDevice_update(regCloudAddr_maximumSetTemp,550);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_1,0x0600);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_1,160);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_2,0x0800);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_2,200);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_3,0x1200);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_3,260);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_4,0x1600);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_4,200);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_5,0x1800);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_5,260);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_6,0x2200);
        app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_6,160);
        app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysTime_1,0x0600);
        app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysSetT_1,200);
        app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysTime_2,0x2300);
        app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysSetT_2,160);
        app_pushControlDataFromDevice_update(regCloudAddr_programCfg,0);
        app_pushControlDataFromDevice_update(regCloudAddr_sensorCfg,1);
        app_pushControlDataFromDevice_update(regCloudAddr_floorSensorType,2);
        app_pushControlDataFromDevice_update(regCloudAddr_floorLimitTemp,320);
        app_pushControlDataFromDevice_update(regCloudAddr_floorTemperature,450);
        app_pushControlDataFromDevice_update(regCloudAddr_childLock,0);
    }*/
    
}
//**************************超时时间扫描****************************************
//------------------------------E N D-------------------------------------------





