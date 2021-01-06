#include ".\APP_Cfg.h"



#define KEY_MENU       KeyAloneBit0
#define KEY_ADD        KeyAloneBit1
#define KEY_DEC        KeyAloneBit3
#define KEY_POW        KeyAloneBit2
#define KEY_SUN        KeyAloneBit4


#define KEY_CHILD      KeyGroupBit0
#define KEY_LINK       KeyGroupBit1
#define KEY_PARA       KeyGroupBit2



#define DDR_ADD                   true   //加
#define DDR_DEC                   false  //减
#define DATA_LOOP                 true   //循环
#define DATA_KEEP                 false  //保持



int16_t keyOperation(bool ddr,bool loop,int16_t step,int16_t max,int16_t min,int16_t dat)
{
    int16_t DateCalculation;
    
    DateCalculation = dat;
    
    if(DDR_ADD == ddr)
    {         
        if(DateCalculation >= max)
        {
            if(loop)
            {
                DateCalculation = min;
            }
            else
            {
                DateCalculation = max; 
            }           
        }
        else
        {
            DateCalculation += step;            
        }
    }
    else if(DDR_DEC == ddr)
    {
        if(DateCalculation <= min)
        {
            if(loop)
            {
                DateCalculation = max;
            }
            else
            {
                DateCalculation = min; 
            }
        }
        else
        {
            DateCalculation -= step;
        }
    }
    
    return DateCalculation;
}

void app_key_powOffMode(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    bool powUpFlag = false;
    
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSys->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);       
        if(TIMMING_STATUS_RUN == ptSys->timming.status)
        {
            ptSys->timming.status = TIMMING_STATUS_IDLE;
            ptSys->timming.disFlag = false;
        }                       
    }
    else if(PUSH_DOWN & mod_keyOperation(KEY_MENU,ms_5000,NOCONTINUE))
    {             
        ptSys->record.sysRunStatus = SYS_STATUS_TIMING_SET;
        ptSys->timming.setDdr = TIMMING_SET_DDR_ON;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_10000);
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.halfHour = 0;       
    }
    else if(PUSH_UP & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
        ptSys->workMode = WORK_MODE_SUN;
        ptSys->runSetTemp = ptSys->record.sunSetTemp;
        ptSys->record.manSetTemp = ptSys->runSetTemp;      
        ptSys->record.sysRunStatus = SYS_STATUS_RUN_MODE;
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false;
    }
    else if(PUSH_DOWN & mod_keyOperation(KEY_ADD,NOCONTINUE,NOCONTINUE))
    {
        powUpFlag = true;
    }
    else if(PUSH_DOWN & mod_keyOperation(KEY_DEC,NOCONTINUE,NOCONTINUE))
    {
        powUpFlag = true;
    }
    if(powUpFlag)
    {
        ptSys->record.sysRunStatus = SYS_STATUS_RUN_MODE; 
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false;
    }
}

