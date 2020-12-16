/*******************************************************************************
*
*	模块名称 : 从机Modbus演示
*	文件名称 : ExMain.c
*	版    本 : V1.0
*	说    明 : 演示Mod_Modbus的接口函数使用，要使用Mod函数APP层需要定义
*                  的变量和函数。
*                  本函数在1402的硬件上测试通过。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-18  zhoujh  正式发布
*	Copyright (C), 2018-2022,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun) 
*       MCU      ：STM8L052R8
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************包含的头文件*****************************************
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_Modbus\Src\Mod_ModbusSlave.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************预定义例程*****************************************
#define Example_1
#define Example_2
#define Example_3
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************地址范围和读写虚拟寄存器个数边界保护**************************

//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********定义本机的地址、校验方式、波特率和虚拟寄存器***********************
static   INT8U         MyAddress  = 0x01;  //初始化从机地址0x01
static   Parity_Def    MyParity   = NoneParity;  //初始化校验为无检验
static   INT16U        MyBaudrate = 2400;  //初始化波特率9600
static   INT8U         TestReg[300];
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************写寄存器*****************************************
void App_WriteRegister(INT16U _In_RegAddr,INT8U _In_RegLength,INT8U* _In_pDataBuff)
{
      INT8U WriteRegLength;
      INT8U i = 0;
      WriteRegLength = _In_RegLength;
 
      while(WriteRegLength)
      {
           TestReg[_In_RegAddr*2]   = _In_pDataBuff[i];  
           TestReg[_In_RegAddr*2+1] = _In_pDataBuff[i+1]; 
           WriteRegLength--;
           i += 2;
           _In_RegAddr++;
      }
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************读寄存器*****************************************
void App_ReadRegister(INT16U _In_RegAddr,INT8U _In_RegLength,INT8U* _Out_pDataBuff)
{
      INT16U WriteRegLength;
      WriteRegLength = _In_RegLength;
      INT8U i = 0;
      
      while(WriteRegLength)
      {
           _Out_pDataBuff[i]   = TestReg[_In_RegAddr*2];  
           _Out_pDataBuff[i+1] = TestReg[_In_RegAddr*2+1]; 
           WriteRegLength--;
           i+=2;
           _In_RegAddr++;
      }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************检测窗口软件配置是否更新必须放在Task前面处理******************
void App_CheckModbusSlaveSoftConfigure(void)
{  
    SlaveModbusMod_Parameter_Def App_SlaveModbusParameter;
    App_SlaveModbusParameter.Baudrate              = MyBaudrate;
    App_SlaveModbusParameter.Parity                = MyParity;
    App_SlaveModbusParameter.CLK                   = Poll_HSIFrequency();
    App_SlaveModbusParameter.SlaveAddress          = MyAddress;
    App_SlaveModbusParameter.WriteRegister         = App_WriteRegister;
    App_SlaveModbusParameter.ReadRegister          = App_ReadRegister;
    Mod_SlaveModbusUsartParameterConfigure(&App_SlaveModbusParameter);    
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************虚拟寄存器赋值实验用************************************
void App_TestRegConfigure(void)
{
    INT8U i;
    
    for(i=0;i<255;i++)
    {
        TestReg[i] = i;  
    }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************APPModbus扫描任务*************************************
void App_ModbusSlaveTask(void)
{
      App_CheckModbusSlaveSoftConfigure(); //实际应用中定时1-3S调用，不需要实时调用
      Mod_SalveModbus_ScanTask();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************演示函数*****************************************
void main(void)
{
    ITF_Clock_Configure();        //配置内部16M
    ITF_Systick_Configure();      //1毫秒钟系统脉动
    App_TestRegConfigure();       //配置初始化实验用
    while(1)           
    {
        SystickTimeRun();
        HSICalibration_Task();
        App_ModbusSlaveTask();    //可以打开串口做演示实验。
    }
}
//------------------------------E N D-------------------------------------------


//End++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++