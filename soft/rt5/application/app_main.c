#include ".\APP_Cfg.h"



void main(void)
{
    #ifdef NDEBUG
    #ifndef __WIFI_CONTROL__
    mod_watchdog_cfg();       
    #endif 
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
        systickTimeRun_task();          
        app_backlight_scanTask();
        app_control_scanTask();
        app_outputLogic_scanTask();
        app_sto_eeprom_scanTask();                  
        app_key_scanTask();
        app_display_scanTask();
        app_relayOut_scanTask();                                
        app_rtc_task();
        app_openWindow_task();
        app_interval_task();  
        app_preheat_task();
        #ifdef __WIFI_CONTROL__  
        HSICalibration_Task();
        app_wifi_scanTask();
        app_updateViaWIFI_task();
        app_tuyaInterface_task();            
        #endif
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++