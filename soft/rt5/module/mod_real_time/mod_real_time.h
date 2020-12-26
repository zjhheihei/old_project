//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_REAL_TIME_H
#define _MOD_REAL_TIME_H
//------------------------------E N D-------------------------------------------
#include <stdbool.h>
#include <stdint.h>


#define   __MCU_8_BIT__
//#defined  __MCU_32_BIT__
typedef union
{
    struct
    {
        #if defined (__MCU_8_BIT__)
        uint8_t second;
        uint8_t minute; 
        uint8_t hour;
        uint8_t week;
        uint8_t day;
        uint8_t month;
        uint8_t year;
        #elif defined (__MCU_32_BIT__)
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t week;
        uint8_t hour;
        uint8_t minute; 
        uint8_t second;        
        #endif
    };
    uint8_t realRtcBuf[7];
}local_rtc_t;

#if defined (__MCU_8_BIT__)
#define SECOND_ADDR   0
#define MINUTE_ADDR   1
#define HOUR_ADDR     2
#define WEEK_ADDR     3
#endif


bool mod_real_time_write(void);
bool mod_real_time_read(void);
void mod_real_time_configure(uint8_t *ptRtc);
#endif


//-----------------------Mod_Keyboard.h--END------------------------------------