void app_key_runMode(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    bool runSetTempChangeFlag = false;
    bool ddr;   
    keyActionType_t currentKey = PUSH_NONE;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSys->record.sysRunStatus = SYS_STATUS_POWER_OFF;       
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false;    
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
    {         
        ddr = DDR_ADD;
        runSetTempChangeFlag = true;
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
    {                   
        ddr = DDR_DEC;
        runSetTempChangeFlag = true;        
    }
    else if((PUSH_UP | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_MENU,ms_3000,NOCONTINUE)))
    {               
        if(PUSH_UP == currentKey)
        {
            ptSys->record.sysRunStatus = SYS_STATUS_TIMING_SET;
            ptSys->timming.setDdr = TIMMING_SET_DDR_OFF;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_10000);
            ptSys->timming.status = TIMMING_STATUS_IDLE;
            ptSys->timming.halfHour = 0;        
        }
        else if(PUSH_CONTINUE == currentKey)
        {
            if(app_real_time_getVaildStatus())
            {
                ptSys->tempControlMode = (tempControlMode_t)keyOperation(DDR_ADD,\
                  DATA_LOOP,1,TEMP_CONTROL_MODE_TIME,TEMP_CONTROL_MODE_MANUAL,(int16_t)ptSys->tempControlMode);
            } 
            mod_key_maskOnceOperation(KEY_MENU);
        }       
    }
    else if(PUSH_UP & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
        ptSys->workMode = (workMode_t)keyOperation(DDR_ADD,\
          DATA_LOOP,1,WORK_MODE_MOON,WORK_MODE_SUN,(int16_t)ptSys->workMode); 
        int16_t *ptSet = &ptSys->record.sunSetTemp;
        if(WORK_MODE_MOON == ptSys->workMode)
        {
            ptSet = &ptSys->record.moonSetTemp;
        }
        ptSys->runSetTemp = *ptSet;
        ptSys->record.manSetTemp = ptSys->runSetTemp;
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false; 
        ptSys->tempControlMode = TEMP_CONTROL_MODE_MANUAL; 
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_LINK,ms_3000,NOCONTINUE))
    {
        #ifdef __WIFI_CONTROL__
        ptSys->record.sysRunStatus = SYS_STATUS_SMARTLINK;
        ptSys->wifi.linkType = SMART_CONFIG;
        ptSys->wifi.linkDis = 0;
        ptSys->wifi.linkStatus = WIFI_LINK_DELAY;  
        app_entryWifiCfg_state((tuya_configType_def)ptSys->wifi.linkType);        
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
        #endif
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_PARA,ms_5000,NOCONTINUE))
    {
        ptSys->record.sysRunStatus = SYS_STATUS_HIGH_SETTING;
        ptSys->highSet.menu = HIGH_SETTING_NTC_ADJ;
        ptSys->highSet.disAdjTempFlag = false;
        ptSys->highSet.facStatus = FAC_IDLE;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);   
        app_control_timeOut_set(DELAY_AUTO_GOTO_FAC,ms_60000); 
        ptSys->autoGotoFacTime = 0;
        ptSys->highSet.blinkFlag = true;
    }   
    
    if(runSetTempChangeFlag)
    {
        if(ptSys->timming.disFlag)
        {
            ptSys->timming.disFlag = false;
            app_control_timeOut_set(DELAY_TIMMING,ms_5000);
        }
        else
        {
            ptSys->runSetTemp = keyOperation(ddr,DATA_KEEP,5,ptSys->record.maxSetTemp,ptSys->record.minSetTemp,ptSys->runSetTemp);
            if(TEMP_CONTROL_MODE_MANUAL == ptSys->tempControlMode)
            {
                ptSys->record.manSetTemp = ptSys->runSetTemp;      
            }
            else if(TEMP_CONTROL_MODE_TIME == ptSys->tempControlMode)
            {
                ptSys->tempControlMode = TEMP_CONTROL_MODE_TEMPORARY;
            }              
        }              
    }    
}

