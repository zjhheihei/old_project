//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_UARTFULL_H
#define _BSP_UARTFULL_H
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//Task
//------------------------------------------------------------------------------
//Event
//------------------------------------------------------------------------------
//Status
//------------------------------------------------------------------------------
//Function
//名称: 读取接收队列一个字节的数据
//功能: 
//入口: 接收指针
//
//出口: BN_TRUE 有数据

BOOLEAN bsp_read_uartRxdQueue_byte(INT8U* out_byte);
//------------------------------------------------------------------------------
//名称: 写入发送队列一个字节的数据
//功能: 
//入口: 发送数据
//
//出口: BN_TRUE 写入成功,BN_FALSE 缓冲区满,需要等待

BOOLEAN bsp_write_uartTxdQueue_byte(INT8U in_byte);
//------------------------------------------------------------------------------
void wifi_usart_configure(void);
//------------------------------------------------------------------------------
void Recfg_Buadrate_wifi_usart(INT32U ClockFreqValue);
//------------------------------------------------------------------------------
void enable_wifi_reset(void);
//------------------------------------------------------------------------------
void disable_wifi_reset(void);
//------------------------------------------------------------------------------
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++