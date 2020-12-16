
/*******************************************************************************
*
*	ģ������ : Modbusģ��
*	�ļ����� : Mod_ModbusSlave.c
*	��    �� : V1.0
*	˵    �� : ģ���дӻ�Modbus�������ɨ�衣
*                  �����а�������RX���ݺ���Ҫ���͵�TX����
*                  �����Ӳ����ʱģ��
*                  ��ȡApp��Ĵ����ʹ���������ù���
*                   
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2018-07-02  zhoujh  ��ʽ����
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_ModbusSlave.h"
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************Mod�����*******************************************
static SlaveModbusMod_Parameter_Def Mod_SlaveModbusParameter;  
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************0x17���ܺ궨��**************************************
#define Function17_RRA  0
#define Function17_RRC  1
#define Function17_WRA  2
#define Function17_WRC  3
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************CRC�������,���ֽڲ��****************************
const INT16U chCRCTalbe[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,       
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,       
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,       
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,       
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,       
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,       
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,       
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,       
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,       
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,       
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040                                                        
};
void Crc16CalculateOfByte(INT8U *ptr,INT16U len,INT8U *Ret)  
{
    INT16U crc;
    INT8U da;
    crc=0xffff;
    while((len--)!=0)
    {
        da=crc;                  //ȡCRC�ĵ�8λ
        da=da^(*ptr);            //��ñ�����
        crc=crc>>8;              //��8λ�����8λ
        crc=crc^chCRCTalbe[da];  //����ʽ��
        ptr++;
    }
    Ret[0]=crc;
    Ret[1]=crc>>8; 
} 
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++


    
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************�ӻ���ַ������У�鷽ʽ���üĴ�����ַ��Χ*************************
void Mod_SlaveModbusUsartParameterConfigure(SlaveModbusMod_Parameter_Def *_In_me)
{
      static INT8U ConfigureStatus = 0;
      static Parity_Def  Keep_Parity     = NoneParity;  
      static INT16U      Keep_Baudrate   = 9600;  
      static INT8U       Keep_SlaveAddress = 0x01;
      static INT32U      Keep_CLK;
      static StopBits_Def Keep_StopBits  = Bit_1_0;
              //BOOLEAN     Keep_CLKHasChanged = BN_FALSE;
      
      if(0 == ConfigureStatus)// �ϵ����� 
      {
            Mod_SlaveModbusParameter.Parity                 =  _In_me->Parity;
            Mod_SlaveModbusParameter.Baudrate               =  _In_me->Baudrate;
            Mod_SlaveModbusParameter.CLK                    =  _In_me->CLK;
            Mod_SlaveModbusParameter.SlaveAddress           =  _In_me->SlaveAddress;
            Mod_SlaveModbusParameter.StopBits               =  _In_me->StopBits;
            Mod_SlaveModbusParameter.WriteRegister          =  _In_me->WriteRegister;
            Mod_SlaveModbusParameter.ReadRegister           =  _In_me->ReadRegister;
            Keep_Parity       = Mod_SlaveModbusParameter.Parity;
            Keep_Baudrate     = Mod_SlaveModbusParameter.Baudrate;
            Keep_SlaveAddress = Mod_SlaveModbusParameter.SlaveAddress;
            Keep_StopBits     = Mod_SlaveModbusParameter.StopBits;
            ConfigureStatus++;
            Keep_CLK = Mod_SlaveModbusParameter.CLK;
      }
      else
      {
            if(Keep_CLK >_In_me->CLK)
            {
                  if((Keep_CLK - _In_me->CLK) > 16000)
                  {
                      Keep_CLK = _In_me->CLK;
                      //Keep_CLKHasChanged = BN_TRUE;
                       Mod_SlaveModbusParameter.UsartClkChangEvent = BN_TRUE;
                  }
                  else
                  {
                      Mod_SlaveModbusParameter.CLK = Keep_CLK;
                  }
            }
            else if(Keep_CLK <_In_me->CLK)
            {
                  if((Keep_CLK -  _In_me->CLK)>16000)
                  {
                      Keep_CLK = _In_me->CLK;
                      //Keep_CLKHasChanged = BN_TRUE;
                      Mod_SlaveModbusParameter.UsartClkChangEvent = BN_TRUE;
                  }
                  else
                  {
                      Mod_SlaveModbusParameter.CLK = Keep_CLK;
                  }
            }
            else
            {
                  Mod_SlaveModbusParameter.CLK = Keep_CLK;
            }
            
            if((Keep_Parity != _In_me->Parity)||
               (Keep_Baudrate != _In_me->Baudrate)||
               (Keep_SlaveAddress != _In_me->SlaveAddress)||
               (Keep_StopBits != _In_me->StopBits))
            {
                  Mod_SlaveModbusParameter.Parity                 =  _In_me->Parity;
                  Mod_SlaveModbusParameter.Baudrate               =  _In_me->Baudrate;
                  Mod_SlaveModbusParameter.SlaveAddress           =  _In_me->SlaveAddress;
                  Mod_SlaveModbusParameter.StopBits               =  _In_me->StopBits;
                  //Keep_CLKHasChanged = BN_FALSE;
                  Mod_SlaveModbusParameter.UsartSoftConfigureEvent  =  BN_TRUE;
                  Keep_Parity       = Mod_SlaveModbusParameter.Parity;
                  Keep_Baudrate     = Mod_SlaveModbusParameter.Baudrate;
                  Keep_SlaveAddress = Mod_SlaveModbusParameter.SlaveAddress;
                  Keep_StopBits     = Mod_SlaveModbusParameter.StopBits;
            }
      }
      
      
}
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************��ȡУ�鷽ʽ����ֵ**********************************
INT8U Mod_SalveModbus_GetParity(void)
{
     INT8U ParityTyep;
     
     if(NoneParity == Mod_SlaveModbusParameter.Parity)
     {
          ParityTyep = 0x00; //��У��
     }
     else if(EvenParity == Mod_SlaveModbusParameter.Parity)
     {
          ParityTyep = 0x01; //żУ��
     }
     else if(OddParity == Mod_SlaveModbusParameter.Parity)
     {
          ParityTyep = 0x02; //��У��
     }
     
     return ParityTyep;
}
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************��ȡУ�鷽ʽ����ֵ**********************************
INT8U Mod_SalveModbus_GetStopBits(void)
{
     INT8U StopBitsTyep;
     
     if(Bit_1_0 == Mod_SlaveModbusParameter.StopBits)
     {
          StopBitsTyep = 0x00; //��У��
     }
     else if(Bit_1_5 == Mod_SlaveModbusParameter.StopBits)
     {
          StopBitsTyep = 0x01; //żУ��
     }
     else if(Bit_2_0 == Mod_SlaveModbusParameter.StopBits)
     {
          StopBitsTyep = 0x02; //��У��
     }
     
     return StopBitsTyep;
}
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************���ִ��󷵻�����״̬***********************************
void Mod_SlaveModbusGoBackToWait(void)
{
       Mod_SlaveModbusParameter.TaskStatus = Rx_Wait;  //У������󷵻صȴ�     
       BSP_StartRx();
}
//+++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************mod�����ݴ�������***********************************
BOOLEAN Mod_ModbusSalveDateProcessCentre(void)
{     
      INT8U CalcCRC[2];       
      INT16U RegAddress;
      INT16U RegLength;
      INT16U Function0x17[4];
      BOOLEAN  ACK = BN_FALSE;
      INT8U LastSlaveAddress = 0x00;
      
      
      if((Mod_SlaveModbusParameter.SlaveAddress == Mod_SlaveModbusParameter.Buff[1]) || (0xFA == Mod_SlaveModbusParameter.Buff[1]))//�ӻ���ַ��ȷ�����ַУ��
      {
           Crc16CalculateOfByte(&Mod_SlaveModbusParameter.Buff[1],(Mod_SlaveModbusParameter.Buff[0]-2),&CalcCRC[0]);
           if((CalcCRC[1] == Mod_SlaveModbusParameter.Buff[Mod_SlaveModbusParameter.Buff[0]])&& 
              (CalcCRC[0] == Mod_SlaveModbusParameter.Buff[Mod_SlaveModbusParameter.Buff[0]-1]))          
           {
                switch(Mod_SlaveModbusParameter.Buff[2])  //������
                {
                    case 0x03:
                    {
                          if(8 == Mod_SlaveModbusParameter.Buff[0])    //01 03 00 00 00 01 CRH CRL-----03ָ��8λ
                          {      
                                RegLength  =  Mod_SlaveModbusParameter.Buff[5];      //��Ҫ��ȡ�ĳ���
                                RegLength  =  RegLength<<8;
                                RegLength |=  Mod_SlaveModbusParameter.Buff[6];
                                
                                RegAddress  = Mod_SlaveModbusParameter.Buff[3];   //��ַ��λ
                                RegAddress  = RegAddress<<8;
                                RegAddress |= Mod_SlaveModbusParameter.Buff[4];   //��ַ��λ
                                                                    
                                if(RegLength<=120)    
                                {    
                                      Mod_SlaveModbusParameter.Buff[1] = Mod_SlaveModbusParameter.SlaveAddress; //��ȡ��ַ
                                      Mod_SlaveModbusParameter.Buff[2] = 0x03;                               //������
                                      Mod_SlaveModbusParameter.Buff[3] = (INT8U)RegLength*2;                        //�ض��Ĵ�������
                                      Mod_SlaveModbusParameter.ReadRegister(RegAddress,RegLength,&Mod_SlaveModbusParameter.Buff[4]);//��4λ��ʼ�洢
                                      Crc16CalculateOfByte(&Mod_SlaveModbusParameter.Buff[1],(Mod_SlaveModbusParameter.Buff[3]+3),(&Mod_SlaveModbusParameter.Buff[Mod_SlaveModbusParameter.Buff[3]]+4));
                                      Mod_SlaveModbusParameter.Buff[0] = Mod_SlaveModbusParameter.Buff[3]+5;
                                      ACK = BN_TRUE;
                                }
                                else
                                {
                                     Mod_SlaveModbusGoBackToWait();
                                }
                               
                          }
                          else
                          {
                              Mod_SlaveModbusGoBackToWait();
                          }
                          break;          
                    }
                    case 0x06:
                    {
                          if(8 == Mod_SlaveModbusParameter.Buff[0])                  //01 06 xx xx 00 01 CRH CRL-----03ָ��8λ
                          {      
                                RegLength = 1;                                     //д�Ĵ����ĳ���
                                RegAddress  = Mod_SlaveModbusParameter.Buff[3];   //��ַ��λ
                                RegAddress  = RegAddress<<8;
                                RegAddress |= Mod_SlaveModbusParameter.Buff[4];  //��ַ��λ
                               
                                if(RegLength<=120)
                                {   
                                    LastSlaveAddress = Mod_SlaveModbusParameter.SlaveAddress;
                                    Mod_SlaveModbusParameter.WriteRegister(RegAddress,RegLength,&Mod_SlaveModbusParameter.Buff[5]);                  //д��Ĵ���                                     
                                    Mod_SlaveModbusParameter.Buff[1] = Mod_SlaveModbusParameter.SlaveAddress;                                        //��ȡ��ַ
                                    
                                    if( 
                                        (0x25 == Mod_SlaveModbusParameter.Buff[5]) && (0x91 == Mod_SlaveModbusParameter.Buff[6]) &&
                                        (0x15 == Mod_SlaveModbusParameter.Buff[3]) && (0x00 == Mod_SlaveModbusParameter.Buff[4])
                                       )
                                    {
                                          Mod_SlaveModbusParameter.Buff[1] = LastSlaveAddress;            
                                    }
                                    
                                    Mod_SlaveModbusParameter.Buff[2] = 0x06;                                                                          //������
                                    Mod_SlaveModbusParameter.Buff[3] = RegAddress>>8;                                                                 //�ض��Ĵ�������
                                    Mod_SlaveModbusParameter.Buff[4] = RegAddress;
                                    Mod_SlaveModbusParameter.ReadRegister(RegAddress,RegLength,&Mod_SlaveModbusParameter.Buff[5]);                    //��5λ��ʼ�洢
                                    Crc16CalculateOfByte(&Mod_SlaveModbusParameter.Buff[1],6,&Mod_SlaveModbusParameter.Buff[7]);
                                    Mod_SlaveModbusParameter.Buff[0] = 8;
                                    ACK = BN_TRUE;
                                }
                                else
                                {
                                     Mod_SlaveModbusGoBackToWait();
                                }
                              
                          }
                          else
                          {
                              Mod_SlaveModbusGoBackToWait();
                          }
                          break;          
                     }
                     case 0x10:
                     {
                          if(Mod_SlaveModbusParameter.Buff[0] == (9 +Mod_SlaveModbusParameter.Buff[7]))                  //01 10 00 00 00 02 04 xx xx xx xx CH CL-----03ָ��8λ
                          {      
                                RegLength  = Mod_SlaveModbusParameter.Buff[5];
                                RegLength  = RegLength<<8;
                                RegLength |= Mod_SlaveModbusParameter.Buff[6];                         //д�Ĵ����ĳ���
                                
                                RegAddress  = Mod_SlaveModbusParameter.Buff[3];   //��ַ��λ
                                RegAddress  = RegAddress<<8;
                                RegAddress |= Mod_SlaveModbusParameter.Buff[4];  //��ַ��λ
                               
                               
                                if((RegLength<=120) && (Mod_SlaveModbusParameter.Buff[7] ==  RegLength*2))   //����߽��� //���256- 01 03 RegLength*****CRCH CRCL
                                {  
                                    Mod_SlaveModbusParameter.WriteRegister(RegAddress,RegLength,&Mod_SlaveModbusParameter.Buff[8]);                  //д��Ĵ���
                                    
                                    Mod_SlaveModbusParameter.Buff[1] = Mod_SlaveModbusParameter.SlaveAddress;                   //��ȡ��ַ
                                    Mod_SlaveModbusParameter.Buff[2] = 0x10;                                                    //������
                                    Mod_SlaveModbusParameter.Buff[3] = RegAddress>>8;                                              //�ض��Ĵ�������
                                    Mod_SlaveModbusParameter.Buff[4] = RegAddress;
                                    Mod_SlaveModbusParameter.Buff[5] = RegLength>>8;
                                    Mod_SlaveModbusParameter.Buff[6] = RegLength;
                                    Crc16CalculateOfByte(&Mod_SlaveModbusParameter.Buff[1],6,&Mod_SlaveModbusParameter.Buff[7]);
                                    Mod_SlaveModbusParameter.Buff[0] = 8;
                                    ACK = BN_TRUE;
                                } 
                                else
                                {
                                    Mod_SlaveModbusGoBackToWait();
                                }
                               
                          }
                          else                                                
                          {
                              Mod_SlaveModbusGoBackToWait();
                          }
                          break;          
                     }
                     
                     case 0x17:
                     {
                          if(Mod_SlaveModbusParameter.Buff[0] == (13+Mod_SlaveModbusParameter.Buff[11]))                  //01 10 00 00 00 02 04 xx xx xx xx CH CL-----03ָ��8λ
                          {     
                            
                                Function0x17[Function17_RRA]  = Mod_SlaveModbusParameter.Buff[3];   
                                Function0x17[Function17_RRA]<<= 8;
                                Function0x17[Function17_RRA] |= Mod_SlaveModbusParameter.Buff[4];  //Read Reg Address
                                
                                Function0x17[Function17_RRC]  = Mod_SlaveModbusParameter.Buff[5];
                                Function0x17[Function17_RRC]<<= 8;
                                Function0x17[Function17_RRC] |= Mod_SlaveModbusParameter.Buff[6];  //Read Reg Count
                               
                                Function0x17[Function17_WRA]  = Mod_SlaveModbusParameter.Buff[7];
                                Function0x17[Function17_WRA]<<= 8;
                                Function0x17[Function17_WRA] |= (INT8U)Mod_SlaveModbusParameter.Buff[8];
                                
                                Function0x17[Function17_WRC]  = Mod_SlaveModbusParameter.Buff[9];
                                Function0x17[Function17_WRC]<<= 8;
                                Function0x17[Function17_WRC] |= (INT8U)Mod_SlaveModbusParameter.Buff[10];
                                
                                if((Function0x17[Function17_WRC]<=120)&&(Mod_SlaveModbusParameter.Buff[11] ==  Function0x17[Function17_WRC]*2))
                                {
                                      Mod_SlaveModbusParameter.WriteRegister(Function0x17[Function17_WRA],Function0x17[Function17_WRC],&Mod_SlaveModbusParameter.Buff[12]);
                                      Mod_SlaveModbusParameter.Buff[1] = Mod_SlaveModbusParameter.SlaveAddress;
                                      Mod_SlaveModbusParameter.Buff[2] = 0x17;
                                      Mod_SlaveModbusParameter.Buff[3] = Function0x17[Function17_RRC]*2;
                                      Mod_SlaveModbusParameter.ReadRegister(Function0x17[Function17_RRA],Function0x17[Function17_RRC],&Mod_SlaveModbusParameter.Buff[4]);   
                                      Crc16CalculateOfByte(&Mod_SlaveModbusParameter.Buff[1],(Mod_SlaveModbusParameter.Buff[3]+3),(&Mod_SlaveModbusParameter.Buff[Mod_SlaveModbusParameter.Buff[3]]+4));
                                      Mod_SlaveModbusParameter.Buff[0] = Mod_SlaveModbusParameter.Buff[3]+5;
                                      ACK = BN_TRUE;
                                }
                                else
                                {
                                      Mod_SlaveModbusGoBackToWait();
                                }          
                          }
                          else
                          {
                              Mod_SlaveModbusGoBackToWait();
                          }
                          break;          
                     }
                     default: 
                     {
                         Mod_SlaveModbusGoBackToWait();
                         break;
                     }
                }
           }
           else
           {
                Mod_SlaveModbusGoBackToWait();
           }
     }
     else
     {
         Mod_SlaveModbusGoBackToWait();
     }
     
     return ACK;
}
//++++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************��ʱ�ж�����***************************************
void Mod_SalaveModbusTimeOutTask(void)
{
 
     static DelayTickClk_Def  ModbusSoftwareTimeOut={Type_Second,5,0}; 
     static DelayTickClk_Def  ModbusHardwareTimeOut={Type_Minute,60,0}; 
     static TaskStatus_Def    CurrentTaskStatus = UsartMod_ReStart;
     
     DelayTickClockRun(&ModbusHardwareTimeOut);
     DelayTickClockRun(&ModbusSoftwareTimeOut);
     
     if(CurrentTaskStatus != Mod_SlaveModbusParameter.TaskStatus) 
     {
          CurrentTaskStatus = Mod_SlaveModbusParameter.TaskStatus;
          
          if(Rx_Wait == CurrentTaskStatus ) 
          {
                Mod_SlaveModbusParameter.TimeOutSwitch = HardwareSw;
                ReloadDealyTickClock(&ModbusHardwareTimeOut,60);
          }
          else if(Tx_Wait == CurrentTaskStatus) 
          {
                Mod_SlaveModbusParameter.TimeOutSwitch = SoftwareSw;
                ReloadDealyTickClock(&ModbusSoftwareTimeOut,5);
                ReloadDealyTickClock(&ModbusHardwareTimeOut,60);
          }
          else
          {
                ReloadDealyTickClock(&ModbusHardwareTimeOut,60); 
          }
     }
     
     if(HardwareSw == Mod_SlaveModbusParameter.TimeOutSwitch)
     {
            if(DelayTickClockFinished(&ModbusHardwareTimeOut))
            {
                  CurrentTaskStatus = UsartMod_ReStart;
                  Mod_SlaveModbusParameter.TaskStatus = UsartMod_ReStart;
            }
     }
     else if(SoftwareSw == Mod_SlaveModbusParameter.TimeOutSwitch)
     {
            if(DelayTickClockFinished(&ModbusSoftwareTimeOut))
            {
                  BSP_StartRx(); 
                  Mod_SlaveModbusParameter.TaskStatus = Rx_Wait;
            }
     }     
}
//++++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************�ӻ�ɨ������****************************************
void Mod_SalveModbus_ScanTask(void)
{    
    switch(Mod_SlaveModbusParameter.TaskStatus)
    {
        case UsartMod_ReStart:
        {
             BSP_Usart3_Configure(Mod_SlaveModbusParameter.Baudrate,Mod_SalveModbus_GetParity(),Mod_SlaveModbusParameter.CLK,Mod_SalveModbus_GetStopBits());                //Ӳ����λ���߳�ʼ��   
             Mod_SlaveModbusParameter.TaskStatus = Rx_Wait;
             break;
        }
        
        case Rx_Wait:                                                                       //�ӻ�û�з���ͨѶͣ�������״̬
        {
             if(BN_TRUE == Mod_SlaveModbusParameter.UsartSoftConfigureEvent)                  //�ӻ��ȴ���������÷����ı䣬�����ʻ�У�鷽ʽ�����ı�
             {
                 Mod_SlaveModbusParameter.UsartSoftConfigureEvent = BN_FALSE;
                 Mod_SlaveModbusParameter.TaskStatus = UsartMod_ReStart;                    //������������
             }
       
             if(BSP_ModbusGetEventCentre(RxFinishEvent))                                    //���ͨѶ����¼�������BSP��
             {
                  if(BSP_ModbusDateTransmit_BSP_Mod(Mod_SlaveModbusParameter.Buff))
                  {     
                      //Mod_SlaveModbusParameter.TaskStatus = Tx_Wait; 
                      if(Mod_ModbusSalveDateProcessCentre())   //Tx���ݴ������
                       {
                            if(BSP_ModbusDateTransmit_Mod_BSP(Mod_SlaveModbusParameter.Buff))        //�������ݵ�BSP��
                            {
                                    BSP_StartTx();                                                   //��������ʹ�ܺͷ�����
                                    Mod_SlaveModbusParameter.TaskStatus = Tx_Sending;
                            }       
                       }
                  }    
             }
                         
             break;
        }
        
        case Tx_Wait:                                              
        {
             /*if(Mod_ModbusSalveDateProcessCentre())   //Tx���ݴ������
             {
                  if(BSP_ModbusDateTransmit_Mod_BSP(Mod_SlaveModbusParameter.Buff))        //�������ݵ�BSP��
                  {
                          BSP_StartTx();                                                   //��������ʹ�ܺͷ�����
                          Mod_SlaveModbusParameter.TaskStatus = Tx_Sending;
                  }       
             }*/
            
             break;
        }
        
        case Tx_Sending:
        {
              if(BSP_ModbusGetEventCentre(TxFinishEvent))                                  //������ɺ�BSP�Զ��л�Rx,����Ӳ����ʱʱ��60��
              {
                    //BSP_StartRx();
                    Mod_SlaveModbusParameter.TaskStatus = Rx_Wait;
                    if(BN_TRUE == Mod_SlaveModbusParameter.UsartClkChangEvent)                  //�ӻ��ȴ���������÷����ı䣬�����ʻ�У�鷽ʽ�����ı�
                    {
                           Mod_SlaveModbusParameter.UsartClkChangEvent = BN_FALSE;
                           Mod_SlaveModbusParameter.TaskStatus = UsartMod_ReStart;                  //������������                          
                    }
              }
              
              break;
        }
        default:
        {
              break;
        }
    }
    Mod_SalaveModbusTimeOutTask();
}
//++++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++



