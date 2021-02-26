#include ".\APP_Cfg.h"



#define KEY_MENU       KeyAloneBit0
#define KEY_ADD        KeyAloneBit1
#define KEY_DEC        KeyAloneBit3
#define KEY_POW        KeyAloneBit2
#define KEY_SUN        KeyAloneBit4


#define KEY_CHILD      KeyGroupBit0
#define KEY_LINK       KeyGroupBit1
#define KEY_PARA       KeyGroupBit2
#define KEY_AI         KeyGroupBit3


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
    SysPara_t *para;  
    para = controler_getSysParaPt();
    bool powUpFlag = false;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);                       
    }
    else if(PUSH_DOWN & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
    {     
        powUpFlag = true;
    }
    else if(PUSH_DOWN & mod_keyOperation(KEY_SUN,ms_5000,NOCONTINUE))
    {     
        powUpFlag = true;
        para->record.manSetTemp = para->record.saveSetTemp;
        para->fast_set_blink_flag = true;
        app_control_timeOut_set(DELAY_FAST_SET_BLINK_TIME,ms_5000);     
        para->runSetTemp = para->record.manSetTemp;
        mod_key_maskOnceOperation(KEY_SUN);
        
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
        para->record.sysRunStatus = SYS_STATUS_RUN_MODE;        
    }
}

static void runMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    bool runSetTempChangeFlag = false;
    bool ddr;   
    keyActionType_t currentKey = PUSH_NONE;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        if(app_openWinow_run())
        {
            app_openWindow_clr();
        }
        para->record.sysRunStatus = SYS_STATUS_POWER_OFF; 
        para->fast_set_blink_flag = false;
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
    {         
        ddr = DDR_ADD;
        runSetTempChangeFlag = true;
        para->fast_set_blink_flag = false;
        //para->dis_save_flag = false;
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
    {                   
        ddr = DDR_DEC;
        runSetTempChangeFlag = true; 
        para->fast_set_blink_flag = false;
        //para->dis_save_flag = false;
    }
    else if((PUSH_UP | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_MENU,ms_5000,NOCONTINUE)))
    {    
       
        if(PUSH_UP == currentKey)
        {        
             para->fast_set_blink_flag = false;
            if(TEMP_CONTROL_MODE_TIME == para->record.tempControlMode)
            {
              para->record.tempControlMode = TEMP_CONTROL_MODE_MANUAL;
            }
            else
            {
              para->record.tempControlMode = TEMP_CONTROL_MODE_TIME; 
            }
            para->blink_RTC_TEMP_flag = false;
            app_control_timeOut_set(DELAY_TIMMING_BLINK_RTC_TEMP,ms_5000);               
        }
        else if(PUSH_CONTINUE == currentKey)
        {
             para->fast_set_blink_flag = false;
            para->record.sysRunStatus = SYS_STATUS_RTC_SET;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);  
            para->rtcSet.enbaleBlinkFlag = true;
            para->rtcSet.menu = 0;
        }        
    }
    else if((PUSH_UP | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_SUN,ms_5000,NOCONTINUE)))    
    {
        if(PUSH_UP == currentKey)
        {
            para->record.tempControlMode = TEMP_CONTROL_MODE_MANUAL;
            para->record.manSetTemp = para->record.saveSetTemp;
            //para->dis_save_flag = true;
            para->runSetTemp = para->record.manSetTemp;
            para->blink_RTC_TEMP_flag = false;
            app_control_timeOut_set(DELAY_TIMMING_BLINK_RTC_TEMP,ms_5000);  
            para->fast_set_blink_flag = true;
            app_control_timeOut_set(DELAY_FAST_SET_BLINK_TIME,ms_5000); 
        }
        else if(PUSH_CONTINUE == currentKey)
        {
             para->fast_set_blink_flag = false;
            para->record.sysRunStatus = SYS_STATUS_PRG_SET;
            para->prgSet.enbaleBlinkFlag = true;
            para->prgSet.menu = 0;
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000); 
        }        
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_LINK,ms_3000,NOCONTINUE))
    {
        #ifdef __WIFI_CONTROL__
        para->record.sysRunStatus = SYS_STATUS_SMARTLINK;
        para->wifi.linkType = SMART_CONFIG;
        para->wifi.linkDis = 0;
        para->wifi.linkStatus = WIFI_LINK_DELAY;  
        app_entryWifiCfg_state((tuya_configType_def)para->wifi.linkType);        
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
         para->fast_set_blink_flag = false;
        #endif
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_PARA,ms_5000,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_HIGH_SETTING;
        para->highSet.menu = HIGH_SETTING_NTC_ADJ;
        para->highSet.disAdjTempFlag = false;
        para->highSet.facStatus = FAC_IDLE;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);   
        app_control_timeOut_set(DELAY_AUTO_GOTO_FAC,ms_60000); 
        para->autoGotoFacTime = 0;
        para->highSet.blinkFlag = true;
         para->fast_set_blink_flag = false;
    }   
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_AI,ms_5000,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_AI_ADJ_SET;
        para->aiAdjSetMenu        = AI_ADJ_MENU_T;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);  
         para->fast_set_blink_flag = false;
    }
    if(runSetTempChangeFlag)
    {     
        para->blink_RTC_TEMP_flag = false;
        app_control_timeOut_set(DELAY_TIMMING_BLINK_RTC_TEMP,ms_5000);   
        para->runSetTemp = keyOperation(ddr,DATA_KEEP,5,para->record.maxSetTemp,para->record.minSetTemp,para->runSetTemp);
        if(TEMP_CONTROL_MODE_MANUAL == para->record.tempControlMode)
        {
            para->record.manSetTemp = para->runSetTemp;      
        }
        else if(TEMP_CONTROL_MODE_TIME == para->record.tempControlMode)
        {
            para->record.tempControlMode = TEMP_CONTROL_MODE_TEMPORARY;
        }                                    
    }    
}

