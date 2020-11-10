#include ".\APP_Cfg.h"


static bool updataFlagEnable = false;
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
    app_pushControlDataFromDevice_update(regCloudAddr_ctlMode,(uint16_t)ptSysPara->record.tempControlMode);
    app_pushControlDataFromDevice_update(regCloudAddr_setTemperature,(uint16_t)ptSysPara->runSetTemp);
    app_pushControlDataFromDevice_update(regCloudAddr_measureTemperature,(uint16_t)ptSysPara->runMeasTemp);
    app_pushControlDataFromDevice_update(regCloudAddr_heatingStatus,(uint16_t)(!ptSysPara->floorRelayFlag));
    
    if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
    {
        dat = 0x0001;       
    } 
    else
    {
        dat = 0x0000;      
    }
    app_pushControlDataFromDevice_update(regCloudAddr_notFrozen,dat);
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
    
    
    app_pushControlDataFromDevice_update(regCloudAddr_programCfg,(uint16_t)ptSysPara->record.weekPrg);
//    app_pushControlDataFromDevice_update(regCloudAddr_sensorCfg,(uint16_t)ptSysPara->record.sersonType);    
//    app_pushControlDataFromDevice_update(regCloudAddr_floorSensorType,(uint16_t)ptSysPara->record.senMode);
//    app_pushControlDataFromDevice_update(regCloudAddr_floorLimitTemp,(uint16_t)ptSysPara->record.lifTemp);
    app_pushControlDataFromDevice_update(regCloudAddr_floorLimitTemp,(uint16_t)ptSysPara->record.lifTemp);
    #if defined(__TUYA716__) || defined(__TUYA716_5G__)
    app_pushControlDataFromDevice_update(regCloudAddr_sensorCfg,(uint16_t)ptSysPara->record.sersonType);    
    app_pushControlDataFromDevice_update(regCloudAddr_floorSensorType,(uint16_t)ptSysPara->record.senMode);    
    dat = (uint16_t)(mod_ntc3380_getMeasTemp(OUTSIDE_SERSON) +\
      ptSysPara->record.outAdjTemp);
    #else
    app_pushControlDataFromDevice_update(regCloudAddr_sensorCfg,0);    
    app_pushControlDataFromDevice_update(regCloudAddr_floorSensorType,0);    
    dat = 995;
    #endif
    app_pushControlDataFromDevice_update(regCloudAddr_floorTemperature,dat);
    app_pushControlDataFromDevice_update(regCloudAddr_childLock,(uint16_t)ptSysPara->record.lockStatus);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowSwitch,(uint16_t)ptSysPara->record.owPara.enableFlag);   
    if(OW_STATUS_RUN == ptSysPara->owStatus)
    {
        dat = 1;
    }
    else
    {
        dat = 0; 
    }
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowStatus,dat);   
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowCheckDownTemp,(uint16_t)ptSysPara->record.owPara.downTemp);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowCheckDownTemp,(uint16_t)ptSysPara->record.owPara.checkTime);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowCheckDownTemp,(uint16_t)ptSysPara->record.owPara.backTime);
    app_pushControlDataFromDevice_update(regCloudAddr_openWindowCheckDownTemp,(uint16_t)ptSysPara->record.preheat.switchflag);    
}        
//void app_wifi_pushAllDatToCloud(void)
//{
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();
//    uint16_t dat[39];
//    if(SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)
//    {
//        dat[1] = 0x0000;
//    }
//    else
//    {
//        dat[1] = 0x0001;
//    }
//    dat[2] = (uint16_t)ptSysPara->record.tempControlMode; 
//    dat[3] = (uint16_t)ptSysPara->runSetTemp;
//    dat[4] = (uint16_t)ptSysPara->runMeasTemp;
//    if(ptSysPara->floorRelayFlag)
//    {
//       dat[5] = 0x0001; 
//    }
//    else
//    {
//       dat[5] = 0x0000; 
//    }
//    // = (uint16_t)ptSysPara->floorRelayFlag;
//    if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
//    {
//        dat[6] = 0x0001;       
//    } 
//    else
//    {
//        dat[6] = 0x0000;      
//    }
//    dat[7] = (uint16_t)ptSysPara->record.maxSetTemp;
//    uint8_t i = 8;
//    for(;i < 0x18;i++)
//    {
//        intervalDataChang_bcdToHex(&ptSysPara->record.interval[i - 8].hour,&dat[i]);
//        i++;
//        dat[i] = (uint16_t)ptSysPara->record.interval[i - 9].temp;
//    }
//    dat[24] = (uint16_t)ptSysPara->record.weekPrg;
//    dat[32] = (uint16_t)ptSysPara->record.sersonType;
//    dat[33] = 0x0000;
//    dat[34] = (uint16_t)ptSysPara->record.lifTemp;
//    dat[36] = (uint16_t)(mod_ntc3380_getMeasTemp(OUTSIDE_SERSON) +\
//      ptSysPara->record.outAdjTemp); 
//    dat[38] = (uint16_t)ptSysPara->record.lockStatus;
//    
//    for(i = 1;i < 38;i++)
//    {
//        if((0x19 > i) || (0x1F < i))
//        {
//            if((i != 0x23) && (i != 0x25))
//            {
////                if(i == 5)
////                {
////                   app_pushControlDataFromDevice_update(regCloudAddr_heatingStatus,0x0000); 
////                }
////                else
////                {
//                   app_pushControlDataFromDevice_update(i,dat[i]); 
//               // }
//                
//            }
//        }        
//    }
//}

