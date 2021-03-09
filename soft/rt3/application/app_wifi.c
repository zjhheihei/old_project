#include ".\APP_Cfg.h"

typedef enum
{
    WIFI_SEND_STATUS_IDLE      = ((uint8_t)0x00),
    WIFI_SEND_STATUS_ALONE_DP,
    WIFI_SEND_STATUS_WARM_AI_DP,
    WIFI_SEND_STATUS_COLD_AI_DP,    
    WIFI_SEND_STATUS_OEM_AI_DP,
    WIFI_SEND_STATUS_OHTER_SET_DP,
}wifiSendStatus_t;

static bool     updataFlagEnable = false;
static uint16_t sendDelayTime = 0;
static bool     onlineFlag       = false;
static wifiSendStatus_t wifiSendStatus = WIFI_SEND_STATUS_IDLE;    

void raw_push(void)
{
    uint8_t i = 0;
    uint8_t buf[64];  
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    switch(wifiSendStatus)
    {
        case WIFI_SEND_STATUS_WARM_AI_DP:
        {
            #ifndef __RT5__
            if(app_logic_runAtMakeWarmMode())
            {
                buf[0] = (uint8_t)ptSys->tempControlMode;
            }
            else
            {
                buf[0] = 0;
            }
            for(i = 0; i < WEEK_PRG_LENGTH; i++)
            {
                buf[i * 4 + 1] = ptSys->record.interval[i].hour;
                buf[i * 4 + 2] = ptSys->record.interval[i].minute;
                buf[i * 4 + 3] = (uint8_t)((ptSys->record.interval[i].temp >> 8) & 0x00ff);
                buf[i * 4 + 4] = (uint8_t)((ptSys->record.interval[i].temp >> 0) & 0x00ff);
            }
            raw_dp_updat_to_cloud(REG_CLOUD_ADDR_AI_PRO_WARM,(AI_PRG_LENGTH + 1),buf);
            #endif
            wifiSendStatus = WIFI_SEND_STATUS_COLD_AI_DP;
            break;
        }
        case WIFI_SEND_STATUS_COLD_AI_DP:
        {
            #if defined(__AIR__) || defined(__ALL_1__)
            if(app_logic_runAtMakeWarmMode())
            {
                buf[0] = 0;
            }
            else
            {
                buf[0] = (uint8_t)ptSys->tempControlMode;
            }    
            for(i = 0; i < WEEK_PRG_LENGTH; i++)
            {
                buf[i * 4 + 1] = ptSys->record.interval[i + 8].hour;
                buf[i * 4 + 2] = ptSys->record.interval[i + 8].minute;
                buf[i * 4 + 3] = (uint8_t)((ptSys->record.interval[i + 8].temp >> 8) & 0x00ff);
                buf[i * 4 + 4] = (uint8_t)((ptSys->record.interval[i + 8].temp >> 0) & 0x00ff);
            }
            raw_dp_updat_to_cloud(REG_CLOUD_ADDR_AI_PRO_COLD,(AI_PRG_LENGTH + 1),buf);  
            #endif  
            wifiSendStatus = WIFI_SEND_STATUS_OEM_AI_DP;
            break;
        }
        case WIFI_SEND_STATUS_OEM_AI_DP:
        {
            #ifdef __RT5__
            for(i = 0; i < WEEK_PRG_LENGTH; i++)
            {
                buf[i * 4 + 0] = ptSys->record.interval[i].hour;
                buf[i * 4 + 1] = ptSys->record.interval[i].minute;
                buf[i * 4 + 2] = (uint8_t)((ptSys->record.interval[i].temp >> 8) & 0x00ff);
                buf[i * 4 + 3] = (uint8_t)((ptSys->record.interval[i].temp >> 0) & 0x00ff);
            }
            raw_dp_updat_to_cloud(REG_CLOUD_ADDR_AI_PRO_OEM,AI_PRG_LENGTH,buf);
            #endif
            wifiSendStatus = WIFI_SEND_STATUS_OHTER_SET_DP;
            break;
        }
        case WIFI_SEND_STATUS_OHTER_SET_DP:
        {
            #if defined(__HEATING__) 
            buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_CFG]       = (uint8_t)ptSys->record.outputTimeControFlag;
            buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_WORK_TIME] = (uint8_t)ptSys->record.outputOpenLastTime;
            buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_REST_TIME] = (uint8_t)ptSys->record.outputCloseLastTime; 
            #else
            buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_CFG]       = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_WORK_TIME] = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_REST_TIME] = 0xff;
            #endif
            
            ///////////////////////////////?????`э////////////////////////////////////////////
            #if defined(__OUT_SIDE_CONTROL__)   
            buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_SENSOR_TYPE]      = (uint8_t)ptSys->record.sensorType;
            #else
            buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_SENSOR_TYPE]      = 0xff;
            #endif   
            ///////////////////////////////?????`э/////////////////////////////////////////////
            
            buf[REG_CLOUD_OHTER_SET_ADDR_SENSOR_STATUS]          = (uint8_t)ptSys->systemError;
            
            ///////////////////////////////????//////////////////////////////////////////////////
            #if defined(__OUT_SIDE_CONTROL__)    
            buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_H]      = (uint8_t)((ptSys->runLifTemp >> 8) & 0x00ff);
            buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_L]      = (uint8_t)((ptSys->runLifTemp >> 0) & 0x00ff);
            #else
            buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_H]      = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_L]      = 0xff;
            #endif 
            ///////////////////////////////????//////////////////////////////////////////////////
            
            ///////////////////////////////j?????//////////////////////////////////////////////
            #if defined(__LINK_CONTROL__) 
            buf[REG_CLOUD_OHTER_SET_ADDR_LINK_DELAY_TIME]        = (uint8_t)ptSys->record.linkRelayDelayTime;   
            #else
            buf[REG_CLOUD_OHTER_SET_ADDR_LINK_DELAY_TIME]        = 0xff;   
            #endif 
            ///////////////////////////////j?????//////////////////////////////////////////////
            
            ///////////////////////////////?????485/////////////////////////////////////////////////
            #if defined(__LTP_CONTROL__)
            if(MIN_LTP_TEMP >= ptSys->record.ltpSetTemp)
            {
                buf[REG_CLOUD_OHTER_SET_ADDR_LTP_SET_TEMP]           = 0;
            }
            else
            {
                buf[REG_CLOUD_OHTER_SET_ADDR_LTP_SET_TEMP]           = (uint8_t)ptSys->record.ltpSetTemp;  
            }
            #else
            buf[REG_CLOUD_OHTER_SET_ADDR_LTP_SET_TEMP] = 0xff;
            #endif
            
            #if defined(__RS485_CONGTROL__)
            buf[REG_CLOUD_OHTER_SET_ADDR_RS485_ADDR] = (uint8_t)ptSys->record.rs485Para.addr;  
            buf[REG_CLOUD_OHTER_SET_ADDR_RS485_BAUD] = (uint8_t)ptSys->record.rs485Para.baud;  
            buf[REG_CLOUD_OHTER_SET_ADDR_RS485_PARITY] = (uint8_t)ptSys->record.rs485Para.parity;  
            #else    
            buf[REG_CLOUD_OHTER_SET_ADDR_RS485_ADDR]   = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_RS485_BAUD]   = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_RS485_PARITY]   = 0xff;
            #endif
            ///////////////////////////////?????485/////////////////////////////////////////////////
            
            
            ///////////////////////////////??????/////////////////////////////////////////////////
            #if defined(__RT5__) 
            buf[REG_CLOUD_OHTER_SET_ADDR_PREHEAT_CFG]            = 0;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CFG]        = 1;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_STATUS]     = 0;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_SET_TEMP]   = 30;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CHECK_TIME] = 15;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_BACK_TIME]  = 30;
            #else
            buf[REG_CLOUD_OHTER_SET_ADDR_PREHEAT_CFG]            = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CFG]        = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_STATUS]     = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_SET_TEMP]   = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CHECK_TIME] = 0xff;
            buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_BACK_TIME]  = 0xff;
            #endif
            ///////////////////////////////??????/////////////////////////////////////////////////
            raw_dp_updat_to_cloud(REG_CLOUD_ADDR_OTHER_SET,OHTER_SET_LENGTH,buf); 
            wifiSendStatus = WIFI_SEND_STATUS_IDLE;
            break;
        }
    }       
}

