//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\BSP_CLOCK.h"
#include ".\mod_sysClock.h"



BOOLEAN Poll_LSEError(void)
{
    return(LSE_Error);
}



INT32U Poll_HSIFrequency(void)
{
    return(HSIFrequency);
}



void HSI_Check_Configure(BOOLEAN _In_NewStatus)
{
     //TIM2_Configure(_In_NewStatus);
}


void HSICalibration_Task(void)
{
    bsp_hsi_adj_scanTask();
}

void mod_clock_configure(void)
{
    BSP_Clock_Configure();
}
