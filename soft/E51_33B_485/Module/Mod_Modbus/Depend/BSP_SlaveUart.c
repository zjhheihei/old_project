/*******************************************************************************
*
*	ģ������ : Modbus
*	�ļ����� : BSP_SlaveUart.c
*	��    �� : V1.0
*	˵    �� : 1��ģ������������������κ�ģ��
*                  2����Mod_SlaveUart����
                   3������USAR3����RS485ͨѶ������TIM4����T3_5���
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-14  zhoujh  ��ʽ����
*	Copyright (C), 2015-2022,   menredGroup
*            
*******************************************************************************/
                    

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"     
#include "intrinsics.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#define EnableTxInterrupt()   USART3_CR2_TIEN = 1  //Tx�ж�
#define DisableTxInterrupt()  USART3_CR2_TIEN = 0
//#define EnableTxInterrupt()   USART3_CR2_TCIEN = 1  //Tx�ж�
//#define DisableTxInterrupt()  USART3_CR2_TCIEN = 0

#define EnbaleRxInterrupt()   USART3_CR2_RIEN  = 1  //Rx�ж�
#define DisbaleRxInterrupt()  USART3_CR2_RIEN  = 0 


#define MaxLenght               255  //���������256BYTE
#define T35BaudRate19200        8
#define T35BaudRate9600         9   // 4mS  3�ֲ����ʶ�Ӧ��T35ʱ��
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

#define RS485DDR_Pin_ODR    PG_ODR_ODR2  //RS485����_IO
#define RS485DDR_Pin_DDR    PG_DDR_DDR2
#define RS485DDR_Pin_CR1    PG_CR1_C12
#define RS485DDR_Pin_CR2    PG_CR2_C22

#define RS485DDR_Rx()       RS485DDR_Pin_ODR = 0 //Rx
#define RS485DDR_Tx()       RS485DDR_Pin_ODR = 1 //Tx
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************Rx״̬����*****************************************
typedef enum
{
    Rx_WaitFirstByte  =0x00,
    Rx_Receiving      =0x01,
}ModbusRxStatus_Def;                  //��˫������״̬
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************TIM4�ж�״̬����*************************************
typedef enum
{
    FirstInToISR   =0x00,
    NormalInToISR  =0x01,
}TIM4ISRStatus_Def;                  //��˫������״̬
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************���պͷ�����ɶ���************************************
typedef enum
{
    NoneFinish  = 0x00,
    TxFinish    = 0x01,
    RxFinish    = 0x02,
}EventCentre_Def;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************BSP���������****************************************
typedef struct
{
    INT8U                  Buff[MaxLenght];  //���ͽ��ջ�����      
    INT8U                  ModbusT3_5;       //T3-5ʱ��
    EventCentre_Def        EventCentre;      //�¼�����
    ModbusRxStatus_Def     RxStatus;         //����״̬
}BSP_ModbusParameter_Def;