//void app_wifi_pushAllDatToCloud(void)
//{
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();
//    switch(wifiSendStatus)
//    {
//        case WIFI_SEND_STATUS_ALONE_DP:
//        {
//            break;
//        }
//    }
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_POWER_SWITCH,(uint16_t)((bool)ptSysPara->record.sysRunStatus)); 
//    //app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_FAC_SET,(uint16_t)((bool)ptSysPara->record.sysRunStatus)); 
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_POF_SET,(uint16_t)(!ptSysPara->record.powerOnOffFlag));
//    
//    #if defined(__AIR__) 
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_FCO_SET,(uint16_t)(!ptSysPara->record.fcoFlag)); 
//    #endif
//    
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_RUN_SET_TEMP,(uint16_t)ptSysPara->runSetTemp);
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_MAX_SET_TEMP,(uint16_t)ptSysPara->record.maxSetTemp);   
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_RUN_MEAS_TEMP,(uint16_t)ptSysPara->runMeasTemp);         
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_MIN_SET_TEMP,(uint16_t)ptSysPara->record.minSetTemp);
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_DIF_SET_TEMP,(uint16_t)ptSysPara->record.diffTemp);
//    
//    #if defined(__RT1__) || defined(__RT3__) || defined(__RT81__) || defined(__RT5__)
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_SUN_SET_TEMP,(uint16_t)ptSysPara->record.sunSetTemp);
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_MOON_SET_TEMP,(uint16_t)ptSysPara->record.moonSetTemp);
//    #endif
//    
//    #if defined(__OUT_SIDE_CONTROL__)
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_LIF_SET_TEMP,(uint16_t)ptSysPara->record.lifSetTemp);
//    #endif
//    
//    #if defined(__AIR__)
//    uint16_t mode = 0;
//    if(RUN_MODE_AT_AREFACTION == ptSysPara->record.workMode)
//    {
//        mode = 0x02;
//    }
//    else if(RUN_MODE_AT_WARM == ptSysPara->record.workMode)
//    {
//        mode = 0x01;
//    }    
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,mode);
//    #elif defined(__ALL_1__)
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,(uint16_t)ptSysPara->record.workMode);
//    #else   
//    #if defined(__RT5__) 
//    uint16_t mode = (uint16_t)ptSysPara->tempControlMode;
//    if(2 == mode)
//    {
//        mode = 1;
//    }
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,mode);
//    #else//?????????
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,(uint16_t)ptSysPara->workMode);
//    #endif
//    #endif
//    
//    #if defined(__HEATING__)
//    #if defined(__RT1__) || defined(__RT3__) || defined(__RT5__)
//    uint16_t status = 0;
//    if(!ptSys->floorRelayFlag)
//    {
//        if(TIME_CON_STATUS_REST == ptSys->timeContorl.status)
//        {
//            status = 0x02;
//        }
//        else
//        {
//            status = 0x01;
//        }      
//    }
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_STATUS,status);
//    #else
//    #endif
//    #endif
//      
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WEEK_PRG_SET,(uint16_t)ptSysPara->record.weekPrg); 
//    
//    #if defined(__HEATING__)
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_VALVE_STATUS,(uint16_t)ptSysPara->floorRelayFlag);
//    #elif defined(__AIR__)
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_VALVE_STATUS,(uint16_t)app_logic_fanOut());
//    #elif defined(__ALL_1__)
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_VALVE_STATUS,(uint8_t)app_logic_fanOut | (uint8_t)app_out_radiationOut());     
//    #endif
//    
//    
//    #if defined(__OUT_SIDE_CONTROL__) 
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_SERSON_SECELT,(uint16_t)ptSysPara->record.sensorMode);
//    #endif
//    
//    #if defined(__AIR__) || defined(__ALL_1__) 
//    uint16_t setSpeed = 0;
//    if(SET_WIND_SPEED_AI == ptSysPara->record.setWindSpeed)
//    {
//        setSpeed = 3;
//    }
//    else
//    {
//        setSpeed = (uint8_t)ptSysPara->record.setWindSpeed - 1;
//    }
//    app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_SET_WIND_SPEED,setSpeed);
//    #else
//    
//    #endif
//    raw_push();    
//}       

