#include ".\APP_Cfg.h"


const uint8_t bcdCodeTable[17] = 
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
#define DIG_7_ADD   6
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
    _out_buf[0] = bcdCodeTable[temp / 100];
    _out_buf[1] = bcdCodeTable[temp % 100 / 10];
    _out_buf[2] = bcdCodeTable[temp % 10];
    if(negativeFlag)
    {
        _out_buf[0] = bcdCodeTable[TableNegative];        
    }
    else
    {
        if(bcdCodeTable[0] == _out_buf[0])
        {
            _out_buf[0] = bcdCodeTable[TABLE_BLACK];
        }  
    }         
}

void app_dis_datToBcd(uint8_t dat,uint8_t *_out_buf)
{
    _out_buf[0] = bcdCodeTable[dat / 100];
    _out_buf[1] = bcdCodeTable[dat % 100 / 10];
    _out_buf[2] = bcdCodeTable[dat % 10];  
    if(_out_buf[0]  == bcdCodeTable[0])
    {
        _out_buf[0] = bcdCodeTable[TABLE_BLACK];           
        if(_out_buf[1] == bcdCodeTable[0])
        {
            _out_buf[1] = bcdCodeTable[TABLE_BLACK];
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

void char_3bit(uint8_t *ptdat)
{
    uint8_t i = 0;
    for(;i < 3; i++)
    {
        digBuf[i + DIG_1_ADD] = ptdat[i];
    }   
}

void char_4bit(uint8_t *ptdat)
{
    uint8_t i = 0;
    for(;i < 4; i++)
    {
        digBuf[i + DIG_4_ADD] = ptdat[i];
    }   
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

static void tempTop(int16_t temp)
{
    app_dis_tempToBcd(temp,&digBuf[DIG_1_ADD]);
    mod_lcd_char(DIS_DP1,BN_TRUE);
    mod_lcd_char(DIS_MEAS_C,BN_TRUE);
  
}

void tempBot(int16_t temp)
{
    app_dis_tempToBcd(temp,&digBuf[DIG_4_ADD]);
    mod_lcd_char(DIS_DP2,BN_TRUE);
    mod_lcd_char(DIS_SET_C,BN_TRUE);
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
            tempTop(ptSys->runMeasTemp);
            mod_lcd_char(DIS_MEAS,BN_TRUE);           
        }
        
        if(errNum)
        {
            if(ptSys->blinkEvent)
            {
                digBuf[DIG_1_ADD] = CHAR_E;
                digBuf[DIG_2_ADD] = CHAR_R;
                digBuf[DIG_3_ADD] = bcdCodeTable[errNum - 1];                 
            }                
        }     
    }   
}

void app_dis_setTemp(int16_t setTemp,bool disFlag)
{
    if(disFlag)
    {
        tempBot(setTemp);
        mod_lcd_char(DIS_SET,BN_TRUE);
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


void app_dis_tempControl(tempControlMode_t mode,bool blink)
{
    if(TEMP_CONTROL_MODE_MANUAL != mode)
    {
        if(blink)
        {
            mod_lcd_char(DIS_AI,BN_TRUE);
        }        
    }
}




static void dis_col(bool disFlag)
{
    if(disFlag)
    {
        mod_lcd_char(DIS_COL,BN_TRUE); 
    }
}

static void dis_week(uint8_t week,bool disFlag)
{
    if(disFlag)
    {
        mod_lcd_char((rt1_lcd_char_number_t)(week + 1),BN_TRUE);   
    }  
}

static void dis_hour(uint8_t hour,bool disFlag)
{
    if(disFlag)
    {
        digBuf[DIG_4_ADD] = bcdCodeTable[hour / 10];
        digBuf[DIG_5_ADD] = bcdCodeTable[hour % 10];
    }  
}

static void dis_minute(uint8_t minute,bool disFlag)
{
    if(disFlag)
    {
        digBuf[DIG_6_ADD] = bcdCodeTable[minute / 10];
        digBuf[DIG_7_ADD] = bcdCodeTable[minute % 10];
    }      
}

static void dis_interval(uint8_t interval)
{
    uint8_t dis_inter = 0;
    if(interval < REST_INT0) 
    {
        dis_inter = interval;
    }
    else
    {
        if(REST_INT0 == interval)
        {
            dis_inter = 0;
        }
        else
        {
            dis_inter = 3;
        }
    }
    mod_lcd_char((rt1_lcd_char_number_t)(dis_inter + DIS_INTERVAL_1),BN_TRUE); 
}


static void room_output(bool outFlag,bool blinkFlag)
{
    if(outFlag && blinkFlag)
    {      
        mod_lcd_char(DIS_FLOOR_OUT,BN_FALSE);
    }       
}
static void room_openWindowStatus(bool blink)
{
    if(app_openWinow_run() && blink)
    {
        mod_lcd_char(DIS_WINDOW,BN_TRUE);
    }
}

static void room_preheatStatus(bool blink)
{
    if(app_preheat_getStatus() && blink)
    {
        mod_lcd_char(DIS_PREHEAT,BN_TRUE);
    }
}

static void room(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    mod_lcd_char(DIS_HOUSE,BN_TRUE);
    mod_lcd_char(DIS_FLOOR_WARM,BN_TRUE);
    mod_lcd_char(DIS_FLOOR_OUT,BN_TRUE);
    room_output(para->floorRelayFlag,para->blinkEvent);
    room_openWindowStatus(para->blinkEvent);
    room_preheatStatus(para->blinkEvent);
    mod_lcd_char(DIS_HIGH_SET,BN_TRUE);
    mod_lcd_char(DIS_LOCK,BN_TRUE);
    mod_lcd_char(DIS_ADD_DEC,BN_TRUE);
    mod_lcd_char(DIS_5S,BN_TRUE);
    mod_lcd_char(DIS_LOCK,BN_TRUE);
    if(KEY_STATUS_BLINK == para->record.lockStatus)
    {
        if(para->blinkEvent)
        {
            mod_lcd_char(DIS_LOCK,BN_FALSE);
        }
    }
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
        mod_lcd_char(DIS_LTP,BN_TRUE);
        if(ptSys->floorRelayFlag && ptSys->blinkEvent)
        {
            mod_lcd_char(DIS_LTP,BN_FALSE);
        }
    }   
}

static void powOffMode(void)
{   
    app_dis_measTemp();    
    ltp_handle();
    #ifdef __WIFI_CONTROL__
    SysPara_t *para;  
    para = controler_getSysParaPt();
    app_dis_wifi_run(app_pullTuyaWifiStateFormThisModule(),para->blinkEvent);
    #endif          
    digBuf[DIG_5_ADD] = CHAR_O;
    digBuf[DIG_6_ADD] = CHAR_F;
    digBuf[DIG_7_ADD] = CHAR_F;       
}

static void runMode_rtc(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    realTime_t *realTime;
    realTime = app_real_time_getTime();
    bool disTempFlag = true;
    dis_week(realTime->week,true);
    
    if(TEMP_CONTROL_MODE_MANUAL != para->record.tempControlMode)
    {
        dis_interval(app_inter_runInter());
    }  
    
    if(para->blink_RTC_TEMP_flag)
    {
        dis_hour(realTime->hour,true);
        dis_minute(realTime->minute,true);
        if(para->blinkEvent)
        {
            dis_col(true);   
        }
        disTempFlag = false;
    }
    
    if(TEMP_CONTROL_MODE_MANUAL != para->record.tempControlMode)
    {
        mod_lcd_char(DIS_AI,BN_TRUE);
        if(TEMP_CONTROL_MODE_TEMPORARY == para->record.tempControlMode)
        {
            mod_lcd_char(DIS_MAN,BN_TRUE);
        }
    }
    else
    {
        mod_lcd_char(DIS_MAN,BN_TRUE);
        if(para->fast_set_blink_flag)
        {
            if(para->blinkEvent)
            {
                mod_lcd_char(DIS_MAN,BN_FALSE);
            }           
        }
    }    
    
    if(disTempFlag)
    {
        app_dis_setTemp(para->runSetTemp,true);   
    }
}

static void runMode(void)
{    
    app_dis_measTemp();   
    runMode_rtc();
    room();   
    #ifdef __WIFI_CONTROL__
    SysPara_t *para;  
    para = controler_getSysParaPt();
    app_dis_wifi_run(app_pullTuyaWifiStateFormThisModule(),para->blinkEvent);  
    mod_lcd_char(DIS_BLUE_TOOTH,BN_TRUE);
    #endif
}

static void highSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();   
    uint8_t menu = 0;
    menu = (uint8_t)para->highSet.menu;
    uint8_t ten ,unit;
    if(menu < 98)
    {
        ten = (menu + 1) / 10;
        unit = (menu + 1) % 10;
        if(ten)
        {
            digBuf[DIG_4_ADD] = bcdCodeTable[ten];
        }
        digBuf[DIG_5_ADD] = bcdCodeTable[unit];
    }
    
    mod_lcd_char(DIS_HIGH_SET,BN_TRUE);
    bool disTempFlag = false;
    bool measFlag = false;
    int16_t *ptTemp;
    
    uint8_t menu_buf[19][4] = {CHAR_1,CHAR_A,CHAR_D,CHAR_J,\
      CHAR_2,CHAR_S,CHAR_E,CHAR_N,CHAR_3,CHAR_H,CHAR_I,CHAR_T,
      CHAR_4,CHAR_L,CHAR_I,CHAR_T,CHAR_5,CHAR_D,CHAR_I,CHAR_F,
      CHAR_6,CHAR_P,CHAR_o,CHAR_F,CHAR_7,CHAR_L,CHAR_I,CHAR_F,                       
      CHAR_8,CHAR_L,CHAR_T,CHAR_P,CHAR_9,CHAR_P,CHAR_R,CHAR_G,                        
      CHAR_A,CHAR_R,CHAR_L,CHAR_E,CHAR_B,CHAR_D,CHAR_L,CHAR_Y,                           
      CHAR_C,CHAR_o,CHAR_I,CHAR_D,CHAR_D,CHAR_o,CHAR_D,CHAR_T,
      CHAR_E,CHAR_o,CHAR_T,CHAR_F,CHAR_F,CHAR_o,CHAR_R,CHAR_T,
      CHAR_G,CHAR_A,CHAR_S,CHAR_H,CHAR_H,CHAR_S,CHAR_E,CHAR_T, 
      CHAR_I,CHAR_F,CHAR_A,CHAR_S,CHAR_J,CHAR_F,CHAR_A,CHAR_C,
    };
    char_4bit(&menu_buf[menu][0]);    
    switch(para->highSet.menu)
    {
        case HIGH_SETTING_NTC_ADJ:
        {                          
            if(para->highSet.disAdjTempFlag)
            {
                disTempFlag = true;       
                ptTemp = &para->record.inAdjTemp;
                if(SENSOR_OUT == para->record.sensorMode)
                {
                    ptTemp++;                   
                }                                                         
            }
            else
            {
                uint8_t err = 0;
                if(SENSOR_OUT == para->record.sensorMode)
                {
                    if(OUTSIDE_ERROR & para->systemError)
                    {                       
                        err = 2;
                    }
                }
                else
                {
                    if(INSIDE_ERROR & para->systemError)
                    {                       
                        err = 1;
                    }                 
                }
                
                if(err)
                {
                    digBuf[DIG_1_ADD] = CHAR_E;
                    digBuf[DIG_2_ADD] = CHAR_R;
                    digBuf[DIG_3_ADD] = bcdCodeTable[err - 1];
                }
                else
                {
                    ptTemp = &para->runMeasTemp;
                    disTempFlag = true; 
                    measFlag = true;                 
                }
            }
            break;
        }
        case HIGH_SETTING_NTC_MODE_SECELT:
        {
            uint8_t ntc_buf[3][3] = {CHAR_I,CHAR_N,BLACK,CHAR_O,CHAR_U,CHAR_T,CHAR_A,CHAR_L,CHAR_L};
            char_3bit(&ntc_buf[(uint8_t)para->record.sensorMode][0]);    
            break;
        }
        case HIGH_SETTING_MAX_SET_TEMP:
        {                       
            disTempFlag = true;
            ptTemp = &para->record.maxSetTemp;
            break;
        }
        case HIGH_SETTING_MIN_SET_TEMP:
        {                       
            disTempFlag = true;
            ptTemp = &para->record.minSetTemp;
            break;
        }
        case HIGH_SETTING_DIF:
        {                        
            disTempFlag = true;
            ptTemp = &para->record.diffTemp;
            break;
        }
        case HIGH_SETTING_POF:
        {   
            uint8_t pof_buf[2][3] = {CHAR_O,CHAR_F,CHAR_F,CHAR_O,CHAR_N,BLACK};
            char_3bit(&pof_buf[(uint8_t)para->record.powerOnOffFlag][0]);               
            break;
        }        
        case HIGH_SETTING_LIF:
        {           
            disTempFlag = true;
            ptTemp = &para->record.lifSetTemp;
            break;
        }
        case HIGH_SETTING_LTP:
        {
            ptTemp = &para->record.ltpSetTemp;
            if(MIN_LTP_TEMP < *ptTemp)
            {
                disTempFlag = true;
            }
            else
            {
                uint8_t ltp_buf[3] = {CHAR_O,CHAR_F,CHAR_F};
                char_3bit(&ltp_buf[0]);    
            }
            break;
        }
        case HIGH_SETTING_PRG:
        {           
            uint8_t i = 0;
            uint8_t prg = (uint8_t)para->record.weekPrg + 5;
            for(;i < prg; i++)
            {
              if(i < 6)
              {
                  mod_lcd_char((rt1_lcd_char_number_t)(2 + i),BN_TRUE);
              }
              else
              {
                  mod_lcd_char((rt1_lcd_char_number_t)(1),BN_TRUE);
              }
            }
            digBuf[DIG_2_ADD] = bcdCodeTable[prg];           
            break;
        }      
        case HIGH_SETTING_RELAY_OUT_TYPE:
        {           
            digBuf[DIG_1_ADD] = CHAR_0;
            digBuf[DIG_2_ADD] = bcdCodeTable[(uint8_t)para->record.relayOutType % 10];
            break;
        }
        case HIGH_SETTING_LINK_DELAY_TIME:
        {
            digBuf[DIG_2_ADD] = bcdCodeTable[(uint8_t)para->record.linkRelayDelayTime % 10];
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_SWITCH:
        {
            uint8_t pof_buf[2][3] = {CHAR_O,CHAR_F,CHAR_F,CHAR_O,CHAR_N,BLACK};
            char_3bit(&pof_buf[(uint8_t)para->record.window.switch_flag][0]);   
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_TIME_CYC:
        {
            digBuf[DIG_1_ADD] = bcdCodeTable[para->record.window.check_time / 10];
            digBuf[DIG_2_ADD] = bcdCodeTable[para->record.window.check_time % 10];
            if(digBuf[DIG_1_ADD] == 0)
            {
                digBuf[DIG_1_ADD] = BLACK;
            }
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_DOWN_TEMP:
        {
            tempTop(para->record.window.down_temp);           
            break;
        }
        case HIGH_SETTING_WINDOW_CHECK_BACK_TIME:
        {
            digBuf[DIG_1_ADD] = bcdCodeTable[para->record.window.back_time / 10];
            digBuf[DIG_2_ADD] = bcdCodeTable[para->record.window.back_time % 10];            
            break;
        }
        case HIGH_SETTING_WINDOW_PREHEAT_SWITCH:
        {
            uint8_t pof_buf[2][3] = {CHAR_O,CHAR_F,CHAR_F,CHAR_O,CHAR_N,BLACK};
            char_3bit(&pof_buf[(uint8_t)para->record.preheat.switchflag][0]);      
            break;
        }
        case HIGH_SETTING_NTC_TYPE_SECELT:
        {
            uint8_t buf[2][3] = {CHAR_3,CHAR_3,CHAR_8,CHAR_3,CHAR_9,CHAR_5};
            char_3bit(&buf[(uint8_t)para->record.sensorType][0]);      
            break;
        }
        case HIGH_SETTING_SAVE_SET_TEMP:
        {
            disTempFlag = true;
            ptTemp = &para->record.saveSetTemp;
            break;
        }
        case HIGH_SETTING_FAC:
        {                      
            if(FAC_IDLE == para->highSet.facStatus)
            {
                if(para->blinkEvent)
                {
                    digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
                }              
            }
            else if(FAC_START == para->highSet.facStatus)
            {
                digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
            }
            else if(FAC_BUSY == para->highSet.facStatus)
            {
                digBuf[DIG_1_ADD] = CHAR_NEGATIVE;
                digBuf[DIG_2_ADD] = CHAR_NEGATIVE;
            }
            else if(FAC_FINISH == para->highSet.facStatus)
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
        tempTop(*ptTemp);
        if(measFlag)
        {          
            mod_lcd_char(DIS_MEAS,BN_TRUE);
        }
    }    
    if(para->blinkEvent && para->highSet.blinkFlag)
    {
        mod_lcd_char(DIS_ADD_DEC,BN_TRUE);
    }
}

static void versionMode(void)
{  
    #ifdef __RT5_13__
    digBuf[0] = CHAR_1;
    digBuf[1] = CHAR_3;
    digBuf[2] = BLACK;
    #elif defined __RT5_36t__
    digBuf[0] = CHAR_3;
    digBuf[1] = CHAR_6;
    digBuf[2] = CHAR_T;
    #elif defined __RT5_23i__
    digBuf[0] = CHAR_2;
    digBuf[1] = CHAR_3;
    digBuf[2] = CHAR_I;
    #elif defined __RT5_36i__
    digBuf[0] = CHAR_3;
    digBuf[1] = CHAR_6;
    digBuf[2] = CHAR_I;
    #endif 
    
    uint8_t *pt = MCU_VER;
    digBuf[3] = bcdCodeTable[pt[0] & (~0x30)];
    digBuf[4] = bcdCodeTable[pt[2] & (~0x30)];
    digBuf[5] = bcdCodeTable[pt[4] & (~0x30)];
}

static void allMode(void)
{
    SysPara_t *ptSysPara;  
    ptSysPara = controler_getSysParaPt();
    mod_lcd_show_all(BN_TRUE);   
    if(ptSysPara->record.autoGotoFac)
    {
//        mod_lcd_char(DIS_HIGH_SET,BN_FALSE);
//        mod_lcd_char(DIS_TEMP_ADJ,BN_FALSE);
//        mod_lcd_char(DIS_MAX_SET_TEMP,BN_FALSE);
//        mod_lcd_char(DIS_MIN_SET_TEMP,BN_FALSE);
//        mod_lcd_char(DIS_DIF,BN_FALSE);
//        mod_lcd_char(DIS_POW_ON_STATUS,BN_FALSE);
//        mod_lcd_char(DIS_POW_OFF,BN_FALSE);
//        mod_lcd_char(DIS_SENSOR,BN_FALSE);
//        mod_lcd_char(DIS_INSIDE_SONSOR,BN_FALSE);
//        mod_lcd_char(DIS_OUTSIDE_SONSOR,BN_FALSE);
//        mod_lcd_char(DIS_LIF,BN_FALSE);
//        mod_lcd_char(DIS_MINUTE,BN_FALSE);
//        mod_lcd_char(DIS_OUTPUT_WORK,BN_FALSE);
//        mod_lcd_char(DIS_OUTPUT_REST,BN_FALSE);
//        mod_lcd_char(DIS_FAC_SET,BN_FALSE);
//        mod_lcd_char(DIS_HOURS_LATER,BN_FALSE);
//        mod_lcd_char(DIS_POW_ON,BN_FALSE);
//        mod_lcd_char(DIS_TEMP_CN,BN_FALSE);
//        mod_lcd_char(DIS_OUTPUT_TYPE,BN_FALSE);
//        mod_lcd_char(DIS_ERR,BN_FALSE);
//        mod_lcd_char(DIS_LTP_CN,BN_FALSE);
//        mod_lcd_char(DIS_SET_TEMP_CN,BN_FALSE);
//        #ifndef __WIFI_CONTROL__
//        mod_lcd_char(DIS_MODE_AI,BN_FALSE);
//        mod_lcd_char(DIS_BLUE_TOOTH,BN_FALSE);
//        #endif
    }        
}

static void smartlinkMode(void)
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

static void aiAdjSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    int16_t *pt = &para->record.aiAdjSetPara.t_max;
    uint8_t  menu = (uint8_t)para->aiAdjSetMenu; 
    pt += menu;   
    tempTop(*pt);
    uint8_t buf[2][4] = {CHAR_T,CHAR_H,CHAR_0,CHAR_S,CHAR_R,CHAR_E,CHAR_L,CHAR_Y};
    char_4bit(&buf[menu][0]);    
}

#define BLNK_MINUTE  0X01
#define BLNK_HOUR    0X02
#define BLNK_WEEK    0X04
static void rtcSetMode(void)
{
    SysPara_t *para;  
    para = controler_getSysParaPt();
    realTime_t *realTime;
    realTime = app_real_time_getTime(); 
    dis_col(true);    
    if((para->blinkEvent && para->rtcSet.enbaleBlinkFlag) ||\
      (!para->rtcSet.enbaleBlinkFlag || (para->rtcSet.menu != SET_MENU_MINUTE)))
    {
        dis_minute(realTime->minute,true);
    }
    
    if((para->blinkEvent && para->rtcSet.enbaleBlinkFlag) ||\
      (!para->rtcSet.enbaleBlinkFlag || (para->rtcSet.menu != SET_MENU_HOUR)))
    {
        dis_hour(realTime->hour,true);
    }
     if((para->blinkEvent && para->rtcSet.enbaleBlinkFlag) ||\
       (!para->rtcSet.enbaleBlinkFlag || (para->rtcSet.menu != SET_MENU_WEEK)))
    {
        dis_week(realTime->week,true);
    }   
}

static void prgSetMode(void)
{
   SysPara_t *para;  
   para = controler_getSysParaPt();
   uint8_t menu = para->prgSet.menu;
   interval_t *pt_inter = &para->record.interval[0];
   pt_inter += (menu / 2);  
   dis_interval(menu / 2);
    
   if((para->blinkEvent && para->prgSet.enbaleBlinkFlag) ||  (!para->prgSet.enbaleBlinkFlag || (menu % 2)))
   {      
      dis_col(true);   
      dis_minute(pt_inter->minute,true);
      dis_hour(pt_inter->hour,true); 
   }
    
   if((para->blinkEvent && para->prgSet.enbaleBlinkFlag) ||  (!para->prgSet.enbaleBlinkFlag || (0 == menu % 2)))
   {
      tempTop(pt_inter->temp);
   }
     
   if(menu < WEEK_PRG_MENU_REST_INT0_HM)
   {
       uint8_t i = 0;
       for(i = WEEK_1;i < WEEK_6; i++)
       {
          dis_week(i,true);
       }   
       if(PROGRAM_5 < para->record.weekPrg)
       {
          dis_week(WEEK_6,true);
          if(PROGRAM_7 == para->record.weekPrg)
          {
              dis_week(WEEK_7,true);
          }          
       }     
   } 
   else
   {      
      dis_week(WEEK_7,true);
      if(PROGRAM_5 == para->record.weekPrg)
      {          
          dis_week(WEEK_6,true);
      } 
   }
}

static void updata(systemRunStatus_t sysRunStatus)
{
    switch(sysRunStatus)
    {               
       case SYS_STATUS_POWER_OFF     :        powOffMode();       break;
       case SYS_STATUS_RUN_MODE      :        runMode();          break;
       case SYS_STATUS_HIGH_SETTING  :        highSetMode();      break;     
       case SYS_STATUS_SHOW_VERSIONS :        versionMode();      break;   
       case SYS_STATUS_SHOW_ALL      :        allMode();          break;  
       case SYS_STATUS_SMARTLINK     :        smartlinkMode();    break;
       case SYS_STATUS_RTC_SET       :        rtcSetMode();       break;
       case SYS_STATUS_PRG_SET       :        prgSetMode();       break;
       case SYS_STATUS_AI_ADJ_SET    :        aiAdjSetMode();     break;
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
    static bool initFlag = true;
    if(initFlag)
    {
        mod_lcd_configure();
        initFlag = false;
    }
    if(ptSysPara->dispUpdataEvent)
    {
        ptSysPara->dispUpdataEvent = false;
        #ifndef __LCD_STATUS_DEBUG__
        app_dis_clr();
        updata(ptSysPara->record.sysRunStatus); 
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




