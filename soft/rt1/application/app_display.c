#include ".\APP_Cfg.h"


const uint8_t BcdCodeTable[17] = 
{0X3F,0X06,0X5B,0X4F,0X66,
 0X6D,0X7D,0X07,0X7F,0X6F,
 0x00,0x40,0x61,0x38,0X37,
 0X77,0x00};



static uint8_t digBuf[MAX_DIG_RAM];
#define BITS_3  0
#define BITS_4  3


#define DIG_1_ADD   0
#define DIG_2_ADD   1
#define DIG_3_ADD   2
#define DIG_4_ADD   3
#define DIG_5_ADD   4
#define DIG_6_ADD   5

void app_dis_tempToBcd(int16_t temp,uint8_t *_out_buf)
{
    bool negativeFlag = false;
    if(0 > temp)
    {
        negativeFlag = true;
        temp = 0 - temp;
    } 
    if(temp > 995)
    {
        temp = 995;
    }
    _out_buf[0] = BcdCodeTable[temp / 100];
    _out_buf[1] = BcdCodeTable[temp % 100 / 10];
    _out_buf[2] = BcdCodeTable[temp % 10];
    if(negativeFlag)
    {
        _out_buf[0] = BcdCodeTable[TableNegative];        
    }
    else
    {
        if(BcdCodeTable[0] == _out_buf[0])
        {
            _out_buf[0] = BcdCodeTable[TABLE_BLACK];
        }  
    }         
}

void app_dis_datToBcd(uint8_t dat,uint8_t *_out_buf)
{
    _out_buf[0] = BcdCodeTable[dat / 100];
    _out_buf[1] = BcdCodeTable[dat % 100 / 10];
    _out_buf[2] = BcdCodeTable[dat % 10];  
    if(_out_buf[0]  == BcdCodeTable[0])
    {
        _out_buf[0] = BcdCodeTable[TABLE_BLACK];           
        if(_out_buf[1] == BcdCodeTable[0])
        {
            _out_buf[1] = BcdCodeTable[TABLE_BLACK];
        }                  
    }
}

void app_dis_3Bits_char(uint8_t bit1,uint8_t bit2,uint8_t bit3)
{
    digBuf[0] = bit1;
    digBuf[1] = bit2;
    digBuf[2] = bit3;
}

void app_dis_4Bits_char(uint8_t bit1,uint8_t bit2,uint8_t bit3,uint8_t bit4)
{
    digBuf[3] = bit1;
    digBuf[4] = bit2;
    digBuf[5] = bit3;
    //digBuf[6] = bit4;
}

void app_dis_bool(bool flag)
{
    if(flag)
    {
        digBuf[DIG_1_ADD] = CHAR_0;
        digBuf[DIG_2_ADD] = CHAR_N;       
    }
    else
    {
        digBuf[DIG_1_ADD] = CHAR_0;
        digBuf[DIG_2_ADD] = CHAR_F;      
        digBuf[DIG_3_ADD] = CHAR_F;      
    }
}

void app_dis_tempTop(int16_t temp)
{
    app_dis_tempToBcd(temp,&digBuf[DIG_1_ADD]);
    mod_lcd_char(DIS_DP1,BN_TRUE);
    mod_lcd_char(DIS_MEAS_TEMP_C,BN_TRUE);
  
}

void app_dis_tempBot(int16_t temp)
{
    app_dis_tempToBcd(temp,&digBuf[DIG_4_ADD]);
    mod_lcd_char(DIS_DP2,BN_TRUE);
    mod_lcd_char(DIS_SET_TEMP_C,BN_TRUE);
}

