///****************************************Copyright (c)*************************
//**                               版权所有 (C), 2015-2020, 涂鸦科技
//**
//**                                 http://www.tuya.com
//**
//**--------------文件信息-------------------------------------------------------
//**文   件   名: protocol.c
//**描        述: 下发/上报数据处理函数
//**使 用 说 明 :
//
//                  *******非常重要，一定要看哦！！！********
//
//** 1、用户在此文件中实现数据下发/上报功能
//** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
//** 3、当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
//**
//**--------------当前版本修订---------------------------------------------------
//** 版  本: v2.5.3
//** 日　期: 2019年11月26日
//** 描　述: 1:增加红外状态通知
//           2:增加红外进入收发产测
//           3:修复串口数据接收函数uart_receive_input的BUG(不能判断出其中一种缓存区满的情况)
//           4:修改串口数据缓存区的数组名称和长度宏定义的名称
//           
//** 版  本: v2.5.2
//** 日　期: 2019年7月5日
//** 描　述: 1:增加WiFi功能性测试（连接指定路由）
//           2:简化流服务过程，用户只需要调用流服务传输接口即可使用流服务
//           3:增加同步上报指令
//           4:增加获取当前wifi联网状态指令
//           5:增加获取格林时间指令
//           6:优化ota升级流程，启动ota升级时用户可以选择传输包大小
//           7:增加获取模块mac地址指令
//           
//** 版  本: v2.5.1
//** 日　期: 2018年10月27日
//** 描　述: 1:默认关闭流服务功能
//           2:增加03协议wifi状态宏定义
//		   3:更新与修改部分函数注释
//		   
//** 版  本: v2.5.0
//** 日　期: 2018年4月18日
//** 描　述: 1:协议版本改为0x03
//           2:增加WIFI模组心跳关闭功能
//           3:增加天气功能
//
//** 版  本: v2.3.8
//** 日　期: 2018年1月17日
//** 描　述: 1:变量添加volatile防止编译器优化
//           2:添加#error提示
//
//** 版  本: v2.3.7
//** 日　期: 2017年4月18日
//** 描　述: 1:优化串口队列接收处理
//		   
//** 版  本: v2.3.6
//** 日　期: 2016年7月21日
//** 描　述: 1:修复获取本地时间错误
//           2:添加hex_to_bcd转换函数
//		   
//** 版  本: v2.3.5
//** 日　期: 2016年6月3日
//** 描　述: 1:修改返回协议版本为0x01
//           2:固件升级数据偏移量修改为4字节
//
//** 版  本: v2.3.4
//** 日　期: 2016年5月26日
//** 描　述: 1:优化串口解析函数
//           2:优化编译器兼容性,取消enum类型定义
//
//** 版  本: v2.3.3
//** 日　期: 2016年5月24日
//** 描　述: 1:修改mcu获取本地时间函数
//           2:添加wifi功能测试
//
//** 版  本: v2.3.2
//** 日　期: 2016年4月23日
//** 描　述: 1:优化串口数据解析
//           2:优化MCU固件升级流程
//           3:优化上报流程
//
//** 版  本: v2.3.1
//** 日　期: 2016年4月15日
//** 描　述: 1:优化串口数据解析
//
//** 版  本: v2.3
//** 日　期: 2016年4月14日
//** 描　述: 1:支持MCU固件在线升级
//
//** 版  本: v2.2
//** 日　期: 2016年4月11日
//** 描　述: 1:修改串口数据接收方式
//
//** 版  本: v2.1
//** 日　期: 2016年4月8日
//** 描　述: 1:加入某些编译器不支持函数指针兼容选项
//
//** 版  本: v2.0
//** 日　期: 2016年3月29日
//** 描　述: 1:优化代码结构
//           2:节省RAM空间
//**
//**-----------------------------------------------------------------------------
//******************************************************************************/
#include "..\.\APP_Cfg.h"
#include "tuya_wifi.h"

#ifdef WEATHER_ENABLE
/******************************************************************************
                        天气数据参数选择数组
          **用户可以自定义需要的参数，注释或者取消注释即可，注意更改**         
******************************************************************************/
const char weather_choose[WEATHER_CHOOSE_CNT][10] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",*/
 };