void highSet_action(bool ddr)
{
    SysPara_t       *para;    
    para = controler_getSysParaPt();  
    switch(para->highSet.menu)
    {
        case HIGH_SETTING_NTC_ADJ:
        {                       
            if(para->highSet.disAdjTempFlag)
            {
                int16_t *ptTemp = &para->record.inAdjTemp;
                if(SENSOR_OUT == para->record.sensorMode)
                {
                    ptTemp = &para->record.outAdjTemp;         
                }
                *ptTemp = keyOperation(ddr,DATA_KEEP,5,MAX_ADJ_TEMP,MIN_ADJ_TEMP,*ptTemp);
            }            
            para->highSet.disAdjTempFlag = true;
            app_control_timeOut_set(DELAY_HIGH_SET,ms_5000);            
            break;
        }
        case HIGH_SETTING_NTC_MODE_SECELT:
        {
            para->record.sensorMode = (sensorMode_t)keyOperation(ddr,DATA_LOOP,1,\
              SENSOR_ALL,SENSOR_IN,(uint8_t)para->record.sensorMode);
            break;
        }
        case HIGH_SETTING_MAX_SET_TEMP:
        {
            para->record.maxSetTemp = keyOperation(ddr,DATA_KEEP,5,\
              MAX_MAX_SET_TEMP,para->record.minSetTemp,para->record.maxSetTemp);
            break;
        }
        case HIGH_SETTING_MIN_SET_TEMP:
        {
            para->record.minSetTemp = keyOperation(ddr,DATA_KEEP,5,\
              para->record.maxSetTemp,MIN_MIN_SET_TEMP,para->record.minSetTemp);
            break;
        }
        case HIGH_SETTING_DIF:
        {
            para->record.diffTemp = keyOperation(ddr,DATA_KEEP,5,MAX_DIF_TEMP,\
              MIN_DIF_TEMP,para->record.diffTemp);
            break;
        }
        case HIGH_SETTING_POF:
        {
            para->record.powerOnOffFlag = (bool)keyOperation(ddr,DATA_LOOP,1,1,\
              0,(int16_t)para->record.powerOnOffFlag);
            break;
        }
        case HIGH_SETTING_LIF:
        {
            #ifdef __OUT_SIDE_CONTROL__
            para->record.lifSetTemp = keyOperation(ddr,DATA_KEEP,10,MAX_LIF_TEMP,\
              MIN_LIF_TEMP,para->record.lifSetTemp);
            #endif
            break;
        }         
        case HIGH_SETTING_LTP:
        {
            para->record.ltpSetTemp = keyOperation(ddr,DATA_KEEP,5,MAX_LTP_TEMP,\
              MIN_LTP_TEMP,para->record.ltpSetTemp);          
            break;
        } 
        case HIGH_SETTING_PRG:
        {
            para->record.weekPrg = (weekProgramCfg_t)keyOperation(ddr,DATA_LOOP,\
              1,PROGRAM_7,PROGRAM_5,(int16_t)para->record.weekPrg);
            break;
        }   
        case HIGH_SETTING_RELAY_OUT_TYPE:
        {
            para->record.relayOutType = (relayOutType_t)keyOperation(ddr,\
              DATA_KEEP,1,RELAY_OUT_TYPE_MAIN_P_ASSIST_N_OFF,\
                RELAY_OUT_TYPE_MAIN_P_ASSIST_P,(int16_t)para->record.relayOutType);
            break;
        }
        case HIGH_SETTING_LINK_DELAY_TIME:
        {
            para->record.linkRelayDelayTime = (uint8_t)keyOperation(ddr,DATA_KEEP,\
              1,MAX_LINK_RELAY_DELAY,MIN_LINK_RELAY_DELAY,(int16_t)para->record.linkRelayDelayTime);
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_SWITCH:
        {
            para->record.window.switch_flag = (bool)keyOperation(ddr,DATA_LOOP,\
              1,1,0,(int16_t)para->record.window.switch_flag);
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_TIME_CYC:
        {
            para->record.window.check_time = (uint8_t)keyOperation(ddr,DATA_KEEP,\
              1,MAX_OW_CHECK_TIME,MIN_OW_CHECK_TIME,(int16_t)para->record.window.check_time);
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_DOWN_TEMP:
        {
            para->record.window.down_temp = keyOperation(ddr,DATA_KEEP,5,\
              MAX_OW_DOWN_TEMP,MIN_OW_DOWN_TEMP,para->record.window.down_temp);
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_BACK_TIME:
        {
            para->record.window.back_time = (uint8_t)keyOperation(ddr,DATA_KEEP,\
              1,MAX_OW_BACK_TIME,MIN_OW_BACK_TIME,(int16_t)para->record.window.back_time);
            break;
        }
        case HIGH_SETTING_WINDOW_PREHEAT_SWITCH:
        {
            para->record.preheat.switchflag = (bool)keyOperation(ddr,DATA_LOOP,\
              1,1,0,(int16_t)para->record.preheat.switchflag);
            break;
        }
        case HIGH_SETTING_NTC_TYPE_SECELT:
        {
            para->record.sensorType = (sensorType_t)keyOperation(ddr,DATA_LOOP,\
              1,B3950_5K,B3380_10K,(int16_t)para->record.sensorType);
            mod_ntc3380_outsideSensorType((uint8_t)para->record.sensorType);
            break;
        }
        case HIGH_SETTING_SAVE_SET_TEMP:
        {
            para->record.saveSetTemp = keyOperation(ddr,DATA_KEEP,5,para->record.maxSetTemp,\
              para->record.minSetTemp,para->record.saveSetTemp);       
            break;
        }
        case HIGH_SETTING_FAC:
        {                 
            para->highSet.facStatus = FAC_IDLE;
            app_control_timeOut_set(DELAY_HIGH_SET,ms_500);
            para->record.autoGotoFac = true;
            app_con_gotoFac(); 
            app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_5000);   
            break;   
        }
             
        default:break;
    }
}

static void highSetMode(void)
{
    SysPara_t       *para;    
    para = controler_getSysParaPt(); 
    
           bool setFlag = false;
           bool ddrFlag = DDR_ADD;
    keyActionType_t currentKey = PUSH_NONE;
    app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_30000);
    app_control_timeOut_set(DELAY_AUTO_GOTO_FAC,ms_60000); 
    para->autoGotoFacTime = 0;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_POWER_OFF;      
    }  
    else if(PUSH_DOWN & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
    {   
        if(++para->highSet.menu >= HIGH_SETTING_MAX_INDEX)
        {
            para->highSet.menu = HIGH_SETTING_NTC_ADJ;
        }  
        para->highSet.facStatus = FAC_IDLE;
        #ifndef __OUT_SIDE_CONTROL__
        if(HIGH_SETTING_NTC_MODE_SECELT == para->highSet.menu)
        {            
            para->highSet.menu = HIGH_SETTING_MAX_SET_TEMP;
        }
        if(HIGH_SETTING_LIF == para->highSet.menu)
        {
            para->highSet.menu = HIGH_SETTING_LTP;
        }      
        if(HIGH_SETTING_NTC_TYPE_SECELT == para->highSet.menu)
        {
            //para->highSet.menu = HIGH_SETTING_FAC;
            para->highSet.menu = HIGH_SETTING_SAVE_SET_TEMP;
        }
        #endif   
        
        #ifndef __LINK_CONTROL__
        if(HIGH_SETTING_LINK_DELAY_TIME == para->highSet.menu)
        {            
            para->highSet.menu = HIGH_SETTING_WINDOW_CHECK_SWITCH;
        }
        #endif
        if(HIGH_SETTING_WINDOW_CHECK_TIME_CYC == para->highSet.menu)
        {
            if(!para->record.window.switch_flag)
            {
                para->highSet.menu = HIGH_SETTING_WINDOW_PREHEAT_SWITCH;
            }
        }        
    }         
    else if((PUSH_DOWN | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_ADD,ms_1500,ms_200)))
    {    
        if(HIGH_SETTING_FAC != para->highSet.menu)
        {
             setFlag = true;
        }
        else
        {
             if(PUSH_DOWN == currentKey)
             {
                  if((FAC_IDLE == para->highSet.facStatus) || (FAC_FINISH == para->highSet.facStatus))
                  {
                      setFlag = true;
                  }
             }
        }                                  
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_DEC,ms_3000,ms_200)))
    {    
        if(HIGH_SETTING_FAC != para->highSet.menu)
        {
             setFlag = true;
             ddrFlag = DDR_DEC;
        }
        else
        {
             if(PUSH_DOWN == currentKey)
             {
                  if((FAC_IDLE == para->highSet.facStatus) || (FAC_FINISH == para->highSet.facStatus))
                  {
                      setFlag = true;
                      ddrFlag = DDR_DEC;
                  }
             }
        }         
    }   
    else if(PUSH_DOWN & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
//        para->workMode = WORK_MODE_SUN;
//        para->runSetTemp = para->record.sunSetTemp;
//        para->record.manSetTemp = para->runSetTemp;       
//        para->record.sysRunStatus = SYS_STATUS_RUN_MODE;
//        para->timming.status = TIMMING_STATUS_IDLE;
//        para->timming.disFlag = false;
//        mod_key_maskOnceOperation(KEY_SUN);
    }
    
    if(setFlag)
    {
        highSet_action(ddrFlag); 
        para->highSet.blinkFlag = false;
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
    SysPara_t *para;  
    para = controler_getSysParaPt();
    bool wifiLinkFlag = false;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_POWER_OFF;
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_ADD,ms_2000,NOCONTINUE))
    {
        wifiLinkFlag = true;
        para->wifi.linkType = SMART_CONFIG;        
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_DEC,ms_2000,NOCONTINUE))
    {
        wifiLinkFlag = true;
        para->wifi.linkType = AP_CONFIG;                               
    }   
    else if(PUSH_UP & mod_keyOperation(KEY_SUN,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_RUN_MODE;        
    }
    if(wifiLinkFlag)
    {
        para->wifi.linkDis = 0;
        para->wifi.linkStatus = WIFI_LINK_DELAY;   
        app_entryWifiCfg_state((tuya_configType_def)para->wifi.linkType);   
    }
}

