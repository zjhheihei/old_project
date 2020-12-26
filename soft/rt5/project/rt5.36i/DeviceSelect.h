//------------------------------------------------------------------------------
#ifndef __DEVICETYPE_SLT__
#define __DEVICETYPE_SLT__
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

////////////////////////////////产品系列////////////////////////////////////////
//#define __RT1__
//#define __RT2__
//#define __RT3__
//#define __RT4__
#define __RT5__
//#define __RT81__
//#define __RT82__
//#define __RT83__
////////////////////////////////产品系列////////////////////////////////////////


////////////////////////////////产品子型号//////////////////////////////////////
//#define __RT1_13__
//#define __RT1_23i__
//#define __RT1_36t__
//#define __RT1_36i__
//
//#define __RT2_42__
//#define __RT2_42k__
//#define __RT2_42i__
//
//#define __RT3_13__
//#define __RT3_23__
//#define __RT3_36t__
//#define __RT3_36i__
//
//#define __RT4_42__
//#define __RT4_42k__
//#define __RT4_42i__
//
//#define __RT5_13__
//#define __RT5_23 __
//#define __RT5_36t__
#define __RT5_36i__
//
//#define __RT81_23__
//#define __RT81_23i__
//#define __RT81_36t__
//#define __RT81_36i__
//
//#define __RT82_42__
//#define __RT82_42i__
//#define __RT82_42k__
//
//#define __RT83_50k__
//#define __RT83_50i__
////////////////////////////////产品子型号//////////////////////////////////////


////////////////////////////////产品类型////////////////////////////////////////
#define  __HEATING__
//#define  __AIR__
//#define  __ALL_1__
////////////////////////////////产品类型////////////////////////////////////////


////////////////////////////////是否有外控//////////////////////////////////////
#if defined(__RT1_36t__) || defined(__RT1_36i__) ||\
defined(__RT3_36t__) || defined(__RT3_36i__) ||\
  defined(__RT81_36t__) || defined(__RT81_36i__) ||\
    defined(__RT5_36t__) || defined(__RT5_36i__) 
#define __OUT_SIDE_CONTROL__
#else
#if defined(__RT1_13__) || defined(__RT3_13__) || defined(__RT2_42__)|| defined(__RT5_13__)
#else
#define __LINK_CONTROL__     
#endif
#endif
////////////////////////////////是否有外控//////////////////////////////////////


       
    
      
////////////////////////////////是否有wifi//////////////////////////////////////
#if defined(__RT1_23i__) || defined(__RT1_36i__) || defined(__RT2_42i__) ||\
      defined(__RT3_23i__) || defined(__RT3_36i__) || defined(__RT4_42i__) ||\
        defined(__RT5_23i__) || defined(__RT5_36i__) || defined(__RT81_23i__) ||\
          defined(__RT81_36i__) || defined(__RT82_42i__) || defined(__RT83_50i__)      
#define __WIFI_CONTROL__      
#endif
////////////////////////////////是否有wifi////////////////////////////////////// 
            
            
      
#if defined(__RT81_36i__) || defined(__RT81_23i__) || defined(__RT82_42i__) || defined(__RT83_50i__)
#define __RS485_CONTROL__  
#endif


            
////////////////////////////////是否有485//////////////////////////////////////
#if defined(__RT81__) || defined(__RT82__) || defined(__RT83__) || defined(__RT1__) || defined(__RT3__) 
#define __LTP_CONTROL__  
#endif   
             
            
#if defined(__RT5__)
#define WEEK_PRG_LENGTH        6
#else
#define WEEK_PRG_LENGTH        8
#endif


////////////////////////////////芯片平台////////////////////////////////////////
#define __STM8L052R8__
//#define __HDSC32L136__
#define ST_MCU                 0X00000000
#define HDSC_MCU               0X00000001

#ifdef __STM8L052R8__
#define __MCU_TYEP__           ST_MCU
#elif defined (__HDSC32L136__)
#define __MCU_TYEP__           HDSC_MCU
#endif
////////////////////////////////芯片平台////////////////////////////////////////

#if defined (__RT81_23i__)
#define __DEVICE_485__     0X09A5
#elif defined (__RT81_36i__)            
#define __DEVICE_485__     0X09A6
#elif defined (__RT82_42i__) 
#define __DEVICE_485__     0X09A7
#elif defined (__RT83_50i__) 
#define __DEVICE_485__     0X09A8
#endif

            
#endif
//+++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++