#endif

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

         
/******************************************************************************
                              第一步:初始化
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},              //1        
  {DPID_MODE, DP_TYPE_ENUM},                //2
  {DPID_OPEN_WINDOW_SWITCH, DP_TYPE_BOOL},  //8
  {DPID_TEMP_SET, DP_TYPE_VALUE},           //16
  {DPID_UPPER_TEMP, DP_TYPE_VALUE},         //19
  {DPID_TEMP_CURRENT, DP_TYPE_VALUE},       //24
  {DPID_OPEN_WINDOW_STATUS, DP_TYPE_ENUM},  //25
  {DPID_WORK_DAYS, DP_TYPE_ENUM},           //31 
  {DPID_VALVE_STATE, DP_TYPE_ENUM},         //36
  {DPID_WEEK_PROGRAM1, DP_TYPE_RAW},        //38
  {DPID_CHILD_LOCK, DP_TYPE_BOOL},          //40
  {DPID_SENSOR_CHOOSE, DP_TYPE_ENUM},       //43
  {DPID_FAULT, DP_TYPE_BITMAP},             //45
  {DPID_RESERVE, DP_TYPE_RAW},              //101
  {DPID_REG_RAW, DP_TYPE_RAW},              //102
  {DPID_RESERVE_CHAR, DP_TYPE_STRING},      //103
  {DPID_FLOORTEMP, DP_TYPE_VALUE},          //104
  {DPID_FLOORSENSERTYPE, DP_TYPE_ENUM},     //105
  {DPID_FLOORLIMITTEMP, DP_TYPE_VALUE},     //106
  {DPID_NOTFROZEN, DP_TYPE_BOOL},           //107
};


/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处理
输入参数 : value:串口收到字节数据
返回参数 : 无
使用说明 : 请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
  //#error "请将MCU串口发送函数填入该函数,并删除该行"
/*
  //示例:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //串口发送函数
*/
    app_transmitToUart(value);
}
/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传,实现APP和muc数据同步
输入参数 : 无
返回参数 : 无
使用说明 : 此函数SDK内部需调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数据
*****************************************************************************/
void all_data_update(void)
{
 // #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"
    app_pushAllDataUpdateOnceExecute();
  /* 
  //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
  mcu_dp_bool_update(DPID_SWITCH,当前开关); //BOOL型数据上报;
  mcu_dp_enum_update(DPID_MODE,当前工作模式); //枚举型数据上报;
  mcu_dp_value_update(DPID_TEMP_SET,当前目标温度); //VALUE型数据上报;
  mcu_dp_value_update(DPID_UPPER_TEMP,当前设置温度上限); //VALUE型数据上报;
  mcu_dp_value_update(DPID_TEMP_CURRENT,当前当前温度); //VALUE型数据上报;
  mcu_dp_enum_update(DPID_WORK_DAYS,当前工作日设置); //枚举型数据上报;
  mcu_dp_enum_update(DPID_VALVE_STATE,当前阀门状态); //枚举型数据上报;
  mcu_dp_raw_update(DPID_WEEK_PROGRAM1,当前周程序（自由式）指针,当前周程序（自由式）数据长度); //RAW型数据上报;
  mcu_dp_bool_update(DPID_CHILD_LOCK,当前童锁开关); //BOOL型数据上报;
  mcu_dp_enum_update(DPID_SENSOR_CHOOSE,当前传感器选择); //枚举型数据上报;
  mcu_dp_fault_update(DPID_FAULT,当前故障告警); //故障型数据上报;
  mcu_dp_raw_update(DPID_RESERVE,当前保留指针,当前保留数据长度); //RAW型数据上报;
  mcu_dp_raw_update(DPID_REG_RAW,当前寄存器透传指针,当前寄存器透传数据长度); //RAW型数据上报;
  mcu_dp_string_update(DPID_RESERVE_CHAR,当前保留_char指针,当前保留_char数据长度); //STRING型数据上报;
  mcu_dp_value_update(DPID_FLOORTEMP,当前地面温度); //VALUE型数据上报;
  mcu_dp_enum_update(DPID_FLOORSENSERTYPE,当前地面传感器类型); //枚举型数据上报;
  mcu_dp_value_update(DPID_FLOORLIMITTEMP,当前地面温度限值); //VALUE型数据上报;
  mcu_dp_bool_update(DPID_NOTFROZEN,当前防冻状态); //BOOL型数据上报;

 */
}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_switch_handle
功能描述 : 针对DPID_SWITCH的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char the_switch;
  
  the_switch = mcu_get_dp_download_bool(value,length);
  app_pushContorolDataFromCloud_download(regCloudAddr_power,the_switch);
  /*
  if(the_switch == 0)
  {
    //开关关
  }
  else
  {
    //开关开
  }*/
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_bool_update(DPID_SWITCH,the_switch);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}

