//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\APP_Cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    const INT8U  mcd_reg_addr;    
    INT16U       mcd_reg_details;
    INT8U        mcd_reg_stateBits;
}menred_controlData_def;
//--------------------------------------------------------------------------------------------------
#define mcdRegStateBits_download_ok          0x01
#define mcdRegStateBits_update_ready         0x02
//--------------------------------------------------------------------------------------------------
#define max_mendred_control_reg    35
//--------------------------------------------------------------------------------------------------
static menred_controlData_def menred_controlData[max_mendred_control_reg] =
{
    {.mcd_reg_addr = regCloudAddr_power,                 .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_ctlMode,               .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_setTemperature,        .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_measureTemperature,    .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_heatingStatus,         .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_notFrozen,             .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_maximumSetTemp,        .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysTime_1, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysSetT_1, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysTime_2, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysSetT_2, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysTime_3, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysSetT_3, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysTime_4, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysSetT_4, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysTime_5, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysSetT_5, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysTime_6, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programWorkDaysSetT_6, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programHolidaysTime_1, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programHolidaysSetT_1, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programHolidaysTime_2, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programHolidaysSetT_2, .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_programCfg,            .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_sensorCfg,             .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_floorSensorType,       .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_floorLimitTemp,        .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_floorTemperature,      .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_childLock,             .mcd_reg_details = 0, .mcd_reg_stateBits = 0},   
    {.mcd_reg_addr = regCloudAddr_openWindowSwitch,      .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_openWindowStatus,      .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_openWindowCheckDownTemp,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_openWindowCheckDownTime,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},   
    {.mcd_reg_addr = regCloudAddr_openWindowRunBackTime,    .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
    {.mcd_reg_addr = regCloudAddr_openWindowPreheatSwitch,  .mcd_reg_details = 0, .mcd_reg_stateBits = 0},
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static DelayTickClk_Def timer_amendBuadrate = {Type_Second,0,0};
static DelayTickClk_Def timer_pullWifiStatus = {Type_Second,0,0};
static DelayTickClk_Def timer_pullRtcTime = {Type_Second,0,0};
static DelayTickClk_Def timer_resetWifiModule = {Type_Second,120,0};
//-------------------------------------------------------------------------------------------------
static tuya_wifiState_def tuya_wifiState = tuya_wifiState_unknow;
static net_rtcTime_Def net_rtcTime;
static INT8U wifiModule_online = BN_TRUE;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
INT8U make_program_frame_raw(INT8U* in_pRawbuff,INT8U in_current_index)
{
    INT8U i;
    
    in_pRawbuff[0] = 0x00;
    in_pRawbuff[1] = 0x00;
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        switch(menred_controlData[i].mcd_reg_addr)
        {
            case regCloudAddr_programWorkDaysTime_1:  //raw
            {
                in_pRawbuff[2] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[3] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysSetT_1:  
            {
                in_pRawbuff[4] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[5] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysTime_2:  
            {
                in_pRawbuff[6] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[7] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysSetT_2:
            {
                in_pRawbuff[8] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[9] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysTime_3:  
            {
                in_pRawbuff[10] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[11] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysSetT_3:  
            {
                in_pRawbuff[12] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[13] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysTime_4: 
            {
                in_pRawbuff[14] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[15] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysSetT_4:  
            {
                in_pRawbuff[16] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[17] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysTime_5:  
            {
                in_pRawbuff[18] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[19] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysSetT_5: 
            {
                in_pRawbuff[20] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[21] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysTime_6: 
            {
                in_pRawbuff[22] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[23] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programWorkDaysSetT_6: 
            {
                in_pRawbuff[24] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[25] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programHolidaysTime_1:  
            {
                in_pRawbuff[26] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[27] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programHolidaysSetT_1:  
            {
                in_pRawbuff[28] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[29] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programHolidaysTime_2:  
            {
                in_pRawbuff[30] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[31] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_programHolidaysSetT_2:  
            {
                in_pRawbuff[32] = menred_controlData[i].mcd_reg_details >> 8;
                in_pRawbuff[33] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            default:break;
        }
    }
    menred_controlData[in_current_index].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
    return(34);
}


INT8U make_102_reg_frame_raw(INT8U* _out_rawbuff,INT8U current_index)
{
  INT8U i;     
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        switch(menred_controlData[i].mcd_reg_addr)
        {
            case regCloudAddr_openWindowCheckDownTemp:  //raw
            {
                _out_rawbuff[0] = menred_controlData[i].mcd_reg_details >> 8;
                _out_rawbuff[1] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_openWindowCheckDownTime:  
            {
                _out_rawbuff[2] = menred_controlData[i].mcd_reg_details >> 8;
                _out_rawbuff[3] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_openWindowRunBackTime:  
            {
                _out_rawbuff[4] = menred_controlData[i].mcd_reg_details >> 8;
                _out_rawbuff[5] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }
            case regCloudAddr_openWindowPreheatSwitch:
            {
                _out_rawbuff[6] = menred_controlData[i].mcd_reg_details >> 8;
                _out_rawbuff[7] = menred_controlData[i].mcd_reg_details;
                menred_controlData[i].mcd_reg_stateBits &= (~mcdRegStateBits_update_ready);
                break;
            }           
            default:break;
        }
    }
    menred_controlData[current_index].mcd_reg_stateBits |= mcdRegStateBits_update_ready;
    return(8);
}
//-------------------------------------------------------------------------------------------------

void controlData_update(void)
{
    INT8U i;
    unsigned char ret = FALSE;
    for(i = 0;i < max_mendred_control_reg;i++)
    {
        if(mcdRegStateBits_update_ready & menred_controlData[i].mcd_reg_stateBits)
        { 
            ret = FALSE;
            switch(menred_controlData[i].mcd_reg_addr)
            {
                case regCloudAddr_power:
                {
                    ret = mcu_dp_bool_update(DPID_SWITCH,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_ctlMode:
                {
                    ret = mcu_dp_enum_update(DPID_MODE,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_setTemperature:
                {
                    ret = mcu_dp_value_update(DPID_TEMP_SET,(INT32S)((INT16S)menred_controlData[i].mcd_reg_details));
                    break;
                }
                case regCloudAddr_measureTemperature:
                {
                    ret = mcu_dp_value_update(DPID_TEMP_CURRENT,(INT32S)((INT16S)menred_controlData[i].mcd_reg_details));
                    break;
                }
                case regCloudAddr_heatingStatus:
                {
                    ret = mcu_dp_enum_update(DPID_VALVE_STATE,menred_controlData[i].mcd_reg_details);
                    //testDat = ret;
                    break;
                }
                case regCloudAddr_notFrozen:
                {
                    ret = mcu_dp_bool_update(DPID_NOTFROZEN,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_maximumSetTemp:
                {
                    ret = mcu_dp_value_update(DPID_UPPER_TEMP,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_programWorkDaysTime_1:  //raw
                case regCloudAddr_programWorkDaysSetT_1:  
                case regCloudAddr_programWorkDaysTime_2:  
                case regCloudAddr_programWorkDaysSetT_2:
                case regCloudAddr_programWorkDaysTime_3:  
                case regCloudAddr_programWorkDaysSetT_3:  
                case regCloudAddr_programWorkDaysTime_4: 
                case regCloudAddr_programWorkDaysSetT_4:  
                case regCloudAddr_programWorkDaysTime_5:  
                case regCloudAddr_programWorkDaysSetT_5: 
                case regCloudAddr_programWorkDaysTime_6:  
                case regCloudAddr_programWorkDaysSetT_6: 
                case regCloudAddr_programHolidaysTime_1:  
                case regCloudAddr_programHolidaysSetT_1:  
                case regCloudAddr_programHolidaysTime_2:  
                case regCloudAddr_programHolidaysSetT_2:  
                {
                    INT8U raw_buff[34];
                    INT8U length;
                    length = make_program_frame_raw(raw_buff,i);
                    ret = mcu_dp_raw_update(DPID_WEEK_PROGRAM1,raw_buff,length);
                    break;
                }
                case regCloudAddr_programCfg:
                {
                    ret = mcu_dp_enum_update(DPID_WORK_DAYS,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_sensorCfg:
                {
                    ret = mcu_dp_enum_update(DPID_SENSOR_CHOOSE,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_floorSensorType:
                {
                    ret = mcu_dp_enum_update(DPID_FLOORSENSERTYPE,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_floorLimitTemp:
                {
                    ret = mcu_dp_value_update(DPID_FLOORLIMITTEMP,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_floorTemperature:
                {
                    ret = mcu_dp_value_update(DPID_FLOORTEMP,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_childLock:
                {
                    ret = mcu_dp_bool_update(DPID_CHILD_LOCK,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_openWindowSwitch:
                {
                    ret = mcu_dp_bool_update(DPID_OPEN_WINDOW_SWITCH,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_openWindowStatus:
                {
                    ret = mcu_dp_enum_update(DPID_OPEN_WINDOW_STATUS,menred_controlData[i].mcd_reg_details);
                    break;
                }
                case regCloudAddr_openWindowCheckDownTemp:               
                case regCloudAddr_openWindowCheckDownTime:
                case regCloudAddr_openWindowRunBackTime:
                case regCloudAddr_openWindowPreheatSwitch:
                {
                    INT8U raw_buff[8];
                    INT8U length;
                    length = make_program_frame_raw(raw_buff,i);
                    ret = mcu_dp_raw_update(DPID_REG_RAW,raw_buff,length);
                    break;
                }
                default:break;
            }
            if(SUCCESS == ret)
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