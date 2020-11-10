//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\SnailDataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void ClearWatchDog(void)
{
    IWDG_KR = 0xAA; 
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++