/*****************************************************************************
函数名称 : dp_download_mode_handle
功能描述 : 针对DPID_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char mode;
  
  mode = mcu_get_dp_download_enum(value,length);
  app_pushContorolDataFromCloud_download(regCloudAddr_ctlMode,mode);
  /*
  switch(mode)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    default:
      
      break;
  }*/
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}

/*****************************************************************************
函数名称 : dp_download_mode_handle
功能描述 : 针对DPID_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_openWindowSitch_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char the_switch;
  
  the_switch = mcu_get_dp_download_bool(value,length);
  app_pushContorolDataFromCloud_download(regCloudAddr_openWindowSwitch,the_switch);
  ret = mcu_dp_bool_update(DPID_OPEN_WINDOW_SWITCH,the_switch);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}




/*****************************************************************************
函数名称 : dp_download_temp_set_handle
功能描述 : 针对DPID_TEMP_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_temp_set_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long temp_set;
  
  temp_set = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  app_pushContorolDataFromCloud_download(regCloudAddr_setTemperature,temp_set);
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_TEMP_SET,temp_set);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_upper_temp_handle
功能描述 : 针对DPID_UPPER_TEMP的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_upper_temp_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long upper_temp;
  
  upper_temp = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  app_pushContorolDataFromCloud_download(regCloudAddr_maximumSetTemp,upper_temp);
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_UPPER_TEMP,upper_temp);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}



/*****************************************************************************
函数名称 : dp_download_work_days_handle
功能描述 : 针对DPID_WORK_DAYS的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_work_days_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char work_days;
  
  work_days = mcu_get_dp_download_enum(value,length);
  app_pushContorolDataFromCloud_download(regCloudAddr_programCfg,work_days);  
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_WORK_DAYS,work_days);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_week_program1_handle
功能描述 : 针对DPID_WEEK_PROGRAM1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
#define WEEK_REG_END_ADD    0X18
#define WEEK_REG_START_ADD  regCloudAddr_programWorkDaysTime_1
static unsigned char dp_download_week_program1_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为RAW
  unsigned char ret = SUCCESS;
  /*
  //RAW类型数据处理
  */
  volatile static unsigned int dat[16];
  unsigned char i = 0; 
  unsigned char len = length - 2 / 2;
  for(;i < len ;i++)
  {
      dat[i] = (unsigned int)value[2 + i * 2];
      dat[i] <<= 8;
      dat[i] |= (unsigned int)value[2 + i * 2 + 1];      
  }
  for(i = WEEK_REG_START_ADD;i < WEEK_REG_END_ADD; i++)
  {
      app_pushContorolDataFromCloud_download(i,dat[i - WEEK_REG_START_ADD]);
  }  
  //处理完DP数据后应有反馈
  ret = mcu_dp_raw_update(DPID_WEEK_PROGRAM1,value,length);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_child_lock_handle
功能描述 : 针对DPID_CHILD_LOCK的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_child_lock_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char child_lock;
  
  child_lock = mcu_get_dp_download_bool(value,length);
 /* if(child_lock == 0)
  {
    //开关关
  }
  else
  {
    //开关开
  }*/
  app_pushContorolDataFromCloud_download(regCloudAddr_childLock,child_lock);
  //处理完DP数据后应有反馈
  ret = mcu_dp_bool_update(DPID_CHILD_LOCK,child_lock);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_sensor_choose_handle
