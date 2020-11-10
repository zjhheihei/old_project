
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_TUYAINTERFACE_H
#define _APP_TUYAINTERFACE_H
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum 
{
    tuya_cfg_smart     = SMART_CONFIG,
    tuya_cfg_ap        = AP_CONFIG,
}tuya_configType_def;
//------------------------------------------------------------------------------
typedef enum
{
    tuya_wifiState_smartConfig        = SMART_CONFIG_STATE,//smart 配网
    tuya_wifiState_ap                 = AP_STATE,          //AP  配网
    tuya_wifiState_routeNotConnected  = WIFI_NOT_CONNECTED,
    tuya_wifiState_routeConnected     = WIFI_CONNECTED,
    tuya_wifiState_connCloud          = WIFI_CONN_CLOUD,
    tuya_wifiState_lowPower           = WIFI_LOW_POWER,
    tuya_wifiState_unknow             = WIFI_SATE_UNKNOW,
}tuya_wifiState_def;
//------------------------------------------------------------------------------
typedef struct
{
    BOOLEAN  netRTC_isValid;  //true　网络时间有效
    INT8U    netRTC_minute;
    INT8U    netRTC_hour;
    INT8U    netRTC_week;    //0-6, 0 is means sunday
}net_rtcTime_Def;
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define regCloudAddr_power                     0x01
#define regCloudAddr_ctlMode                   0x02
#define regCloudAddr_setTemperature            0x03//设定温度 int16s
#define regCloudAddr_measureTemperature        0x04//测量温度 int16s
#define regCloudAddr_heatingStatus             0x05//heating
#define regCloudAddr_notFrozen                 0x06
#define regCloudAddr_maximumSetTemp            0x07//最高设定温度
#define regCloudAddr_programWorkDaysTime_1     0x08
#define regCloudAddr_programWorkDaysSetT_1     0x09
#define regCloudAddr_programWorkDaysTime_2     0x0a
#define regCloudAddr_programWorkDaysSetT_2     0x0b
#define regCloudAddr_programWorkDaysTime_3     0x0c
#define regCloudAddr_programWorkDaysSetT_3     0x0d
#define regCloudAddr_programWorkDaysTime_4     0x0e
#define regCloudAddr_programWorkDaysSetT_4     0x0f
#define regCloudAddr_programWorkDaysTime_5     0x10
#define regCloudAddr_programWorkDaysSetT_5     0x11
#define regCloudAddr_programWorkDaysTime_6     0x12
#define regCloudAddr_programWorkDaysSetT_6     0x13
#define regCloudAddr_programHolidaysTime_1     0x14
#define regCloudAddr_programHolidaysSetT_1     0x15
#define regCloudAddr_programHolidaysTime_2     0x16
#define regCloudAddr_programHolidaysSetT_2     0x17
#define regCloudAddr_programCfg                0x18

#define regCloudAddr_sensorCfg                 0x20//传感器配置
#define regCloudAddr_floorSensorType           0x21
#define regCloudAddr_floorLimitTemp            0x22
#define regCloudAddr_floorTemperature          0x24
#define regCloudAddr_childLock                 0x26
#define regCloudAddr_openWindowSwitch          0x27
#define regCloudAddr_openWindowStatus          0x28

#define regCloudAddr_openWindowCheckDownTemp   0x29
#define regCloudAddr_openWindowCheckDownTime   0x2a
#define regCloudAddr_openWindowRunBackTime     0x2b
#define regCloudAddr_openWindowPreheatSwitch   0x2c
#define regCloudAddr_linkRelayOpenDelayTime    0x2d
#define regCloudAddr_notFrozen_set_temp        0x2e

