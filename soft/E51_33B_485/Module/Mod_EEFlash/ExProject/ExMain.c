//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MCU: STM8L052R8T6
//IDE: IARSTM8 2.10.4(EWSTM8-2104-Autorun)
//FCPU: HSI-RC-16MHZ
//-------------------------------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Begin++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "..\..\Mod_EEFlash\Src\Mod_FlashStoBlockOf8Bits.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
union
{
    INT8U StoBuffer[10];
    struct
    {
        INT8U Sto_Example0;
        INT8U Sto_Example1;
        INT16U Sto_Example2;
    };
}ExStoData;
//-------------------------------------------------------------------------------------------------
void main(void)
{

    EEPFlash_Configure(sizeof(ExStoData));    
    
    ExStoData.Sto_Example0 = 0x12;
    ExStoData.Sto_Example1 = 0x34;
    ExStoData.Sto_Example2 = 0x7788;
    
    if(ReadBlockOfEEPFlash(&ExStoData.StoBuffer[0]))
    {
        ExStoData.Sto_Example0 = 0;
    }
    else
    { 
        ExStoData.Sto_Example0++;
    }
    
    WriteBlockOfEEPFlash(&ExStoData.StoBuffer[0]);
    
    ExStoData.Sto_Example0 = 0xFF;
    ExStoData.Sto_Example1 = 0xFF;
    ExStoData.Sto_Example2 = 0xFFFF;
    

 //------------------------------------------------------------------------------------------------      
    while(1)
    {

    }
}
//End++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++