功能描述 : 针对DPID_SENSOR_CHOOSE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_sensor_choose_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char sensor_choose;
  
  sensor_choose = mcu_get_dp_download_enum(value,length);
  /*switch(sensor_choose)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    default:
      
      break;
  }*/
  app_pushContorolDataFromCloud_download(regCloudAddr_sensorCfg,sensor_choose);
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_SENSOR_CHOOSE,sensor_choose);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_reserve_handle
功能描述 : 针对DPID_RESERVE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_reserve_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为RAW
  unsigned char ret;
  /*
  //RAW类型数据处理
  
  */
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_raw_update(DPID_RESERVE,value,length);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_reg_raw_handle
功能描述 : 针对DPID_REG_RAW的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
#define RAW_REG_END_ADD    0X2D
#define RAW_REG_START_ADD  regCloudAddr_openWindowCheckDownTemp
static unsigned char dp_download_reg_raw_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为RAW
  unsigned char ret = SUCCESS;
  
  volatile static unsigned int dat[4];
  unsigned char i = 0; 
  unsigned char len = length - 2 / 2;
  for(;i < len ;i++)
  {
      dat[i] = (unsigned int)value[2 + i * 2];
      dat[i] <<= 8;
      dat[i] |= (unsigned int)value[2 + i * 2 + 1];      
  }
  for(i = RAW_REG_START_ADD;i < RAW_REG_END_ADD; i++)
  {
      app_pushContorolDataFromCloud_download(i,dat[i - RAW_REG_START_ADD]);
  }  
   
  //处理完DP数据后应有反馈
  ret = mcu_dp_raw_update(DPID_REG_RAW,value,length);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_reserve_char_handle
功能描述 : 针对DPID_RESERVE_CHAR的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_reserve_char_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为STRING
  unsigned char ret;
  /*
  //STRING类型数据处理
  unsigned char string_data[8];
  
  string_data[0] = value[0];
  string_data[1] = value[1];
  string_data[2] = value[2];
  string_data[3] = value[3];
  string_data[4] = value[4];
  string_data[5] = value[5];
  string_data[6] = value[6];
  string_data[7] = value[7];
  */
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_string_update(DPID_RESERVE_CHAR,value,length);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_floortemp_handle
功能描述 : 针对DPID_FLOORTEMP的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_floortemp_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long floortemp;
  
  floortemp = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */

  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_FLOORTEMP,floortemp);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_floorsensertype_handle
功能描述 : 针对DPID_FLOORSENSERTYPE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_floorsensertype_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char floorsensertype;
  
  floorsensertype = mcu_get_dp_download_enum(value,length);
  /*switch(floorsensertype)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    default:
      
      break;
  }*/
  app_pushContorolDataFromCloud_download(regCloudAddr_floorSensorType,floorsensertype);
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_FLOORSENSERTYPE,floorsensertype);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_floorlimittemp_handle
功能描述 : 针对DPID_FLOORLIMITTEMP的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_floorlimittemp_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long floorlimittemp;
  
  floorlimittemp = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  app_pushContorolDataFromCloud_download(regCloudAddr_floorLimitTemp,floorlimittemp);
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_FLOORLIMITTEMP,floorlimittemp);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_notfrozen_handle
功能描述 : 针对DPID_NOTFROZEN的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_notfrozen_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char notfrozen;
  
  notfrozen = mcu_get_dp_download_bool(value,length);
  /*if(notfrozen == 0)
  {
    //开关关
  }
  else
  {
    //开关开
  }*/
  app_pushContorolDataFromCloud_download(regCloudAddr_notFrozen,notfrozen);
  //处理完DP数据后应有反馈
  ret = mcu_dp_bool_update(DPID_NOTFROZEN,notfrozen);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}