void app_dis_measTemp(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    uint8_t errNum = 0;
    bool disMeasTempFlag = true;   
    if(disMeasTempFlag)
    {
        if(INSIDE_ERROR & ptSys->systemError)
        {       
            errNum = 1;
        }
        else if(OUTSIDE_ERROR & ptSys->systemError)
        {
            errNum = 2;
        }
        else
        {
            app_dis_tempTop(ptSys->runMeasTemp);
            mod_lcd_char(DIS_MEAS_TEMP_CHAR,BN_TRUE);           
        }
        
        if(errNum)
        {
            digBuf[DIG_1_ADD] = CHAR_E;
            digBuf[DIG_2_ADD] = CHAR_R;
            digBuf[DIG_3_ADD] = BcdCodeTable[errNum - 1];
            if(ptSys->blinkEvent)
            {                
                uint8_t errBuf[2] = {DIS_INSIDE_SONSOR,DIS_OUTSIDE_SONSOR};
                mod_lcd_char((rt1_lcd_char_number_t)errBuf[errNum - 1],BN_TRUE);
                mod_lcd_char(DIS_ERR,BN_TRUE);
            }            
        }     
    }   
}

void app_dis_setTemp(int16_t setTemp,bool disFlag)
{
    if(disFlag)
    {
        app_dis_tempBot(setTemp);
        mod_lcd_char(DIS_SET_TEMP_CN,BN_TRUE);
    }   
}

void app_dis_timming(void)
{   
    SysPara_t *ptSys;         
    ptSys = controler_getSysParaPt();
    bool disFlag = false;
    if(SYS_STATUS_TIMING_SET == ptSys->record.sysRunStatus)
    {
        disFlag = true;
    }
    else
    {
        if(ptSys->timming.disFlag)
        {
            if(TIMMING_STATUS_RUN == ptSys->timming.status)
            {
                disFlag = true;
            }           
        }
    }
    
    if(disFlag)
    {
        if(48 >= ptSys->timming.halfHour)
        {
            bool disFlag = true;                
            uint8_t addr[2] = {DIS_POW_OFF,DIS_POW_ON};
            mod_lcd_char(DIS_HOURS_LATER,BN_TRUE);
            mod_lcd_char((rt1_lcd_char_number_t)addr[(uint8_t)ptSys->timming.setDdr],BN_TRUE);              
            if(TIMMING_STATUS_IDLE == ptSys->timming.status)
            {
                if(ptSys->blinkEvent)
                {
                    disFlag = false;
                }
            }
            if(disFlag)
            {
                digBuf[DIG_4_ADD] = BcdCodeTable[(ptSys->timming.halfHour / 2) / 10];
                if(digBuf[DIG_4_ADD] == BcdCodeTable[0])
                {
                    digBuf[DIG_4_ADD] = BLACK;
                }
                digBuf[DIG_5_ADD] = BcdCodeTable[(ptSys->timming.halfHour / 2) % 10];
                if(ptSys->timming.halfHour % 2)
                {
                    digBuf[DIG_6_ADD] = CHAR_5;
                }
                else
                {
                    digBuf[DIG_6_ADD] = CHAR_0;
                }
                mod_lcd_char(DIS_DP2,BN_TRUE);              
            }           
        } 
    }
}

void app_dis_lock(lockStatus_t status,bool blink)
{
    mod_lcd_char(DIS_LOCK,BN_TRUE);
    if(KEY_STATUS_BLINK == status)
    {
        if(blink)
        {
            mod_lcd_char(DIS_LOCK,BN_FALSE);
        }
    }
}

void app_dis_leftBox(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    
    //mod_lcd_char(DIS_IN_TO_HIGH_SET,BN_TRUE);
    mod_lcd_char(DIS_LEFT_BOX,BN_TRUE);
    if(ptSys->floorRelayFlag)
    {        
        mod_lcd_char(DIS_OUTPUT_HEAT_CN,BN_TRUE);
    }
    else
    {
        if(ptSys->record.outputTimeControFlag)
        {     
            if(TIME_CON_STATUS_REST == ptSys->timeContorl.status)
            {
                mod_lcd_char(DIS_TIME_CONROTL,BN_TRUE);
            }  
            else
            {
                mod_lcd_char(DIS_TEMP_KEEP,BN_TRUE);
            }
        }
        else
        {
            mod_lcd_char(DIS_TEMP_KEEP,BN_TRUE);
        }       
    }    
    //mod_lcd_char(DIS_FLOOR_WARM_MARK,BN_TRUE);
}
 