static bool timeControChangFlag = false;
void app_key_highSet_action(bool ddr)
{
    SysPara_t       *ptSys;    
    ptSys = controler_getSysParaPt();  
    switch(ptSys->highSet.menu)
    {
        case HIGH_SETTING_NTC_ADJ:
        {                       
            if(ptSys->highSet.disAdjTempFlag)
            {
                int16_t *ptTemp = &ptSys->record.inAdjTemp;
                if(SENSOR_OUT == ptSys->record.sensorMode)
                {
                    ptTemp = &ptSys->record.outAdjTemp;         
                }
                *ptTemp = keyOperation(ddr,DATA_KEEP,5,MAX_ADJ_TEMP,MIN_ADJ_TEMP,*ptTemp);
            }            
            ptSys->highSet.disAdjTempFlag = true;
            app_control_timeOut_set(DELAY_HIGH_SET,ms_5000);            
            break;
        }
        case HIGH_SETTING_MAX_SET_TEMP:
        {
            ptSys->record.maxSetTemp = keyOperation(ddr,DATA_KEEP,5,MAX_MAX_SET_TEMP,ptSys->record.minSetTemp,ptSys->record.maxSetTemp);
            break;
        }
        case HIGH_SETTING_MIN_SET_TEMP:
        {
            ptSys->record.minSetTemp = keyOperation(ddr,DATA_KEEP,5,ptSys->record.maxSetTemp,MIN_MIN_SET_TEMP,ptSys->record.minSetTemp);
            break;
        }
        case HIGH_SETTING_DIF:
        {
            ptSys->record.diffTemp = keyOperation(ddr,DATA_KEEP,5,MAX_DIF_TEMP,MIN_DIF_TEMP,ptSys->record.diffTemp);
            break;
        }
        case HIGH_SETTING_POF:
        {
            ptSys->record.powerOnOffFlag = (bool)keyOperation(ddr,DATA_LOOP,1,1,0,(int16_t)ptSys->record.powerOnOffFlag);
            break;
        }
        case HIGH_SETTING_SENSOR_TYPE:
        {         
            #ifdef __OUT_SIDE_CONTROL__
            ptSys->record.sensorMode = (sensorMode_t)keyOperation(ddr,DATA_LOOP,1,SENSOR_ALL,SENSOR_IN,(int16_t)ptSys->record.sensorMode);          
            #endif
            break;
        }
        case HIGH_SETTING_LIF:
        {
            #ifdef __OUT_SIDE_CONTROL__
            ptSys->record.lifSetTemp = keyOperation(ddr,DATA_KEEP,10,MAX_LIF_TEMP,MIN_LIF_TEMP,ptSys->record.lifSetTemp);
            #endif
            break;
        }
        case HIGH_SETTING_COMFORTABLE_SET_TEMP:
        {
            ptSys->record.sunSetTemp = keyOperation(ddr,DATA_KEEP,5,MAX_COMFORTABLE_SET_TEMP,MIN_COMFORTABLE_SET_TEMP,ptSys->record.sunSetTemp);
            app_con_tempLegalProtect(ptSys->record.maxSetTemp,ptSys->record.minSetTemp,&ptSys->record.sunSetTemp);
            break;
        }
        case HIGH_SETTING_SAVE_SET_TEMP:
        {
            ptSys->record.moonSetTemp = keyOperation(ddr,DATA_KEEP,5,MAX_SAVE_SET_TEMP,MIN_SAVE_SET_TEMP,ptSys->record.moonSetTemp); 
            app_con_tempLegalProtect(ptSys->record.maxSetTemp,ptSys->record.minSetTemp,&ptSys->record.moonSetTemp);
            break;
        }
        case HIGH_SETTING_OUTPUT_OPEN_TIME:
        case HIGH_SETTING_OUTPUT_CLOSE_TIME:
        {
            uint8_t *mainLastTime = &ptSys->record.outputOpenLastTime;           
            uint8_t *assistLastTime = &ptSys->record.outputCloseLastTime;
            if(HIGH_SETTING_OUTPUT_CLOSE_TIME == ptSys->highSet.menu)
            {
                mainLastTime = &ptSys->record.outputCloseLastTime;
                assistLastTime = &ptSys->record.outputOpenLastTime;    
            }
            if(ptSys->record.outputTimeControFlag)
            {               
                if(MIN_LAST_TIME == *mainLastTime)
                {
                    if(DDR_DEC == ddr)
                    {
                        ptSys->record.outputTimeControFlag = false;
                        *assistLastTime = MIN_LAST_TIME;
                    }
                }
                *mainLastTime = (uint8_t)keyOperation(ddr,DATA_KEEP,\
                  5,MAX_LAST_TIME,MIN_LAST_TIME,(int16_t)*mainLastTime);
                if(timeControChangFlag)
                {
                    *assistLastTime = *mainLastTime;
                }
            }
            else
            {
                if(DDR_ADD == ddr)
                {
                    ptSys->record.outputTimeControFlag = true;
                    timeControChangFlag = true;
                }
            }
            break;
        } 
        case HIGH_SETTING_LTP:
        {
            ptSys->record.ltpSetTemp = keyOperation(ddr,DATA_KEEP,5,MAX_LTP_TEMP,MIN_LTP_TEMP,ptSys->record.ltpSetTemp);          
            break;
        }
        case HIGH_SETTING_RELAY_OUT_TYPE:
        {
            ptSys->record.relayOutType = (relayOutType_t)keyOperation(ddr,DATA_KEEP,1,MAX_RELAY_OUT_TYPE,MIN_RELAY_OUT_TYPE,(int16_t)ptSys->record.relayOutType);      
            break;
        }
        case HIGH_SETTING_FAC:
        {                 
            ptSys->highSet.facStatus = FAC_IDLE;
            app_control_timeOut_set(DELAY_HIGH_SET,ms_500);
            ptSys->record.autoGotoFac = true;
            app_con_gotoFac(); 
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_5000);   
            break;   
        }
             
        default:break;
    }
}