//------------------------------------------------------------------------------
//@regCloudAddr_power
#define regCloudAddr_power_off         0x00
#define regCloudAddr_power_on          0x01
//------------------------------------------------------------------------------
//@regCloudAddr_ctlMode
#define regCloudAddr_ctlMode_man       0x00//
#define regCloudAddr_ctlMode_prgAuto   0x01
#define regCloudAddr_ctlMode_prgTemp   0x02
//------------------------------------------------------------------------------
//@regCloudAddr_childLock
#define regCloudAddr_childLock_unlock       0x00
#define regCloudAddr_childLock_locked       0x01
//------------------------------------------------------------------------------
//@regCloudAddr_programCfg
#define regCloudAddr_programCfg_easy            0x00
#define regCloudAddr_programCfg_fiveWorkDay     0x01
#define regCloudAddr_programCfg_sixWorkDay      0x02
#define regCloudAddr_programCfg_sevenWorkDay    0x03
//------------------------------------------------------------------------------
//@regCloudAddr_sensorCfg 
#define regCloudAddr_sensorCfg_in               0x00
#define regCloudAddr_sensorCfg_out              0x01
#define regCloudAddr_sensorCfg_all              0x02
//------------------------------------------------------------------------------
//@regCloudAddr_floorSensorType
#define regCloudAddr_floorSensorType_10k3380    0x00
#define regCloudAddr_floorSensorType_5k3950     0x01
#define regCloudAddr_floorSensorType_10k3950    0x02
//------------------------------------------------------------------------------
//@regCloudAddr_heatingStatus
#define regCloudAddr_heatingStatus_notHeat     0x00
#define regCloudAddr_heatingStatus_heating     0x01
//------------------------------------------------------------------------------
//@regCloudAddr_notFrozen
#define regCloudAddr_notFrozen_disable         0x00
#define regCloudAddr_notFrozen_enable          0x01
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//Event
//------------------------------------------------------------------------------
//Status
//------------------------------------------------------------------------------
//Function
//名称: 涂鸦接口任务
//功能: 涂鸦接口支持任务,放入主循环调用
//入口: 无
//
//出口: 无

void app_tuyaInterface_task(void);

//------------------------------------------------------------------------------
//名称: 发送数据到串口
//功能: 发送一个字节的数据到串口,供涂鸦SDK调用
//入口: in_data 发送的数据
//
//出口: 无

void app_transmitToUart(INT8U in_data);
//------------------------------------------------------------------------------
//名称: 修正一次串口波特率
//功能: 修正波特率会关闭串口，在接收完毕，应答前修正最合适，由涂鸦SDK调用
//入口: 
//
//出口: 无
void app_onceAmend_baudrate(void);
//------------------------------------------------------------------------------
//名称: push一次网络RTC数据到这个模块
//功能: 
//入口: NET_RTC结构体
//
//出口: 无
void app_pushNetRtcTimeToThisMoudle(net_rtcTime_Def in_net_rtcTime);
//------------------------------------------------------------------------------
//名称: 获取一次网络时间
//功能: 
//入口: 无
//
//出口: NET_RTC结构体
net_rtcTime_Def app_pullNetRtcTimeFromThisModule(void);
//------------------------------------------------------------------------------
//名称: 获取网络状态
//功能: 
//入口: 无
//
//出口: 状态枚举
tuya_wifiState_def app_pullTuyaWifiStateFormThisModule(void);
//------------------------------------------------------------------------------
//名称: 进入wifi配网状态
//功能: 
//入口: 配网类型,tuya_cfg_smart: smart config推荐
//               tuya_cfg_ap:AP配网模式
//
//出口: none
void app_entryWifiCfg_state(tuya_configType_def in_cfgType);
//------------------------------------------------------------------------------
//名称: 执行一次全部数据上传
//功能: 置位全部数据上传的信号量，一般供云端查询调用，也可供设备端调用
//入口: 
//    
//
//出口: 
void app_pushAllDataUpdateOnceExecute(void);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//名称: 由云端推入数据到接口模块
//功能: SDK接收到数据后，推入此接口模块
//入口: 寄存器地址,寄存器内容
//    
//
//出口: 
void app_pushContorolDataFromCloud_download(INT8U in_reg_addr,INT16U in_reg_details);
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//以下3个函数由设备端调用,用于云端的数据交互
//------------------------------------------------------------------------------
//名称: 获取一次全部数据上传的信号量状态
//功能: 获取信号量的状态，信号量自动复位,一般由主设备应用代码调用
//入口: 
//    
//
//出口: BN_TRUE: 需要上传一次全部数据
BOOLEAN app_pullAllDataUpdateOnceStatus(void);
//------------------------------------------------------------------------------
//名称: 由设备端推入数据到接口模块
//功能: 设备主应用程序把需要发送到云端的数据推入此接口模块，接口模块自动更新到云端
//入口: 寄存器地址,寄存器内容
//    
//
//出口: 寄存器地址,寄存器内容
void app_pushControlDataFromDevice_update(INT8U in_reg_addr,INT16U in_reg_details);
//------------------------------------------------------------------------------
//名称: 从接口模块拉取新的数据
//功能: 一般由设备主应用程序拉取新的数据，数据来自云端。
//入口: 寄存器地址,寄存器内容
//    
//
//出口: BN_TRUE:接口模块有新数据
BOOLEAN app_pullControlDataFromThis(INT8U* out_reg_addr,INT16U* out_reg_details);
//------------------------------------------------------------------------------
//名称: 获取wifi模块uart通讯在线状态
//功能: 
//入口: 
//    
//
//出口: BN_TRUE:wifi模块uart在线
BOOLEAN app_pullWifiModuleOnline(void);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++



