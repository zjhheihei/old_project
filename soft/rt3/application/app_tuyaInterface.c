//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\APP_Cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    const INT8U   mcd_reg_addr;    
          INT16U  mcd_reg_details;
          INT8U   mcd_reg_stateBits;
}menred_controlData_def;
//--------------------------------------------------------------------------------------------------
#define mcdRegStateBits_download_ok          0x01
#define mcdRegStateBits_update_ready         0x02
//--------------------------------------------------------------------------------------------------
#define max_mendred_control_reg    23
//--------------------------------------------------------------------------------------------------
static menred_controlData_def menred_controlData[max_mendred_control_reg] =
{
    {.mcd_reg_addr = REG_CLOUD_ADDR_POWER_SWITCH   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//00
    {.mcd_reg_addr = REG_CLOUD_ADDR_FAC_SET        ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//01
    {.mcd_reg_addr = REG_CLOUD_ADDR_POF_SET        ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//02
    {.mcd_reg_addr = REG_CLOUD_ADDR_FCO_SET        ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//03   
    
    {.mcd_reg_addr = REG_CLOUD_ADDR_RUN_SET_TEMP   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//04
    {.mcd_reg_addr = REG_CLOUD_ADDR_MAX_SET_TEMP   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//05    
    {.mcd_reg_addr = REG_CLOUD_ADDR_RUN_MEAS_TEMP  ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//06
    {.mcd_reg_addr = REG_CLOUD_ADDR_MIN_SET_TEMP   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//07
    {.mcd_reg_addr = REG_CLOUD_ADDR_DIF_SET_TEMP   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//08
    {.mcd_reg_addr = REG_CLOUD_ADDR_SUN_SET_TEMP   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//09
    {.mcd_reg_addr = REG_CLOUD_ADDR_MOON_SET_TEMP  ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//10    
    {.mcd_reg_addr = REG_CLOUD_ADDR_LIF_SET_TEMP   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//11
   
    
    {.mcd_reg_addr = REG_CLOUD_ADDR_WORK_MODE      ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//12
    {.mcd_reg_addr = REG_CLOUD_ADDR_WORK_STATUS    ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//13
    {.mcd_reg_addr = REG_CLOUD_ADDR_WEEK_PRG_SET   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//14
    {.mcd_reg_addr = REG_CLOUD_ADDR_VALVE_STATUS   ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//15
    {.mcd_reg_addr = REG_CLOUD_ADDR_SERSON_SECELT  ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//16
    {.mcd_reg_addr = REG_CLOUD_ADDR_SET_WIND_SPEED ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//17    

    
    {.mcd_reg_addr = REG_CLOUD_ADDR_AI_PRO_WARM    ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//18
    {.mcd_reg_addr = REG_CLOUD_ADDR_AI_PRO_COLD    ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//19
    {.mcd_reg_addr = REG_CLOUD_ADDR_AI_PRO_OEM     ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//20
    {.mcd_reg_addr = REG_CLOUD_ADDR_OTHER_SET      ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//21
    
    {.mcd_reg_addr = REG_CLOUD_ADDR_FAULT          ,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},//22  
};

static DelayTickClk_Def   timer_amendBuadrate   = {Type_Second,0  ,0};
static DelayTickClk_Def   timer_pullWifiStatus  = {Type_Second,0  ,0};
static DelayTickClk_Def   timer_pullRtcTime     = {Type_Second,0  ,0};
static DelayTickClk_Def   timer_resetWifiModule = {Type_Second,120,0};
//-------------------------------------------------------------------------------------------------
static tuya_wifiState_def tuya_wifiState        = tuya_wifiState_unknow;
static net_rtcTime_Def    net_rtcTime;
static INT8U wifiModule_online = BN_TRUE;


#define RESERVE_RAW_ADDR      15 
#define WEEK_RAW_ADDR         16


#define REG_AI_PRG_WARM_ADDR   18     
#define REG_AI_PRG_COLD_ADDR   19     
#define REG_AI_PRG_OEM_ADDR    20     
#define REG_OHTER_SET_ADDR     21 

static INT8U raw_warm_buf[33];
static INT8U raw_cold_buf[33];
static INT8U raw_oem_buf[24];
static INT8U raw_ohter_buf[18];
void raw_dp_updat_to_cloud(uint8_t regAddr,uint8_t length,uint8_t *dat)
{
    uint8_t i = 0;   
    switch(regAddr)
    {
        case REG_CLOUD_ADDR_AI_PRO_WARM:
        {
            for(i = 0; i < length; i++)
            {
                raw_warm_buf[i] = dat[i];       
            } 
            menred_controlData[REG_AI_PRG_WARM_ADDR].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
            menred_controlData[REG_AI_PRG_WARM_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_download_ok);
            break;
        }
        case REG_CLOUD_ADDR_AI_PRO_COLD:
        {
            for(i = 0; i < length; i++)
            {
                raw_cold_buf[i] = dat[i];       
            } 
            menred_controlData[REG_AI_PRG_COLD_ADDR].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
            menred_controlData[REG_AI_PRG_COLD_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_download_ok);           
            break;
        }
        case REG_CLOUD_ADDR_AI_PRO_OEM:
        {
            for(i = 0; i < length; i++)
            {
                raw_oem_buf[i] = dat[i];       
            } 
            menred_controlData[REG_AI_PRG_OEM_ADDR].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
            menred_controlData[REG_AI_PRG_OEM_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_download_ok);           
            break;
        }
        case REG_CLOUD_ADDR_OTHER_SET:
        {
            for(i = 0; i < length; i++)
            {
                raw_ohter_buf[i] = dat[i];       
            } 
            menred_controlData[REG_OHTER_SET_ADDR].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
            menred_controlData[REG_OHTER_SET_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_download_ok);    
            break;
        }
        default:break;
    }    
}

void raw_dp_download_from_cloud(uint8_t regAddr,uint8_t length,uint8_t *dat)
{
    uint8_t i = 0;
    switch(regAddr)
    {
        case REG_CLOUD_ADDR_AI_PRO_WARM:
        {
            for(; i < length; i++)
            {
                raw_warm_buf[i] = dat[i];       
            }
            menred_controlData[REG_AI_PRG_WARM_ADDR].mcd_reg_stateBits |= mcdRegStateBits_download_ok;
            menred_controlData[REG_AI_PRG_WARM_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);            
            break;
        }
        case REG_CLOUD_ADDR_AI_PRO_COLD:
        {
            for(; i < length; i++)
            {
                raw_cold_buf[i] = dat[i];       
            }
            menred_controlData[REG_AI_PRG_COLD_ADDR].mcd_reg_stateBits |= mcdRegStateBits_download_ok;
            menred_controlData[REG_AI_PRG_COLD_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready); 
            break;
        }
        case REG_CLOUD_ADDR_AI_PRO_OEM:
        {
            for(; i < length; i++)
            {
                raw_oem_buf[i] = dat[i];       
            }
            menred_controlData[REG_AI_PRG_OEM_ADDR].mcd_reg_stateBits |= mcdRegStateBits_download_ok;
            menred_controlData[REG_AI_PRG_OEM_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready); 
            break;
        }
        case REG_CLOUD_ADDR_OTHER_SET:
        {
            for(; i < length; i++)
            {
                raw_ohter_buf[i] = dat[i];       
            }
            menred_controlData[REG_OHTER_SET_ADDR].mcd_reg_stateBits |= mcdRegStateBits_download_ok;
            menred_controlData[REG_OHTER_SET_ADDR].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready); 
            break;
        }
        default:break;
    }         
}

uint8_t raw_dp_updat_to_deceive(uint8_t regAddr,uint8_t *_out_dat)
{
    uint8_t i = 0;
    uint8_t maxLength  = 0;
    switch(regAddr)
    {
        case REG_CLOUD_ADDR_AI_PRO_WARM:
        case REG_CLOUD_ADDR_AI_PRO_COLD:
        {
            uint8_t *pt = raw_warm_buf;
            if(REG_CLOUD_ADDR_AI_PRO_COLD == regAddr)
            {
                 pt = raw_cold_buf;
            }
            for(i = 0; i < (AI_PRG_LENGTH + 1); i++)
            {
               _out_dat[i] = pt[i];       
            } 
            maxLength = 33;
            break;
        }             
        case REG_CLOUD_ADDR_AI_PRO_OEM:
        {
            for(i = 0; i < 24; i++)
            {
               _out_dat[i] = raw_oem_buf[i];       
            }  
            maxLength = 24;
            break;
        }
        case REG_CLOUD_ADDR_OTHER_SET:
        {
            for(i = 0; i < 18; i++)
            {
               _out_dat[i] = raw_ohter_buf[i];       
            }   
            maxLength = 18;
            break;
        }
        default:break;
    }      
    return maxLength;
}

void controlData_update(void)
{
    INT8U i;
    unsigned char ret = FALSE;
    #ifndef NDEBUG
    volatile static unsigned char testDat = FALSE;
    volatile static unsigned char testRet = FALSE;
    #endif
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        if(mcdRegStateBits_update_ready & menred_controlData[i].mcd_reg_stateBits)
        { 
            ret = FALSE;
            switch(menred_controlData[i].mcd_reg_addr)
            {
                case REG_CLOUD_ADDR_POWER_SWITCH   :  
                case REG_CLOUD_ADDR_FAC_SET        :  
                case REG_CLOUD_ADDR_POF_SET        : 
                case REG_CLOUD_ADDR_FCO_SET        :  
                {       
                    uint8_t bool_dpid_Buf[4] = {DPID_SWITCH,DPID_FAC,DPID_POF,DPID_PCO};     
                    ret = mcu_dp_bool_update(bool_dpid_Buf[menred_controlData[i].mcd_reg_addr - REG_CLOUD_ADDR_POWER_SWITCH],menred_controlData[i].mcd_reg_details);
                    break;  
                }
              
                case REG_CLOUD_ADDR_RUN_SET_TEMP   :
                case REG_CLOUD_ADDR_MAX_SET_TEMP   :
                case REG_CLOUD_ADDR_RUN_MEAS_TEMP  :  
                case REG_CLOUD_ADDR_MIN_SET_TEMP   : 
                case REG_CLOUD_ADDR_DIF_SET_TEMP   :
                case REG_CLOUD_ADDR_SUN_SET_TEMP   :
                case REG_CLOUD_ADDR_MOON_SET_TEMP  :
                case REG_CLOUD_ADDR_LIF_SET_TEMP   :                
                {
                    uint8_t value_dpid_Buf[8] = {DPID_TEMP_SET,DPID_MAX_SET_TEMP,DPID_TEMP_CURRENT,DPID_MIN_SET_TEMP,DPID_DIF_TEMP,DPID_SUN_SET_TEMP,DPID_MOON_SET_TEMP,DPID_LIF};             
                    ret = mcu_dp_value_update(value_dpid_Buf[menred_controlData[i].mcd_reg_addr - REG_CLOUD_ADDR_RUN_SET_TEMP],(INT32S)((INT16S)menred_controlData[i].mcd_reg_details));break;     
                    break;
                }
                case REG_CLOUD_ADDR_WORK_MODE      :        
                case REG_CLOUD_ADDR_WORK_STATUS    :    
                case REG_CLOUD_ADDR_WEEK_PRG_SET   :
                case REG_CLOUD_ADDR_VALVE_STATUS   : 
                case REG_CLOUD_ADDR_SERSON_SECELT  : 
                case REG_CLOUD_ADDR_SET_WIND_SPEED :                                                       
                {
                    uint8_t enum_dpid_Buf[6] = {DPID_WORK_MODE,DPID_WORK_STATUS,DPID_WORK_DAYS,DPID_VALVE_STATE,DPID_SENSOR_SELECT,DPID_SET_WIND_SPEED};                     
                    ret = mcu_dp_enum_update(enum_dpid_Buf[menred_controlData[i].mcd_reg_addr - REG_CLOUD_ADDR_WORK_MODE],menred_controlData[i].mcd_reg_details);  
                    break;                   
                }
                case REG_CLOUD_ADDR_AI_PRO_WARM:
                {
                    ret = mcu_dp_raw_update(DPID_AI_PROGRAM_WARM,&raw_warm_buf[0],33);       
                    break;
                }
                case REG_CLOUD_ADDR_AI_PRO_COLD:
                {
                    ret = mcu_dp_raw_update(DPID_AI_PROGRAM_COLD,&raw_cold_buf[0],33);       
                    break;
                }
                case REG_CLOUD_ADDR_AI_PRO_OEM :
                {
                    ret = mcu_dp_raw_update(DPID_AI_PROGRAM_ODM,&raw_oem_buf[0],24);       
                    break;
                }
                case REG_CLOUD_ADDR_OTHER_SET  :
                {
                    ret = mcu_dp_raw_update(DPID_OTHER_SET,&raw_ohter_buf[0],18);       
//                    uint8_t raw_dpid_Buf[4] = {DPID_AI_PROGRAM_WARM,DPID_AI_PROGRAM_COLD,DPID_AI_PROGRAM_ODM,DPID_OTHER_SET};
//                    uint8_t lenth_buf[4] = {33,33,24,18};
//                    uint8_t lenth_addr[4] = {0,40,80,105};                   
//                    uint8_t dpid    = raw_dpid_Buf[menred_controlData[i].mcd_reg_addr - REG_CLOUD_ADDR_AI_PRO_WARM];
//                    uint8_t length  = lenth_buf[menred_controlData[i].mcd_reg_addr - REG_CLOUD_ADDR_AI_PRO_WARM];
//                    uint8_t bufAddr = lenth_addr[menred_controlData[i].mcd_reg_addr - REG_CLOUD_ADDR_AI_PRO_WARM];                   
//                    ret = mcu_dp_raw_update(dpid,&raw_buf[bufAddr],length);                 
                    break;
                }
                case REG_CLOUD_ADDR_FAULT:
                {
                    break;
                }
                default:break;
            }
            if(TUYA_SUCCESS == ret)
            {
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
            }
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void app_tuyaInterface_task(void)
{
    static BOOLEAN tuyaWifi_cfged = BN_FALSE;   
    if(tuyaWifi_cfged)
    {
        INT8U rd_usart_dr;
        while(mod_read_uartRxdQueue_byte(&rd_usart_dr)) 
        {
            uart_receive_input(rd_usart_dr);
        }
        wifi_uart_service();
        controlData_update();
    }
    else
    {
        tuyaWifi_cfged = BN_TRUE;
        wifi_protocol_init();
        mod_uartFull_cfg();
        ReloadDealyTickClock(&timer_amendBuadrate,15); 
        ReloadDealyTickClock(&timer_pullWifiStatus,3);
        ReloadDealyTickClock(&timer_pullRtcTime,3);
        net_rtcTime.netRTC_isValid = BN_FALSE;       
    }
    
    tuya_wifiState_def new_state;
    new_state = (tuya_wifiState_def)mcu_get_wifi_work_state();
    if(tuya_wifiState != new_state)
    {
        tuya_wifiState = new_state;
        if(tuya_wifiState_connCloud == tuya_wifiState)
        {
            wifiModule_online = BN_TRUE;
            ReloadDealyTickClock(&timer_pullRtcTime,2);
        }
    }
    
    DelayTickClockRun(&timer_amendBuadrate);
    if(DelayTickClockFinished(&timer_amendBuadrate))
    {
        app_onceAmend_baudrate();
    }
    
    DelayTickClockRun(&timer_pullWifiStatus);
    if(DelayTickClockFinished(&timer_pullWifiStatus))
    {
        if(tuya_wifiState_connCloud != tuya_wifiState)
        {
            ReloadDealyTickClock(&timer_pullWifiStatus,2);
        }
        else
        {
            ReloadDealyTickClock(&timer_pullWifiStatus,10);
        }
        mcu_get_wifi_connect_status();
    }
    
    DelayTickClockRun(&timer_pullRtcTime);
    if(DelayTickClockFinished(&timer_pullRtcTime))
    {
        ReloadDealyTickClock(&timer_pullRtcTime,40);
        mcu_get_system_time();
    }
    
    DelayTickClockRun(&timer_resetWifiModule);
    if(DelayTickClockFinished(&timer_resetWifiModule))
    {
        wifiModule_online = BN_FALSE;
        volatile INT32U reset_timer;
        ReloadDealyTickClock(&timer_resetWifiModule,120);
        mod_enable_wifi_reset();
        for(reset_timer = 0; reset_timer < 2500; reset_timer++);  //reset pulse width 27ms
        mod_disable_wifi_reset();
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_transmitToUart(INT8U in_data)
{
    BOOLEAN completed;
    do
    {
        completed = mod_write_uartTxdQueue_byte(in_data);   
    }while(BN_FALSE == completed);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_onceAmend_baudrate(void)
{
     recfg_buadrate_uart(Poll_HSIFrequency());
     ReloadDealyTickClock(&timer_amendBuadrate,300);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_pushNetRtcTimeToThisMoudle(net_rtcTime_Def in_net_rtcTime)
{
    static INT8U not_connCloud_cnt;
    ReloadDealyTickClock(&timer_resetWifiModule,120);
    net_rtcTime = in_net_rtcTime;
    wifiModule_online = BN_TRUE;
    if(tuya_wifiState_connCloud == tuya_wifiState)
    {
        not_connCloud_cnt = 0;
    }
    else
    {
        #define cnt_first_times     10
        #define cnt_second_times    24
        not_connCloud_cnt ++;
        if((not_connCloud_cnt == cnt_first_times) || (not_connCloud_cnt == (cnt_first_times*2)) || (not_connCloud_cnt == (cnt_first_times*3)))
        {
            ReloadDealyTickClock(&timer_resetWifiModule,0);
        }
        else if(not_connCloud_cnt == (cnt_first_times*3+cnt_second_times))
        {
            not_connCloud_cnt = not_connCloud_cnt-cnt_second_times;
            ReloadDealyTickClock(&timer_resetWifiModule,0);
        }
    }
}
//-------------------------------------------------------------------------------------------------
net_rtcTime_Def app_pullNetRtcTimeFromThisModule(void)
{
    return(net_rtcTime);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
tuya_wifiState_def app_pullTuyaWifiStateFormThisModule(void)
{
    return(tuya_wifiState);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_entryWifiCfg_state(tuya_configType_def in_cfgType)
{
    mcu_set_wifi_mode(in_cfgType);
    if(tuya_cfg_smart == in_cfgType)
    {
        tuya_wifiState = tuya_wifiState_smartConfig;
    }
    else
    {
        tuya_wifiState = tuya_wifiState_ap;
    }
    ReloadDealyTickClock(&timer_pullWifiStatus,2);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//云端下发一个数据
//-------------------------------------------------------------------------------------------------
void app_pushContorolDataFromCloud_download(INT8U in_reg_addr,INT16U in_reg_details)
{
    INT8U i;
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        if(in_reg_addr == menred_controlData[i].mcd_reg_addr)
        {
            menred_controlData[i].mcd_reg_details = in_reg_details;
            menred_controlData[i].mcd_reg_stateBits |= mcdRegStateBits_download_ok;
            menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);           
        }
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//设备上传一个数据
//-------------------------------------------------------------------------------------------------
void app_pushControlDataFromDevice_update(INT8U in_reg_addr,INT16U in_reg_details)
{
    INT8U i;
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        if(in_reg_addr == menred_controlData[i].mcd_reg_addr)
        {
            menred_controlData[i].mcd_reg_details = in_reg_details;
            menred_controlData[i].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
            menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_download_ok);
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//从此模块获取一个控制数据
//-------------------------------------------------------------------------------------------------
BOOLEAN app_pullControlDataFromThis(INT8U* out_reg_addr,INT16U* out_reg_details)
{
    #ifndef NDEBUG
    volatile static INT8U checkData = 0;
    #endif
    INT8U i;
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        if(menred_controlData[i].mcd_reg_stateBits & mcdRegStateBits_download_ok)
        {
            *out_reg_addr = menred_controlData[i].mcd_reg_addr;
            *out_reg_details = menred_controlData[i].mcd_reg_details;
            menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_download_ok);
            #ifndef NDEBUG
            checkData = *out_reg_addr;
            #endif
            
            return(BN_TRUE);
        }
    }
    return(BN_FALSE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static BOOLEAN allDataUpdate_once;
void app_pushAllDataUpdateOnceExecute(void)
{
    allDataUpdate_once = BN_TRUE;
}
//-------------------------------------------------------------------------------------------------
BOOLEAN app_pullAllDataUpdateOnceStatus(void)
{
    BOOLEAN rd_tmp;
    
    rd_tmp = allDataUpdate_once;
    allDataUpdate_once = BN_FALSE;
    return(rd_tmp);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN app_pullWifiModuleOnline(void)
{
    return(wifiModule_online);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++