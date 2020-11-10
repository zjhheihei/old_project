#include ".\APP_Cfg.h"


const uint8_t BcdCodeTable[17] = {0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,
0X6F,0x00,0x40,0x61,0x38,0X37,0X77,0x00};



static uint8_t digBuf[MAX_DIG_RAM];
static uint8_t lcdBuf[MAX_LCD_RAM];
#define BITS_3  0
#define BITS_4  3




void app_dis_tempToBcd(int16_t temp,uint8_t *_out_buf)
{
    bool negativeFlag = false;
    if(0 > temp)
    {
        negativeFlag = true;
        temp = 0 - temp;
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
    digBuf[6] = bit4;
}

void app_dis_bool(bool flag)
{
    if(flag)
    {
        app_dis_3Bits_char(BLACK,CHAR_O,CHAR_N);
    }
    else
    {
        app_dis_3Bits_char(CHAR_O,CHAR_F,CHAR_F);
    }
}

void app_dis_3Bits_temp(int16_t temp)
{
    app_dis_tempToBcd(temp,&digBuf[BITS_3]);
    lcdBuf[DIS_P_ADD] |= DIS_P_DAT;
    lcdBuf[DIS_MEAS_TEMP_MARK_ADD] |= DIS_MEAS_TEMP_MARK_DAT;
}

void app_dis_4Bits_temp(int16_t temp)
{
    
    app_dis_tempToBcd(temp,&digBuf[BITS_4]);
    lcdBuf[DIS_SET_TEMP_MARK_ADD] |= DIS_SET_TEMP_MARK_DAT;
    lcdBuf[DIS_P2_ADD] |= DIS_P2_DAT;
    digBuf[6] = BcdCodeTable[TableUnitC];
}

void app_dis_measTemp(int16_t temp,uint8_t error)
{
    if(error)
    {
        if(INSIDE_ERROR & error)
        {
            app_dis_3Bits_char(CHAR_E,CHAR_R,CHAR_0);
        }
        else if(OUTSIDE_ERROR & error)
        {
            app_dis_3Bits_char(CHAR_E,CHAR_R,CHAR_1);
        }
    }
    else
    {
        app_dis_3Bits_temp(temp);
        lcdBuf[DIS_MEAS_TEMP_ADD] |= DIS_MEAS_TEMP_DAT;
    }  
}

void app_dis_setTemp(int16_t temp,bool flag,bool blink)
{
    if(flag)
    {
        if(blink)
        {
             app_dis_4Bits_temp(temp);
        }       
        lcdBuf[DIS_SET_TEMP_ADD] |= DIS_SET_TEMP_DAT;
    }   
}

void app_dis_runRtc(bool disFlag,uint16_t time,bool blink)
{
    if(!disFlag)
    {
        uint8_t buf[4];
        buf[0] = BcdCodeTable[(time >> 8) / 10];
        buf[1] = BcdCodeTable[(time >> 8) % 10];
        buf[2] = BcdCodeTable[(time & 0x00ff) / 10];
        buf[3] = BcdCodeTable[(time & 0x00ff) % 10];
        app_dis_4Bits_char(buf[0],buf[1],buf[2],buf[3]);
        if(blink)
        {
            lcdBuf[DIS_COL_ADD] |= DIS_COL_DAT;
        }
    }   
}

void app_dis_runWeek(uint8_t week)
{
    switch(week)
    {
        case 0: lcdBuf[DIS_SUNDAY_ADD] |= DIS_SUNDAY_DAT;       break;
        case 1: lcdBuf[DIS_MONDAY_ADD] |= DIS_MONDAY_DAT;       break;
        case 2: lcdBuf[DIS_TUESDAY_ADD] |= DIS_TUESDAY_DAT;     break;
        case 3: lcdBuf[DIS_WEDNESDAY_ADD] |= DIS_WEDNESDAY_DAT; break;
        case 4: lcdBuf[DIS_THURSDAY_ADD] |= DIS_THURSDAY_DAT;   break;
        case 5: lcdBuf[DIS_FRIDAY_ADD] |= DIS_FRIDAY_DAT;       break;
        case 6: lcdBuf[DIS_SATURDAY_ADD] |= DIS_SATURDAY_DAT;   break;
        case DIS_WEEK_12345:
        case DIS_WEEK_123456:
        case DIS_WEEK_1234567: 
        {
            lcdBuf[DIS_MONDAY_ADD] |= DIS_MONDAY_DAT;
            lcdBuf[DIS_TUESDAY_ADD] |= DIS_TUESDAY_DAT;
            lcdBuf[DIS_WEDNESDAY_ADD] |= DIS_WEDNESDAY_DAT;
            lcdBuf[DIS_THURSDAY_ADD] |= DIS_THURSDAY_DAT;
            lcdBuf[DIS_FRIDAY_ADD] |= DIS_FRIDAY_DAT;
            if(DIS_WEEK_12345 != week)
            {
               lcdBuf[DIS_SATURDAY_ADD] |= DIS_SATURDAY_DAT; 
            }
            if(DIS_WEEK_1234567 == week)
            {
               lcdBuf[DIS_SUNDAY_ADD] |= DIS_SUNDAY_DAT; 
            }
            break;
        }
        case DIS_WEEK_7:
        case DIS_WEEK_67:
        {
            lcdBuf[DIS_SUNDAY_ADD] |= DIS_SUNDAY_DAT;
            if(DIS_WEEK_67 == week)
            {
                lcdBuf[DIS_SATURDAY_ADD] |= DIS_SATURDAY_DAT; ;
            }
            break;
        }        
        default:
        {
            lcdBuf[DIS_SUNDAY_ADD] &= ~DIS_SUNDAY_DAT;
            lcdBuf[DIS_MONDAY_ADD] &= ~DIS_MONDAY_DAT;
            lcdBuf[DIS_TUESDAY_ADD] &= ~DIS_TUESDAY_DAT;
            lcdBuf[DIS_WEDNESDAY_ADD] &= ~DIS_WEDNESDAY_DAT;
            lcdBuf[DIS_THURSDAY_ADD] &= ~DIS_THURSDAY_DAT;
            lcdBuf[DIS_FRIDAY_ADD] &= ~DIS_FRIDAY_DAT;
            lcdBuf[DIS_SATURDAY_ADD] &= ~DIS_SATURDAY_DAT;
            break; 
        }         
    }
}

void app_dis_interval(uint8_t interval)
{
    switch(interval)
    {
        case 0: lcdBuf[DIS_INTERVAL_0_ADD] |= DIS_INTERVAL_0_DAT; break;
        case 1: lcdBuf[DIS_INTERVAL_1_ADD] |= DIS_INTERVAL_1_DAT; break;
        case 2: lcdBuf[DIS_INTERVAL_2_ADD] |= DIS_INTERVAL_2_DAT; break;
        case 3: lcdBuf[DIS_INTERVAL_3_ADD] |= DIS_INTERVAL_3_DAT; break;
        case 4: lcdBuf[DIS_INTERVAL_4_ADD] |= DIS_INTERVAL_4_DAT; break;
        case 5: lcdBuf[DIS_INTERVAL_5_ADD] |= DIS_INTERVAL_5_DAT; break;
        case 6: lcdBuf[DIS_INTERVAL_0_ADD] |= DIS_INTERVAL_0_DAT; break;
        case 7: lcdBuf[DIS_INTERVAL_5_ADD] |= DIS_INTERVAL_5_DAT; break;
        default:break;
    }
}

void app_dis_tempControlMode(SysPara_t *pt)
{
    bool disTempFlag = true;
    bool blink = true;
    if(PROGRAM_OFF != pt->record.weekPrg)
    {        
        switch(pt->record.tempControlMode)
        {
            case TEMP_CONTROL_MODE_MANUAL:
            {
                lcdBuf[DIS_MOD_MAN_SET_ADD] |= DIS_MOD_MAN_SET_DAT;
                disTempFlag = true;
                break;
            } 
            default:
            {
                if(TEMP_CONTROL_MODE_TEMPORARY == pt->record.tempControlMode)
                {
                    lcdBuf[DIS_MOD_MAN_SET_ADD] |= DIS_MOD_MAN_SET_DAT;
                }
                
                if(app_preheat_getStatus())
                {
                    if(pt->blinkEvent)
                    {
                         lcdBuf[DIS_MOD_TIME_SET_ADD] |= DIS_MOD_TIME_SET_DAT;                        
                    }
                    blink = pt->blinkEvent;
                }
                else
                {
                    lcdBuf[DIS_MOD_TIME_SET_ADD] |= DIS_MOD_TIME_SET_DAT;
                }
                
                disTempFlag = pt->blink_5second_flag;                             
                app_dis_runRtc(pt->blink_5second_flag,pt->rtc.hourMinute,pt->blinkEvent);
                app_dis_runWeek(pt->rtc.week);
                app_dis_interval(app_interval_getInterval());
                break;
            }         
        }
    }
    
    if(OW_STATUS_RUN == pt->owStatus)
    {
        if(disTempFlag)
        {
            app_dis_4Bits_char(CHAR_O,CHAR_P,CHAR_E,CHAR_N);
        }
    }
    else
    {
        app_dis_setTemp(pt->runSetTemp,disTempFlag,blink);  
    }     
}

void app_dis_ltp(int16_t ltpTemp)
{
    if(MIN_LOW_PROTECT_TEMP < ltpTemp)
    {
        lcdBuf[DIS_LTP_ADD] |= DIS_LTP_DAT; 
    }
}

void app_dis_output(bool outputFlag,bool lifFlag,bool blinkFlag)
{
    if(lifFlag)
    {
        if(blinkFlag)
        {
            lcdBuf[DIS_OUTPUT_FLOOR_ADD] |= DIS_OUTPUT_FLOOR_DAT;
        }
    }
    else
    {
        if(outputFlag)
        {
            lcdBuf[DIS_OUTPUT_FLOOR_ADD] |= DIS_OUTPUT_FLOOR_DAT;
        }
    }
}

void app_dis_lock(lockStatus_t status)
{
    if(KEY_LOCK == status)
    {
        lcdBuf[DIS_LOCK_ADD] |= DIS_LOCK_DAT;
    }
}

void app_dis_wifi(bool flag)
{
    if(flag)
    {
        lcdBuf[DIS_WIFI_ADD] |= DIS_WIFI_DAT;
    }
}

void app_dis_wifi_run(tuya_wifiState_def status)
{
    if(status == WIFI_CONN_CLOUD)
    {
         lcdBuf[DIS_WIFI_ADD] |= DIS_WIFI_DAT;
    }
}

void app_dis_powOffMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    app_dis_measTemp(ptSysPara->runMeasTemp,ptSysPara->systemError);
    app_dis_4Bits_char(CHAR_0,CHAR_F,CHAR_F,BLACK);
    app_dis_ltp(ptSysPara->record.ltpTemp);
    app_dis_output(ptSysPara->floorRelayFlag,false,ptSysPara->blinkEvent);
    app_dis_lock(ptSysPara->record.lockStatus);
    app_dis_wifi_run(app_pullTuyaWifiStateFormThisModule());
}

