#include ".\APP_Cfg.h"


static bool updataFlagEnable = false;
static uint16_t sendDelayTime = 0;
static bool onlineFlag       = false;

void intervalDataChang_hexToBcd(uint16_t dat,uint8_t *_out_hm)
{
    uint8_t ten;
    uint8_t units;
    ten = (uint8_t)(((dat >> 8) & 0x00ff) / 16);
    units = (uint8_t)(((dat >> 8) & 0x00ff) % 16);
    _out_hm[0] = ten*10 + units;
    ten = (uint8_t)(((dat) & 0x00ff) / 16);
    units = (uint8_t)(((dat) & 0x00ff) % 16);
    _out_hm[1] = ten*10 + units;
    if(_out_hm[0] > 23)
    {
       _out_hm[0] = 23; 
    }
    if(_out_hm[1] > 59)
    {
       _out_hm[1] = 59; 
    }
}

void intervalDataChang_bcdToHex(uint8_t *hm,uint16_t *_out_hm)
{
    uint8_t ten;
    uint8_t units;
    ten = hm[0] / 10;
    units = hm[0] % 10;
    *_out_hm = ten*16 + units;
    *_out_hm <<= 8;   
    ten = hm[1] / 10;  
    units = hm[1] % 10;
    *_out_hm |= ten*16 + units;      
}

void app_wifi_pushAllDatToCloud(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    uint16_t dat = 0;
    if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
    {
        dat = 0x0000;
    }
    else
    {
        dat = 0x0001;
    }
    
    app_pushControlDataFromDevice_update(regCloudAddr_power,dat); 
    volatile static uint16_t tMode ; 
    tMode = (uint16_t)ptSysPara->tempControlMode;
    app_pushControlDataFromDevice_update(regCloudAddr_ctlMode,tMode);
    app_pushControlDataFromDevice_update(regCloudAddr_setTemperature,(uint16_t)ptSysPara->runSetTemp);
    app_pushControlDataFromDevice_update(regCloudAddr_measureTemperature,(uint16_t)ptSysPara->runMeasTemp);
    app_pushControlDataFromDevice_update(regCloudAddr_heatingStatus,(uint16_t)(!ptSysPara->floorRelayFlag));   
//    if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
//    {
//        dat = 0x0001;       
//    } 
//    else
//    {
//        dat = 0x0000;      
//    }
    app_pushControlDataFromDevice_update(regCloudAddr_notFrozen,0);
    app_pushControlDataFromDevice_update(regCloudAddr_maximumSetTemp,(uint16_t)ptSysPara->record.maxSetTemp);
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT0].hour,&dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_1,dat);   
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_1,(uint16_t)ptSysPara->record.interval[INT0].temp);
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT1].hour,&dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_2,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_2,(uint16_t)ptSysPara->record.interval[INT1].temp);
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT2].hour,&dat); 
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_3,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_3,(uint16_t)ptSysPara->record.interval[INT2].temp);
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT3].hour,&dat); 
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_4,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_4,(uint16_t)ptSysPara->record.interval[INT3].temp);
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT4].hour,&dat); 
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_5,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_5,(uint16_t)ptSysPara->record.interval[INT4].temp);
    
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT5].hour,&dat); 
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysTime_6,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programWorkDaysSetT_6,(uint16_t)ptSysPara->record.interval[INT5].temp);
    
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT6].hour,&dat); 
    app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysTime_1,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysSetT_1,(uint16_t)ptSysPara->record.interval[INT6].temp);
    
    intervalDataChang_bcdToHex(&ptSysPara->record.interval[INT7].hour,&dat); 
    app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysTime_2,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_programHolidaysSetT_2,(uint16_t)ptSysPara->record.interval[INT7].temp);
    
    app_pushControlDataFromDevice_update(regCloudAddr_programCfg,(uint16_t)ptSysPara->weekPrg); 
    
    
    #if defined(__RT1_23WIFI__) 
    app_pushControlDataFromDevice_update(regCloudAddr_sensorCfg,0);
    app_pushControlDataFromDevice_update(regCloudAddr_floorSensorType,0);
    app_pushControlDataFromDevice_update(regCloudAddr_floorLimitTemp,995);
    app_pushControlDataFromDevice_update(regCloudAddr_floorTemperature,995);
    #elif defined(__RT1_36WIFI__)
    app_pushControlDataFromDevice_update(regCloudAddr_sensorCfg,(uint16_t)ptSysPara->record.sensorMode);
    app_pushControlDataFromDevice_update(regCloudAddr_floorSensorType,(uint16_t)ptSysPara->record.sensorType);
    app_pushControlDataFromDevice_update(regCloudAddr_floorLimitTemp,(uint16_t)ptSysPara->record.lifTemp);
    if(OUTSIDE_ERROR & mod_ntc3380_error())
    {
        dat = 995;
    }
    else
    {
        dat = (uint16_t)(mod_ntc3380_getMeasTemp(OUTSIDE_SERSON) + ptSysPara->record.outAdjTemp);   
    }    
    app_pushControlDataFromDevice_update(regCloudAddr_floorTemperature,dat);
    #endif  
    app_pushControlDataFromDevice_update(regCloudAddr_childLock,(uint16_t)ptSysPara->record.lockStatus);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowSwitch,0);      
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowCheckDownTemp,0);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowCheckDownTime,0);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowRunBackTime,0);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowPreheatSwitch,0);  
    #if defined(__RT1_23WIFI__) 
    app_pushControlDataFromDevice_update(regCloudAddr_linkRelayOpenDelayTime,(uint16_t)ptSysPara->record.linkRelayDelayTime);
    #elif defined(__RT1_36WIFI__)
    app_pushControlDataFromDevice_update(regCloudAddr_linkRelayOpenDelayTime,0);
    #endif
    app_pushControlDataFromDevice_update(regCloudAddr_notFrozen_set_temp,0);  
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
            app_wifi_pushAllDatToCloud();
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
    static bool              outputFlag = false;
    static lockStatus_t      lockStatus = KEY_STATUS_UNLOCK;
    //static tempControlMode_t mode = TEMP_CONTROL_MODE_MANUAL;
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
//        if(mode != ptSysPara->tempControlMode)
//        {
//            mode = ptSysPara->tempControlMode;
//            updataFlagEnable = true;     
//        }
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
    
        if(outputFlag != ptSysPara->floorRelayFlag)
        {
            outputFlag =  ptSysPara->floorRelayFlag;
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        }
        
        if(lockStatus != ptSysPara->record.lockStatus)
        {
            lockStatus = ptSysPara->record.lockStatus;
            updataFlagEnable = true;
            sendDelayTime = GetSysTickMillisecond();
        }               
    }    
}

