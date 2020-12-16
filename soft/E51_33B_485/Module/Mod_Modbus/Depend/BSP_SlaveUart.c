/*******************************************************************************
*
*	模块名称 : Modbus
*	文件名称 : BSP_SlaveUart.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_SlaveUart调用
                   3、配置USAR3用于RS485通讯。配置TIM4用于T3_5检测
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-14  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*            
*******************************************************************************/
                    

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"     
#include "intrinsics.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#define EnableTxInterrupt()   USART3_CR2_TIEN = 1  //Tx中断
#define DisableTxInterrupt()  USART3_CR2_TIEN = 0
//#define EnableTxInterrupt()   USART3_CR2_TCIEN = 1  //Tx中断
//#define DisableTxInterrupt()  USART3_CR2_TCIEN = 0

#define EnbaleRxInterrupt()   USART3_CR2_RIEN  = 1  //Rx中断
#define DisbaleRxInterrupt()  USART3_CR2_RIEN  = 0 


#define MaxLenght               255  //缓冲区最大256BYTE
#define T35BaudRate19200        8
#define T35BaudRate9600         9   // 4mS  3种波特率对应的T35时间
#define T35BaudRate4800         10  // 8ms
#define T35BaudRate2400         11  // 12ms


#define NoParity            0x00
#define EvenParity          0x01
#define OddParity           0x02
 

#define TX_Pin_ODR          PG_ODR_ODR1  //Tx_IO
#define TX_Pin_DDR          PG_DDR_DDR1
#define TX_Pin_CR1          PG_CR1_C11
#define TX_Pin_CR2          PG_CR2_C21



#define RX_Pin_DDR          PG_DDR_DDR0  //Rx_IO
#define RX_Pin_CR1          PG_CR1_C10
#define RX_Pin_CR2          PG_CR2_C20

#define RS485DDR_Pin_ODR    PG_ODR_ODR2  //RS485方向_IO
#define RS485DDR_Pin_DDR    PG_DDR_DDR2
#define RS485DDR_Pin_CR1    PG_CR1_C12
#define RS485DDR_Pin_CR2    PG_CR2_C22

#define RS485DDR_Rx()       RS485DDR_Pin_ODR = 0 //Rx
#define RS485DDR_Tx()       RS485DDR_Pin_ODR = 1 //Tx
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************Rx状态定义*****************************************
typedef enum
{
    Rx_WaitFirstByte  =0x00,
    Rx_Receiving      =0x01,
}ModbusRxStatus_Def;                  //半双工串口状态
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************TIM4中断状态定义*************************************
typedef enum
{
    FirstInToISR   =0x00,
    NormalInToISR  =0x01,
}TIM4ISRStatus_Def;                  //半双工串口状态
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************接收和发送完成定义************************************
typedef enum
{
    NoneFinish  = 0x00,
    TxFinish    = 0x01,
    RxFinish    = 0x02,
}EventCentre_Def;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************BSP层参数定义****************************************
typedef struct
{
    INT8U                  Buff[MaxLenght];  //发送接收缓冲区      
    INT8U                  ModbusT3_5;       //T3-5时间
    EventCentre_Def        EventCentre;      //事件中心
    ModbusRxStatus_Def     RxStatus;         //接收状态
}BSP_ModbusParameter_Def;