void app_dis_runMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    app_dis_measTemp(ptSysPara->runMeasTemp,ptSysPara->systemError);
    app_dis_output(ptSysPara->floorRelayFlag,relay_out_limitFlag(),ptSysPara->blinkEvent);
    
    app_dis_tempControlMode(ptSysPara);
    app_dis_lock(ptSysPara->record.lockStatus);
    app_dis_wifi_run(app_pullTuyaWifiStateFormThisModule());
}

void app_dis_highSetMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    switch(ptSysPara->highSet.menu)
    {
        case HIGH_SETTING_NTC_ADJ:
        {
            app_dis_4Bits_char(CHAR_1,CHAR_A,CHAR_D,CHAR_J);
            if(ptSysPara->disKeepFlag)
            {
                if(SENSOR_OUT == ptSysPara->record.sersonType)
                {
                    app_dis_3Bits_temp(ptSysPara->record.outAdjTemp);                                                         
                }
                else 
                {
                    app_dis_3Bits_temp(ptSysPara->record.inAdjTemp);                                    
                }                
            }
            else
            {
                if(SENSOR_OUT == ptSysPara->record.sersonType)
                {
                    if(OUTSIDE_ERROR & ptSysPara->systemError)
                    {
                        app_dis_3Bits_char(CHAR_E,CHAR_R,CHAR_1);
                    }
                    else
                    {
                        app_dis_3Bits_temp(ptSysPara->runMeasTemp);
                    }
                }
                else
                {
                    if(INSIDE_ERROR & ptSysPara->systemError)
                    {
                        app_dis_3Bits_char(CHAR_E,CHAR_R,CHAR_0);
                    }
                    else
                    {
                        app_dis_3Bits_temp(ptSysPara->runMeasTemp);
                    }
                }               
            }
            break;
        }
        case HIGH_SETTING_OUTPUT_TPYE:
        {
            app_dis_4Bits_char(CHAR_2,CHAR_R,CHAR_L,CHAR_E);            
            app_dis_datToBcd((int16_t)ptSysPara->record.outputType,&digBuf[BITS_3]);
            digBuf[BITS_3 + 1] = CHAR_0; 
            break;
        }
        case HIGH_SETTING_LINK_DELAY:
        {
            app_dis_4Bits_char(CHAR_3,CHAR_D,CHAR_L,CHAR_Y);
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,CHAR_NEGATIVE);
            #elif defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)
            app_dis_datToBcd((int16_t)ptSysPara->record.linkRelayDelayTime,&digBuf[BITS_3]);
            #endif
            break;
        }
        case HIGH_SETTING_SERSON_TYPE:
        {
            app_dis_4Bits_char(CHAR_4,CHAR_S,CHAR_E,CHAR_N);
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            uint8_t buf[3][3] = {BLACK,CHAR_I,CHAR_N,CHAR_O,CHAR_U,CHAR_T,CHAR_A,CHAR_L,CHAR_L};
            uint8_t cnt = (uint8_t)ptSysPara->record.sersonType;           
            app_dis_3Bits_char(buf[cnt][0],buf[cnt][1],buf[cnt][2]);
            #elif defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)
            app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,CHAR_NEGATIVE);
            #endif
            break;
        }
        case HIGH_SETTING_LIF:
        {
            app_dis_4Bits_char(CHAR_5,CHAR_L,CHAR_I,CHAR_F);
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            app_dis_3Bits_temp(ptSysPara->record.lifTemp);
            #elif defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)
            app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,CHAR_NEGATIVE);
            #endif
            break;
        }
        case HIGH_SETTING_DIF:
        {
            app_dis_4Bits_char(CHAR_6,CHAR_D,CHAR_I,CHAR_F);
            app_dis_3Bits_temp(ptSysPara->record.diffTemp);
            break;
        }
        case HIGH_SETTING_LTP:
        {
            app_dis_4Bits_char(CHAR_7,CHAR_L,CHAR_T,CHAR_P);
            if(MIN_LOW_PROTECT_TEMP < ptSysPara->record.ltpTemp)
            {
                app_dis_3Bits_temp(ptSysPara->record.ltpTemp);
            }
            else
            {
                app_dis_3Bits_char(CHAR_O,CHAR_F,CHAR_F);
            }
            break;
        }
        case HIGH_SETTING_MAX_SET:
        {
            app_dis_4Bits_char(CHAR_8,CHAR_H,CHAR_I,CHAR_T);
            app_dis_3Bits_temp(ptSysPara->record.maxSetTemp);
            break;
        }
        case HIGH_SETTING_WEEK_PRG:
        {
            app_dis_4Bits_char(CHAR_9,CHAR_P,CHAR_R,CHAR_G);
            if(PROGRAM_OFF == ptSysPara->record.weekPrg)
            {
                app_dis_3Bits_char(CHAR_O,CHAR_F,CHAR_F);
            }
            else
            {
                app_dis_datToBcd((uint8_t)(ptSysPara->record.weekPrg + 4),&digBuf[BITS_3]);
            }            
            break;
        }
        case HIGH_SETTING_QUICK:
        {
            app_dis_4Bits_char(CHAR_A,CHAR_F,CHAR_I,CHAR_T);
            app_dis_3Bits_temp(ptSysPara->record.quickTemp);
            break;
        }
        case HIGH_SETTING_POWUP_OFF:
        {
            app_dis_4Bits_char(CHAR_B,CHAR_P,CHAR_O,CHAR_F);
            app_dis_bool(ptSysPara->record.powerOnOffFlag);
            break;
        }
        case HIGH_SETTING_DESCALE:
        {
            app_dis_4Bits_char(CHAR_C,CHAR_D,CHAR_E,CHAR_S);
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,CHAR_NEGATIVE);           
            #elif defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)
            app_dis_bool(ptSysPara->record.cleanFlag);
            #endif            
            break;
        }
        case HIGH_SETTING_DEMO:
        {
            app_dis_4Bits_char(CHAR_D,CHAR_D,CHAR_E,CHAR_O);
            app_dis_bool(ptSysPara->record.demoFlag);
            break;
        }
        case HIGH_SETTING_OW_ENABNE:
        {
            app_dis_4Bits_char(CHAR_E,CHAR_O,CHAR_P,CHAR_N);
            app_dis_bool(ptSysPara->record.owPara.enableFlag);
            break;
        }
        case HIGH_SETTING_OW_CHECK_TIME:
        {
            app_dis_4Bits_char(CHAR_F,CHAR_O,CHAR_D,CHAR_T);
            app_dis_datToBcd(ptSysPara->record.owPara.checkTime,&digBuf[BITS_3]);
            break;
        }
        case HIGH_SETTING_OW_CHECK_TEMP:
        {
            app_dis_4Bits_char(CHAR_G,CHAR_T,CHAR_D,CHAR_L);
            app_dis_3Bits_temp(ptSysPara->record.owPara.downTemp);
            break;
        }
        case HIGH_SETTING_OW_BACK_TIME:
        {
            app_dis_4Bits_char(CHAR_H,CHAR_B,CHAR_N,CHAR_T);
            app_dis_datToBcd(ptSysPara->record.owPara.backTime,&digBuf[BITS_3]);
            break;
        }
        case HIGH_SETTING_PREHEAT_ENABLE:
        {
            app_dis_4Bits_char(CHAR_I,CHAR_A,CHAR_S,CHAR_H);
            app_dis_bool(ptSysPara->record.preheat.switchflag);
            break;
        }
        case HIGH_SETTING_SENSOR_TYEP:
        {
            
            app_dis_4Bits_char(CHAR_J,CHAR_S,CHAR_E,CHAR_T);          
            #if defined(__TUYA716__) || defined(__TUYA716_5G__)
            uint8_t buf[2][3] = {CHAR_3,CHAR_3,CHAR_8,CHAR_3,CHAR_9,CHAR_5};//,CHAR_3,CHAR_9,CHAR_6};
            uint8_t cnt = (uint8_t)ptSysPara->record.senMode;           
            app_dis_3Bits_char(buf[cnt][0],buf[cnt][1],buf[cnt][2]);
            #elif defined(__TUYA7X3__) || defined(__TUYA7X3_5G__)
            app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,CHAR_NEGATIVE);
            #endif
            break;
        }
        case HIGH_SETTING_FAC:
        {
            app_dis_4Bits_char(CHAR_K,CHAR_F,CHAR_A,CHAR_C);
            if(FAC_IDLE == ptSysPara->highSet.facStatus)
            {
                if(ptSysPara->blinkEvent)
                {
                    app_dis_3Bits_char(CHAR_NEGATIVE,BLACK,BLACK);
                }              
            }
            else if(FAC_START == ptSysPara->highSet.facStatus)
            {
                app_dis_3Bits_char(CHAR_NEGATIVE,BLACK,BLACK);
            }
            else if(FAC_BUSY == ptSysPara->highSet.facStatus)
            {
                app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,BLACK);
            }
            else if(FAC_FINISH == ptSysPara->highSet.facStatus)
            {
                app_dis_3Bits_char(CHAR_NEGATIVE,CHAR_NEGATIVE,CHAR_NEGATIVE);
            }
            break;
        }
        default:break;
    }
}

