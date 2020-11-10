//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
#include ".\data_typedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//PHY use uart3 PG0--U3RX  PG1--U3TX
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\public\pbc_ringQueue\pbc_ringQueue.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//PHY use uart3 PG0--U3RX  PG1--U3TX
//wifi reset PG2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//input floating
#define macro_reset_cfg      PG_ODR |= (0x04); PG_CR1 &= (~0x04); PG_CR2 &= (~0x04); PG_DDR |= (0x04)
#define macro_reset_enable   PG_ODR &= (~0x04);
#define macro_reset_disable  PG_ODR |= (0x04);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
void enable_wifi_reset(void)
{
    macro_reset_enable;
}
//-------------------------------------------------------------------------------------------------
void disable_wifi_reset(void)
{
    macro_reset_disable;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Recfg_Buadrate_USART3(INT32U ClockFreqValue)
{
    static INT32U ClkFreBakup;
    INT32U DifferentFre;
    INT32U BaudRate_Mantissa;
    
    if(ClkFreBakup > ClockFreqValue)
    {
        DifferentFre = ClkFreBakup-ClockFreqValue;
    }
    else
    {
        DifferentFre = ClockFreqValue-ClkFreBakup;
    }
    if(DifferentFre>8000)  //0.05% error
    {
        ClkFreBakup=ClockFreqValue;

        USART3_CR1_bit.USARTD = 1;  //Disable USART3
        BaudRate_Mantissa  = ClockFreqValue/9600;
        /* Set the fraction of USARTDIV */
         
        USART3_BRR2 = (INT8U)((BaudRate_Mantissa >> (INT8U)8) & (INT8U)0xF0);
        /* Set the MSB mantissa of USARTDIV */
        USART3_BRR2 |= (INT8U)(BaudRate_Mantissa & (INT8U)0x0F);
        /* Set the LSB mantissa of USARTDIV */
        USART3_BRR1 = (INT8U)(BaudRate_Mantissa >> (INT8U)4);  
        USART3_CR1_bit.USARTD = 0; //Enable USART3
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//UART3口的配置
//PG0--U3RX  PG1--U3TX
//波特率:9600,无校验
//-------------------------------------------------------------------------------------------------
void Uart3_Configure(void)
{
    CLK_PCKENR3_bit.PCKEN34 = 1;
    PG_DDR &= (~0x01); PG_CR1 |= (0x01); PG_CR2 &= (~0x01); PG_ODR &= (~0x01);  //RXD
    PG_DDR |= (0x02);  PG_CR1 |= (0x02); PG_CR2 &= (~0x02); PG_ODR |= (0x02);   //TXD
    macro_reset_cfg;
    
    
    USART3_CR1 = 0x00;   //0000 0000 8Data_Bits Parity_None
    USART3_CR2 = 0x0C;   //0000 1100 TEN REN
    USART3_CR3 = 0x00;   //0000 0000 1STOP_Bit
    USART3_CR4 = 0x00;
    USART3_CR5 = 0x00;
    USART3_GTR = 0x00;
    USART3_PSCR = 0x00;
    Recfg_Buadrate_USART3(16000000);
    USART3_CR2_bit.RIEN = 1;  //Receiver interrupt Enable
//-------------------------------------------------------------------------------------------------
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define rxd_buff_size 32
static volatile INT8U read_buffer[rxd_buff_size];
static volatile INT8U rbf_in = 0;
static INT8U rbf_out = 0;
//-------------------------------------------------------------------------------------------------
#pragma vector = USART3_R_RXNE_vector
__interrupt void ISR_Uart3_Rxd(void)
{
    INT8U read_reg;

    if(USART3_SR_bit.PE)  //过滤掉校验错误的数据
    {
        read_reg=USART3_DR;
    }
    else
    {
        read_reg=USART3_DR; 
        
        read_buffer[rbf_in] = read_reg;
        rbf_in ++;
        if(rbf_in > (rxd_buff_size-1))
        {
            rbf_in = 0;
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//read one byte from uart rxd queue
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN bsp_read_uartRxdQueue_byte(INT8U* out_byte)
{
    INT8U read_rbf_in;
    
    read_rbf_in = rbf_in;
    
    if(read_rbf_in != rbf_out)
    {
        *out_byte = read_buffer[rbf_out];
        rbf_out ++;
        if(rbf_out > (rxd_buff_size-1))
        {
            rbf_out = 0;
        }
        return(BN_TRUE);
    }
    else
    {
        return(BN_FALSE);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define txd_buff_size 64
static volatile INT8U write_buffer[txd_buff_size];
static volatile INT8U wbf_out = 0;
static volatile INT8U wbf_in = 0;
//-------------------------------------------------------------------------------------------------
#pragma vector = USART3_T_TXE_vector//USART3_T_TC_vector
__interrupt void ISR_Uart3_Txd(void)
{
    INT8U read_wbf_in;    
    
    if(USART3_CR2_bit.TIEN)
    {
        if(USART3_SR_bit.TXE) 
        {
            read_wbf_in = wbf_in;
            if(read_wbf_in != wbf_out)
            {
                USART3_DR = write_buffer[wbf_out];
                wbf_out ++;
                if(wbf_out > (txd_buff_size-1))
                {
                    wbf_out = 0;
                }
            }
            else
            {
                USART3_CR2_bit.TIEN=0;   //禁用发生中断
            }
        }        
    }
    else if(USART3_CR2_bit.TCIEN)
    {
        if(USART3_SR_bit.TC)
        {
            USART3_CR2_bit.TCIEN = 0;  //发送完毕
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN bsp_write_uartTxdQueue_byte(INT8U in_byte)
{
    INT8U read_wbf_out;
    INT8U pre_wbf_in;
    
    read_wbf_out = wbf_out;
    pre_wbf_in = wbf_in;
    pre_wbf_in ++;
    if(pre_wbf_in > (txd_buff_size-1))
    {
        pre_wbf_in = 0;
    }
    if(pre_wbf_in == read_wbf_out) //buff is full, program wait
    {
        return(BN_FALSE);
    }
//--------------------------------------------------------------------------------------------------
    pre_wbf_in = wbf_in;
    write_buffer[pre_wbf_in] = in_byte;
    pre_wbf_in ++;
    if(pre_wbf_in > (txd_buff_size-1))
    {
        pre_wbf_in = 0;
    }
    wbf_in = pre_wbf_in;
    if(0 == USART3_CR2_bit.TIEN)
    {
        USART3_CR2_bit.TIEN = 1;
    }
    return(BN_TRUE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++