void app_dis_tempControl(tempControlMode_t mode,bool blink)
{
    if(TEMP_CONTROL_MODE_MANUAL != mode)
    {
        if(blink)
        {
            mod_lcd_char(DIS_MODE_AI,BN_TRUE);
        }        
    }
}

void app_dis_output(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    if(ptSys->floorRelayFlag)
    {      
        mod_lcd_char(DIS_OUTPUT_HEAT_CHAR,BN_TRUE);
    }
    else
    {
        #ifdef __OUT_SIDE_CONTROL__
        if(relay_out_limitFlag())
        {
            if(ptSys->blinkEvent)
            {
                mod_lcd_char(DIS_OUTPUT_HEAT_CHAR,BN_TRUE);
                mod_lcd_char(DIS_LIF,BN_TRUE);    
                mod_lcd_char(DIS_TEMP_CN,BN_TRUE);     
            }
        }
        #endif
    }
    if(ptSys->record.outputTimeControFlag)
    {
        if(TIME_CON_STATUS_REST == ptSys->timeContorl.status)
        {
            if(ptSys->blinkEvent)
            {
                mod_lcd_char(DIS_OUTPUT_REST,BN_TRUE);
            }
//             if(!ptSys->timeContorl.disTimeControlFlag) 
//             {
//                  
////                  uint8_t leftMinute = 0;
////                  leftMinute = ptSys->record.outputCloseLastTime - ptSys->timeContorl.runMinute;
////                  uint8_t buf[3];
////                  buf[0] = BcdCodeTable[leftMinute / 100];
////                  buf[1] = BcdCodeTable[leftMinute % 100 / 10];
////                  buf[2] = BcdCodeTable[leftMinute % 10];    
////                  if(leftMinute > 100)
////                  {
////                      digBuf[0] = buf[0];
////                      digBuf[1] = buf[1];
////                      digBuf[2] = buf[2];
////                  }
////                  else
////                  {
////                      if(leftMinute > 10)
////                      {
////                          digBuf[0] = buf[1];
////                          digBuf[1] = buf[2];
////                          digBuf[2] = BLACK;
////                      }
////                      else
////                      {
////                          digBuf[0] = BLACK;
////                          digBuf[1] = buf[2];
////                          digBuf[2] = BLACK;
////                      }
////                  }
////                  mod_lcd_char(DIS_MINUTE,BN_TRUE);
////                  mod_lcd_char(DIS_DP1,BN_FALSE);
////                  mod_lcd_char(DIS_MEAS_TEMP_MARK,BN_FALSE);
////                  mod_lcd_char(DIS_MEAS_TEMP_CHAR,BN_FALSE);
////                  mod_lcd_char(DIS_OUTPUT_WORK,BN_TRUE);
//             }
////             else
////             {
////                  if(ptSys->blinkEvent)
////                  {
////                      mod_lcd_char(DIS_OUTPUT_STOP,BN_TRUE);
////                  }
////             }
        }          
    }                  
}

void app_dis_rightBox(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    mod_lcd_char(DIS_RIGHT_BOX,BN_TRUE);
    app_dis_lock(ptSys->record.lockStatus,ptSys->blinkEvent);
    mod_lcd_char(DIS_FIVE_SECOND,BN_TRUE);       
    mod_lcd_char(DIS_ADD_AND_DEC,BN_TRUE);   
    //mod_lcd_char(DIS_MODE_MARK,BN_TRUE);
    if(WORK_MODE_SUN == ptSys->workMode)
    {      
        mod_lcd_char(DIS_MOD_SUN,BN_TRUE);       
    }  
    else
    {
        mod_lcd_char(DIS_MODE_MOON,BN_TRUE);
    }
}

void app_dis_runRtc(bool disFlag,uint16_t time,bool blink)
{
   
}

void app_dis_ltp(int16_t ltpTemp)
{

}

void app_dis_wifi(bool flag)
{
    if(flag)
    {
        mod_lcd_char(DIS_WIFI,BN_TRUE);       
    }
}