/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_GREEN_TIME
/*****************************************************************************
函数名称 : mcu_get_greentime
功能描述 : 获取到的格林时间
输入参数 : 无
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_get_greentime(unsigned char time[])
{
//#error "请自行完成相关代码,并删除该行"
  /*
  time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
  time[1] 为 年 份 , 0x00 表 示2000 年
  time[2]为月份，从 1 开始到12 结束
  time[3]为日期，从 1 开始到31 结束
  time[4]为时钟，从 0 开始到23 结束
  time[5]为分钟，从 0 开始到59 结束
  time[6]为秒钟，从 0 开始到59 结束
*/
  if(time[0] == 1)
  {
    //正确接收到wifi模块返回的格林数据
  }
  else
  {
  	//获取格林时间出错,有可能是当前wifi模块未联网
  }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : 无
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  //#error "请自行完成RTC时钟写入代码,并删除该行"
  /*
  time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
  time[1] 为 年 份 , 0x00 表 示2000 年
  time[2]为月份，从 1 开始到12 结束
  time[3]为日期，从 1 开始到31 结束
  time[4]为时钟，从 0 开始到23 结束
  time[5]为分钟，从 0 开始到59 结束
  time[6]为秒钟，从 0 开始到59 结束
  time[7]为星期，从 1 开始到 7 结束，1代表星期一
 */
  net_rtcTime_Def now_rtcTime;
  if(time[0] == 1)
  {
    //正确接收到wifi模块返回的本地时钟数据 
      now_rtcTime.netRTC_isValid = BN_TRUE;
      now_rtcTime.netRTC_minute = time[5];
      now_rtcTime.netRTC_hour = time[4];
      now_rtcTime.netRTC_week = time[7];
      if(now_rtcTime.netRTC_week > 6)
      {
          now_rtcTime.netRTC_week = 0;
      }
  }
  else
  {
      now_rtcTime.netRTC_isValid = BN_FALSE;
  	//获取本地时钟数据出错,有可能是当前wifi模块未联网
  }
  app_pushNetRtcTimeToThisMoudle(now_rtcTime);
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
函数名称 : wifi_test_result
功能描述 : wifi功能测试反馈
输入参数 : result:wifi功能测试结果;0:失败/1:成功
           rssi:测试成功表示wifi信号强度/测试失败表示错误类型
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
  //#error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
  if(result == 0)
  {
    //测试失败
    if(rssi == 0x00)
    {
      //未扫描到名称为tuya_mdev_test路由器,请检查
    }
    else if(rssi == 0x01)
    {
      //模块未授权
    }
  }
  else
  {
    //测试成功
    //rssi为信号强度(0-100, 0信号最差，100信号最强)
  }
  
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/*****************************************************************************
函数名称 : wifi_connect_test_result
功能描述 : 路由信息接收结果通知
输入参数 : result:模块是否成功接收到正确的路由信息;0:失败/1:成功
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void wifi_connect_test_result(unsigned char result)
{
  //#error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
  if(result == 0)
  {
    //路由信息接收失败，请检查发出的路由信息包是否是完整的JSON数据包
  }
  else
  {
    //路由信息接收成功，产测结果请注意WIFI_STATE_CMD指令的wifi工作状态
  }
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : mac：mac[0]为是否获取mac成功标志，0x00 表示成功，为0x01表示失败
  mac[1]~mac[6]:当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_get_mac(unsigned char mac[])
{
 // #error "请自行完成mac获取代码,并删除该行"
  /*
  mac[0]为是否获取mac成功标志，0x00 表示成功，为0x01表示失败
  mac[1]~mac[6]:当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
 */
 
  if(mac[0] == 1)
  {
  	//获取mac出错
  }
  else
  {
    //正确接收到wifi模块返回的mac地址 
  }
}
#endif





#ifdef GET_WIFI_STATUS_ENABLE
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : 获取 WIFI 状态结果
输入参数 : result:指示 WIFI 工作状态
           0x00:wifi状态 1 smartconfig 配置状态
           0x01:wifi状态 2 AP 配置状态
           0x02:wifi状态 3 WIFI 已配置但未连上路由器
           0x03:wifi状态 4 WIFI 已配置且连上路由器
           0x04:wifi状态 5 已连上路由器且连接到云端
           0x05:wifi状态 6 WIFI 设备处于低功耗模式
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void get_wifi_status(unsigned char result)
{
  //#error "请自行完成mac获取代码,并删除该行"
 
  switch(result) {
    case 0:
      //wifi工作状态1
      break;
  
    case 1:
      //wifi工作状态2
      break;
      
    case 2:
      //wifi工作状态3
      break;
      
    case 3:
      //wifi工作状态4
      break;
      
    case 4:
      //wifi工作状态5
      break;
      
    case 5:
      //wifi工作状态6
      break;
      
    default:
    break;
  }
}
#endif

#ifdef MCU_DP_UPLOAD_SYN
/*****************************************************************************
函数名称 : get_upload_syn_result
功能描述 : 状态同步上报结果
输入参数 : result:0x00失败  0x01成功
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void get_upload_syn_result(unsigned char result)
{
  //#error "请自行完成mac获取代码,并删除该行"
  /*
  mac[0]为是否获取mac成功标志，0x00 表示成功，为0x01表示失败
  mac[1]~mac[6]:当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
 */
 
  if(result == 0)
  {
  	//获取mac出错
  }
  else
  {
    //正确接收到wifi模块返回的mac地址 
  }
}
#endif