void app_wifi_pushAllDatToCloud_scanTask(void)
{    
    switch(wifiSendStatus)
    {
        case WIFI_SEND_STATUS_IDLE:break;
        case WIFI_SEND_STATUS_ALONE_DP:
        {
            SysPara_t *ptSysPara;  
            ptSysPara = controler_getSysParaPt();
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_POWER_SWITCH,(uint16_t)((bool)ptSysPara->record.sysRunStatus)); 
            //app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_FAC_SET,(uint16_t)((bool)ptSysPara->record.sysRunStatus)); 
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_POF_SET,(uint16_t)(!ptSysPara->record.powerOnOffFlag));
            
            #if defined(__AIR__) 
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_FCO_SET,(uint16_t)(!ptSysPara->record.fcoFlag)); 
            #endif
            
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_RUN_SET_TEMP,(uint16_t)ptSysPara->runSetTemp);
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_MAX_SET_TEMP,(uint16_t)ptSysPara->record.maxSetTemp);   
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_RUN_MEAS_TEMP,(uint16_t)ptSysPara->runMeasTemp);         
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_MIN_SET_TEMP,(uint16_t)ptSysPara->record.minSetTemp);
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_DIF_SET_TEMP,(uint16_t)ptSysPara->record.diffTemp);
            
            #if defined(__RT1__) || defined(__RT3__) || defined(__RT81__) || defined(__RT5__)
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_SUN_SET_TEMP,(uint16_t)ptSysPara->record.sunSetTemp);
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_MOON_SET_TEMP,(uint16_t)ptSysPara->record.moonSetTemp);
            #endif
            
            #if defined(__OUT_SIDE_CONTROL__)
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_LIF_SET_TEMP,(uint16_t)ptSysPara->record.lifSetTemp);
            #endif
            
            #if defined(__AIR__)
            uint16_t mode = 0;
            if(RUN_MODE_AT_AREFACTION == ptSysPara->record.workMode)
            {
                mode = 0x02;
            }
            else if(RUN_MODE_AT_WARM == ptSysPara->record.workMode)
            {
                mode = 0x01;
            }    
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,mode);
            #elif defined(__ALL_1__)
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,(uint16_t)ptSysPara->record.workMode);
            #else   
            #if defined(__RT5__) 
            uint16_t mode = (uint16_t)ptSysPara->tempControlMode;
            if(2 == mode)
            {
                mode = 1;
            }
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,mode);
            #else//?????????
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_MODE,(uint16_t)ptSysPara->workMode);
            #endif
            #endif
            
            #if defined(__HEATING__)
            #if defined(__RT1__) || defined(__RT3__) || defined(__RT5__)
            uint16_t status = 0;
            if(!ptSysPara->floorRelayFlag)
            {
                if(TIME_CON_STATUS_REST == ptSysPara->timeContorl.status)
                {
                    status = 0x02;
                }
                else
                {
                    status = 0x01;
                }      
            }
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WORK_STATUS,status);
            #else
            #endif
            #endif
              
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_WEEK_PRG_SET,(uint16_t)ptSysPara->record.weekPrg); 
            
            #if defined(__HEATING__)
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_VALVE_STATUS,(uint16_t)ptSysPara->floorRelayFlag);
            #elif defined(__AIR__)
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_VALVE_STATUS,(uint16_t)app_logic_fanOut());
            #elif defined(__ALL_1__)
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_VALVE_STATUS,(uint8_t)app_logic_fanOut() | (uint8_t)app_out_radiationOut());     
            #endif
            
            
            #if defined(__OUT_SIDE_CONTROL__) 
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_SERSON_SECELT,(uint16_t)ptSysPara->record.sensorMode);
            #endif
            
            #if defined(__AIR__) || defined(__ALL_1__) 
            uint16_t setSpeed = 0;
            if(SET_WIND_SPEED_AI == ptSysPara->record.setWindSpeed)
            {
                setSpeed = 3;
            }
            else
            {
                setSpeed = (uint8_t)ptSysPara->record.setWindSpeed - 1;
            }
            app_pushControlDataFromDevice_update(REG_CLOUD_ADDR_SET_WIND_SPEED,setSpeed);
            #else
            
            #endif
            wifiSendStatus = WIFI_SEND_STATUS_WARM_AI_DP;
            break;
        }
        default:raw_push();break;
    }       
}