void app_dis_wifi_run(tuya_wifiState_def status,bool blink)
{
    mod_lcd_char(DIS_WIFI,BN_TRUE);
    if(status != WIFI_CONN_CLOUD)
    {
        if(blink)
        {
            mod_lcd_char(DIS_WIFI,BN_FALSE);            
        }
    } 
}

void ltp_handle(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    if(MIN_LTP_TEMP < ptSys->record.ltpSetTemp)
    {
        mod_lcd_char(DIS_LTP_CN,BN_TRUE);
        if(ptSys->floorRelayFlag && ptSys->blinkEvent)
        {
            mod_lcd_char(DIS_LTP_CN,BN_FALSE);
        }
    }   
}

void app_dis_powOffMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    app_dis_measTemp();
    app_dis_timming();  
    ltp_handle();
    #ifdef __WIFI_CONTROL__
    app_dis_wifi_run(app_pullTuyaWifiStateFormThisModule(),ptSysPara->blinkEvent);
    #endif
    if(!ptSysPara->timming.disFlag)
    {
        mod_lcd_char(DIS_POW_OFF,BN_TRUE);
        digBuf[DIG_4_ADD] = CHAR_O;
        digBuf[DIG_5_ADD] = CHAR_F;
        digBuf[DIG_6_ADD] = CHAR_F;
    }   
}

void app_dis_runMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    app_dis_measTemp();   
    app_dis_setTemp(ptSysPara->runSetTemp,!ptSysPara->timming.disFlag);
    app_dis_timming();
    app_dis_leftBox();
    app_dis_rightBox(); 
    app_dis_output();
    #ifdef __WIFI_CONTROL__
    app_dis_wifi_run(app_pullTuyaWifiStateFormThisModule(),ptSysPara->blinkEvent);  
    mod_lcd_char(DIS_BLUE_TOOTH,BN_TRUE);
    app_dis_tempControl(ptSysPara->tempControlMode,ptSysPara->blink_1000ms_flag);
    #endif
}