void app_key_highSetMode(void)
{
    SysPara_t       *ptSys;    
    ptSys = controler_getSysParaPt(); 
    
           bool setFlag = false;
           bool ddrFlag = DDR_ADD;
    keyActionType_t currentKey = PUSH_NONE;
    app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);
    app_control_timeOut_set(DELAY_AUTO_GOTO_FAC,ms_60000); 
    ptSys->autoGotoFacTime = 0;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSys->record.sysRunStatus = SYS_STATUS_POWER_OFF;      
    }  
    else if(PUSH_DOWN & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
    {   
        if(++ptSys->highSet.menu >= HIGH_SETTING_MAX_INDEX)
        {
            ptSys->highSet.menu = HIGH_SETTING_NTC_ADJ;
        }  
        timeControChangFlag = false;
        ptSys->highSet.facStatus = FAC_IDLE;
        #ifndef __OUT_SIDE_CONTROL__
        if(HIGH_SETTING_SENSOR_TYPE == ptSys->highSet.menu)
        {
            ptSys->highSet.menu = HIGH_SETTING_COMFORTABLE_SET_TEMP;
        }
        #endif        
    }         
    else if((PUSH_DOWN | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_ADD,ms_1500,ms_200)))
    {    
        if(HIGH_SETTING_FAC != ptSys->highSet.menu)
        {
             setFlag = true;
        }
        else
        {
             if(PUSH_DOWN == currentKey)
             {
                  if((FAC_IDLE == ptSys->highSet.facStatus) || (FAC_FINISH == ptSys->highSet.facStatus))
                  {
                      setFlag = true;
                  }
             }
        }                                  
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_DEC,ms_3000,ms_200)))
    {    
        if(HIGH_SETTING_FAC != ptSys->highSet.menu)
        {
             setFlag = true;
             ddrFlag = DDR_DEC;
        }
        else
        {
             if(PUSH_DOWN == currentKey)
             {
                  if((FAC_IDLE == ptSys->highSet.facStatus) || (FAC_FINISH == ptSys->highSet.facStatus))
                  {
                      setFlag = true;
                      ddrFlag = DDR_DEC;
                  }
             }
        }         
    }   
    else if(PUSH_DOWN & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
        ptSys->workMode = WORK_MODE_SUN;
        ptSys->runSetTemp = ptSys->record.sunSetTemp;
        ptSys->record.manSetTemp = ptSys->runSetTemp;       
        ptSys->record.sysRunStatus = SYS_STATUS_RUN_MODE;
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false;
        mod_key_maskOnceOperation(KEY_SUN);
    }
    
    if(setFlag)
    {
        app_key_highSet_action(ddrFlag); 
        ptSys->highSet.blinkFlag = false;
        app_control_timeOut_set(DELAY_HIGH_SET_BLINK_STOP,ms_3000);
    }
}