static BSP_ModbusParameter_Def BSP_ModbusParameter; //仅供本模块自己使用
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************开启RS485发送功能和中断，关闭接收功能和中断*****************
void BSP_StartTx(void)
{
      RS485DDR_Tx();
      DisbaleRxInterrupt();      
      EnableTxInterrupt();
      
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************开启RS485接收功能和中断，关闭发送功能和中断*******************
void BSP_StartRx(void)
{      
      DisableTxInterrupt();
      EnbaleRxInterrupt();
      RS485DDR_Rx();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************RS485管脚配置***************************************
void BSP_Usar3_RS485PinConfigure(void)
{    
    TX_Pin_DDR = 1;  //OD输出
    TX_Pin_CR1 = 1;  
    TX_Pin_CR2 = 0;  
    TX_Pin_ODR = 1;
    
    RX_Pin_DDR = 0; //悬浮输入
    RX_Pin_CR1 = 1;  
    RX_Pin_CR2 = 0;
                 
    RS485DDR_Pin_DDR = 1;  //推挽输出，RS485通信方向默认收 
    RS485DDR_Pin_CR1 = 1;  
    RS485DDR_Pin_CR2 = 0;
    //RS485DDR_Rx();     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************TIM4开启关闭****************************************
void BSP_TIM4_Cmd(BOOLEAN _In_NewStatus)
{   
    if(_In_NewStatus)
    {
     
        TIM4_SR1_bit.UIF = 0;
        TIM4_IER_bit.UIE = 1;
        TIM4_CR1_bit.CEN = 1;
    }
    else
    {
        TIM4_CR1_bit.CEN=0;
        TIM4_IER_bit.UIE=0;   
        TIM4_CNTR = 0;
        TIM4_SR1_bit.UIF = 0;
          
    }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************TIM4配置******************************************
void BSP_TIM4_Configure(void)
{
    CLK_PCKENR1_bit.PCKEN12 = 1;   
    
    TIM4_IER_bit.UIE = 0;
    TIM4_SR1_bit.UIF = 0; 
    
    TIM4_PSCR = BSP_ModbusParameter.ModbusT3_5;
    TIM4_EGR_UG = 1;
    TIM4_SR1_bit.UIF = 0;
    TIM4_ARR = 125;  
    
    TIM4_SR1_bit.UIF = 0;
}
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*******************波特率和主频传入计算串口配置********************************
INT16U BSP_Usar3_BaudrateAmendment(INT16U _In_Baudrate,INT32U _In_CLK)
{
       
       INT16U  BaudrateHigh = 0;
       INT16U  BaudrateLow  = 0;  
       BaudrateHigh = _In_CLK/_In_Baudrate;
       BaudrateLow  = _In_CLK%_In_Baudrate;
       if(BaudrateLow>(_In_Baudrate/2))
       {
           BaudrateHigh += 1; 
       }
       
       return   BaudrateHigh;    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************串口3和TIM4配置**************************************
void BSP_Usart3_Configure(INT16U _In_Baudrate,INT8U _In_ParitySelect,INT32U _In_CLK,INT8U _In_StopBits)//重启Usart3
{    
      INT16U BaudrateDate = 0;
      
      CLK_PCKENR3_bit.PCKEN34 = 1;               //开Usart3时钟
      CLK_PCKENR1_bit.PCKEN12 = 1;               //开TIM4时钟
      USART3_SR;
      USART3_DR;
      USART3_CR1_USARTD = 1;  
      if(NoParity == _In_ParitySelect)         //无校验
      {   
           USART3_CR1_M = 0; 
           USART3_CR1_PCEN = 0;
           USART3_CR1_PIEN = 0;
      }
      else if(EvenParity == _In_ParitySelect)  //偶校验
      {
          USART3_CR1_M = 1; 
          USART3_CR1_PCEN  = 1;
          USART3_CR1_PS = 0;
          USART3_CR1_PIEN = 1;
      }
      else if(OddParity == _In_ParitySelect)  //奇校验
      {
          USART3_CR1_M = 1; 
          USART3_CR1_PCEN  = 1;
          USART3_CR1_PS = 1;
          USART3_CR1_PIEN = 1;
      }
      else
      {
            USART3_CR1_PCEN = 0;
            USART3_CR1_M = 0;
            USART3_CR1_PIEN = 0;
      }
      
      USART3_CR2 |= 0x0C;            //允许Tx和Rx
      
      if(0 == _In_StopBits)
      {
            USART3_CR3_STOP = 0;           //1T停止位
      }
      else if(1 == _In_StopBits)
      {
            USART3_CR3_STOP = 3;           //1.5T停止位
      }
      else if(2 == _In_StopBits)
      {
            USART3_CR3_STOP = 2;           //2T一个停止位
      }
      
      
      BaudrateDate = BSP_Usar3_BaudrateAmendment(_In_Baudrate,_In_CLK);
      
      USART3_BRR2 = (INT8U)((BaudrateDate&0x000F)|((BaudrateDate>>8)&0x00F0));//Baudrate 的低4位 和高4位组成
      USART3_BRR1 = (INT8U)((BaudrateDate>>4)&0x00FF);                        //取中间的8位
      
      if(9600 == _In_Baudrate)
      {
          BSP_ModbusParameter.ModbusT3_5 = T35BaudRate9600;
      }
      else if(4800 == _In_Baudrate)
      {
          BSP_ModbusParameter.ModbusT3_5 = T35BaudRate4800;
      }
      else if(2400 == _In_Baudrate)
      {
          BSP_ModbusParameter.ModbusT3_5 = T35BaudRate2400;
      }
      else if(19200 == _In_Baudrate)
      {
          BSP_ModbusParameter.ModbusT3_5 = T35BaudRate19200;
      }
      else
      {
          BSP_ModbusParameter.ModbusT3_5 = T35BaudRate9600; 
      }
      USART3_CR1_USARTD = 0;            //开串口 
      BSP_Usar3_RS485PinConfigure();  
      BSP_TIM4_Configure(); 
      BSP_StartRx();                     //开启接收，自动关闭发送
          
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************BSP库事件查询中心，2个事件，发送完成或接收完成  ***************  
BOOLEAN BSP_ModbusGetEventCentre(BOOLEAN EventType) //0x00代表读完成事件，0xff代表写完成事件
{
    if(EventType)
    {
          if(TxFinish == BSP_ModbusParameter.EventCentre)    
          {
                BSP_ModbusParameter.EventCentre = NoneFinish;
                return BN_TRUE;
          }
    }
    else
    {
          if(RxFinish == BSP_ModbusParameter.EventCentre)
          {
                BSP_ModbusParameter.EventCentre = NoneFinish;
                return BN_TRUE;
          }
    }
    
    return BN_FALSE;
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************BSP向Mod传输RX收到的数据，第1位保存数据长度******************** 
BOOLEAN BSP_ModbusDateTransmit_BSP_Mod(INT8U *_Out_buff)
{
      INT8U i;
      for(i = 0;i<(BSP_ModbusParameter.Buff[0]+1);i++)
      { 
           _Out_buff[i] = BSP_ModbusParameter.Buff[i];
      }
      return BN_TRUE;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********Mod向BSP传输整理好需要Tx的数据，MOD层数据的第0位是长度****************
BOOLEAN BSP_ModbusDateTransmit_Mod_BSP(INT8U *_In_buff)
{
      INT8U i;
      for(i = 0;(i<_In_buff[0]+1);i++)
      {
          BSP_ModbusParameter.Buff[i] = _In_buff[i];
      }
      return BN_TRUE;
}
//------------------------------E N D-------------------------------------------    



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************TIM4发送中断任务*************************************
void BSP_TIM4_ISR(void)//放入发送中断
{
      if(TIM4_SR1_bit.UIF) //进入TIM4中断代表，接收大帧完成。
      {   
            DisbaleRxInterrupt();// 关闭接收中断            
            BSP_ModbusParameter.EventCentre = RxFinish;//触发接收完成事 
            BSP_ModbusParameter.RxStatus = Rx_WaitFirstByte;
            BSP_TIM4_Cmd(BN_FALSE);
            TIM4_SR1_bit.UIF = 0;       
      }      
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************串口发送中断任务*************************************
void BSP_Modbus_TxTask_ISR(void)//放入发送中断
{
     static  INT8U TxCount = 0; 
      
      /*if(USART3_SR_TC)                                    //发送完成
      {    
          if(TxCount == BSP_ModbusParameter.Buff[0])       //所有数据发送完成
          {
               BSP_ModbusParameter.EventCentre = TxFinish;          //修改发送完成事件标志
               BSP_StartRx();                                       //关闭发送中断，使能接收和接收中断
               TxCount = 0;
          }
          else
          {
               USART3_DR = BSP_ModbusParameter.Buff[TxCount+1];
               TxCount++;      
          }
          
      }*/
      if(USART3_SR_TXE)
      {
            if(TxCount == BSP_ModbusParameter.Buff[0])
            {                 
                  while(0 == USART3_SR_TC);
                  BSP_ModbusParameter.EventCentre = TxFinish;          //修改发送完成事件标志
                  BSP_StartRx();                                       //关闭发送中断，使能接收和接收中断
                  TxCount = 0;
            }
            else
            {                                 
                  USART3_DR = BSP_ModbusParameter.Buff[TxCount+1];
                  TxCount++;
            }
      }
      /*if(USART3_SR_TXE)                                             //发送完成
      {    
          if((TxCount - 1) == BSP_ModbusParameter.Buff[0])          //第一帧没用所有数据发送完成
          {
               BSP_ModbusParameter.EventCentre = TxFinish;          //修改发送完成事件标志
               TxCount = 0;
               BSP_StartRx();                                       //关闭发送中断，使能接收和接收中断               
          }
          else
          {
               USART3_DR = BSP_ModbusParameter.Buff[TxCount+1];
               TxCount++;      
          }
          
      }*/
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************串口接收中断任务**************************************
void BSP_Modbus_RxTask_ISR(void)                              //放入接收中断
{  
  
    /*if(USART3_SR_PE)
    {
         USART3_DR;
    }*/
    
    if(USART3_SR_RXNE)                                         //接收中断
    {
         
         BSP_TIM4_Cmd(BN_FALSE);
         if(USART3_SR_PE)
         {
             USART3_DR;  
         }
         else
         {
         
         
         if(Rx_WaitFirstByte == BSP_ModbusParameter.RxStatus) //头一位
         {      
              if(0 != BSP_ModbusParameter.Buff[0])                              //如果字长不为0，必须先清零                             
              {
                  BSP_ModbusParameter.Buff[0] = 0;                        
              }                          
              BSP_ModbusParameter.RxStatus = Rx_Receiving;                      //状态变为接收中状态
         }  
         
         BSP_ModbusParameter.Buff[BSP_ModbusParameter.Buff[0]+1] = USART3_DR;      //缓冲区的第一位开始记录数据         
         BSP_ModbusParameter.Buff[0]++;                                          //缓冲区的
                                                                                //开启T35检测  
         if(BSP_ModbusParameter.Buff[0]>254)                                  //边界保护，接收超过255个字节
         {
             BSP_ModbusParameter.Buff[0] = 0;                                    
             BSP_ModbusParameter.RxStatus = Rx_WaitFirstByte;  
              BSP_TIM4_Cmd(BN_FALSE);
         } 
         else
         {                                                        
             BSP_TIM4_Cmd(BN_TRUE);
         }
         }
    }
     
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************串口接收中断****************************************
#pragma vector=USART3_R_RXNE_vector
__interrupt void USART3_RX_ISR(void)
{
      BSP_Modbus_RxTask_ISR();     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************串口发送中断****************************************
#pragma vector = USART3_T_TXE_vector
__interrupt void USART3_TX_ISR(void)
{        
     BSP_Modbus_TxTask_ISR();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************TIM4中断*******************************************
#pragma vector = TIM4_UIF_vector//USART3_T_TXE_vector
__interrupt void TIM4_ISR(void)
{        
     BSP_TIM4_ISR();  
}
//------------------------------E N D-------------------------------------------