static void prgSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    bool setFlag = false;
    bool ddr;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_POWER_OFF;       
    }
    else
    {   
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
        if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
        {
            setFlag = true;            
            ddr = DDR_ADD;        
        }
        else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
        {       
            setFlag = true;
            ddr = DDR_DEC; 
        }
        else if(PUSH_DOWN  & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
        {
            para->prgSet.enbaleBlinkFlag = true;           
            if(++para->prgSet.menu > WEEK_PRG_MENU_REST_INT1_TEMP)
            {     
                para->prgSet.menu = WEEK_PRG_MENU_WROK_INT0_HM;
                                            
            }
            if(PROGRAM_7 == para->record.weekPrg)                
            {
                if(WEEK_PRG_MENU_WROK_INT3_TEMP < para->prgSet.menu)
                {
                  para->prgSet.menu = WEEK_PRG_MENU_WROK_INT0_HM;
                }                   
            }   
        }       
    } 
    if(setFlag)
    {             
        para->prgSet.enbaleBlinkFlag = false;
        app_control_timeOut_set(DELAY_TIME_PRG_SET,ms_5000); 
        interval_t *ptInt = &para->record.interval[0];
        ptInt += para->prgSet.menu / 2;
              
        if(para->prgSet.menu % 2)
        {
            ptInt->temp = keyOperation(ddr,DATA_KEEP,5,para->record.maxSetTemp,para->record.minSetTemp,ptInt->temp);
        }
        else
        {
            if(DDR_ADD == ddr) 
            {
                ptInt->minute += 10;
                if(ptInt->minute >= 60)
                {
                    ptInt->minute = 0;
                    if(++ptInt->hour >= 24)
                    {
                        ptInt->hour = 0;
                    }
                }               
            }
            else
            {
                if(ptInt->minute)
                {
                    if(ptInt->minute < 10)
                    {
                        ptInt->minute = 0;
                    }
                    else
                    {
                        ptInt->minute -= 10;
                    }
                }
                else
                {
                    ptInt->minute = 50;
                    if(0 == ptInt->hour)
                    {
                        ptInt->hour = 23;
                    }
                    else
                    {
                        ptInt->hour --;
                    }
                }
            }                                  
        }       
    }   
}