#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : upgrade_package_choose
功能描述 : 升级包大小选择
输入参数 : package_sz:升级包大小
           0x00：默认 256byte
           0x01：512byte 
           0x02：1024byte

返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void upgrade_package_choose(unsigned char package_sz)
{
//  #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
  unsigned short length = 0;
  length = set_wifi_uart_byte(length,package_sz);
  wifi_uart_write_frame(UPDATE_START_CMD,length);
}



/*****************************************************************************
函数名称 : mcu_firm_update_handle
功能描述 : MCU进入固件升级模式
输入参数 : value:固件缓冲区
           position:当前数据包在于固件位置
           length:当前固件包长度(固件包长度为0时,表示固件包发送完成)
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    static unsigned char cnt = 0;
  //#error "请自行完成MCU固件升级代码,完成后请删除该行"
  if(length == 0)
  {
    //固件数据发送完成
    
      //app_pushBlock_toThisUpdate((INT8U*)(&value[0]));

  }
  else
  {
    //固件数据处理
    //unsigned long trycnt = 0;
    
    //for(trycnt = 0; trycnt < 60000;trycnt++);
    
      if(cnt == 1)
      {
          cnt = 0;
      }
          if(256 == length)
          {
              app_pushBlock_toThisUpdate((INT8U*)(&value[0]));
              app_pushBlock_toThisUpdate((INT8U*)(&value[128]));             
          }
          else if(128 == length)
          {
              app_pushBlock_toThisUpdate((INT8U*)(&value[0]));
          }         

  }
  cnt ++;
  return SUCCESS;
}
#endif






















#ifdef GET_IR_STATUS_ENABLE
/*****************************************************************************
函数名称 : get_ir_status
功能描述 : 获取红外状态结果
输入参数 : result:指示红外状态
           0x00:wifi状态 1 正在发送红外码
           0x01:wifi状态 2 发送红外码结束
           0x02:wifi状态 3 红外学习开始
           0x03:wifi状态 4 红外学习结束
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void get_ir_status(unsigned char result)
{
  #error "请自行完成红外状态代码,并删除该行"
  unsigned short length = 0;
    
  switch(result) {
    case 0:
      //红外状态1
      break;
  
    case 1:
      //红外状态2
      break;
      
    case 2:
      //红外状态3
      break;
      
    case 3:
      //红外状态4
      break;
      
    default:
    break;
  }
  
  wifi_uart_write_frame(GET_IR_STATUS_CMD,length);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/*****************************************************************************
函数名称 : ir_tx_rx_test_result
功能描述 : 红外进入收发产测结果通知
输入参数 : result:模块是否成功接收到正确的信息;0:失败/1:成功
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void ir_tx_rx_test_result(unsigned char result)
{
  #error "请自行实现红外进入收发产测功能测试成功/失败代码,完成后请删除该行"
  if(result == 0)
  {
    //红外进入收发产测成功
  }
  else
  {
    //红外进入收发产测失败，请检查发出的数据包
  }
}
#endif





/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/



/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  当前函数处理可下发/可上报数据调用                    
  具体函数内需要实现下发数据处理
  完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_SWITCH:
      //开关处理函数
      ret = dp_download_switch_handle(value,length);
      break;
    case DPID_MODE:
      //工作模式处理函数
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_OPEN_WINDOW_SWITCH:
      ret = dp_download_openWindowSitch_handle(value,length);
      break;
    case DPID_TEMP_SET:
      //目标温度处理函数
      ret = dp_download_temp_set_handle(value,length);
      break;
    case DPID_UPPER_TEMP:
      //设置温度上限处理函数
      ret = dp_download_upper_temp_handle(value,length);
      break;
    case DPID_WORK_DAYS:
      //工作日设置处理函数
      ret = dp_download_work_days_handle(value,length);
      break;
    case DPID_WEEK_PROGRAM1:
      //周程序（自由式）处理函数   
      ret = dp_download_week_program1_handle(value,length);
      break;
    case DPID_CHILD_LOCK:
      //童锁开关处理函数
      ret = dp_download_child_lock_handle(value,length);
      break;
    case DPID_SENSOR_CHOOSE:
      //传感器选择处理函数
      ret = dp_download_sensor_choose_handle(value,length);
      break;
    case DPID_RESERVE:
      //保留处理函数
      ret = dp_download_reserve_handle(value,length);
      break;
    case DPID_REG_RAW:
      //寄存器透传处理函数
      ret = dp_download_reg_raw_handle(value,length);
      break;
    case DPID_RESERVE_CHAR:
      //保留_char处理函数
      ret = dp_download_reserve_char_handle(value,length);
      break;
    case DPID_FLOORTEMP:
      //地面温度处理函数
      ret = dp_download_floortemp_handle(value,length);
      break;
    case DPID_FLOORSENSERTYPE:
      //地面传感器类型处理函数
      ret = dp_download_floorsensertype_handle(value,length);
      break;
    case DPID_FLOORLIMITTEMP:
      //地面温度限值处理函数
      ret = dp_download_floorlimittemp_handle(value,length);
      break;
    case DPID_NOTFROZEN:
      //防冻状态处理函数
      ret = dp_download_notfrozen_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}

#ifdef WEATHER_ENABLE

/*****************************************************************************
函数名称 : weather_open_return
功能描述 : 打开天气功能返回用户自处理函数
输入参数 : 1:res:打开天气功能返回结果，1:成功；0：失败

返回参数 : 无
*****************************************************************************/
void weather_open_return_handle(unsigned char res, unsigned char err)
{
  //#error "请自行完成M打开天气功能返回数据处理代码,完成后请删除该行"
  unsigned char err_num = 0;
  
  if(res == 1)
  {
    //打开天气返回成功
  }
  else if(res == 0)
  {
    //打开天气返回失败
    err_num = err;//获取错误码
  }
}

