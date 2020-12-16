//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_CONFIGURE_H
#define _APP_CONFIGURE_H
//-------------------------------------------------------------------------------------------------
#include "..\.\Module\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\.\Module\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\.\Module\Mod_Modbus\Src\Mod_ModbusSlave.h"
#include "..\.\Module\Mod_EEFlash\Src\Mod_FlashStoBlockOf8Bits.h"
#include "..\.\Module\Mod_LCD\Src\Mod_LCD.h"
#include "..\.\Module\Mod_NTC3380\Src\Mod_NTC3380.h"
#include "..\.\Module\Mod_Backlight\Src\Mod_Backlight.h"
#include "..\.\Module\Mod_Keyboard\Src\Mod_Keyboard.h"
#include "..\.\Module\Mod_Relay\Src\Mod_RelayPWM.h"
#include "..\.\Module\Mod_SD3078\Src\Mod_SD3078.h"
#include "..\.\Module\Mod_IWDG\Src\Mod_IWDG.h"


#include ".\APP_RTC.h"
#include ".\APP_Display.h"
#include ".\APP_KeyBoard.h"
#include ".\APP_Temperature.h"
#include ".\APP_Storage.h"
#include ".\APP_Backlight.h"
#include ".\APP_ThreeFanOut.h"
#include ".\APP_Rs485.h" 


#define  _DeviceType_    0x8109  //E51.33B_Rs485产品编码
//#define  _Modified_      0x1801 第一次测试程序
//#define  _Modified_      0x1802 修正RS485功能一个，无法恢复出厂设置
//#define  _Modified_      0x1803   //陈工发现波特率寄存器配置异常，原先先写BR1再写BR2修改为先写BR2在再写BR1，
//#define  _Modified_      0x1804   //书写问题，屏蔽了TIM2校准初始化。
//#define  _Modified_      0x1805     //1增加波特率19200和停止位选择
                                    //修改测温模块内外置逻辑错误
                                    //修改波特率修正值，原先1500修正修改为10000修正
//#define  _Modified_      0x1808     //将频率修正重启串口模块放在应答完成后
//#define  _Modified_        0x1909    //背光通电时间修正为1分钟
#define  _Modified_        0x2001      //背光通电时间修正为1分钟
                                    
                                    //修改波特率修正值，原先10000修正修改为16000修正
#define  FristInToTask   0 
//-------------------------------------------------------------------------------------------------
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