static void rtcSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    bool setFlag = false;
    bool ddr;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_RUN_MODE;   
        app_real_time_write_rtc();
    }
    else
    {
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
        if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
        {
            setFlag = true;            
            ddr = DDR_ADD; 
        }
        else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
        {                   
            setFlag = true;
            ddr = DDR_DEC; 
        }      
        else if(PUSH_DOWN & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
        {
            para->rtcSet.enbaleBlinkFlag = true;
            if(++para->rtcSet.menu > SET_MENU_WEEK)
            {
                para->rtcSet.menu = SET_MENU_MINUTE;
            }
        }  
    } 
    if(setFlag)
    {
        uint8_t *pt = NULL;       
        realTime_t *realTime;
        realTime = app_real_time_getTime();
        uint8_t max[3] = {59,23,6};         
        switch(para->rtcSet.menu)
        {
            case SET_MENU_MINUTE:pt = &realTime->minute;break;           
            case SET_MENU_HOUR:  pt = &realTime->hour;  break;          
            case SET_MENU_WEEK:  pt = &realTime->week;  break;              
            default:                                    break;
        }
        *pt = (uint8_t)keyOperation(ddr,DATA_LOOP,1,max[para->rtcSet.menu],0,(int16_t)*pt);  
        para->rtcSet.enbaleBlinkFlag = false;
        app_control_timeOut_set(DELAY_TIME_RTC_SET,ms_5000);
    }
}

