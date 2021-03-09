//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\data_typedef.h"
#include ".\depend\bsp_uartFull.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN mod_read_uartRxdQueue_byte(INT8U* out_byte)
{
    return(bsp_read_uartRxdQueue_byte(out_byte));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mod_uartFull_cfg(void)
{
    wifi_usart_configure();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN mod_write_uartTxdQueue_byte(INT8U in_byte)
{
    return(bsp_write_uartTxdQueue_byte(in_byte));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void recfg_buadrate_uart(INT32U in_clockFreqValue)
{
    Recfg_Buadrate_wifi_usart(in_clockFreqValue);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mod_enable_wifi_reset(void)
{
    enable_wifi_reset();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mod_disable_wifi_reset(void)
{
    disable_wifi_reset();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++