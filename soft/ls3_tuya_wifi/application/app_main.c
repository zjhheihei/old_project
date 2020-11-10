//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\APP_Cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    #ifdef NDEBUG
    //mod_watchdog_cfg(); //cfg 由boot区代码完成
    mod_watchdog_reload();
    #endif
    mod_clock_configure();
    mod_systick_configure();
    app_sto_eeprom_configure();
    __enable_interrupt(); 
    app_control_init();
    while(1)
    {
        #ifdef NDEBUG
        mod_watchdog_reload();
        #endif
        HSICalibration_Task();
        systickTimeRun_task();
        app_updateViaWIFI_task();
        app_tuyaInterface_task();
        app_backlight_scanTask();
        app_control_scanTask();
        app_outputLogic_scanTask();
        app_sto_eeprom_scanTask();
        app_interval_scanTask();
        app_softRtc_scanTask();
        app_display_scanTask();
        app_key_scanTask();
        app_relayOut_scanTask();
        app_wifi_scanTask();
        app_preheat_scanTask();
        app_openWindow_scanTask();
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++