void app_dis_showVersion(void)
{  
    digBuf[0] = BcdCodeTable[(uint8_t)((__TUYA__PRODUCE__ >> 8) & 0x000f)]; 
    digBuf[1] = BcdCodeTable[(uint8_t)((__TUYA__PRODUCE__ >> 4) & 0x000f)];
    digBuf[2] = BcdCodeTable[(uint8_t)(__TUYA__PRODUCE__ & 0x000f)];
//    digBuf[3] = BcdCodeTable[(uint8_t)((__SOFT_VERSION__ >> 12) & 0x000f)]; 
//    digBuf[4] = BcdCodeTable[(uint8_t)((__SOFT_VERSION__ >> 8) & 0x000f)];
//    digBuf[5] = BcdCodeTable[(uint8_t)((__SOFT_VERSION__ >> 4) & 0x000f)];
//    digBuf[6] = BcdCodeTable[(uint8_t)(__SOFT_VERSION__ & 0x000f)];
       
    uint8_t *pt = MCU_VER;  
    digBuf[3] = BcdCodeTable[pt[0] & (~0x30)];
    digBuf[4] = BcdCodeTable[pt[2] & (~0x30)];
    digBuf[5] = BcdCodeTable[pt[4] & (~0x30)];  
}

void app_dis_showAll(void)
{
    mod_lcd_showAll(BN_TRUE);
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
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    int16_t  temp = ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].temp;
    uint16_t hourMinute = ptSysPara->record.interval[ptSysPara->weekPrgMainMenu].hourMinute;
    uint8_t  disWeek = 0;
    
    if(ptSysPara->weekPrgMainMenu < WEEK_PRG_MENU_MAIN_INT6)
    {
        disWeek = (uint8_t)ptSysPara->record.weekPrg + 6;
    }
    else
    {
        disWeek = (uint8_t)ptSysPara->record.weekPrg + 9;
    }
    app_dis_runWeek(disWeek);
    app_dis_3Bits_temp(temp);
    app_dis_runRtc(false,hourMinute,true);
    app_dis_interval(ptSysPara->weekPrgMainMenu);
    if((ptSysPara->blinkEvent) && (!ptSysPara->disKeepFlag)) 
    {
        if(WEEK_PRG_MENU_SON_TIME == ptSysPara->weekPrgSonMenu)
        {
             app_dis_4Bits_char(BLACK,BLACK,BLACK,BLACK);
             lcdBuf[DIS_COL_ADD] &= ~DIS_COL_DAT;
             
        }
        else if(WEEK_PRG_MENU_SON_TEMP == ptSysPara->weekPrgSonMenu)
        {
             app_dis_3Bits_char(BLACK,BLACK,BLACK);
             lcdBuf[DIS_P_ADD] &= ~DIS_P_DAT;
             lcdBuf[DIS_MEAS_TEMP_MARK_ADD] &= ~DIS_MEAS_TEMP_MARK_DAT;
        }
    }       
}

