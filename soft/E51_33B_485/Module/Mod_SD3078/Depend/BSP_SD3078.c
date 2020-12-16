/*******************************************************************************
*
*	模块名称 : SD3078底层模块
*	文件名称 : BSP_SD3078.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_SD3078调用
                   3、BSP层处理SD3078和IO口有关的操作。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-7  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*            
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\DataTypedef.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"

//------------------------------E N D-------------------------------------------

#define SD3078_SDA_In    PC_IDR_IDR0
#define SD3078_SDA_Out   PC_ODR_ODR0   
#define SD3078_SDA_DDR   PC_DDR_DDR0
#define SD3078_SDA_CR1   PC_CR1_C10
#define SD3078_SDA_CR2   PC_CR2_C20

#define SD3078_SCL_Out   PC_ODR_ODR1   
#define SD3078_SCL_DDR   PC_DDR_DDR1
#define SD3078_SCL_CR1   PC_CR1_C11
#define SD3078_SCL_CR2   PC_CR2_C21

#define I2C_SDA_In    SD3078_SDA_In
#define I2C_SDA_Out   SD3078_SDA_Out   
#define I2C_SDA_DDR   SD3078_SDA_DDR
#define I2C_SDA_CR1   SD3078_SDA_CR1
#define I2C_SDA_CR2   SD3078_SDA_CR2

#define I2C_SCL_Out   SD3078_SCL_Out   
#define I2C_SCL_DDR   SD3078_SCL_DDR
#define I2C_SCL_CR1   SD3078_SCL_CR1
#define I2C_SCL_CR2   SD3078_SCL_CR2



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************硬延时0.0625****************************************
void BSP_I2CDelay(INT8U _In_Time)
{       
     INT8U i;
     for(i=0;i<_In_Time;i++);   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************SDA开漏输出****************************************
void BSP_I2CSDA_DDR_ODOut(void)
{
     I2C_SDA_DDR = 1; //开漏输出
     I2C_SDA_CR1 = 0;
     I2C_SDA_CR2 = 0;     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************SCL开漏输出****************************************
void BSP_I2CSCL_DDR_ODOut(void)
{
     I2C_SCL_Out = 0;
     I2C_SCL_DDR = 1;
     I2C_SCL_CR1 = 0;
     I2C_SCL_CR2 = 0;    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************SDA开漏输入***************************************
void BSP_I2CSDA_DDR_ODIn(void)
{
     I2C_SDA_DDR = 0; //OD输入
     I2C_SDA_CR1 = 0;
     I2C_SDA_CR2 = 0;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************I2C端口初始化，初始化使用或错误复位使用************************
void BSP_I2C_IO_Configure(void)
{
     BSP_I2CSCL_DDR_ODOut();
     BSP_I2CSDA_DDR_ODOut();
     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************I2C开始信号****************************************
void BSP_I2CStartSignal(void)
{    
     I2C_SCL_Out = 1;
     I2C_SDA_Out = 1;
     BSP_I2CDelay(2); 
     I2C_SDA_Out = 0;     
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************I2C结束信号****************************************
void BSP_I2CStopSignal(void)
{
     I2C_SDA_Out = 0;
     I2C_SCL_Out = 1;
     //BSP_I2CDelay(2);
     I2C_SDA_Out = 1;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************写一个字节*****************************************
BOOLEAN BSP_I2CWriteOneByte(INT8U _In_Date)
{
     INT8U Error;
     INT8U MoveBit = 0x80;
     
     while(MoveBit)
     {
          I2C_SCL_Out = 0;
          if(MoveBit & _In_Date)
          {
                I2C_SDA_Out = 1;
          }
          else
          {
                I2C_SDA_Out = 0;
          }
          
          I2C_SCL_Out = 1;
          
          MoveBit >>= 1;
     }
     I2C_SCL_Out = 0;
     
     BSP_I2CSDA_DDR_ODIn();//设置OD输入，外部有上拉电阻
     
     I2C_SCL_Out = 1;
     Error = I2C_SDA_In;   //获取应答
     I2C_SCL_Out = 0;
     I2C_SDA_Out = 1;
     BSP_I2CSDA_DDR_ODOut();
     
     if(Error)
     {
          return BN_TRUE;
     }
     else
     {
          return BN_FALSE;
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************写一个字节****************************************
INT8U BSP_I2CReadOneByte(BOOLEAN _In_AckStatus)
{
     INT8U ReadByte = 0;
     INT8U MoveBit = 0x80;
     
     BSP_I2CSDA_DDR_ODIn();
     while(MoveBit)
     {
          I2C_SCL_Out = 1;
          
          if(I2C_SDA_In)
          {
              ReadByte |= MoveBit;
          }
          
          I2C_SCL_Out = 0;
          MoveBit >>= 1;         
     }
     
     BSP_I2CSDA_DDR_ODOut();
     if(_In_AckStatus)
     {
          I2C_SDA_Out = 0;
     }
     else
     {
          I2C_SDA_Out = 1;
     }
     
     I2C_SCL_Out = 1;
     MoveBit++;MoveBit++;MoveBit++;MoveBit++;
     I2C_SCL_Out = 0;
     
     return ReadByte;
}
//------------------------------E N D-------------------------------------------