/*****************************************************************************
函数名称 : weather_data_user_handle
功能描述 : 天气数据用户自处理函数
输入参数 : name:参数名
           type:参数类型，0：int型；1：string型
           data:参数值的地址
返回参数 : 无
*****************************************************************************/
void weather_data_user_handle(char *name, unsigned char type, char *data)
{
  //#error "这里仅给出示例，请自行完善天气数据处理代码,完成后请删除该行"
  int value_int;
  char value_string[50];//由于有的参数内容较多，这里默认为50。您可以根据定义的参数，可以适当减少该值
  
  my_memset(value_string, '/0', 50);
  
  //首先获取数据类型
  if(type == 0)//参数是INT型
  {
    value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
  }
  else if(type == 1)
  {
    my_strcpy(value_string, data);
  }
  
  //注意要根据所选参数类型来获得参数值！！！
  if(my_strcmp(name, "temp") == 0)
  {
    //printf("temp value is:%d", value_int);            //int型
  }
  else if(my_strcmp(name, "humidity") == 0)
  {
    //printf("humidity value is:%d", value_int);        //int型
  }
  else if(my_strcmp(name, "pm25") == 0)
  {
    //printf("pm25 value is:%d", value_int);            //int型
  }
  else if(my_strcmp(name, "condition") == 0)
  {
    //printf("condition value is:%s", value_string);    //string型
  }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/*****************************************************************************
函数名称 : stream_file_trans
功能描述 : 流服务文件发送
输入参数 : id:ID号
          buffer:发送包的地址
          buf_len:发送包长度
返回参数 : 无
*****************************************************************************/
unsigned char stream_file_trans(unsigned int id, unsigned char *buffer, unsigned long buf_len)
{
  //#error "这里仅给出示例，请自行完善流服务处理代码,完成后请删除该行"
  unsigned short length = 0;
  unsigned long map_offset = 0;
  unsigned int pack_num = 0;
  unsigned int rest_length = 0;

  if(stop_update_flag == ENABLE)
    return SUCCESS;

  pack_num = buf_len / STREM_PACK_LEN;
  rest_length = buf_len - pack_num * STREM_PACK_LEN;
  if (rest_length > 0)
  {
    pack_num++;
  }

  int this_len = STREM_PACK_LEN;
  for (int cnt = 0; cnt < pack_num; cnt++)
  {
    if (cnt == pack_num - 1 && rest_length > 0)
    {
      this_len = rest_length;
    }
    else
    {
      this_len = STREM_PACK_LEN;
    }

    if(SUCCESS == stream_trans(id, map_offset, buffer + map_offset, this_len))
    {
      //mcu正在升级中，不可以进行流服务传输
      //printf("is upgrade\n");
      return SUCCESS;
    }

    //while(stream_status == 0xff);//收到返回
    
    if(stream_status != 0)
    {
      return ERROR;
    }
  }
  
  return SUCCESS;
}

#endif
