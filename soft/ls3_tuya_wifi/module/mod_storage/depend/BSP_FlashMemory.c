//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include ".\iostm8l052r8.h"
#include "intrinsics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//STM8L052系列 Flash读写支持模块
//采用128Byte片写的方式访问
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PrgFlashStartAddr  0x008000
#define BlockSize          128
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//程序空间块写操作
//入口:BlockNumber 块号 0-511 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称:程序空间块写操作
//功能:写入一个块的数据到程序空间
//入口:   _In_BlockNumber   块编号 从0x0000开始
//        _In_pData         需要存入Flash的数据指针
//-----------------------------------------------------------------------------
INT8U FlashAddr[3]; 
#define Addr_H FlashAddr[0] //FlashAddr[0] --Add_H
#define Addr_M FlashAddr[1] //FlashAddr[1] --Add_M
#define Addr_L FlashAddr[2] //FlashAddr[2] --Add_L
INT8U ReadData;
//-----------------------------------------------------------------------------
__ramfunc void ProgramFlash_WriteBlock(INT16U _In_BlockNumber,INT8U* _In_pData)
{   
    asm("SIM");    //__disable_interrupt
    
    INT8U i;
    
    Addr_L=(PrgFlashStartAddr&0x0000FF);
    Addr_M=((PrgFlashStartAddr>>8)&0x0000FF);
    Addr_H=0x00;
    while(_In_BlockNumber)
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
        _In_BlockNumber--;
    }
    FLASH_CR1_bit.FIX=1;
    FLASH_PUKR=0x56;
    FLASH_PUKR=0xAE;
    while(0==(FLASH_IAPSR&0x02));     //waite PUL Unlock
    FLASH_CR2=(0x01);                 //Program Block
    for(i=0;i<128;i++)
    {
        ReadData=_In_pData[i];
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
    
    FLASH_IAPSR &= ~0x02;   //Lock
    asm("RIM");   //__enable_interrupt();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称:EEPROM空间块写操作
//功能:写入一个块的数据到EEPROM空间
//入口:   _In_BlockNumber   块编号 从0x0000开始
//        _In_pData         需要存入Flash的数据指针
//-----------------------------------------------------------------------------
__ramfunc void EepromFalash_WriteBlock(INT16U _In_BlockNumber,INT8U* _In_pData)
{
    asm("SIM");      //__disable_interrupt

    INT8U i;
    
    if((_In_BlockNumber==0)||(_In_BlockNumber==1))  //STM8L052R8T6仅有Block zero and Block one
    {
    }
    else
    {
        while(1);
    }
    Addr_L=(0x1000&0x0000FF);
    Addr_M=((0x1000>>8)&0x0000FF);
    Addr_H=0x00;
    while(_In_BlockNumber)
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
        _In_BlockNumber--;
    }

    FLASH_CR1_bit.FIX=1;
    FLASH_DUKR=0xAE;
    FLASH_DUKR=0x56;
    while(0==(FLASH_IAPSR&0x08));  //waite DUL Unlock FlashAddr 0x1000--0x10FF
    FLASH_CR2=(0x01);              //Program Block
    for(i=0;i<128;i++)
    {
        ReadData=_In_pData[i];
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
    
    FLASH_IAPSR &= ~0x08;          //Lock
    asm("RIM");                    //__enable_interrupt();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//写入
//-----------------------------------------------------------------------------
void Prg_WriteFlashBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length)
{
    INT8U BlockBuff[128];
    INT8U i;
    INT8U BuffIndex=0;
    INT8U BuffSize=sizeof(BlockBuff);
    INT16U FlashBlockNumber;
    
    if(_In_Length>128)
    {
    }
    else
    {
        _ClearArrayData(BlockBuff[BuffIndex],BuffIndex,BuffSize);
        for(i=0;i<_In_Length;i++)
        {
            BlockBuff[i]=_In_pWRData[i];
        }
        FlashBlockNumber=(_In_FlashAddr-0x8000)/128;
        ProgramFlash_WriteBlock(FlashBlockNumber,&BlockBuff[0]);        
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Prg_WriteEepromBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length)
{
    INT8U BlockBuff[128];
    INT8U i;
    INT8U BuffIndex=0;
    INT8U BuffSize=sizeof(BlockBuff);
    INT16U FlashBlockNumber;
    
    if(_In_Length>128)
    {
    }
    else
    {
        _ClearArrayData(BlockBuff[BuffIndex],BuffIndex,BuffSize);
        for(i=0;i<_In_Length;i++)
        {
            BlockBuff[i]=_In_pWRData[i];
        }
        FlashBlockNumber=(_In_FlashAddr-0x1000)/128;
        EepromFalash_WriteBlock(FlashBlockNumber,&BlockBuff[0]);      
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void Prg_ReadFlashBytes(INT8U* pRead,INT32U Flashaddr,INT8U Length)
{
    INT8U i;
    
    for(i=0;i<Length;i++)
    {
        pRead[i]=*((__far INT8U*)(Flashaddr+i));
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++