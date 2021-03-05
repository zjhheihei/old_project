#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
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
void Recfg_Buadrate_wifi_usart(INT32U ClockFreqValue)
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
void wifi_usart_configure(void)
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
    Recfg_Buadrate_wifi_usart(16000000);
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

#elif defined(__HDSC32L136__)
#include "gpio.h"
#include "uart.h"
#include "stdint.h"
#include <stdbool.h>

#define __USE_UARTCH0__

#ifdef __USE_UARTCH0__
#define WIFI_USART            UARTCH0
#define USART_SYS_CLOCK       SysctrlPeripheralUart0
#define USART_PORT            GpioPortB
#define TX_PIN                GpioPin8
#define RX_PIN                GpioPin9
#define TX_REG_DR             M0P_UART0->SBUF
#define WIFI_USAR             M0P_UART0
#define BAUD_9600             9600 
#define wifi_IRQHandler       UART0_IRQHandler
#define wifi_IRQ              UART0_IRQn
#endif

#define WIFI_RESET_POART   GpioPortB
#define WIFI_RESET_PIN     GpioPin7
  
void macro_reset_cfg(void)
{
    stc_gpio_config_t wifiCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    wifiCfg.enDir = GpioDirOut;
    wifiCfg.enDrv = GpioDrvH;
    wifiCfg.enPuPd = GpioNoPuPd;
    wifiCfg.enOD = GpioOdDisable;
    wifiCfg.enCtrlMode = GpioAHB;
    Gpio_Init(WIFI_RESET_POART,WIFI_RESET_PIN,&wifiCfg);
    
}

void enable_wifi_reset(void)
{
    Gpio_SetIO(WIFI_RESET_POART,WIFI_RESET_PIN);
}

void disable_wifi_reset(void)
{
    Gpio_ClrIO(WIFI_RESET_POART,WIFI_RESET_PIN); 
}

void Recfg_Buadrate_wifi_usart(INT32U ClockFreqValue)
{
    
}


void bsp_wifi_usart_hardwareConfigure(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stc_gpio_config_t usartGpioCfg;
    DDL_ZERO_STRUCT(usartGpioCfg);
    usartGpioCfg.enDir = GpioDirOut;
    Gpio_Init(USART_PORT,TX_PIN,&usartGpioCfg);
    Gpio_SetAfMode(USART_PORT,TX_PIN,GpioAf7);
    usartGpioCfg.enDir = GpioDirIn;
    Gpio_Init(USART_PORT,RX_PIN,&usartGpioCfg);
    Gpio_SetAfMode(USART_PORT,RX_PIN,GpioAf7);
}


void bsp_wifi_usart_set_baud(uint16_t runBaud,uint8_t mode,uint32_t sysClock)
{   
    static stc_uart_baud_t stcBaud; 
    uint16_t getBaud = 0;
    DDL_ZERO_STRUCT(stcBaud);    
    Uart_SetClkDiv(WIFI_USART,Uart8Or16Div);//采样分频
    //stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.u32Pclk   = sysClock;
    stcBaud.enRunMode = (en_uart_mode_t)mode;
    stcBaud.u32Baud   = runBaud;
    getBaud = Uart_CalScnt(WIFI_USART,&stcBaud);//波特率计算
    Uart_SetBaud(WIFI_USART,getBaud);//波特率设置
}

void bsp_wifi_usart_softWareConfigure(void)
{    
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    
    Sysctrl_SetPeripheralGate(USART_SYS_CLOCK,TRUE);//使能uart1模块时钟  
    stcConfig.bTouchNvic = TRUE;
  
    if(TRUE == stcConfig.bTouchNvic)
    {
        EnableNvic(wifi_IRQ,IrqLevel3,TRUE);
    }
    stcConfig.enRunMode = UartMode1;             //模式1
    stcConfig.enStopBit = Uart1bit;              //1bit停止位

    stcMulti.enMulti_mode = UartNormal;          //正常工作模式
    Uart_SetMultiMode(WIFI_USART,&stcMulti);     //多主机单独配  
    Uart_Init(WIFI_USART, &stcConfig);           //串口初始化   
    bsp_wifi_usart_set_baud(BAUD_9600,(uint8_t)(stcConfig.enRunMode),Sysctrl_GetPClkFreq());
    
    Uart_ClrStatus(WIFI_USART,UartRC);            //清接收请求
    Uart_EnableIrq(WIFI_USART,UartRxIrq);         //使能串口中断  
    Uart_EnableFunc(WIFI_USART,UartRx);           //使能收发
}

void bsp_wifi_usart_tx_interrupt_enable(BOOLEAN newStatus)
{
    if(BN_TRUE == newStatus)
    {
        Uart_EnableIrq(WIFI_USART,UartTxEIrq);
    }
    else
    {
        Uart_DisableIrq(WIFI_USART,UartTxEIrq);
    }
}


void wifi_usart_configure(void)
{
    bsp_wifi_usart_hardwareConfigure();
    bsp_wifi_usart_softWareConfigure();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define rxd_buff_size 32
static volatile uint8_t read_buffer[rxd_buff_size];
static volatile uint8_t rbf_in = 0;
static uint8_t rbf_out = 0;
//-------------------------------------------------------------------------------------------------
void bsp_rx_isr(void)
{
    uint8_t read_reg;   
    read_reg = Uart_ReceiveData(WIFI_USART);         
    read_buffer[rbf_in] = read_reg;
    rbf_in ++;
    if(rbf_in > (rxd_buff_size-1))
    {
        rbf_in = 0;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//read one byte from uart rxd queue
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN bsp_read_uartRxdQueue_byte(uint8_t* out_byte)
{
    uint8_t read_rbf_in;
    
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
static volatile uint8_t write_buffer[txd_buff_size];
static volatile uint8_t wbf_out = 0;
static volatile uint8_t wbf_in = 0;
//-------------------------------------------------------------------------------------------------
void bsp_tx_isr(void)
{
    uint8_t read_wbf_in; 

     read_wbf_in = wbf_in;
     if(read_wbf_in != wbf_out)
     {
        TX_REG_DR = write_buffer[wbf_out];
        wbf_out ++;
        if(wbf_out > (txd_buff_size-1))
        {
            wbf_out = 0;
        }
     }
     else
     {                         
        bsp_wifi_usart_tx_interrupt_enable(BN_FALSE);
     }
   
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN bsp_write_uartTxdQueue_byte(uint8_t in_byte)
{
    uint8_t read_wbf_out;
    uint8_t pre_wbf_in;
    
    read_wbf_out = wbf_out;
    pre_wbf_in = wbf_in;
    pre_wbf_in ++;
    if(pre_wbf_in > (txd_buff_size - 1))
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
   
    if(0 == WIFI_USAR->SCON_f.TXEIE)
    {
        bsp_wifi_usart_tx_interrupt_enable(BN_TRUE);
    }
    return(BN_TRUE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++






void wifi_IRQHandler(void)
{
    if(Uart_GetStatus(WIFI_USART,UartTxe))
    {
        bsp_tx_isr();
    }
    
    if(Uart_GetStatus(WIFI_USART,UartRC))
    {
        Uart_ClrStatus(WIFI_USART,UartRC);
        bsp_rx_isr();
    }     
}


#endif