void app_key_showVersion(void)
{
  
}

void app_key_showAll(void)
{ 
  
}

void app_key_smartlinkMode(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    bool wifiLinkFlag = false;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSys->record.sysRunStatus = SYS_STATUS_POWER_OFF;
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_ADD,ms_2000,NOCONTINUE))
    {
        wifiLinkFlag = true;
        ptSys->wifi.linkType = SMART_CONFIG;        
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_DEC,ms_2000,NOCONTINUE))
    {
        wifiLinkFlag = true;
        ptSys->wifi.linkType = AP_CONFIG;                               
    }   
    else if(PUSH_UP & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
        ptSys->workMode = WORK_MODE_SUN;
        ptSys->runSetTemp = ptSys->record.sunSetTemp;
        ptSys->record.manSetTemp = ptSys->runSetTemp;
        //ptSys->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
        //app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000); 
        ptSys->record.sysRunStatus = SYS_STATUS_RUN_MODE;
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false;
    }
    if(wifiLinkFlag)
    {
        ptSys->wifi.linkDis = 0;
        ptSys->wifi.linkStatus = WIFI_LINK_DELAY;   
        app_entryWifiCfg_state((tuya_configType_def)ptSys->wifi.linkType);   
    }
}

void app_key_programMode(void)
{
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();
//    bool setFlag = false;
//    bool ddr;
//    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
//    {
//        ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;       
//    }
//    else
//    {   
//        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
//        if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
//        {
//            setFlag = true;            
//            ddr = DDR_ADD;        
//        }
//        else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
//        {       
//            setFlag = true;
//            ddr = DDR_DEC; 
//        }
//        else if(PUSH_DOWN  & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
//        {
//            ptSysPara->disKeepFlag = false;           
//            if(++ptSysPara->weekPrgSonMenu > WEEK_PRG_MENU_SON_TEMP)
//            {
//                ptSysPara->weekPrgSonMenu = WEEK_PRG_MENU_SON_TIME;
//                uint8_t max = 0;                
//                if(PROGRAM_7 == ptSysPara->record.weekPrg)                
//                {
//                    max = WEEK_PRG_MENU_MAIN_INT5;
//                }
//                else
//                {
//                    max = WEEK_PRG_MENU_MAIN_INT7;
//                }
//                if(++ptSysPara->weekPrgMainMenu > max)
//                {
//                     ptSysPara->weekPrgMainMenu = WEEK_PRG_MENU_MAIN_INT0;
//                }
//            }
//        }       
//    } 
//    if(setFlag)
//    {        
//        uint16_t max = 0;
//        uint16_t min = 0;
//        ptSysPara->disKeepFlag = true;
//        app_control_timeOut_set(DELAY_DIS_KEEP,ms_5000);               
//        if(WEEK_PRG_MENU_SON_TIME == ptSysPara->weekPrgSonMenu)
//        {            
//             uint16_t hourMinute = 0;
//             ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute = (ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute / 10) *10;
//             hourMinute = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].hour * 60)\
//               +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute);
//             switch(ptSysPara->weekPrgMainMenu) 
//             {
//                case WEEK_PRG_MENU_MAIN_INT0:               
//                case WEEK_PRG_MENU_MAIN_INT1:
//                case WEEK_PRG_MENU_MAIN_INT2:
//                case WEEK_PRG_MENU_MAIN_INT3:
//                case WEEK_PRG_MENU_MAIN_INT4:
//                case WEEK_PRG_MENU_MAIN_INT6:
//                {
//                    max = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu +1].hour * 60)\
//                      +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu + 1].minute) - 10; 
//                    if((WEEK_PRG_MENU_MAIN_INT0 == ptSysPara->weekPrgMainMenu) ||\
//                      (WEEK_PRG_MENU_MAIN_INT6 == ptSysPara->weekPrgMainMenu))
//                    {
//                        min = 0;
//                    }
//                    else
//                    {
//                        min = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu -1].hour * 60)\
//                      +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu - 1 ].minute) + 10; 
//                    }
//                    break;
//                }
//                case WEEK_PRG_MENU_MAIN_INT5:
//                case WEEK_PRG_MENU_MAIN_INT7:
//                {
//                    max = 1430;
//                    min = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu -1].hour * 60)\
//                      +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu - 1 ].minute) + 10; 
//                    break;
//                }
//             }           
//             ///hourMinute = datOperation(ddr,10,min,max,DATA_LOOP,hourMinute);
//             ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].hour = hourMinute / 60;
//             ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute = hourMinute % 60;              
//        }
//        else if(WEEK_PRG_MENU_SON_TEMP == ptSysPara->weekPrgSonMenu)
//        {
////             int16_t *pt_temp;
////             pt_temp = &ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].temp;
////             max = ptSysPara->record.maxSetTemp;
////             min = MIN_SET_TEMP;
//             ///tempOperation(ddr,5,min,max,pt_temp);
//        }
//    }   
}

