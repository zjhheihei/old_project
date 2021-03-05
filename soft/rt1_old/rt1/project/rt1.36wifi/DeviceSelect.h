//------------------------------------------------------------------------------
#ifndef __DEVICETYPE_SLT__
#define __DEVICETYPE_SLT__
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define __RT1_36WIFI__
#define __STM8L052R8__
#define __SOFT_VERSION__       0x0101

#define ST_MCU                 0X00000000
#define HDSC_MCU               0X00000001

#ifdef __STM8L052R8__
#define __MCU_TYEP__           ST_MCU
#elif defined (__HDSC32L136__)
#define __MCU_TYEP__           HDSC_MCU
#endif

#endif
//+++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++