void app_dis_highSetMode(void)
{
    SysPara_t *ptSys;  
    ptSys = controler_getSysParaPt();
    
    uint8_t menu = 0;
    menu = (uint8_t)ptSys->highSet.menu;
    uint8_t ten ,unit;
    if(menu < 98)
    {
        ten = (menu + 1) / 10;
        unit = (menu + 1) % 10;
        if(ten)
        {
            digBuf[DIG_4_ADD] = BcdCodeTable[ten];
        }
        digBuf[DIG_5_ADD] = BcdCodeTable[unit];
    }
    
    mod_lcd_char(DIS_HIGH_SET,BN_TRUE);
    bool disTempFlag = false;
    bool measFlag = false;
    int16_t *ptTemp;
    switch(ptSys->highSet.menu)
    {
        case HIGH_SETTING_NTC_ADJ:
        {               
            mod_lcd_char(DIS_TEMP_ADJ,BN_TRUE);
            if(ptSys->highSet.disAdjTempFlag)
            {
                disTempFlag = true;       
                ptTemp = &ptSys->record.inAdjTemp;
                if(SENSOR_OUT == ptSys->record.sensorMode)
                {
                    ptTemp++;                   
                }                                                         
            }
            else
            {
                uint8_t err = 0;
                if(SENSOR_OUT == ptSys->record.sensorMode)
                {
                    if(OUTSIDE_ERROR & ptSys->systemError)
                    {                       
                        err = 2;
                    }
                }
                else
                {
                    if(INSIDE_ERROR & ptSys->systemError)
                    {                       
                        err = 1;
                    }                 
                }
                
                if(err)
                {
                    digBuf[DIG_1_ADD] = CHAR_E;
                    digBuf[DIG_2_ADD] = CHAR_R;
                    digBuf[DIG_3_ADD] = BcdCodeTable[err - 1];
                }
                else
                {
                    ptTemp = &ptSys->runMeasTemp;
                    disTempFlag = true; 
                    measFlag = true;                 
                }
            }
            break;
        }
        case HIGH_SETTING_MAX_SET_TEMP:
        {            
            mod_lcd_char(DIS_MAX_SET_TEMP,BN_TRUE);
            disTempFlag = true;
            ptTemp = &ptSys->record.maxSetTemp;
            break;
        }
        case HIGH_SETTING_MIN_SET_TEMP:
        {            
            mod_lcd_char(DIS_MIN_SET_TEMP,BN_TRUE);
            disTempFlag = true;
            ptTemp = &ptSys->record.minSetTemp;
            break;
        }
        case HIGH_SETTING_DIF:
        {            
            mod_lcd_char(DIS_DIF,BN_TRUE);
            disTempFlag = true;
            ptTemp = &ptSys->record.diffTemp;
            break;
        }
        case HIGH_SETTING_POF:
        {    
            mod_lcd_char(DIS_POW_ON_STATUS,BN_TRUE);           
            mod_lcd_char(DIS_POW_OFF,BN_TRUE);
            
            if(ptSys->record.powerOnOffFlag)
            {                             
                digBuf[0] = CHAR_O;
                digBuf[1] = CHAR_N;
            }
            else
            {                             
                digBuf[0] = CHAR_O;
                digBuf[1] = CHAR_F;
                digBuf[2] = CHAR_F;
            }
            break;
        }
        case HIGH_SETTING_SENSOR_TYPE:
        {                
            mod_lcd_char(DIS_SENSOR,BN_TRUE);
            uint8_t addr[3] = {DIS_INSIDE_SONSOR,DIS_OUTSIDE_SONSOR,DIS_LIF};           
            mod_lcd_char((rt1_lcd_char_number_t)addr[0],BN_TRUE);     
            mod_lcd_char((rt1_lcd_char_number_t)addr[1],BN_TRUE); 
            mod_lcd_char((rt1_lcd_char_number_t)addr[2],BN_TRUE); 
            if(ptSys->blinkEvent)
            {
                switch(ptSys->record.sensorMode)
                {
                    case SENSOR_IN  : mod_lcd_char(DIS_INSIDE_SONSOR,BN_FALSE);break;
                    case SENSOR_OUT : mod_lcd_char(DIS_OUTSIDE_SONSOR,BN_FALSE);break;
                    case SENSOR_ALL : mod_lcd_char(DIS_INSIDE_SONSOR,BN_FALSE);mod_lcd_char(DIS_LIF,BN_FALSE);break;
                }                              
            }                         
            break;
        }
        case HIGH_SETTING_LIF:
        {
            mod_lcd_char(DIS_LIF,BN_TRUE);
            mod_lcd_char(DIS_TEMP_CN,BN_TRUE);
            disTempFlag = true;
            ptTemp = &ptSys->record.lifSetTemp;
            break;
        }
        case HIGH_SETTING_COMFORTABLE_SET_TEMP:
        {           
            mod_lcd_char(DIS_MOD_SUN,BN_TRUE);
            disTempFlag = true;
            ptTemp = &ptSys->record.sunSetTemp;
            break;
        }
        case HIGH_SETTING_SAVE_SET_TEMP:
        {           
            mod_lcd_char(DIS_MODE_MOON,BN_TRUE);
            disTempFlag = true;
            ptTemp = &ptSys->record.moonSetTemp;
            break;
        }
        case HIGH_SETTING_OUTPUT_OPEN_TIME:
        case HIGH_SETTING_OUTPUT_CLOSE_TIME:
        {
            uint8_t addr[2] = {DIS_OUTPUT_WORK,DIS_OUTPUT_REST}; 
            mod_lcd_char(DIS_MINUTE,BN_TRUE);
            mod_lcd_char(DIS_TIME_CONROTL,BN_TRUE);           
            if(HIGH_SETTING_OUTPUT_OPEN_TIME <= menu)
            {                
                mod_lcd_char((rt1_lcd_char_number_t)(addr[menu - HIGH_SETTING_OUTPUT_OPEN_TIME]),BN_TRUE);               
                uint8_t *ptTime = &ptSys->record.outputOpenLastTime;
                if(ptSys->record.outputTimeControFlag)
                {
                    if(HIGH_SETTING_OUTPUT_CLOSE_TIME == menu)
                    {
                        ptTime++;
                    }
                    if(*ptTime <= 240)
                    {
                        digBuf[0] = BcdCodeTable[*ptTime / 100];
                        digBuf[1] = BcdCodeTable[*ptTime % 100 / 10];
                        digBuf[2] = BcdCodeTable[*ptTime % 10];
                        if(digBuf[0] == BcdCodeTable[0])
                        {
                            digBuf[0] = BLACK;
                            if(digBuf[1] == BcdCodeTable[0])
                            {
                                digBuf[1] = BLACK;
                            }
                        }
                    }
                }
                else
                {
                    digBuf[0] = CHAR_O;
                    digBuf[1] = CHAR_F;
                    digBuf[2] = CHAR_F;
                }              
            }              
            break;
        }
        case HIGH_SETTING_LTP:
        {
            mod_lcd_char(DIS_LTP_CN,BN_TRUE);
            if(MIN_LTP_TEMP == ptSys->record.ltpSetTemp)
            {
                digBuf[0] = CHAR_O;
                digBuf[1] = CHAR_F;
                digBuf[2] = CHAR_F;
            }
            else
            {
                disTempFlag = true;
                ptTemp = &ptSys->record.ltpSetTemp;
            }
            break;
        }
        case HIGH_SETTING_RELAY_OUT_TYPE:
        {
            mod_lcd_char(DIS_OUTPUT_TYPE,BN_TRUE);
            digBuf[0] = BcdCodeTable[(uint8_t)ptSys->record.relayOutType / 10];
            digBuf[1] = BcdCodeTable[(uint8_t)ptSys->record.relayOutType % 10];
            break;
        }
        case HIGH_SETTING_FAC:
        {           
            mod_lcd_char(DIS_FAC_SET,BN_TRUE);
            if(FAC_IDLE == ptSys->highSet.facStatus)
            {
                if(ptSys->blinkEvent)
                {
                    digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
                }              
            }
            else if(FAC_START == ptSys->highSet.facStatus)
            {
                digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
            }
            else if(FAC_BUSY == ptSys->highSet.facStatus)
            {
                digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
                digBuf[DIG_2_ADD] = CHAR_NEGATIVE;
            }
            else if(FAC_FINISH == ptSys->highSet.facStatus)
            {
                digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
                digBuf[DIG_2_ADD] = CHAR_NEGATIVE;
                digBuf[DIG_3_ADD] = CHAR_NEGATIVE;
            }
            break;
        }
        default:break;
    }
    if(disTempFlag)
    {
        app_dis_tempTop(*ptTemp);
        if(measFlag)
        {          
            mod_lcd_char(DIS_MEAS_TEMP_CHAR,BN_TRUE);
        }
    }
    
    if(ptSys->blinkEvent && ptSys->highSet.blinkFlag)
    {
        mod_lcd_char(DIS_ADD_AND_DEC,BN_TRUE);
    }
}