void app_key_rtcSetMode(void)
{
//    SysPara_t *ptSysPara;  
//    ptSysPara = controler_getSysParaPt();
//    bool setFlag = false;
//    bool ddr;
//    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
//    {
//        ptSysPara->record.sysRunStatus = SYS_STATUS_RUN_MODE;      
//    }
//    else
//    {
//        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
//        if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
//        {
//            setFlag = true;            
//            ddr = DDR_ADD; 
//        }
//        else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
//        {                   
//            setFlag = true;
//            ddr = DDR_DEC; 
//        }      
////        else if(PUSH_DOWN & mod_keyOperation(KEY_RTC,NOCONTINUE,NOCONTINUE))
////        {
////            ptSysPara->disKeepFlag = false;
////            if(++ptSysPara->rtcSetMenu > SET_MENU_WEEK)
////            {
////                ptSysPara->rtcSetMenu = SET_MENU_MINUTE;
////            }
////        }  
//    } 
//    if(setFlag)
//    {
//        uint8_t *pt = NULL;
//        uint16_t max = 0;
//        uint16_t min = 0;
//        uint16_t dat = 0;
//        switch(ptSysPara->rtcSetMenu)
//        {
//            case SET_MENU_MINUTE:
//            {   
//                pt = &ptSysPara->rtc.minute;
//                max = MAX_MINUTE;
//                min = MIN_MINUTE;
//                dat = ptSysPara->rtc.minute;
//                break;
//            }
//            case SET_MENU_HOUR:
//            {
//                pt = &ptSysPara->rtc.hour;
//                max = MAX_HOUR;
//                min = MIN_HOUR;
//                dat = ptSysPara->rtc.hour;
//                break;
//            }
//            case SET_MENU_WEEK:
//            {
//                pt = &ptSysPara->rtc.week;
//                max = MAX_WEEK;
//                min = MIN_WEEK;
//                dat = ptSysPara->rtc.week;
//                break;
//            }
//            default:break;
//        }
//        //dat = datOperation(ddr,1,min,max,DATA_LOOP,dat);
//        *pt = (uint8_t)dat;
//        ptSysPara->disKeepFlag = true;
//        app_control_timeOut_set(DELAY_DIS_KEEP,ms_5000);
//    }
}

void app_key_timmingSetMode(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    bool setFlag = false;            
    bool ddr = DDR_ADD;     
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        if(ptSys->timming.halfHour)
        {         
            ptSys->timming.status = TIMMING_STATUS_RUN;                
        }
        else
        {
            ptSys->timming.status = TIMMING_STATUS_IDLE;
            ptSys->timming.disFlag = false;
        }
        
        if(TIMMING_SET_DDR_OFF == ptSys->timming.setDdr)
        {
            ptSys->record.sysRunStatus = SYS_STATUS_RUN_MODE;
            if(TIMMING_STATUS_RUN == ptSys->timming.status)
            {
                app_control_timeOut_set(DELAY_TIMMING,ms_5000);
            }           
        }
        else
        {
            ptSys->record.sysRunStatus = SYS_STATUS_POWER_OFF;
            if(TIMMING_STATUS_RUN == ptSys->timming.status)
            {
                ptSys->timming.disFlag = true;
            }           
        }     
    }
