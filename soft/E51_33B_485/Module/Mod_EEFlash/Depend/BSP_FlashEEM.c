//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\BSP_FlashEEM.h"
#include ".\iostm8l052r8.h"
#include "..\Src\Mod_FlashStoBlockOf8Bits.h"
#include "intrinsics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数组清零
//BuffIndex=0;
//BuffSize=sizeof(UpdateLcdBuffer);
//_ClearArrayData(UpdateLcdBuffer[BuffIndex],BuffIndex,BuffSize);
//-----------------------------------------------------------------------------
#define _ClearArrayData(_ArrayName,_Index,_Size) \
do\
{\
   while(_Size)\
   {\
     _ArrayName=0;\
     _Index++;\
     _Size--;\
   }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//STM8L052R8T6 内置Flash型EEPROM的BSP支持函数
//STM8L052R8T6 EE size 256Bytes  1 pages  2 blocks
//address        0x00 1000---0x00 10FF
//1page=2block=256bytes   1block=128bytes
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define BlockSize          128
//-----------------------------------------------------------------------------
INT8U FlashAddr[3]; 
#define Addr_H FlashAddr[0] 
#define Addr_M FlashAddr[1] 
#define Addr_L FlashAddr[2] 
//FlashAddr[0] --Add_H
//FlashAddr[1] --Add_M
//FlashAddr[2] --Add_L
INT8U ReadData;
//-----------------------------------------------------------------------------
__ramfunc void Inram_EEData_WriteBlock(INT16U BlockNumber,INT8U* pData)
{
    INT8U i;
    
    if((BlockNumber==0)||(BlockNumber==1))  //STM8L052R8T6仅有Block zero and Block one
    {
    }
    else
    {
        while(1);
    }
    Addr_L=(0x1000&0x0000FF);
    Addr_M=((0x1000>>8)&0x0000FF);
    Addr_H=0x00;
    while(BlockNumber)
    {
        Addr_L+=BlockSize;
        if(Addr_L<BlockSize)
        {
            Addr_M+=1;   //进位
            if(Addr_M<1)
            {
                Addr_H+=1;
            }
        }
        BlockNumber--;
    }

    FLASH_CR1_bit.FIX=1;
    FLASH_DUKR=0xAE;
    FLASH_DUKR=0x56;
    while(0==(FLASH_IAPSR&0x08));  //waite DUL FlashAddr 0x1000--0x10FF
    FLASH_CR2=(0x01);              //Program Block
    for(i=0;i<128;i++)
    {
        ReadData=pData[i];
        asm("LD   A,ReadData"); 
        asm("LDF [FlashAddr.e],A"); 
        Addr_L+=1;
        if(Addr_L<1)
        {
            Addr_M+=1;   //进位
            if(Addr_M<1)
            {
                Addr_H+=1;
            }
        }
    }
    while(0==(FLASH_IAPSR&0x04));  //Waite EOF
    while(0==(FLASH_IAPSR&0x40));  //Waite HVOF
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//EEData块写
//-----------------------------------------------------------------------------
void EEData_WriteBlock(INT16U BlockNumber,INT8U* pData)
{
    __disable_interrupt();
    Inram_EEData_WriteBlock(BlockNumber,pData);
    __enable_interrupt();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//写入
//-----------------------------------------------------------------------------
void EEP_WriteFlashBytes(INT8U* WrData,INT32U FlashAddr,INT8U Length)
{
    INT8U BlockBuff[128];
    INT8U i;
    INT8U BuffIndex=0;
    INT8U BuffSize=sizeof(BlockBuff);
    INT16U FlashBlockNumber;
    
    _ClearArrayData(BlockBuff[BuffIndex],BuffIndex,BuffSize);
    for(i=0;i<Length;i++)
    {
        BlockBuff[i]=WrData[i];
    }
    if(0x00001000==FlashAddr)
    {
        FlashBlockNumber=0;
    }
    else if(0x00001080==FlashAddr)
    {
        FlashBlockNumber=1;
    }
    else
    {
        while(1);
    }
    EEData_WriteBlock(FlashBlockNumber,&BlockBuff[0]);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void EEP_ReadFalshBytes(INT8U* pRead,INT32U Flashaddr,INT8U Length)
{
    INT8U i;
    
    for(i=0;i<Length;i++)
    {
        pRead[i]=*((__far INT8U*)(Flashaddr+i));
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++