void app_dis_showVersion(void)
{  
    #ifdef __RT1_13__
    digBuf[0] = CHAR_1;
    digBuf[1] = CHAR_3;
    digBuf[2] = BLACK;
    #elif defined __RT1_36t__
    digBuf[0] = CHAR_3;
    digBuf[1] = CHAR_6;
    digBuf[2] = CHAR_T;
    #elif defined __RT1_23i__
    digBuf[0] = CHAR_2;
    digBuf[1] = CHAR_3;
    digBuf[2] = CHAR_I;
    #elif defined __RT1_23__
    digBuf[0] = CHAR_2;
    digBuf[1] = CHAR_3;
    //digBuf[2] = CHAR_I;
    #elif defined __RT1_36i__
    digBuf[0] = CHAR_3;
    digBuf[1] = CHAR_6;
    digBuf[2] = CHAR_I;
    #endif 
    
    uint8_t *pt = MCU_VER;
    digBuf[3] = BcdCodeTable[pt[0] & (~0x30)];
    digBuf[4] = BcdCodeTable[pt[2] & (~0x30)];
    digBuf[5] = BcdCodeTable[pt[4] & (~0x30)];
}

void app_dis_showAll(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    mod_lcd_show_all(BN_TRUE);   
    if(ptSysPara->record.autoGotoFac)
    {
        mod_lcd_char(DIS_HIGH_SET,BN_FALSE);
        mod_lcd_char(DIS_TEMP_ADJ,BN_FALSE);
        mod_lcd_char(DIS_MAX_SET_TEMP,BN_FALSE);
        mod_lcd_char(DIS_MIN_SET_TEMP,BN_FALSE);
        mod_lcd_char(DIS_DIF,BN_FALSE);
        mod_lcd_char(DIS_POW_ON_STATUS,BN_FALSE);
        mod_lcd_char(DIS_POW_OFF,BN_FALSE);
        mod_lcd_char(DIS_SENSOR,BN_FALSE);
        mod_lcd_char(DIS_INSIDE_SONSOR,BN_FALSE);
        mod_lcd_char(DIS_OUTSIDE_SONSOR,BN_FALSE);
        mod_lcd_char(DIS_LIF,BN_FALSE);
        mod_lcd_char(DIS_MINUTE,BN_FALSE);
        mod_lcd_char(DIS_OUTPUT_WORK,BN_FALSE);
        mod_lcd_char(DIS_OUTPUT_REST,BN_FALSE);
        mod_lcd_char(DIS_FAC_SET,BN_FALSE);
        mod_lcd_char(DIS_HOURS_LATER,BN_FALSE);
        mod_lcd_char(DIS_POW_ON,BN_FALSE);
        mod_lcd_char(DIS_TEMP_CN,BN_FALSE);
        mod_lcd_char(DIS_OUTPUT_TYPE,BN_FALSE);
        mod_lcd_char(DIS_ERR,BN_FALSE);
        mod_lcd_char(DIS_LTP_CN,BN_FALSE);
        mod_lcd_char(DIS_SET_TEMP_CN,BN_FALSE);
        #ifndef __WIFI_CONTROL__
        mod_lcd_char(DIS_MODE_AI,BN_FALSE);
        mod_lcd_char(DIS_BLUE_TOOTH,BN_FALSE);
        #endif
    }        
}

