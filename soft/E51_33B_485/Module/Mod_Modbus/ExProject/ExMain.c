/*******************************************************************************
*
*	ģ������ : �ӻ�Modbus��ʾ
*	�ļ����� : ExMain.c
*	��    �� : V1.0
*	˵    �� : ��ʾMod_Modbus�Ľӿں���ʹ�ã�Ҫʹ��Mod����APP����Ҫ����
*                  �ı����ͺ�����
*                  ��������1402��Ӳ���ϲ���ͨ����
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-18  zhoujh  ��ʽ����
*	Copyright (C), 2018-2022,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun) 
*       MCU      ��STM8L052R8
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************������ͷ�ļ�*****************************************
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_Modbus\Src\Mod_ModbusSlave.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************Ԥ��������*****************************************
#define Example_1
#define Example_2
#define Example_3
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************��ַ��Χ�Ͷ�д����Ĵ��������߽籣��**************************

//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********���屾���ĵ�ַ��У�鷽ʽ�������ʺ�����Ĵ���***********************
static   INT8U         MyAddress  = 0x01;  //��ʼ���ӻ���ַ0x01
static   Parity_Def    MyParity   = NoneParity;  //��ʼ��У��Ϊ�޼���
static   INT16U        MyBaudrate = 2400;  //��ʼ��������9600
static   INT8U         TestReg[300];
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************д�Ĵ���*****************************************
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
//*****************************���Ĵ���*****************************************
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
//****************��ⴰ����������Ƿ���±������Taskǰ�洦��******************
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
//**********************����Ĵ�����ֵʵ����************************************
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
//************************APPModbusɨ������*************************************
void App_ModbusSlaveTask(void)
{
      App_CheckModbusSlaveSoftConfigure(); //ʵ��Ӧ���ж�ʱ1-3S���ã�����Ҫʵʱ����
      Mod_SalveModbus_ScanTask();
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************��ʾ����*****************************************
void main(void)
{
    ITF_Clock_Configure();        //�����ڲ�16M
    ITF_Systick_Configure();      //1������ϵͳ����
    App_TestRegConfigure();       //���ó�ʼ��ʵ����
    while(1)           
    {
        SystickTimeRun();
        HSICalibration_Task();
        App_ModbusSlaveTask();    //���Դ򿪴�������ʾʵ�顣
    }
}
//------------------------------E N D-------------------------------------------


//End++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++