void app_wifi_sendDelay(void)
{
           uint16_t          diffTick = 0;
    diffTick = GetSysTickMillisecond() - sendDelayTime;
    if(updataFlagEnable)
    {           
        if(ms_500 < diffTick)
        {
            updataFlagEnable = false;
            wifiSendStatus = WIFI_SEND_STATUS_ALONE_DP;  
            //app_wifi_pushAllDatToCloud();
        }              
    }
    else
    {
        sendDelayTime = GetSysTickMillisecond();
    }
}

void app_wifi_importantDatChanged(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    static systemRunStatus_t lastSysStatus = SYS_STATUS_POWER_OFF;
    static int16_t           runSetTemp = 220;
    static int16_t           runMeasTemp = 220;
    static tempControlMode_t mode = TEMP_CONTROL_MODE_MANUAL;  
    static bool              outputFlag = false; 
    
    #if defined(__AIR__) || defined(__ALL_1__)
    static setWindSpeed_t    setWindSpeed = SET_WIND_SPEED_LOW;
    #endif
    
    static workMode_t workMode   = (workMode_t)0;
    
    
    #ifdef __HEATING__  
    static uint8_t    workStatus = 0;
    #endif
    
    static uint16_t          lastTick = 0;
           uint16_t          diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastTick;
    if(ms_500 <= diffTick)
    {
        lastTick = GetSysTickMillisecond();
        if(lastSysStatus != ptSysPara->record.sysRunStatus)
        {
            lastSysStatus = ptSysPara->record.sysRunStatus;
            updataFlagEnable = true;   
            sendDelayTime = GetSysTickMillisecond();
        }
        
        if(runSetTemp != ptSysPara->runSetTemp)
        {
            runSetTemp = ptSysPara->runSetTemp;
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        }
        
        if(runMeasTemp != ptSysPara->runMeasTemp)
        {
            runMeasTemp = ptSysPara->runMeasTemp;
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        }
        
        if(mode != ptSysPara->tempControlMode)
        {
            mode = ptSysPara->tempControlMode;
            updataFlagEnable = true;   
            sendDelayTime = GetSysTickMillisecond();                                
        }
        #if defined(__HEATING__)
        if(outputFlag != ptSysPara->floorRelayFlag)
        {
            outputFlag =  ptSysPara->floorRelayFlag;
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        } 
        #elif defined(__AIR__)
        if(outputFlag != app_logic_fanOut())
        {
            outputFlag = app_logic_fanOut();
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        } 
        #elif defined(__ALL_1__)
        bool outFlag = false;
        if(app_logic_fanOut() || app_out_radiationOut())
        {
            outFlag = true;
        }       
        if(outputFlag != outFlag)
        {
            outputFlag = outFlag;
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        } 
        #endif
        
        #if defined(__AIR__) || defined(__ALL_1__)
        if(setWindSpeed != ptSysPara->record.setWindSpeed)
        {
            setWindSpeed = ptSysPara->record.setWindSpeed;
            updataFlagEnable = true;   
            sendDelayTime = GetSysTickMillisecond();
        }
        #endif  
        
        #ifdef __HEATING__
        if(workMode != ptSysPara->workMode)
        {
            workMode = ptSysPara->workMode;
            updataFlagEnable = true;   
            sendDelayTime = GetSysTickMillisecond();
        }
        #else 
        if(workMode != ptSysPara->record.workMode)
        {
            workMode = ptSysPara->record.workMode;
            updataFlagEnable = true;   
            sendDelayTime = GetSysTickMillisecond();
        }
        #endif
        
        #ifdef __HEATING__       
        uint8_t currentStatus = 0;
        if(!ptSysPara->floorRelayFlag)
        {
            if(TIME_CON_STATUS_REST == ptSysPara->timeContorl.status)
            {
                currentStatus = 2;
            }
            else
            {
                currentStatus = 1;
            }
        }
        if(workStatus != currentStatus)
        {
            workStatus = currentStatus;
            updataFlagEnable = true;   
            sendDelayTime = GetSysTickMillisecond();
        }
        #endif
    }    
}