//    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_MENU,ms_3000,ms_200))
//    {
//        setFlag = true;            
//    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
    {
        setFlag = true;            
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
    {       
        setFlag = true;
        ddr = DDR_DEC; 
    }
    else if(PUSH_UP & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
        ptSys->workMode = WORK_MODE_SUN;
        ptSys->runSetTemp = ptSys->record.sunSetTemp;
        ptSys->record.manSetTemp = ptSys->runSetTemp;
        //ptSys->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
        //app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000); 
        ptSys->record.sysRunStatus = SYS_STATUS_RUN_MODE;
        ptSys->timming.status = TIMMING_STATUS_IDLE;
        ptSys->timming.disFlag = false;
    }
    if(setFlag)
    {
        ptSys->timming.status = TIMMING_STATUS_SET;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_10000);
        app_control_timeOut_set(DELAY_TIMMING,ms_3000);
        ptSys->timming.halfHour = (uint8_t)keyOperation(ddr,DATA_LOOP,1,48,0,(int16_t)ptSys->timming.halfHour);     
    }
}

void app_key_update(systemRunStatus_t _in_sysRunStatus)
{
    switch(_in_sysRunStatus)
    {         
       case SYS_STATUS_POWER_OFF:         app_key_powOffMode();    break;
       case SYS_STATUS_RUN_MODE:          app_key_runMode();       break;
       case SYS_STATUS_HIGH_SETTING:      app_key_highSetMode();   break;
       case SYS_STATUS_SHOW_VERSIONS:     app_key_showVersion();   break;
       case SYS_STATUS_SHOW_ALL:          app_key_showAll();       break;
       case SYS_STATUS_SMARTLINK:         app_key_smartlinkMode(); break;     
       case SYS_STATUS_TIMING_SET:        app_key_timmingSetMode();break;
       default:break;
    }      
}

void app_key_childLock(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    if(KEY_STATUS_LOCKED == ptSys->record.lockStatus)
    {
        ptSys->record.lockStatus = KEY_STATUS_BLINK;
        app_control_timeOut_set(DELAY_LOCK_BLNK_STATUS_LAST_TIME,ms_3000); 
    }
    
    if(PUSH_CONTINUE & mod_keyOperation(KEY_CHILD,ms_5000,NOCONTINUE))
    {
        if(SYS_STATUS_RUN_MODE == ptSys->record.sysRunStatus)
        {          
            if(KEY_STATUS_BLINK == ptSys->record.lockStatus)
            {
                ptSys->record.lockStatus = KEY_STATUS_UNLOCK;
            }
            else if(KEY_STATUS_UNLOCK == ptSys->record.lockStatus)
            {
                ptSys->record.lockStatus = KEY_STATUS_LOCKED;
            }                      
        }      
    }
}

void app_key_scanTask(void)
{          
    mod_key_scanTask(GetSysTickMillisecond());   
    if(mod_key_getEvent())
    {           
        #ifdef __LCD_STATUS_DEBUG__
        app_backLigth_on();          
        if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
        {
            mod_lcd_debug_add_for_key();
        }
        #else
        SysPara_t *ptSysPara;  
        ptSysPara =  controler_getSysParaPt( );
        ptSysPara->dispUpdataEvent = true;
        app_sto_eeprom_write();
        app_backLigth_on();          
        app_key_childLock();
        if(KEY_STATUS_UNLOCK == ptSysPara->record.lockStatus)          
        {
            app_key_update(ptSysPara->record.sysRunStatus); 
        }    
        #endif
    }        
}


