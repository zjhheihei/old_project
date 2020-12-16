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


#define  _DeviceType_    0x8109  //E51.33B_Rs485��Ʒ����
//#define  _Modified_      0x1801 ��һ�β��Գ���
//#define  _Modified_      0x1802 ����RS485����һ�����޷��ָ���������
//#define  _Modified_      0x1803   //�¹����ֲ����ʼĴ��������쳣��ԭ����дBR1��дBR2�޸�Ϊ��дBR2����дBR1��
//#define  _Modified_      0x1804   //��д���⣬������TIM2У׼��ʼ����
//#define  _Modified_      0x1805     //1���Ӳ�����19200��ֹͣλѡ��
                                    //�޸Ĳ���ģ���������߼�����
                                    //�޸Ĳ���������ֵ��ԭ��1500�����޸�Ϊ10000����
//#define  _Modified_      0x1808     //��Ƶ��������������ģ�����Ӧ����ɺ�
//#define  _Modified_        0x1909    //����ͨ��ʱ������Ϊ1����
#define  _Modified_        0x2001      //����ͨ��ʱ������Ϊ1����
                                    
                                    //�޸Ĳ���������ֵ��ԭ��10000�����޸�Ϊ16000����
#define  FristInToTask   0 
//-------------------------------------------------------------------------------------------------
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
