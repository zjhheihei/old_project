

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
    tuya_wifiState_smartConfig        = SMART_CONFIG_STATE,//smart 配网 5G模块为蓝牙方式
    tuya_wifiState_ap                 = AP_STATE,          //AP  配网
    tuya_wifiState_routeNotConnected  = WIFI_NOT_CONNECTED,//没有连接到路由器
    tuya_wifiState_routeConnected     = WIFI_CONNECTED,    //连接到路由器
    tuya_wifiState_connCloud          = WIFI_CONN_CLOUD,   //连接到服务器
    tuya_wifiState_lowPower           = WIFI_LOW_POWER,    //低功耗
    tuya_wifiState_unknow             = WIFI_SATE_UNKNOW,  //未知
}tuya_wifiState_def;
//------------------------------------------------------------------------------
typedef struct
{
    BOOLEAN  netRTC_isValid;  //true　网络时间有效
    INT8U    netRTC_minute;
    INT8U    netRTC_hour;
    INT8U    netRTC_week;    //0-6, 0 is means sunday
    INT8U    netRTC_second;
}net_rtcTime_Def;
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define REG_CLOUD_ADDR_POWER_SWITCH            0x00
#define REG_CLOUD_ADDR_FAC_SET                 0x01
#define REG_CLOUD_ADDR_POF_SET                 0X02
#define REG_CLOUD_ADDR_FCO_SET                 0X03

#define REG_CLOUD_ADDR_RUN_SET_TEMP            0X10
#define REG_CLOUD_ADDR_MAX_SET_TEMP            0X11
#define REG_CLOUD_ADDR_RUN_MEAS_TEMP           0X12
#define REG_CLOUD_ADDR_MIN_SET_TEMP            0X13
#define REG_CLOUD_ADDR_DIF_SET_TEMP            0X14
#define REG_CLOUD_ADDR_SUN_SET_TEMP            0X15
#define REG_CLOUD_ADDR_MOON_SET_TEMP           0X16
#define REG_CLOUD_ADDR_LIF_SET_TEMP            0X17

  
      
#define REG_CLOUD_ADDR_WORK_MODE               0x20
#define REG_CLOUD_ADDR_WORK_STATUS             0x21
#define REG_CLOUD_ADDR_WEEK_PRG_SET            0X22
#define REG_CLOUD_ADDR_VALVE_STATUS            0X23
#define REG_CLOUD_ADDR_SERSON_SECELT           0X24
#define REG_CLOUD_ADDR_SET_WIND_SPEED          0X25

#define REG_CLOUD_ADDR_AI_PRO_WARM             0X30
#define REG_CLOUD_ADDR_AI_PRO_COLD             0X31
#define REG_CLOUD_ADDR_AI_PRO_OEM              0X32
#define REG_CLOUD_ADDR_OTHER_SET               0X33

#define REG_CLOUD_ADDR_FAULT                   0X40



#define REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_CFG        0x00
#define REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_WORK_TIME  0x01
#define REG_CLOUD_OHTER_SET_ADDR_TIME_CONTROL_REST_TIME  0x02
#define REG_CLOUD_OHTER_SET_ADDR_FLOOR_SENSOR_TYPE       0x03
#define REG_CLOUD_OHTER_SET_ADDR_SENSOR_STATUS           0x04
#define REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_H       0x05
#define REG_CLOUD_OHTER_SET_ADDR_FLOOR_MEAS_TEMP_L       0x06
#define REG_CLOUD_OHTER_SET_ADDR_LINK_DELAY_TIME         0x07
#define REG_CLOUD_OHTER_SET_ADDR_LTP_SET_TEMP            0x08
#define REG_CLOUD_OHTER_SET_ADDR_RS485_ADDR              0x09
#define REG_CLOUD_OHTER_SET_ADDR_RS485_BAUD              0x0A
#define REG_CLOUD_OHTER_SET_ADDR_RS485_PARITY            0x0B
#define REG_CLOUD_OHTER_SET_ADDR_PREHEAT_CFG             0x0C
#define REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CFG         0x0D
#define REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_STATUS      0x0E
#define REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_SET_TEMP    0x0F
#define REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_CHECK_TIME  0x10
#define REG_CLOUD_OHTER_SET_ADDR_OPEN_WINDOW_BACK_TIME   0x11


#define AI_PRG_LENGTH      (WEEK_PRG_LENGTH * 4)     //长度
#define OHTER_SET_LENGTH   18                         //长度
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

void raw_dp_updat_to_cloud(uint8_t regAddr,uint8_t length,uint8_t *dat);
void raw_dp_download_from_cloud(uint8_t regAddr,uint8_t length,uint8_t *dat);
uint8_t raw_dp_updat_to_deceive(uint8_t regAddr,uint8_t *_out_dat);
#endif



//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++



