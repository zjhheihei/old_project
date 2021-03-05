//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_CONFIGURE_H
#define _APP_CONFIGURE_H
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include ".\data_typedef.h"
#include <stdint.h>
#include <stdbool.h>
#include "DeviceSelect.h"
//-------------------------------------------------------------------------------------------------
#include "intrinsics.h"
//-------------------------------------------------------------------------------------------------
#include "..\.\Module\mod_sysClock\mod_sysClock.h"
#include "..\.\Module\mod_systemTick\mod_systemTick.h"
#include "..\.\Module\mod_storage\mod_storage.h"

#include "..\.\Module\mod_watchdog\mod_watchdog.h"
#include "..\.\Module\mod_backlight\mod_backlight.h"
#include "..\.\Module\mod_lcd_ls3\mod_lcd_ls3.h"
#include "..\.\Module\mod_key\mod_key.h"
#include "..\.\Module\mod_ntc3380\mod_ntc3380.h"
#include "..\.\Module\mod_relay\mod_relay.h"

 
#include "..\.\Module\mod_uartFull\mod_uartFull.h"
//-------------------------------------------------------------------------------------------------

#include "..\.\application\app_tuya_sdk\tuya_protocol.h"
#include "..\.\application\app_tuya_sdk\tuya_mcu_api.h"
#include "..\.\application\app_tuya_sdk\tuya_wifi.h"
#include "..\.\application\app_tuyaInterface.h"
#include "..\.\application\app_updateFromWIFI.h"
#include "..\.\application\app_wifi.h"
#include "..\.\application\app_backlight.h"
#include "..\.\application\app_control.h"
#include "..\.\application\app_outputLogic.h"
#include "..\.\application\app_storage.h"
#include "..\.\application\app_relay.h"
#include "..\.\application\app_display.h" 
#include "..\.\application\app_key.h"
#include "..\.\application\app_timming.h"
#include "..\.\application\app_interval.h"
#include "..\.\application\app_softRtc.h"
#include "app_preheating.h"
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------

#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
