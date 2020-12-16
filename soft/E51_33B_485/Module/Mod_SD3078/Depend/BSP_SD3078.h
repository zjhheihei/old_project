//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_SD3078_H
#define _BSP_SD3078_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef  Data_Retyped
#include ".\DataTypedef.h"
#endif

#define ACK                     0xff
#define NACK                    0X00
#define WriteMode               0x64
#define ReadMode                0x65

#define WriteEnable             0x00
#define ReadEnable              0xff

#define WriteEnableRegAdd_1     0x10
#define WriteEnableRegAdd_2_3   0x0f

#define WriteEnableCmd_1        0x80
#define WriteEnableCmd_2_3      0xff

#define WriteDisableCmd_1       0x00
#define WriteDisbaleCmd_2_3     0x7b

#define SD3078RTCAddress        0x00

#define BuffSecond              0
#define BuffMinnute             1
#define BuffHour                2

void BSP_I2C_IO_Configure(void);
void BSP_I2CStartSignal(void);
void BSP_I2CStopSignal(void);
BOOLEAN BSP_I2CWriteOneByte(INT8U _In_Date);
INT8U BSP_I2CReadOneByte(BOOLEAN _In_AckStatus);

//------------------------------E N D-------------------------------------------

#endif

//-----------------------BSP_SD3078.h--END------------------------------------
