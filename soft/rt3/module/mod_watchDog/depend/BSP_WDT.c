#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"

void Watchdog_Configure(void)
{
    CLK_ICKCR_bit.LSION = 1;  //LSI = 38K
    CLK_CKDIVR_bit.CKM = 1;  
    
    IWDG_KR = 0xCC;   //Enable IWDG KEY_ENABLE
    
    IWDG_KR = 0x55;   //KEY_ACCESS
    IWDG_PR = 0x06;   //0000 0110  Prescaler = 256;
    
    IWDG_RLR = 200;  //T=38k/256/149  1.3s
    
    IWDG_KR = 0xAA;  //Refreshes  EKY_REFRESH
    while(0x01 != CLK_SCSR);
}

void ClearWatchDog(void)
{
    IWDG_KR = 0xAA; 
}

#elif defined(__HDSC32L136__)

#include "wdt.h"


static void WdtCallback(void)
{
  ;
}
void Watchdog_Configure(void)
{
    stc_wdt_config_t  stcWdt_Config;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
    DDL_ZERO_STRUCT(stcWdt_Config);
    stcWdt_Config.u8LoadValue = 0x0b;
    stcWdt_Config.enResetEnable = WRESET_EN;//WINT_EN;//WRESET_EN;////
    stcWdt_Config.pfnWdtIrqCb = WdtCallback;
    Wdt_Init(&stcWdt_Config);
    Wdt_Start();
}

void ClearWatchDog(void)
{
    Wdt_Feed(); 
}

#endif