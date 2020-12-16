/*******************************************************************************
*
*	模块名称 : Mod时钟模块
*	文件名称 : Mod_SD3078.c
*	版    本 : V1.0
*	说    明 : 
*                 SD3078配置、写函数和读函数。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-07-28  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_SD3078.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************HEX TO BCD****************************************
INT8U OneByteHexToBcdCode(INT8U HexData)
{
    INT8U BcdData=0;
    
    if(HexData>99)
    {
        HexData=99;
    }
    
    BcdData=HexData/10;
    BcdData=BcdData<<4;
    BcdData|=(HexData%10);
    return(BcdData);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************BCD TO HEX****************************************
INT8U OneByteBcdcodeToHex(INT8U BcdData)
{
    INT8U HexData;
        
    HexData=0;
    HexData=BcdData&0x0F;
    HexData+=(BcdData>>4)*10;
    return(HexData);
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************SD3078开写使能***************************************
void Mod_SD3078_Write_Enable(void)
{
     BSP_I2CStartSignal();
     BSP_I2CWriteOneByte(WriteMode);
     BSP_I2CWriteOneByte(WriteEnableRegAdd_1);
     BSP_I2CWriteOneByte(WriteEnableCmd_1);
     BSP_I2CStartSignal();
     BSP_I2CWriteOneByte(WriteMode);
     BSP_I2CWriteOneByte(WriteEnableRegAdd_2_3);
     BSP_I2CWriteOneByte(WriteEnableCmd_2_3);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************SD3078关写使能***************************************
void Mod_SD3078_Write_Disable(void)
{
     BSP_I2CStartSignal();
     BSP_I2CWriteOneByte(WriteMode);
     BSP_I2CWriteOneByte(WriteEnableRegAdd_2_3);
     BSP_I2CWriteOneByte(WriteDisbaleCmd_2_3);
     BSP_I2CWriteOneByte(WriteDisableCmd_1);
     BSP_I2CStopSignal();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************充电允许*****************************************
void Mod_SD3078_Charge_Enable(void)
{
     Mod_SD3078_Write_Enable();       //写使能
     BSP_I2CStartSignal();             
     BSP_I2CWriteOneByte(WriteMode);
     BSP_I2CWriteOneByte(0x18);       //充电寄存器位置
     BSP_I2CWriteOneByte(0x82);       //允许在2K的电阻下充电
     Mod_SD3078_Write_Disable();      //关闭写使能
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************写RTC********************************************
void Mod_SD3078_WriteRTC(Mod_SD3078RTCParameter_Def *_In_me)
{      
       //Mod_SD3078_WriteDateConvert(_In_me); 
       Mod_SD3078RTCParameter_Def me;
       me.Second = OneByteHexToBcdCode(_In_me->Second);
       me.Minite = OneByteHexToBcdCode(_In_me->Minite);
       me.Hour   = OneByteHexToBcdCode(_In_me->Hour);
       me.Week   = OneByteHexToBcdCode(_In_me->Week);
       me.Day    = OneByteHexToBcdCode(_In_me->Day);
       me.Month  = OneByteHexToBcdCode(_In_me->Month);
       me.Year   = OneByteHexToBcdCode(_In_me->Year);
       
       Mod_SD3078_Write_Enable();
       BSP_I2CStartSignal();
       BSP_I2CWriteOneByte(WriteMode);
       BSP_I2CWriteOneByte(SD3078RTCAddress);     
       BSP_I2CWriteOneByte(me.Second);
       BSP_I2CWriteOneByte(me.Minite);
       BSP_I2CWriteOneByte(me.Hour|0x80);
       BSP_I2CWriteOneByte(me.Week);
       BSP_I2CWriteOneByte(me.Day);
       BSP_I2CWriteOneByte(me.Month);
       BSP_I2CWriteOneByte(me.Year);
       Mod_SD3078_Write_Disable();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************读RTC********************************************
void Mod_SD3078_ReadRTC(Mod_SD3078RTCParameter_Def *_Out_me)
{
       Mod_SD3078RTCParameter_Def me;
       BSP_I2CStartSignal();
       BSP_I2CWriteOneByte(ReadMode);
       me.Second = BSP_I2CReadOneByte(ACK);
       me.Minite = BSP_I2CReadOneByte(ACK);
       me.Hour   = (BSP_I2CReadOneByte(ACK)&0x7f);
       me.Week   = BSP_I2CReadOneByte(ACK);
       me.Day    = BSP_I2CReadOneByte(ACK);
       me.Month  = BSP_I2CReadOneByte(ACK);
       me.Year   = BSP_I2CReadOneByte(NACK); 
       
       _Out_me->Second = OneByteBcdcodeToHex(me.Second);
       _Out_me->Minite = OneByteBcdcodeToHex(me.Minite);
       _Out_me->Hour   = OneByteBcdcodeToHex(me.Hour);
       _Out_me->Week   = OneByteBcdcodeToHex(me.Week);     
       _Out_me->Day    = OneByteBcdcodeToHex(me.Day);
       _Out_me->Month  = OneByteBcdcodeToHex(me.Month);
       _Out_me->Year   = OneByteBcdcodeToHex(me.Year);
       
       BSP_I2CStopSignal();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************IO口配置、充电使能、读取RTC并检查是否有超出范围*****************
void Mod_SD3078_Configure(void)
{
      BOOLEAN Status = BN_FALSE;
      Mod_SD3078RTCParameter_Def  Mod_SD3078RTCParameter;
      BSP_I2C_IO_Configure();                             //IO口配置
      Mod_SD3078_Charge_Enable();                         //允许在2K电阻情况下充电
      Mod_SD3078_ReadRTC(&Mod_SD3078RTCParameter);        //读RTC 
      
      if(Mod_SD3078RTCParameter.Second>59)
      {
          Status = BN_TRUE;
      }
      
      if(Mod_SD3078RTCParameter.Minite>59)
      {
          Status = BN_TRUE;
      }
      
      if(Mod_SD3078RTCParameter.Hour>23)
      {
          Status = BN_TRUE;
      }
      
      if(Mod_SD3078RTCParameter.Week>06)
      {
          Status = BN_TRUE;
      }
      
      if(Mod_SD3078RTCParameter.Day>31)
      {
          Status = BN_TRUE;
      }
      
      if(Mod_SD3078RTCParameter.Month>12)
      {
          Status = BN_TRUE;
      }
      
      if(Mod_SD3078RTCParameter.Year>99)
      {
          Status = BN_TRUE;
      }
      
      if(BN_TRUE == Status)
      {
          Mod_SD3078RTCParameter.Second = 0x00; 
          Mod_SD3078RTCParameter.Minite = 0x00;
          Mod_SD3078RTCParameter.Hour   = 0x14;
          Mod_SD3078RTCParameter.Week   = 0x03;
          Mod_SD3078RTCParameter.Day    = 0x08;
          Mod_SD3078RTCParameter.Month  = 0x08;
          Mod_SD3078RTCParameter.Year   = 0x18;
          Mod_SD3078_WriteRTC(&Mod_SD3078RTCParameter);
      }
}
//------------------------------E N D-------------------------------------------


//-----------------------Mod_SD3078.c--END------------------------------------