void app_wifi_downLoadDataFromCloud(void)
{
    uint8_t add = 0;
    INT16U dat = 0;
    if(app_pullControlDataFromThis(&add,&dat))
    {
        app_sto_eeprom_write();
        SysPara_t *ptSys;  
        ptSys = controler_getSysParaPt();          
        updataFlagEnable = true;
        switch(add)
        {
            case REG_CLOUD_ADDR_POWER_SWITCH:
            {
                if(dat)
                {
                    if(SYS_STATUS_POWER_OFF == ptSys->record.sysRunStatus)
                    {
                        ptSys->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
                        #ifdef __HEATING__
                        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);
                        #else
                        app_timeOut_setTime(TIME_OUT_COMMON,ms_1000);
                        #endif
                    }
                }
                else
                {
                    ptSys->record.sysRunStatus = SYS_STATUS_POWER_OFF;
                    //ptSys->record.lockStatus = KEY_STATUS_UNLOCK;
                }
                break;
            }
            case REG_CLOUD_ADDR_FAC_SET:
            {
                if(dat)
                {
                    ptSys->record.autoGotoFac = true;
                    app_con_gotoFac(); 
                }               
                break;
            }
            case REG_CLOUD_ADDR_POF_SET:ptSys->record.powerOnOffFlag = !((bool)dat);break;             
            case REG_CLOUD_ADDR_FCO_SET:
            {
                #if defined(__AIR__) || defined(__ALL_1__) 
                ptSys->record.fcoFlag = (bool)dat;
                #endif
                break; 
            }
              
            case REG_CLOUD_ADDR_RUN_SET_TEMP:
            {
                ptSys->runSetTemp = (int16_t)dat;
                app_con_tempLegalProtect(ptSys->record.maxSetTemp,ptSys->record.minSetTemp,&ptSys->runSetTemp);                        
                if(TEMP_CONTROL_MODE_MANUAL == ptSys->tempControlMode)
                {
                    #if defined(__HEATING__)
                    ptSys->record.manSetTemp = ptSys->runSetTemp;  
                    #else
                    ptSys->ptSetTempSource[0] = ptSys->runSetTemp;                         
                    #endif
                }
                else
                {
                    ptSys->tempControlMode = TEMP_CONTROL_MODE_TEMPORARY;
                }                   
                break;
            }
            case REG_CLOUD_ADDR_MAX_SET_TEMP:
            {
                ptSys->record.maxSetTemp = (int16_t)dat;
                app_con_tempLegalProtect(MAX_MAX_SET_TEMP,MIN_MAX_SET_TEMP,&ptSys->record.maxSetTemp);
                break;
            }
            case REG_CLOUD_ADDR_RUN_MEAS_TEMP:break;
            case REG_CLOUD_ADDR_MIN_SET_TEMP:
            {
                ptSys->record.minSetTemp = (int16_t)dat;
                app_con_tempLegalProtect(MAX_MIN_SET_TEMP,MIN_MIN_SET_TEMP,&ptSys->record.minSetTemp);
                break;
            }
            case REG_CLOUD_ADDR_DIF_SET_TEMP:
            {
                ptSys->record.diffTemp = (int16_t)dat;
                app_con_tempLegalProtect(MAX_DIF_TEMP,MIN_DIF_TEMP,&ptSys->record.diffTemp);
                break;
            }
            case REG_CLOUD_ADDR_SUN_SET_TEMP:
            {
                #ifdef __HEATING__
                ptSys->record.sunSetTemp = (int16_t)dat;
                app_con_tempLegalProtect(ptSys->record.maxSetTemp,ptSys->record.minSetTemp,&ptSys->record.sunSetTemp);
                #endif
                break;
            }
            case REG_CLOUD_ADDR_MOON_SET_TEMP:
            {
                #ifdef __HEATING__
                ptSys->record.moonSetTemp = (int16_t)dat;
                app_con_tempLegalProtect(ptSys->record.maxSetTemp,ptSys->record.minSetTemp,&ptSys->record.moonSetTemp);
                #endif
                break;
            }
            case REG_CLOUD_ADDR_LIF_SET_TEMP:
            {
                #if defined(__OUT_SIDE_CONTROL__) 
                ptSys->record.lifSetTemp = (int16_t)dat;
                app_con_tempLegalProtect(MAX_LIF_TEMP,MIN_LIF_TEMP,&ptSys->record.diffTemp);
                #endif
                break;               
            }
            case REG_CLOUD_ADDR_WORK_MODE:
            {
                #if defined(__AIR__)
                if(2 == dat)//????
                {
                    ptSys->record.workMode = RUN_MODE_AT_AREFACTION;                      
                    ptSys->ptSetTempSource = &ptSys->record.arefactionSetTemp;
                    ptSys->ptSetTempSource[0] = AREFACTION_SET_TEMP;
                }
                else if(1 == dat)//???
                {
                    ptSys->record.workMode = RUN_MODE_AT_WARM;
                }
                else if(0 == dat)//??d
                {
                    ptSys->record.workMode = RUN_MODE_AT_COLD;
                }  
                ptSys->record.setWindSpeed = SET_WIND_SPEED_LOW;
                #elif defined(__ALL_1__)
                ptSys->record.workMode = (workMode_t)dat;              
                #else   
                #if defined(__RT5__) 
                if(app_real_time_getVaildStatus())
                {
                    ptSys->tempControlMode = (tempControlMode_t)dat;       
                }                       
                #else//?????????
                ptSys->workMode = (workMode_t)dat;
                if(WORK_MODE_SUN == ptSys->workMode)
                {
                    ptSys->runSetTemp = ptSys->record.sunSetTemp;
                }
                else
                {
                    ptSys->runSetTemp = ptSys->record.moonSetTemp;
                }
                ptSys->record.manSetTemp = ptSys->runSetTemp;  
                #endif
                #endif
                ptSys->tempControlMode = TEMP_CONTROL_MODE_MANUAL;
                break;
            }
            case REG_CLOUD_ADDR_WORK_STATUS:break;
            case REG_CLOUD_ADDR_WEEK_PRG_SET:
            {
                if(dat < 3)
                {
                    ptSys->record.weekPrg = (weekProgramCfg_t)dat;
                }               
                break; 
            }
            case REG_CLOUD_ADDR_VALVE_STATUS:break;
            case REG_CLOUD_ADDR_SERSON_SECELT:
            {
                #if defined(__OUT_SIDE_CONTROL__)
                if(dat < 3)
                {
                    ptSys->record.sensorMode = (sensorMode_t)dat;
                }                
                #endif
                break;
            }                                       
            case REG_CLOUD_ADDR_SET_WIND_SPEED:
            {
                #if defined(__AIR__) || defined(__ALL_1__) 
                bool setEnableFlag = true;
                #if defined(__ALL_1__) 
                {
                    if(RUN_MODE_RADIETION == ptSys->record.workMode)
                    {
                        setEnableFlag = false;
                    }
                }
                #endif
                if((setEnableFlag) && (dat < 4))
                {                    
                    if(3 == dat)
                    {
                        ptSys->record.setWindSpeed = SET_WIND_SPEED_AI;
                    }
                    else
                    {
                        ptSys->record.setWindSpeed = (setWindSpeed_t)(dat + 1);
                    }                    
                }
                #endif
                break;
            }
            case REG_CLOUD_ADDR_AI_PRO_WARM:
            {               
                #ifndef __RT5__                 
                uint8_t buf[64];
                uint8_t length = 0;
                uint8_t i = 0;
                length = raw_dp_updat_to_deceive(REG_CLOUD_ADDR_AI_PRO_WARM,buf);                               
                if(length == (AI_PRG_LENGTH + 1))
                {
                    if((TEMP_CONTROL_MODE_TEMPORARY > buf[0]) && app_logic_runAtMakeWarmMode() && app_real_time_getVaildStatus()) 
                    {
                        ptSys->tempControlMode = (tempControlMode_t)buf[0];                    
                    }
                    for(i = 0;i < (AI_PRG_LENGTH / 4) ;i++)
                    {
                        ptSys->record.interval[i].hour   = buf[i * 4 + 1];
                        ptSys->record.interval[i].minute = buf[i * 4 + 2];
                        int16_t temp = 0;
                        temp = buf[i * 4 + 3];
                        temp <<= 8 ;
                        temp |= buf[i * 4 + 4];
                        ptSys->record.interval[i].temp = temp;
                    }
                }                
                #endif
                break;
            }
            case REG_CLOUD_ADDR_AI_PRO_COLD:
            {
                #if defined(__AIR__) || defined(__ALL_1__)
                uint8_t buf[64];
                uint8_t length = 0;
                uint8_t i = 0;
                length = raw_dp_updat_to_deceive(REG_CLOUD_ADDR_AI_PRO_COLD,buf);                               
                if(length == (AI_PRG_LENGTH + 1))
                {
                    if((TEMP_CONTROL_MODE_TEMPORARY > buf[0]) && (!app_logic_runAtMakeWarmMode()) && app_real_time_getVaildStatus())
                    {
                        ptSys->tempControlMode = (tempControlMode_t)buf[0];                    
                    }
                    for(i = 0;i < (AI_PRG_LENGTH / 4) ;i++)
                    {
                        ptSys->record.interval[i + 8].hour   = buf[i * 4 + 1];
                        ptSys->record.interval[i + 8].minute = buf[i * 4 + 2];
                        int16_t temp = 0;
                        temp = buf[i * 4 + 3];
                        temp <<= 8 ;
                        temp |= buf[i * 4 + 4];
                        ptSys->record.interval[i + 8].temp = temp;
                    }
                }                
                #endif
                break;
            }
            case REG_CLOUD_ADDR_AI_PRO_OEM:
            {
                #ifdef __RT5__              
                uint8_t buf[64];
                uint8_t length = 0;
                uint8_t i = 0;
                length = raw_dp_updat_to_deceive(REG_CLOUD_ADDR_AI_PRO_WARM,buf);                             
                if(length == AI_PRG_LENGTH)
                {
                    for(i = 0;i < (AI_PRG_LENGTH / 4) ;i++)
                    {
                        ptSys->record.interval[i].hour   = buf[i * 4 + 0];
                        ptSys->record.interval[i].minute = buf[i * 4 + 1];
                        int16_t temp = 0;
                        temp = buf[i * 4 + 2];
                        temp <<= 8 ;
                        temp |= buf[i * 4 + 3];
                        ptSys->record.interval[i].temp = temp;
                    }
                }                
                #endif
                break;
            }
            case REG_CLOUD_ADDR_OTHER_SET:
            {
                uint8_t buf[64];                       
                raw_dp_updat_to_deceive(REG_CLOUD_ADDR_OTHER_SET,buf);                             
                #if defined(__HEATING__) 
                ptSys->record.outputTimeControFlag = (bool)buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_CFG];
                ptSys->record.outputOpenLastTime = buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_WORK_TIME]; 
                ptSys->record.outputCloseLastTime = buf[REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_REST_TIME];                
                #else
                
                #endif
    
                ///////////////////////////////?????`э////////////////////////////////////////////
                #if defined(__OUT_SIDE_CONTROL__)   
                ptSys->record.sensorType = (sensorType_t)buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_SENSOR_TYPE]; 
                mod_ntc3380_outsideSensorType((uint8_t)ptSys->record.sensorType);
                #else                
                #endif   
                ///////////////////////////////?????`э/////////////////////////////////////////////
    
                //buf[REG_CLOUD_OHTER_SET_ADDR_SENSOR_STATUS]          = (uint8_t)ptSys->systemError;
    