void app_wifi_sendDelay(void)
{
    static uint16_t          lastTick = 0;
           uint16_t          diffTick = 0;
    diffTick = GetSysTickMillisecond() - lastTick;
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
        lastTick = GetSysTickMillisecond();
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
    static lockStatus_t      lockStatus = NONE_LOCK;
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
        }
    
        if(runSetTemp != ptSysPara->runSetTemp)
        {
            runSetTemp = ptSysPara->runSetTemp;
            updataFlagEnable = true;
        }
    
        if(runMeasTemp != ptSysPara->runMeasTemp)
        {
            runMeasTemp = ptSysPara->runMeasTemp;
            updataFlagEnable = true;
        }
    
        if(outputFlag != ptSysPara->floorRelayFlag)
        {
            outputFlag =  ptSysPara->floorRelayFlag;
            updataFlagEnable = true;
        }
        
        if(lockStatus != ptSysPara->record.lockStatus)
        {
            lockStatus = ptSysPara->record.lockStatus;
            updataFlagEnable = true;
        }
    }    
}

void app_wifi_downLoadDataFromCloud(void)
{
    uint8_t add = 0;
    INT16U dat = 0;
    //volatile static uint16_t  outStatusWifi = false; 
    if(app_pullControlDataFromThis(&add,&dat))
    {
        SysPara_t *ptSysPara;  
        ptSysPara = controler_getSysParaPt();
        if(ptSysPara->record.demoFlag)
        {
            app_backLigth_on();
        }       
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
                    if(SYS_STATUS_POWER_OFF != ptSysPara->record.sysRunStatus)
                    {
                        ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;
                    }
                }
                break;
            }
            case regCloudAddr_ctlMode:
            {
                ptSysPara->record.tempControlMode = (tempControlMode_t)dat;
                break;
            }
            case regCloudAddr_setTemperature:
            {   
                ptSysPara->runSetTemp =  (int16_t)dat;
                if(ptSysPara->runSetTemp > ptSysPara->record.maxSetTemp)
                {
                    ptSysPara->runSetTemp = ptSysPara->record.maxSetTemp;
                }
                switch(ptSysPara->record.tempControlMode)
                {
                    case TEMP_CONTROL_MODE_MANUAL:
                    {
                        ptSysPara->record.manSetTemp = ptSysPara->runSetTemp;
                        break;
                    }
                    default:
                    {
                        ptSysPara->record.tempControlMode = TEMP_CONTROL_MODE_TEMPORARY;
                        break;
                    }
                }
                break;
            }
            case regCloudAddr_measureTemperature:break;            
            case regCloudAddr_heatingStatus:
            {                  
                 
                  break;  
            }
                    
            case regCloudAddr_notFrozen:
            {
                if(0x0001 == dat)
                {
                    if(MIN_LOW_PROTECT_TEMP >= ptSysPara->record.ltpTemp)
                    {
                        ptSysPara->record.ltpTemp = 50;
                    }
                }
                else if(0x0000 == dat)
                {
                    if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
                    {
                        ptSysPara->record.ltpTemp = MIN_LOW_PROTECT_TEMP;
                    }
                }
                break;
            }
            case regCloudAddr_maximumSetTemp:
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
                ptSysPara->record.weekPrg = (weekPrg_t)dat;
                break;
            }
            case regCloudAddr_sensorCfg:
            {
                #if defined(__TUYA716__) || defined(__TUYA716_5G__)
                ptSysPara->record.sersonType = (senCfg_t)dat;
                #endif
                break;
            }
            case regCloudAddr_floorSensorType:
            {
                 #if defined(__TUYA716__) || defined(__TUYA716_5G__)
                 ptSysPara->record.senMode = (senType_t)dat;
                 mod_ntc3380_outsideSensorType(ptSysPara->record.senMode);
                 #endif
                 break;       
            }             
            case regCloudAddr_floorLimitTemp:
            {
                #if defined(__TUYA716__) || defined(__TUYA716_5G__)
                ptSysPara->record.lifTemp = (int16_t)dat;
                if(ptSysPara->record.lifTemp > MAX_LIF_TEMP)
                {
                    ptSysPara->record.lifTemp = MAX_LIF_TEMP;
                }
                #endif
                break;
            }           
            case regCloudAddr_floorTemperature:break;          
            case regCloudAddr_childLock:
            {
                ptSysPara->record.lockStatus = (lockStatus_t)dat;               
                break;
            }
            case regCloudAddr_openWindowSwitch:
            {
                ptSysPara->record.owPara.enableFlag = (bool)dat;           
                break;
            }
            case regCloudAddr_openWindowStatus:break;
            case regCloudAddr_openWindowCheckDownTemp:
            {
                ptSysPara->record.owPara.downTemp = (int16_t)dat;
                if(ptSysPara->record.owPara.downTemp > MAX_OW_DOWN_TEMP)
                {
                    ptSysPara->record.owPara.downTemp = MAX_OW_DOWN_TEMP;
                }
                else if(ptSysPara->record.owPara.downTemp < MIN_OW_DOWN_TEMP)
                {
                    ptSysPara->record.owPara.downTemp = MIN_OW_DOWN_TEMP;
                }
                break;
            }
            case regCloudAddr_openWindowCheckDownTime:
            {
                ptSysPara->record.owPara.checkTime = (uint8_t)dat;
                if(ptSysPara->record.owPara.checkTime > MAX_OW_CHECK_TIME)
                {
                    ptSysPara->record.owPara.checkTime = MAX_OW_CHECK_TIME;
                }
                else if(ptSysPara->record.owPara.checkTime < MIN_OW_CHECK_TIME)
                {
                    ptSysPara->record.owPara.checkTime = MIN_OW_CHECK_TIME;
                }
                break;
            }
            case regCloudAddr_openWindowRunBackTime:
            {
                ptSysPara->record.owPara.backTime = (uint8_t)dat;
                if(ptSysPara->record.owPara.backTime > MAX_OW_BACK_TIME)
                {
                    ptSysPara->record.owPara.backTime = MAX_OW_BACK_TIME;
                }
                else if(ptSysPara->record.owPara.backTime < MIN_OW_BACK_TIME)
                {
                    ptSysPara->record.owPara.backTime = MIN_OW_BACK_TIME;
                }
                break;
            }
            case regCloudAddr_openWindowPreheatSwitch:
            {
                ptSysPara->record.preheat.switchflag = (bool)dat;
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