void app_dis_rtcSetMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    app_dis_runRtc(false,ptSysPara->rtc.hourMinute,true);
    app_dis_runWeek(ptSysPara->rtc.week);
    if((ptSysPara->blinkEvent) && (!ptSysPara->disKeepFlag)) 
    {
        switch(ptSysPara->rtcSetMenu)
        {
            case SET_MENU_MINUTE:
            {   
                digBuf[5] = BcdCodeTable[TABLE_BLACK];
                digBuf[6] = BcdCodeTable[TABLE_BLACK];
                break;
            }
            case SET_MENU_HOUR:
            {
                digBuf[3] = BcdCodeTable[TABLE_BLACK];
                digBuf[4] = BcdCodeTable[TABLE_BLACK];
                break;
            }
            case SET_MENU_WEEK:
            {
                app_dis_runWeek(0xff); 
                break;
            }
            default:break;
        }
    }   
}

static void aiAdjSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    int16_t *pt = &para->record.aiAdjSetPara.t_max;
    uint8_t  menu = (uint8_t)para->aiAdjSetMenu; 
    pt += menu;   
    app_dis_3Bits_temp(*pt);
    uint8_t buf[2][4] = {CHAR_T,CHAR_H,CHAR_0,CHAR_S,CHAR_R,CHAR_E,CHAR_L,CHAR_Y};
    app_dis_4Bits_char(buf[menu][0],buf[menu][1],buf[menu][2],buf[menu][3]);
}

