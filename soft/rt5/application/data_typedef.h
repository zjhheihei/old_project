//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//用户数据类型重定义
//-------------------------------------------------------------------
#ifndef  data_retyped
#define  data_retyped

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数据重定义
typedef unsigned char        BOOLEAN;                  /*布尔量                         */
typedef          char        INTSTRING;                //字符串类型 
typedef unsigned char        INT8U;                    /* Unsigned  8 bit quantity      */
typedef signed   char        INT8S;                    /* Signed    8 bit quantity      */
typedef unsigned short       INT16U;                   /* Unsigned 16 bit quantity      */
typedef signed   short       INT16S;                   /* Signed   16 bit quantity      */
typedef unsigned long        INT32U;                   /* Unsigned 32 bit quantity      */
typedef signed   long        INT32S;                   /* Signed   32 bit quantity      */
typedef unsigned long long   INT64U;                   /* Unsigned 64 bit quantity      */
typedef signed   long long   INT64S;                   /* Signed   64 bit quantity      */
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define BN_TRUE    0x01                                /*布尔运算真值*/
#define BN_FALSE   0x00                                /*布尔运算假值*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//位运算
#define _Set_Bit(sram,bits)   (sram|=bits)
#define _Clr_Bit(sram,bits)   (sram&=~bits)
#define _Get_Bit(sram,bits)   (sram&bits)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//位定义
#define  T_BIT0     0x0001
#define  T_BIT1     0x0002
#define  T_BIT2     0x0004
#define  T_BIT3     0x0008
#define  T_BIT4     0x0010
#define  T_BIT5     0x0020
#define  T_BIT6     0x0040
#define  T_BIT7     0x0080
#define  T_BIT8     0x0100
#define  T_BIT9     0x0200
#define  T_BIT10    0x0400
#define  T_BIT11    0x0800
#define  T_BIT12    0x1000
#define  T_BIT13    0x2000
#define  T_BIT14    0x4000
#define  T_BIT15    0x8000
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//文本定义
#define _CR_       0x0D  //  \r 回车
#define _LF_       0x0A  //  \n 换行
#define _CTRL_Z_   0x1A  //  CTRL+Z
#define _TLeft_    0x3E  //  \>
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif	
//++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++