//                ///////////////////////////////????//////////////////////////////////////////////////
//                #if defined(__OUT_SIDE_CONTROL__)    
//                buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_H]      = (uint8_t)((ptSys->runLifTemp >> 8) & 0x00ff);
//                buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_L]      = (uint8_t)((ptSys->runLifTemp >> 0) & 0x00ff);
//                #else
//                buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_H]      = 0xff;
//                buf[REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_L]      = 0xff;
//                #endif 
//                ///////////////////////////////????//////////////////////////////////////////////////
    
                ///////////////////////////////j?????//////////////////////////////////////////////
                #if defined(__LINK_CONTROL__)
                ptSys->record.linkRelayDelayTime = buf[REG_CLOUD_OHTER_SET_ADDR_LINK_DELAY_TIME];                  
                #else              
                #endif 
                ///////////////////////////////j?????//////////////////////////////////////////////
    
                ///////////////////////////////?????485/////////////////////////////////////////////////
                #if defined(__LTP_CONTROL__)
                if(0 == buf[REG_CLOUD_OHTER_SET_ADDR_LTP_SET_TEMP])
                {
                    ptSys->record.ltpSetTemp = MIN_LTP_TEMP;
                }
                else
                {
                    ptSys->record.ltpSetTemp = (int16_t)buf[REG_CLOUD_OHTER_SET_ADDR_LTP_SET_TEMP];  
                }               
                #else
                
                #endif
    
                #if defined(__RS485_CONTROL__)
                ptSys->record.rs485Para.addr = buf[REG_CLOUD_OHTER_SET_ADDR_RS485_ADDR];
                ptSys->record.rs485Para.baud = buf[REG_CLOUD_OHTER_SET_ADDR_RS485_ADDR];
                ptSys->record.rs485Para.parity = buf[REG_CLOUD_OHTER_SET_ADDR_RS485_ADDR];                 
                #else                              
                #endif
                ///////////////////////////////?????485/////////////////////////////////////////////////
    
    
                ///////////////////////////////??????/////////////////////////////////////////////////
                #if defined(__RT5__) //产品规划中未开叿