void app_display_updata(systemRunStatus_t sysRunStatus)
{
    switch(sysRunStatus)
    {               
       case SYS_STATUS_POWER_OFF:            app_dis_powOffMode();      break;
       case SYS_STATUS_RUN_MODE:             app_dis_runMode();         break;
       case SYS_STATUS_HIGH_SETTING:         app_dis_highSetMode();     break;     
       case SYS_STATUS_SHOW_VERSIONS:        app_dis_showVersion();     break;   
       case SYS_STATUS_SHOW_ALL:             app_dis_showAll();         break;  
       case SYS_STATUS_SMARTLINK:            app_dis_smartlinkMode();   break;
       case SYS_STATUS_PROGRAM:              app_dis_programMode();     break;
       case SYS_STATUS_RTCSET:               app_dis_rtcSetMode();      break;
       case SYS_STATUS_AISET:                aiAdjSetMode();            break;   
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
    mod_lcd_showAll(BN_FALSE);
}

void app_display_scanTask(void)
{         
    SysPara_t *ptSysPara;  
    ptSysPara =  controler_getSysParaPt();
    static bool initFlag = false;
    if(!initFlag)
    {
        mod_lcd_configure(lcdBuf);
    }
    if(ptSysPara->dispUpdataEvent)
    {
        ptSysPara->dispUpdataEvent = false;
        app_dis_clr();
        app_display_updata(ptSysPara->record.sysRunStatus); 
        mod_lcd_dig(digBuf,lcdBuf);            
        mod_lcd_refresh();
    }
}
//------------------------------E N D-------------------------------------------



//-------------------------APP_Display.c--END-----------------------------------