void app_dis_smartlinkMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    
    if(WIFI_LINK_IDLE == ptSysPara->wifi.linkStatus)
    {
        app_dis_3Bits_char(CHAR_C,0X09,0X0F);
        app_dis_4Bits_char(CHAR_I,CHAR_D,CHAR_L,CHAR_E);
        app_dis_wifi(true);
    }
    else if(WIFI_LINK_DELAY == ptSysPara->wifi.linkStatus)
    {
        app_dis_3Bits_char(CHAR_C,0X09,0X0F);
        if(ptSysPara->blinkEvent)
        {
             app_dis_4Bits_char(CHAR_D,CHAR_E,CHAR_L,CHAR_Y);
        }       
        app_dis_wifi(true);
    }
    else if(WIFI_LINK_SUCCESS == ptSysPara->wifi.linkStatus)
    {
        app_dis_3Bits_char(BLACK,CHAR_O,CHAR_N);
        app_dis_4Bits_char(CHAR_L,CHAR_I,CHAR_N,CHAR_E);
        app_dis_wifi(ptSysPara->wifi.onLine);
    }
    else 
    {
        if(WIFI_LINK_BUSY == ptSysPara->wifi.linkStatus)
        {   
            uint8_t linkData[10]={0x01,0x01,0x01,0x02,0x04,0x08,0x08,0x08,0x10,0x20};// A A A B C D D D E F
            uint8_t linkSelection[10]={1,2,3,3,3,3,2,1,1,1};                         // 1 2 3 3 3 3 2 1 1 1
            digBuf[linkSelection[ptSysPara->wifi.linkDis] -1] = linkData[ptSysPara->wifi.linkDis];       
            
        }
        else if(WIFI_LINK_FAIL == ptSysPara->wifi.linkStatus)
        {
            app_dis_3Bits_char(CHAR_F,CHAR_A,CHAR_L);
        }       
        app_dis_wifi(ptSysPara->blinkEvent);
       
        switch(app_pullTuyaWifiStateFormThisModule())
        {
            case SMART_CONFIG_STATE:
            {
                app_dis_4Bits_char(CHAR_S,CHAR_A,CHAR_R,CHAR_T);
                break;
            }
            case AP_STATE:
            {
                app_dis_4Bits_char(CHAR_A,CHAR_P,BLACK,BLACK);
                break;
            }
            case WIFI_NOT_CONNECTED:
            {
                app_dis_4Bits_char(CHAR_N,CHAR_O,CHAR_T,CHAR_C);
                break;
            }  
            case WIFI_CONNECTED:
            {
                app_dis_4Bits_char(CHAR_C,CHAR_O,CHAR_E,CHAR_D);
                break;
            } 
            case WIFI_CONN_CLOUD:
            {
                app_dis_4Bits_char(CHAR_C,CHAR_O,CHAR_C,CHAR_D);
                break;
            }
            case WIFI_LOW_POWER:
            {
                app_dis_4Bits_char(CHAR_L,CHAR_O,CHAR_P,CHAR_O);
                break;
            }
            case WIFI_SATE_UNKNOW:
            {
                app_dis_4Bits_char(CHAR_U,CHAR_K,CHAR_N,CHAR_O);
                break;
            }
        }
    }
}

