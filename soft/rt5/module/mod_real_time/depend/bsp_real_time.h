//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_REAL_TIME_H
#define _BSP_REAL_TIME_H
//------------------------------E N D-------------------------------------------
#include <stdbool.h>
#include <stdint.h>
#include "iostm8l052r8.h"
#define RTC_ADDRESS    RTC_TR1

bool bsp_real_time_configure(uint8_t *rtcBuf);
bool bsp_real_time_read_enable(void);
#endif

//-----------------------BSP_Keyboard.h--END------------------------------------