//                buf[REG_CLOUD_OHTER_SET_ADDR_PREHEAT_CFG]            = 0;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CFG]        = 1;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_STATUS]     = 0;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_SET_TEMP]   = 30;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CHECK_TIME] = 15;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_BACK_TIME]  = 30;
                #else
//                buf[REG_CLOUD_OHTER_SET_ADDR_PREHEAT_CFG]            = 0xff;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CFG]        = 0xff;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_STATUS]     = 0xff;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_SET_TEMP]   = 0xff;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CHECK_TIME] = 0xff;
//                buf[REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_BACK_TIME]  = 0xff;
                #endif
                ///////////////////////////////??????/////////////////////////////////////////////////         
                break;
            }
            default:break;
        }
    }
}

void app_wifi_cloudAsk(void)
{
    if(app_pullAllDataUpdateOnceStatus()) 
    {
         updataFlagEnable = true;
    } 
}


#define TEN_MINUTES  1 
void app_wifi_timeOut_updatToCloud(void)
{
    static uint16_t          lastTick = 0;
    static uint8_t           minuteCnt = 0;   
           uint16_t          diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastTick;
    
    if(!updataFlagEnable)
    {
        if(ms_60000 < diffTick)
        {
            lastTick = GetSysTickMillisecond();
            if(++minuteCnt > TEN_MINUTES)
            {
                minuteCnt = 0;
                updataFlagEnable = true;
            }
        }
    }
    else
    {
        minuteCnt = 0;
        lastTick = GetSysTickMillisecond();
    }
}

void app_wifi_getWifiStatus(void)
{
    static uint16_t          lastTick = 0;        
           uint16_t          diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastTick;
    if(onlineFlag)
    {      
        if(ms_10000 <= diffTick)
        {
             lastTick = GetSysTickMillisecond();
             if(tuya_wifiState_connCloud != app_pullTuyaWifiStateFormThisModule())
             {
                 onlineFlag = false;
             }
        }
    }
    else
    {
        if(ms_60000 <= diffTick)
        {
             lastTick = GetSysTickMillisecond();
             if(tuya_wifiState_connCloud == app_pullTuyaWifiStateFormThisModule())
             {
                 onlineFlag = true;
             }
        }
    }       
}

void app_wifi_scanTask(void)
{   
    app_wifi_getWifiStatus();    
    app_wifi_importantDatChanged();
    app_wifi_downLoadDataFromCloud();
    app_wifi_timeOut_updatToCloud();
    app_wifi_cloudAsk();  
    app_wifi_sendDelay();   
    app_wifi_pushAllDatToCloud_scanTask();
}