void app_dis_programMode(void)
{
  
}

void app_dis_rtcSetMode(void)
{
}

void app_dis_timmingSetMode(void)
{   
    //lcdBuf[DIS_SET_TEMP_MARK_ADD] |= DIS_SET_TEMP_MARK_DAT; 
    mod_lcd_char(DIS_SET_TEMP_CN,BN_TRUE);
    app_dis_timming();
}



void app_display_updata(systemRunStatus_t sysRunStatus)
{
    switch(sysRunStatus)
    {               
       case SYS_STATUS_POWER_OFF:            app_dis_powOffMode();      break;
       case SYS_STATUS_RUN_MODE:             app_dis_runMode();         break;
       case SYS_STATUS_HIGH_SETTING:         app_dis_highSetMode();     break;     
       case SYS_STATUS_SHOW_VERSIONS:        app_dis_showVersion();     break;   
       case SYS_STATUS_SHOW_ALL:             app_dis_showAll();          break;  
       case SYS_STATUS_SMARTLINK:            app_dis_smartlinkMode();   break;
       case SYS_STATUS_TIMING_SET:           app_dis_timmingSetMode();  break;      
       default:break;
    }      
}

void app_dis_clr(void)
{
    uint8_t i = 0;
    for(;i < 7;i++)
    {
        digBuf[i] = 0x00;      
    }
    mod_lcd_show_all(BN_FALSE);
}

void app_display_scanTask(void)
{         
    SysPara_t *ptSysPara;  
    ptSysPara =  controler_getSysParaPt();
    static bool initFlag = false;
    if(!initFlag)
    {
        initFlag = true;
        mod_lcd_configure();
    }
    if(ptSysPara->dispUpdataEvent)
    {
        ptSysPara->dispUpdataEvent = false;
        #ifndef __LCD_STATUS_DEBUG__
        app_dis_clr();
        app_display_updata(ptSysPara->record.sysRunStatus); 
        mod_lcd_dig(digBuf); 
        #else
        //lcdBuf[DIS_DIG4_B_ADD] |= DIS_DIG4_B_DAT;
        #endif
        mod_lcd_refresh();
    }
}


#if defined (__LCD_STATUS_DEBUG__)

#endif
//------------------------------E N D-------------------------------------------



//-------------------------APP_Display.c--END-----------------------------------