static void aiAdjSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    bool setFlag = false;
    bool ddr;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        para->record.sysRunStatus = SYS_STATUS_RUN_MODE;      
    }
    else
    {
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
        if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_ADD,ms_3000,ms_200))
        {
            setFlag = true;            
            ddr = DDR_ADD; 
        }
        else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
        {                   
            setFlag = true;
            ddr = DDR_DEC; 
        }  
        else if(PUSH_DOWN  & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
        {
            para->aiAdjSetMenu = (aiAdjSetMenu_t)keyOperation(DDR_ADD,1,true,1,0,(uint16_t)para->aiAdjSetMenu);
        }
    } 
    
    if(setFlag)
    {
        uint16_t max[2] = {100,50};
        uint16_t min[2] = {0,0};
        int16_t *pt = &para->record.aiAdjSetPara.t_max;
        uint8_t  menu = (uint8_t)para->aiAdjSetMenu;
        pt += menu;
        *pt = keyOperation(ddr,DATA_KEEP,5,max[menu],min[menu],*pt);
    }
}

void app_key_update(systemRunStatus_t _in_sysRunStatus)
{
    switch(_in_sysRunStatus)
    {         
       case SYS_STATUS_POWER_OFF:         app_key_powOffMode();    break;
       case SYS_STATUS_RUN_MODE:          runMode();               break;
       case SYS_STATUS_HIGH_SETTING:      highSetMode();           break;
       case SYS_STATUS_SHOW_VERSIONS:     app_key_showVersion();   break;
       case SYS_STATUS_SHOW_ALL:          app_key_showAll();       break;
       case SYS_STATUS_SMARTLINK:         app_key_smartlinkMode(); break;
       case SYS_STATUS_RTC_SET:           rtcSetMode();            break;
       case SYS_STATUS_PRG_SET:           prgSetMode();            break; 
       case SYS_STATUS_AI_ADJ_SET:        aiAdjSetMode();          break;
       default:break;
    }      
}

void app_key_childLock(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    if(KEY_STATUS_LOCKED == para->record.lockStatus)
    {
        para->record.lockStatus = KEY_STATUS_BLINK;
        app_control_timeOut_set(DELAY_LOCK_BLNK_STATUS_LAST_TIME,ms_3000); 
    }
    
    if(PUSH_CONTINUE & mod_keyOperation(KEY_CHILD,ms_5000,NOCONTINUE))
    {
        if(SYS_STATUS_RUN_MODE == para->record.sysRunStatus)
        {          
            if(KEY_STATUS_BLINK == para->record.lockStatus)
            {
                para->record.lockStatus = KEY_STATUS_UNLOCK;
            }
            else if(KEY_STATUS_UNLOCK == para->record.lockStatus)
            {
                para->record.lockStatus = KEY_STATUS_LOCKED;
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
        SysPara_t *paraPara;  
        paraPara =  controler_getSysParaPt( );
        paraPara->dispUpdataEvent = true;
        app_sto_eeprom_write();
        app_backLigth_on();          
        app_key_childLock();
        if(KEY_STATUS_UNLOCK == paraPara->record.lockStatus)          
        {
            app_key_update(paraPara->record.sysRunStatus); 
        }    
        #endif
    }        
}


