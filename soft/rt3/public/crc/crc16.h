//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _CRC_16_H
#define _CRC_16_H
//-----------------------------------------------------------------------------


void crc16Make_small(INT8U *ptr,INT16U len,INT8U *ret) ;
BOOLEAN crc16Check_small(INT8U *dat,INT8U len);
void crc16Make_big(INT8U *ptr,INT16U len,INT8U *ret);
BOOLEAN crc16Check_big(INT8U *dat,INT8U len);
#endif
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++