void app_wifi_downLoadDataFromCloud(void)
{
    uint8_t add = 0;
    INT16U dat = 0;
    if(app_pullControlDataFromThis(&add,&dat))
    {
        app_sto_eeprom_write();
        SysPara_t *ptSysPara;  
        ptSysPara = controler_getSysParaPt();          
        updataFlagEnable = true;
        app_sto_eeprom_write();
        switch(add)
        {
            case regCloudAddr_power:
            {
                if(0x0001 == dat)
                {
                    if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
                    {
                        ptSysPara->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
                        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);
                    }
                }
                else if(0x0000 == dat)
                {
                    if(KEY_STATUS_UNLOCK != ptSysPara->record.lockStatus)
                    {
                        ptSysPara->record.lockStatus = KEY_STATUS_UNLOCK;  
                    }
                    if(SYS_STATUS_POWER_OFF != ptSysPara->record.sysRunStatus)
                    {
                        ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;
                    }
                }
                break;
            }
            case regCloudAddr_ctlMode:
            {
                ptSysPara->tempControlMode = (tempControlMode_t)dat;
                break;  
            }       
            case regCloudAddr_setTemperature:
            {   
                ptSysPara->runSetTemp =  (int16_t)dat;
                if(ptSysPara->runSetTemp > ptSysPara->record.maxSetTemp)
                {
                    ptSysPara->runSetTemp = ptSysPara->record.maxSetTemp;
                }
                switch(ptSysPara->tempControlMode)
                {
                    case TEMP_CONTROL_MODE_MANUAL:
                    {
                        ptSysPara->record.manSetTemp = ptSysPara->runSetTemp;
                        break;
                    }
                    default:
                    {
                        ptSysPara->tempControlMode = TEMP_CONTROL_MODE_TEMPORARY;
                        break;
                    }
                }               
                break;
            }
            case regCloudAddr_measureTemperature : break;            
            case regCloudAddr_heatingStatus      : break;                           
            case regCloudAddr_notFrozen          :
            {
//                if(0x0001 == dat)
//                {
//                    if(MIN_LOW_PROTECT_TEMP >= ptSysPara->record.ltpTemp)
//                    {
//                        ptSysPara->record.ltpTemp = 50;
//                    }
//                }
//                else if(0x0000 == dat)
//                {
//                    if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
//                    {
//                        ptSysPara->record.ltpTemp = MIN_LOW_PROTECT_TEMP;
//                    }
//                }
                break;
            }
            case regCloudAddr_maximumSetTemp      :
            {
                ptSysPara->record.maxSetTemp = (int16_t)dat;
                break;
            }            
            case regCloudAddr_programWorkDaysTime_1:
            case regCloudAddr_programWorkDaysTime_2:
            case regCloudAddr_programWorkDaysTime_3:
            case regCloudAddr_programWorkDaysTime_4: 
            case regCloudAddr_programWorkDaysTime_5:
            case regCloudAddr_programWorkDaysTime_6:
            case regCloudAddr_programHolidaysTime_1:
            case regCloudAddr_programHolidaysTime_2:
            {                               
                uint8_t cnt = ((add - 8) / 2);
                intervalDataChang_hexToBcd(dat,&ptSysPara->record.interval[cnt].hour);               
                break;
            }                          
            case regCloudAddr_programWorkDaysSetT_1:                      
            case regCloudAddr_programWorkDaysSetT_2:                                
            case regCloudAddr_programWorkDaysSetT_3:                                 
            case regCloudAddr_programWorkDaysSetT_4:                                 
            case regCloudAddr_programWorkDaysSetT_5:          
            case regCloudAddr_programWorkDaysSetT_6:
            case regCloudAddr_programHolidaysSetT_1:          
            case regCloudAddr_programHolidaysSetT_2:  
            {
                uint8_t cnt = ((add - 9) / 2);
                ptSysPara->record.interval[cnt].temp = (int16_t)dat;
                if(ptSysPara->record.interval[cnt].temp > ptSysPara->record.maxSetTemp)
                {
                    ptSysPara->record.interval[cnt].temp = ptSysPara->record.maxSetTemp;
                }
                break;
            }
            case regCloudAddr_programCfg:
            {
                ptSysPara->weekPrg = (weekProgramCfg_t)dat;
                if(PROGRAM_OFF == ptSysPara->weekPrg)
                {
                    if(TEMP_CONTROL_MODE_MANUAL != ptSysPara->tempControlMode)
                    {
                        ptSysPara->tempControlMode = TEMP_CONTROL_MODE_MANUAL;
                    }
                }
                break;
            }
            case regCloudAddr_sensorCfg:
            {    
                #if defined(__RT1_36WIFI__)
                ptSysPara->record.sensorMode = (sensorMode_t)dat;
                #endif
                break;
            }
            case regCloudAddr_floorSensorType:
            {               
                 #if defined(__RT1_36WIFI__)
                 ptSysPara->record.sensorType = (sensorType_t)dat;
                 #endif
                 break;         
            }           
            case regCloudAddr_floorLimitTemp:
            {
                ptSysPara->record.lifTemp = (int16_t)dat;
                if(ptSysPara->record.lifTemp > MAX_LIF_TEMP)
                {
                    ptSysPara->record.lifTemp = MAX_LIF_TEMP;
                }
                else if(ptSysPara->record.lifTemp < MIN_LIF_TEMP)
                {
                    ptSysPara->record.lifTemp = MIN_LIF_TEMP;
                }
                break;
            }           
            case regCloudAddr_floorTemperature:break;          
            case regCloudAddr_childLock:
            {
                if(KEY_STATUS_UNLOCK == ptSysPara->record.lockStatus)
                {
                    if(SYS_STATUS_RUN_MODE == ptSysPara->record.sysRunStatus)
                    {
                        ptSysPara->record.lockStatus = KEY_STATUS_BLINK;
                        app_control_timeOut_set(DELAY_LOCK_BLNK_STATUS_LAST_TIME,ms_1000);
                    }                   
                }
                else 
                {
                    ptSysPara->record.lockStatus = KEY_STATUS_UNLOCK;       
                }
                
                break;
            }
            case regCloudAddr_openWindowSwitch:
            {
                ////ptSysPara->record.owPara.enableFlag = (bool)dat;           
                break;
            }
            case regCloudAddr_openWindowStatus:break;
            case regCloudAddr_openWindowCheckDownTemp:
            {
//                ptSysPara->record.owPara.downTemp = (int16_t)dat;
//                if(ptSysPara->record.owPara.downTemp > MAX_OW_DOWN_TEMP)
//                {
//                    ptSysPara->record.owPara.downTemp = MAX_OW_DOWN_TEMP;
//                }
//                else if(ptSysPara->record.owPara.downTemp < MIN_OW_DOWN_TEMP)
//                {
//                    ptSysPara->record.owPara.downTemp = MIN_OW_DOWN_TEMP;
//                }
                break;
            }
            case regCloudAddr_openWindowCheckDownTime:
            {
//                ptSysPara->record.owPara.checkTime = (uint8_t)dat;
//                if(ptSysPara->record.owPara.checkTime > MAX_OW_CHECK_TIME)
//                {
//                    ptSysPara->record.owPara.checkTime = MAX_OW_CHECK_TIME;
//                }
//                else if(ptSysPara->record.owPara.checkTime < MIN_OW_CHECK_TIME)
//                {
//                    ptSysPara->record.owPara.checkTime = MIN_OW_CHECK_TIME;
//                }
                break;
            }
            case regCloudAddr_openWindowRunBackTime:
            {
//                ptSysPara->record.owPara.backTime = (uint8_t)dat;
//                if(ptSysPara->record.owPara.backTime > MAX_OW_BACK_TIME)
//                {
//                    ptSysPara->record.owPara.backTime = MAX_OW_BACK_TIME;
//                }
//                else if(ptSysPara->record.owPara.backTime < MIN_OW_BACK_TIME)
//                {
//                    ptSysPara->record.owPara.backTime = MIN_OW_BACK_TIME;
//                }
                break;
            }
            case regCloudAddr_openWindowPreheatSwitch:
            {
//                #ifndef NDEBUG
//                volatile static uint16_t debugDat = 0;
//                debugDat = dat;
//                #endif
//                ptSysPara->record.preheat.switchflag = (bool)dat;
                break;
            }
            case regCloudAddr_linkRelayOpenDelayTime:
            {
                ptSysPara->record.linkRelayDelayTime = (uint8_t)dat;
                if(ptSysPara->record.linkRelayDelayTime > MAX_LINK_RELAY_DELAY)
                {
                    ptSysPara->record.linkRelayDelayTime = MAX_LINK_RELAY_DELAY;
                }                
                break;
            }
            case regCloudAddr_notFrozen_set_temp:
            {
//                ptSysPara->record.ltpTemp = (uint8_t)dat;
//                if(ptSysPara->record.ltpTemp > MAX_LOW_PROTECT_TEMP)
//                {
//                    ptSysPara->record.ltpTemp = MAX_LOW_PROTECT_TEMP;
//                }
//                else if(ptSysPara->record.ltpTemp < MIN_LOW_PROTECT_TEMP)
//                {
//                    ptSysPara->record.ltpTemp = MIN_LOW_PROTECT_TEMP;
//                }               
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
    //if(onlineFlag)
    {
        app_wifi_importantDatChanged();
        app_wifi_downLoadDataFromCloud();
        app_wifi_timeOut_updatToCloud();
        app_wifi_cloudAsk();
    }
    app_wifi_sendDelay();              
}