static BSP_ModbusParameter_Def BSP_ModbusParameter; //������ģ���Լ�ʹ��
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************����RS485���͹��ܺ��жϣ��رս��չ��ܺ��ж�*****************
void BSP_StartTx(void)
{
      RS485DDR_Tx();
      DisbaleRxInterrupt();      
      EnableTxInterrupt();
      
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************����RS485���չ��ܺ��жϣ��رշ��͹��ܺ��ж�*******************
void BSP_StartRx(void)
{      
      DisableTxInterrupt();
      EnbaleRxInterrupt();
      RS485DDR_Rx();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************RS485�ܽ�����***************************************
void BSP_Usar3_RS485PinConfigure(void)
{    
    TX_Pin_DDR = 1;  //OD���
    TX_Pin_CR1 = 1;  
    TX_Pin_CR2 = 0;  
    TX_Pin_ODR = 1;
    
    RX_Pin_DDR = 0; //��������
    RX_Pin_CR1 = 1;  
    RX_Pin_CR2 = 0;
                 
    RS485DDR_Pin_DDR = 1;  //���������RS485ͨ�ŷ���Ĭ���� 
    RS485DDR_Pin_CR1 = 1;  
    RS485DDR_Pin_CR2 = 0;
    //RS485DDR_Rx();     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************TIM4�����ر�****************************************
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
//****************************TIM4����******************************************
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
//*******************�����ʺ���Ƶ������㴮������********************************
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
//*************************����3��TIM4����**************************************
void BSP_Usart3_Configure(INT16U _In_Baudrate,INT8U _In_ParitySelect,INT32U _In_CLK,INT8U _In_StopBits)//����Usart3
{    
      INT16U BaudrateDate = 0;
      
      CLK_PCKENR3_bit.PCKEN34 = 1;               //��Usart3ʱ��
      CLK_PCKENR1_bit.PCKEN12 = 1;               //��TIM4ʱ��
      USART3_SR;
      USART3_DR;
      USART3_CR1_USARTD = 1;  
      if(NoParity == _In_ParitySelect)         //��У��
      {   
           USART3_CR1_M = 0; 
           USART3_CR1_PCEN = 0;
           USART3_CR1_PIEN = 0;
      }
      else if(EvenParity == _In_ParitySelect)  //żУ��
      {
          USART3_CR1_M = 1; 
          USART3_CR1_PCEN  = 1;
          USART3_CR1_PS = 0;
          USART3_CR1_PIEN = 1;
      }
      else if(OddParity == _In_ParitySelect)  //��У��
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
      
      USART3_CR2 |= 0x0C;            //����Tx��Rx
      
      if(0 == _In_StopBits)
      {
            USART3_CR3_STOP = 0;           //1Tֹͣλ
      }
      else if(1 == _In_StopBits)
      {
            USART3_CR3_STOP = 3;           //1.5Tֹͣλ
      }
      else if(2 == _In_StopBits)
      {
            USART3_CR3_STOP = 2;           //2Tһ��ֹͣλ
      }
      
      
      BaudrateDate = BSP_Usar3_BaudrateAmendment(_In_Baudrate,_In_CLK);
      
      USART3_BRR2 = (INT8U)((BaudrateDate&0x000F)|((BaudrateDate>>8)&0x00F0));//Baudrate �ĵ�4λ �͸�4λ���
      USART3_BRR1 = (INT8U)((BaudrateDate>>4)&0x00FF);                        //ȡ�м��8λ
      
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
      USART3_CR1_USARTD = 0;            //������ 
      BSP_Usar3_RS485PinConfigure();  
      BSP_TIM4_Configure(); 
      BSP_StartRx();                     //�������գ��Զ��رշ���
          
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************BSP���¼���ѯ���ģ�2���¼���������ɻ�������  ***************  
BOOLEAN BSP_ModbusGetEventCentre(BOOLEAN EventType) //0x00���������¼���0xff����д����¼�
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
//****************BSP��Mod����RX�յ������ݣ���1λ�������ݳ���******************** 
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
//**********Mod��BSP�����������ҪTx�����ݣ�MOD�����ݵĵ�0λ�ǳ���****************
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
//*************************TIM4�����ж�����*************************************
void BSP_TIM4_ISR(void)//���뷢���ж�
{
      if(TIM4_SR1_bit.UIF) //����TIM4�жϴ������մ�֡��ɡ�
      {   
            DisbaleRxInterrupt();// �رս����ж�            
            BSP_ModbusParameter.EventCentre = RxFinish;//������������� 
            BSP_ModbusParameter.RxStatus = Rx_WaitFirstByte;
            BSP_TIM4_Cmd(BN_FALSE);
            TIM4_SR1_bit.UIF = 0;       
      }      
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************���ڷ����ж�����*************************************
void BSP_Modbus_TxTask_ISR(void)//���뷢���ж�
{
     static  INT8U TxCount = 0; 
      
      /*if(USART3_SR_TC)                                    //�������
      {    
          if(TxCount == BSP_ModbusParameter.Buff[0])       //�������ݷ������
          {
               BSP_ModbusParameter.EventCentre = TxFinish;          //�޸ķ�������¼���־
               BSP_StartRx();                                       //�رշ����жϣ�ʹ�ܽ��պͽ����ж�
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
                  BSP_ModbusParameter.EventCentre = TxFinish;          //�޸ķ�������¼���־
                  BSP_StartRx();                                       //�رշ����жϣ�ʹ�ܽ��պͽ����ж�
                  TxCount = 0;
            }
            else
            {                                 
                  USART3_DR = BSP_ModbusParameter.Buff[TxCount+1];
                  TxCount++;
            }
      }
      /*if(USART3_SR_TXE)                                             //�������
      {    
          if((TxCount - 1) == BSP_ModbusParameter.Buff[0])          //��һ֡û���������ݷ������
          {
               BSP_ModbusParameter.EventCentre = TxFinish;          //�޸ķ�������¼���־
               TxCount = 0;
               BSP_StartRx();                                       //�رշ����жϣ�ʹ�ܽ��պͽ����ж�               
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
//*************************���ڽ����ж�����**************************************
void BSP_Modbus_RxTask_ISR(void)                              //��������ж�
{  
  
    /*if(USART3_SR_PE)
    {
         USART3_DR;
    }*/
    
    if(USART3_SR_RXNE)                                         //�����ж�
    {
         
         BSP_TIM4_Cmd(BN_FALSE);
         if(USART3_SR_PE)
         {
             USART3_DR;  
         }
         else
         {
         
         
         if(Rx_WaitFirstByte == BSP_ModbusParameter.RxStatus) //ͷһλ
         {      
              if(0 != BSP_ModbusParameter.Buff[0])                              //����ֳ���Ϊ0������������                             
              {
                  BSP_ModbusParameter.Buff[0] = 0;                        
              }                          
              BSP_ModbusParameter.RxStatus = Rx_Receiving;                      //״̬��Ϊ������״̬
         }  
         
         BSP_ModbusParameter.Buff[BSP_ModbusParameter.Buff[0]+1] = USART3_DR;      //�������ĵ�һλ��ʼ��¼����         
         BSP_ModbusParameter.Buff[0]++;                                          //��������
                                                                                //����T35���  
         if(BSP_ModbusParameter.Buff[0]>254)                                  //�߽籣�������ճ���255���ֽ�
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
//***************************���ڽ����ж�****************************************
#pragma vector=USART3_R_RXNE_vector
__interrupt void USART3_RX_ISR(void)
{
      BSP_Modbus_RxTask_ISR();     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************���ڷ����ж�****************************************
#pragma vector = USART3_T_TXE_vector
__interrupt void USART3_TX_ISR(void)
{        
     BSP_Modbus_TxTask_ISR();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************TIM4�ж�*******************************************
#pragma vector = TIM4_UIF_vector//USART3_T_TXE_vector
__interrupt void TIM4_ISR(void)
{        
     BSP_TIM4_ISR();  
}
//------------------------------E N D-------------------------------------------


