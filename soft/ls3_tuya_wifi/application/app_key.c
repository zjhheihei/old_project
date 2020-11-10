/*******************************************************************************
*
*	模块名称 : APP按键模块
*	文件名称 : APP.KeyBoard.c
*	版    本 : V1.0
*	说    明 : 1、按键扫描任务
*                  
*                 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-27  zhoujh&Linzl  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/

#include ".\APP_Cfg.h"


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************模块使用的变量**************************************
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************按键宏定义****************************************


#define KEY_MENU       KeyAloneBit4
#define KEY_ADD        KeyAloneBit3
#define KEY_DEC        KeyAloneBit1
#define KEY_POW        KeyAloneBit5
#define KEY_RTC        KeyAloneBit2
#define KEY_QUICK      KeyAloneBit0

//------------------------------E N D-------------------------------------------
#define KEY_CHILD      KeyGroupBit0
#define KEY_LINK       KeyGroupBit1  
#define KEY_AISET      KeyGroupBit2
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************高级设定按键处理************************************

#define DDR_ADD                   true
#define DDR_DEC                   false
#define DATA_LOOP                 true
#define DATA_KEEP                 false
static bool highSetFlag = false;

bool tempOperation(bool ddr,int16_t step,int16_t min,int16_t max,int16_t *_out_Temp)
{   
    int16_t temp;
    temp = *_out_Temp;  
    if(DDR_ADD == ddr)
    {
        if(temp >= max)
        {
            temp = max; 
        }
        else
        {
            temp += step;             
        }
    }
    else if(DDR_DEC == ddr)
    {
        if(temp <= min)
        {
            temp = min; 
        }
        else
        {
            temp -= step;
        }
    }
    
    if(temp != *_out_Temp)
    {
        *_out_Temp = temp;
        return true;
    }
    return false;
}

uint16_t datOperation(bool ddr,uint16_t step,uint16_t min,uint16_t max,bool loop,uint16_t dat)
{
    uint16_t DateCalculation;
    
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
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    keyActionType_t  currentKey;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
         if(highSetFlag)
         {
              highSetFlag = false;
              ptSysPara->record.sysRunStatus = SYS_STATUS_HIGH_SETTING;
              ptSysPara->disKeepFlag = false;
              ptSysPara->highSet.menu = HIGH_SETTING_NTC_ADJ;
              ptSysPara->highSet.facStatus = FAC_IDLE;
              app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);    
         }
         else
         {
              ptSysPara->record.sysRunStatus = SYS_STATUS_SHOW_ALL;
              app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_1000);
         }
         
//         app_pushControlDataFromDevice_update(regCloudAddr_power,regCloudAddr_power_on);
//        // app_pushControlDataFromDevice_update(regCloudAddr_measureTemperature,ptSysPara->runMeasTemp);
//         static INT16S test_t = -10;
//         app_pushControlDataFromDevice_update(regCloudAddr_measureTemperature,(INT16U)test_t++);
    }
    else if((PUSH_DOWN | PUSH_UP) & (currentKey = mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE)))
    {
         if(PUSH_UP == currentKey)
         {
              highSetFlag = false;
         }
         else
         {
             highSetFlag = true; 
         }
    }
       
}

void app_key_runMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    bool runSetTempChangeFlag = false;
    bool quickSetFlag         = false;
    bool    ddr;
    keyActionType_t keyValve;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        if(OW_STATUS_RUN == ptSysPara->owStatus)
        {
            ptSysPara->owStatus = OW_STATUS_INIT;
        }
        else
        {
            ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;
            mod_key_reCaptureOperation(KEY_MENU);
        }       
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
    else if((PUSH_DOWN | PUSH_CONTINUE) & (keyValve = mod_keyOperation(KEY_MENU,ms_3000,NOCONTINUE)))
    {
        if(PUSH_DOWN == keyValve)
        {           
            if(PROGRAM_OFF != ptSysPara->record.weekPrg)
            {
                if(TEMP_CONTROL_MODE_TIME == ptSysPara->record.tempControlMode)
                {
                    ptSysPara->record.tempControlMode = TEMP_CONTROL_MODE_MANUAL;
                }
                else
                {
                    ptSysPara->record.tempControlMode = TEMP_CONTROL_MODE_TIME;
                }  
            }                    
        }
        else if(PUSH_CONTINUE == keyValve)
        {
            if(PROGRAM_OFF != ptSysPara->record.weekPrg)
            {
                ptSysPara->record.sysRunStatus = SYS_STATUS_PROGRAM;
                ptSysPara->weekPrgMainMenu = WEEK_PRG_MENU_MAIN_INT0;
                ptSysPara->weekPrgSonMenu = WEEK_PRG_MENU_SON_TIME;
                app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);
                ptSysPara->disKeepFlag = false;
            }                    
        }      
    }
    else if(PUSH_DOWN & mod_keyOperation(KEY_QUICK,NOCONTINUE,NOCONTINUE))
    {
        
        runSetTempChangeFlag = true; 
        quickSetFlag = true;
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_RTC,ms_3000,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_RTCSET;
        ptSysPara->rtcSetMenu = SET_MENU_MINUTE;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_LINK,ms_3000,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_SMARTLINK;
        ptSysPara->wifi.linkStatus = WIFI_LINK_IDLE;        
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_AISET,ms_3000,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_AISET;
        ptSysPara->aiAdjSetMenu = AI_ADJ_MENU_T;        
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);   
    }
    
    if(runSetTempChangeFlag)
    {
        ptSysPara->blink_5second_flag = true;
        app_control_timeOut_set(DELAY_BILINK_5SECOND,ms_5000);
        if(OW_STATUS_RUN != ptSysPara->owStatus)
        {
            if(quickSetFlag)
            {
                ptSysPara->runSetTemp = ptSysPara->record.quickTemp;
            }
            else
            {
                tempOperation(ddr,5,MIN_SET_TEMP,ptSysPara->record.maxSetTemp,&ptSysPara->runSetTemp);    
            }                 
            if(TEMP_CONTROL_MODE_TIME == ptSysPara->record.tempControlMode)
            {
                ptSysPara->record.tempControlMode = TEMP_CONTROL_MODE_TEMPORARY;
            }
            else if(TEMP_CONTROL_MODE_MANUAL == ptSysPara->record.tempControlMode)
            {
                ptSysPara->record.manSetTemp = ptSysPara->runSetTemp;
            }  
        }       
    }    
}

void app_key_highSet_action(bool ddr)
{
    SysPara_t       *ptSysPara;    
    ptSysPara = controler_getSysParaPt(); 
    switch(ptSysPara->highSet.menu)
    {
        case HIGH_SETTING_NTC_ADJ:
        {
            app_control_timeOut_set(DELAY_DIS_KEEP,ms_5000);
            if(ptSysPara->disKeepFlag)
            {
                if(SENSOR_OUT == ptSysPara->record.sersonType)
                {
                    tempOperation(ddr,STEP_TEMP,MIN_ADJ_TEMP,MAX_ADJ_TEMP,\
                      &ptSysPara->record.outAdjTemp);
                }
                else 
                {
                    tempOperation(ddr,STEP_TEMP,MIN_ADJ_TEMP,MAX_ADJ_TEMP,\
                      &ptSysPara->record.inAdjTemp);
                }
            }
            else
            {
                ptSysPara->disKeepFlag = true;
            }
            break;
        }
        case HIGH_SETTING_OUTPUT_TPYE:
        {
            ptSysPara->record.outputType = (outputType_t)datOperation(ddr,1,0,\
              3,DATA_KEEP,(uint16_t)ptSysPara->record.outputType);
            
            break;
        }
        case HIGH_SETTING_LINK_DELAY:
        {         
            #if defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)           
             ptSysPara->record.linkRelayDelayTime = (uint8_t)datOperation(ddr,1,MIN_LINK_RELAY_DELAY,\
              MAX_LINK_RELAY_DELAY,DATA_KEEP,(uint16_t)ptSysPara->record.linkRelayDelayTime);
            #endif
            break;
        }
        case HIGH_SETTING_SERSON_TYPE:
        {
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)         
            ptSysPara->record.sersonType = (senCfg_t)datOperation(ddr,1,0,\
              2,DATA_LOOP,(uint16_t)ptSysPara->record.sersonType);
            #endif
            break;
        }
        case HIGH_SETTING_LIF:
        {           
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            tempOperation(ddr,STEP_TEMP,160,600,&ptSysPara->record.lifTemp);                     
            #endif
            break;
        }
        case HIGH_SETTING_DIF:
        {
            tempOperation(ddr,STEP_TEMP,5,100,&ptSysPara->record.diffTemp);
            break;
        }
        case HIGH_SETTING_LTP:
        {
            tempOperation(ddr,STEP_TEMP,45,100,&ptSysPara->record.ltpTemp);
            break;
        }
        case HIGH_SETTING_MAX_SET:
        {
            tempOperation(ddr,STEP_TEMP,50,950,&ptSysPara->record.maxSetTemp);
            break;
        }
        case HIGH_SETTING_WEEK_PRG:
        {
            ptSysPara->record.weekPrg = (weekPrg_t)datOperation(ddr,1,0,3,\
              DATA_KEEP,(uint16_t)ptSysPara->record.weekPrg);      
            break;
        }
        case HIGH_SETTING_QUICK:
        {
            tempOperation(ddr,STEP_TEMP,50,ptSysPara->record.maxSetTemp,\
              &ptSysPara->record.quickTemp);
            break;
        }
        case HIGH_SETTING_POWUP_OFF:
        {
            ptSysPara->record.powerOnOffFlag = (bool)datOperation(ddr,1,0,1,\
              DATA_LOOP,(uint16_t)ptSysPara->record.powerOnOffFlag);
            break;
        }
        case HIGH_SETTING_DESCALE:
        {           
            #if defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)
            ptSysPara->record.cleanFlag = (bool)datOperation(ddr,1,0,1,\
              DATA_LOOP,(uint16_t)ptSysPara->record.cleanFlag);                         
            #endif            
            break;
        }
        case HIGH_SETTING_DEMO:
        {
            ptSysPara->record.demoFlag = (bool)datOperation(ddr,1,0,1,\
              DATA_LOOP,(uint16_t)ptSysPara->record.demoFlag);
            break;
        }       
        case HIGH_SETTING_OW_ENABNE:
        {
            ptSysPara->record.owPara.enableFlag = (bool)datOperation(ddr,1,0,1,\
              DATA_LOOP,(uint16_t)ptSysPara->record.owPara.enableFlag );
            break;
        }
        case HIGH_SETTING_OW_CHECK_TIME:
        {
            ptSysPara->record.owPara.checkTime = (uint8_t)datOperation(ddr,1,\
              MIN_OW_CHECK_TIME,MAX_OW_CHECK_TIME,DATA_KEEP,(uint16_t)ptSysPara->record.owPara.checkTime);    
            break;
        }
        case HIGH_SETTING_OW_CHECK_TEMP:
        {
            tempOperation(ddr,STEP_TEMP,MIN_OW_DOWN_TEMP,MAX_OW_DOWN_TEMP,\
              &ptSysPara->record.owPara.downTemp);
            break;
        }
        case HIGH_SETTING_OW_BACK_TIME:
        {
            ptSysPara->record.owPara.backTime = (uint8_t)datOperation(ddr,1,\
              MIN_OW_BACK_TIME,MAX_OW_BACK_TIME,DATA_KEEP,(uint16_t)ptSysPara->record.owPara.backTime);    
            break;
        }
        case HIGH_SETTING_PREHEAT_ENABLE:
        {
            ptSysPara->record.preheat.switchflag = (bool)datOperation(ddr,1,0,1,\
              DATA_LOOP,(uint16_t)ptSysPara->record.preheat.switchflag);
            break;
        }
        case HIGH_SETTING_SENSOR_TYEP:
        {
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            ptSysPara->record.senMode = (senType_t)datOperation(ddr,1,0,1,\
              DATA_LOOP,(uint16_t)ptSysPara->record.senMode);
            mod_ntc3380_outsideSensorType((uint8_t)ptSysPara->record.senMode);
            #endif
            break;
        }
        case HIGH_SETTING_FAC:break;      
        default:break;
    }
}

void app_key_highSetMode(void)
{
    SysPara_t       *ptSysPara; 
    keyActionType_t  currentKey; 
    ptSysPara = controler_getSysParaPt(); 
    static bool GotoFacFlag = false;
    app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);
    
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;
        mod_key_reCaptureOperation(KEY_MENU);
    }  
    else if(PUSH_DOWN & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
    {   
        if(++ptSysPara->highSet.menu >= HIGH_SETTING_MAX_INDEX)
        {
            ptSysPara->highSet.menu = HIGH_SETTING_NTC_ADJ;
        }                
        if(HIGH_SETTING_FAC != ptSysPara->highSet.menu)
        {
             GotoFacFlag = false;             
             ptSysPara->highSet.facStatus = FAC_IDLE;
        }
        ptSysPara->disKeepFlag = false;      
    }         
    else if((PUSH_DOWN | PUSH_CONTINUE) & (currentKey = mod_keyOperation(KEY_ADD,ms_3000,ms_200)))
    {         
        app_key_highSet_action(DDR_ADD);
        if(HIGH_SETTING_FAC == ptSysPara->highSet.menu)
        {
            if(PUSH_CONTINUE == currentKey)
            {
                if(!GotoFacFlag)
                {
                    GotoFacFlag = true;                    
                    app_control_timeOut_set(DELAY_FAC,ms_500);
                    app_con_gotoFac(false);
                }
            }
        }                                      
    }
    else if((PUSH_DOWN | PUSH_CONTINUE) & mod_keyOperation(KEY_DEC,ms_3000,ms_200))
    {        
        app_key_highSet_action(DDR_DEC); 
    }   
}

void app_key_showVersion(void)
{
  
}

void app_key_showAll(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    if(PUSH_DOWN & mod_keyOperation(KEY_MENU,NOCONTINUE,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_HIGH_SETTING;
        ptSysPara->disKeepFlag = false;
        ptSysPara->highSet.menu = HIGH_SETTING_NTC_ADJ;
        ptSysPara->highSet.facStatus = FAC_IDLE;
        app_control_timeOut_set(DELAY_CONTROLER_COMMON,ms_60000);      
    }        
}

void app_key_smartlinkMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    bool wifiLinkFlag = false;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_ADD,ms_3000,NOCONTINUE))
    {
        if(WIFI_LINK_BUSY != ptSysPara->wifi.linkStatus)
        {
            wifiLinkFlag = true;
            ptSysPara->wifi.linkType = SMART_CONFIG;
        }           
    }
    else if(PUSH_CONTINUE & mod_keyOperation(KEY_DEC,ms_3000,NOCONTINUE))
    {
        if(WIFI_LINK_BUSY != ptSysPara->wifi.linkStatus)
        {
            wifiLinkFlag = true;
            ptSysPara->wifi.linkType = AP_CONFIG;                        
        }         
    }      
    if(wifiLinkFlag)
    {
        ptSysPara->wifi.linkDis = 0;
        ptSysPara->wifi.linkStatus = WIFI_LINK_DELAY;   
        app_entryWifiCfg_state((tuya_configType_def)ptSysPara->wifi.linkType);   
    }
}

void app_key_programMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    bool setFlag = false;
    bool ddr;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_POWER_OFF;       
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
            ptSysPara->disKeepFlag = false;           
            if(++ptSysPara->weekPrgSonMenu > WEEK_PRG_MENU_SON_TEMP)
            {
                ptSysPara->weekPrgSonMenu = WEEK_PRG_MENU_SON_TIME;
                uint8_t max = 0;                
                if(PROGRAM_7 == ptSysPara->record.weekPrg)                
                {
                    max = WEEK_PRG_MENU_MAIN_INT5;
                }
                else
                {
                    max = WEEK_PRG_MENU_MAIN_INT7;
                }
                if(++ptSysPara->weekPrgMainMenu > max)
                {
                     ptSysPara->weekPrgMainMenu = WEEK_PRG_MENU_MAIN_INT0;
                }
            }
        }       
    } 
    if(setFlag)
    {        
        uint16_t max = 0;
        uint16_t min = 0;
        ptSysPara->disKeepFlag = true;
        app_control_timeOut_set(DELAY_DIS_KEEP,ms_5000);               
        if(WEEK_PRG_MENU_SON_TIME == ptSysPara->weekPrgSonMenu)
        {            
             uint16_t hourMinute = 0;
             ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute = (ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute / 10) *10;
             hourMinute = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].hour * 60)\
               +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute);
             switch(ptSysPara->weekPrgMainMenu) 
             {
                case WEEK_PRG_MENU_MAIN_INT0:               
                case WEEK_PRG_MENU_MAIN_INT1:
                case WEEK_PRG_MENU_MAIN_INT2:
                case WEEK_PRG_MENU_MAIN_INT3:
                case WEEK_PRG_MENU_MAIN_INT4:
                case WEEK_PRG_MENU_MAIN_INT6:
                {
                    max = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu +1].hour * 60)\
                      +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu + 1].minute) - 10; 
                    if((WEEK_PRG_MENU_MAIN_INT0 == ptSysPara->weekPrgMainMenu) ||\
                      (WEEK_PRG_MENU_MAIN_INT6 == ptSysPara->weekPrgMainMenu))
                    {
                        min = 0;
                    }
                    else
                    {
                        min = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu -1].hour * 60)\
                      +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu - 1 ].minute) + 10; 
                    }
                    break;
                }
                case WEEK_PRG_MENU_MAIN_INT5:
                case WEEK_PRG_MENU_MAIN_INT7:
                {
                    max = 1430;
                    min = ((ptSysPara->record.interval[ptSysPara->weekPrgMainMenu -1].hour * 60)\
                      +ptSysPara->record.interval[ptSysPara->weekPrgMainMenu - 1 ].minute) + 10; 
                    break;
                }
             }           
             hourMinute = datOperation(ddr,10,min,max,DATA_LOOP,hourMinute);
             ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].hour = hourMinute / 60;
             ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].minute = hourMinute % 60;              
        }
        else if(WEEK_PRG_MENU_SON_TEMP == ptSysPara->weekPrgSonMenu)
        {
             int16_t *pt_temp;
             pt_temp = &ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].temp;
             max = ptSysPara->record.maxSetTemp;
             min = MIN_SET_TEMP;
             tempOperation(ddr,5,min,max,pt_temp);
        }
    }   
}

void app_key_rtcSetMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    bool setFlag = false;
    bool ddr;
    if(PUSH_DOWN & mod_keyOperation(KEY_POW,NOCONTINUE,NOCONTINUE))
    {
        ptSysPara->record.sysRunStatus = SYS_STATUS_RUN_MODE;      
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
        else if(PUSH_DOWN & mod_keyOperation(KEY_RTC,NOCONTINUE,NOCONTINUE))
        {
            ptSysPara->disKeepFlag = false;
            if(++ptSysPara->rtcSetMenu > SET_MENU_WEEK)
            {
                ptSysPara->rtcSetMenu = SET_MENU_MINUTE;
            }
        }  
    } 
    if(setFlag)
    {
        uint8_t *pt = NULL;
        uint16_t max = 0;
        uint16_t min = 0;
        uint16_t dat = 0;
        switch(ptSysPara->rtcSetMenu)
        {
            case SET_MENU_MINUTE:
            {   
                pt = &ptSysPara->rtc.minute;
                max = MAX_MINUTE;
                min = MIN_MINUTE;
                dat = ptSysPara->rtc.minute;
                break;
            }
            case SET_MENU_HOUR:
            {
                pt = &ptSysPara->rtc.hour;
                max = MAX_HOUR;
                min = MIN_HOUR;
                dat = ptSysPara->rtc.hour;
                break;
            }
            case SET_MENU_WEEK:
            {
                pt = &ptSysPara->rtc.week;
                max = MAX_WEEK;
                min = MIN_WEEK;
                dat = ptSysPara->rtc.week;
                break;
            }
            default:break;
        }
        dat = datOperation(ddr,1,min,max,DATA_LOOP,dat);
        *pt = (uint8_t)dat;
        ptSysPara->disKeepFlag = true;
        app_control_timeOut_set(DELAY_DIS_KEEP,ms_5000);
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
            para->aiAdjSetMenu = (aiAdjSetMenu_t)datOperation(DDR_ADD,1,0,1,true,(uint16_t)para->aiAdjSetMenu);
        }
    } 
    
    if(setFlag)
    {
        uint16_t max[2] = {100,50};
        uint16_t min[2] = {20,10};
        int16_t *pt = &para->record.aiAdjSetPara.t_max;
        uint8_t  menu = (uint8_t)para->aiAdjSetMenu;
        pt += menu;
        *pt = (int16_t)datOperation(ddr,1,min[menu],max[menu],false,(uint16_t)*pt);
    }
}


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************按键处理******************************************
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
       case SYS_STATUS_PROGRAM:           app_key_programMode();   break;
       case SYS_STATUS_RTCSET:            app_key_rtcSetMode();    break;
       case SYS_STATUS_AISET:              aiAdjSetMode();       break;   
       default:break;
    }      
}
//****************************按键处理******************************************
//++++++++++++++++++++++++++++++++end+++++++++++++++++++++++++++++++++++++++++++





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************按键任务*****************************************
void app_key_scanTask(void)
{      
    SysPara_t *ptSysPara;  
    ptSysPara =  controler_getSysParaPt( );
    mod_key_scanTask(GetSysTickMillisecond());   
    if(mod_key_getEvent())
    {        
        app_sto_eeprom_write();
        app_backLigth_on();
        ptSysPara->dispUpdataEvent = true;
        if(PUSH_CONTINUE & mod_keyOperation(KEY_CHILD,ms_3000,NOCONTINUE))
        {
            if(KEY_LOCK == ptSysPara->record.lockStatus)
            {
                 ptSysPara->record.lockStatus =  NONE_LOCK;
            }
            else if(NONE_LOCK == ptSysPara->record.lockStatus)
            {
                if((SYS_STATUS_POWER_OFF == ptSysPara->record.sysRunStatus)||\
                   (SYS_STATUS_RUN_MODE == ptSysPara->record.sysRunStatus))
                {
                    ptSysPara->record.lockStatus = KEY_LOCK;
                }                
            }
        }
        
        if(NONE_LOCK == ptSysPara->record.lockStatus)          
        {
            app_key_update(ptSysPara->record.sysRunStatus); 
        }                    
    }        
}
//****************************按键任务******************************************
//++++++++++++++++++++++++++++++++end+++++++++++++++++++++++++++++++++++++++++++



//-----------------------APP_KeyBoard.c--END------------------------------------

