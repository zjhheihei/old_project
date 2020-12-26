
#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"


#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"
#define FLASH_START_ADD    0x008000
#define BlockSize          128
INT8U FlashAddr[3]; 
#define Addr_H FlashAddr[0] //FlashAddr[0] --Add_H
#define Addr_M FlashAddr[1] //FlashAddr[1] --Add_M
#define Addr_L FlashAddr[2] //FlashAddr[2] --Add_L
INT8U ReadData;
//-----------------------------------------------------------------------------
BOOLEAN bsp_sto_configure(INT32U u32CfgFreq)
{
    return BN_TRUE;//配套华大版本,华大flash需要初始化
}

__ramfunc void ProgramFlash_WriteBlock(INT16U _In_BlockNumber,INT8U* _In_pData)
{   
    asm("SIM");    //__disable_interrupt
    
    INT8U i;
    
    Addr_L = (FLASH_START_ADD & 0x0000FF);
    Addr_M = ((FLASH_START_ADD >>8) & 0x0000FF);
    Addr_H = 0x00;
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
        FlashBlockNumber=(_In_FlashAddr - 0x8000)/128;
        ProgramFlash_WriteBlock(FlashBlockNumber,&BlockBuff[0]);        
    }
}

void bsp_flash_write_oneBlock(INT16U blockNum,INT8U *ptDat)
{
    ProgramFlash_WriteBlock(blockNum,ptDat);
}

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

void Prg_ReadFlashBytes(INT8U* pRead,INT32U Flashaddr,INT8U Length)
{
    INT8U i;
    
    for(i=0;i<Length;i++)
    {
        pRead[i]=*((__far INT8U*)(Flashaddr+i));
    }
}


#elif defined(__HDSC32L136__)
#include "stdint.h"
#include <stdbool.h>
#include "flash.h"
#include <string.h>

#define FLASH_CONFIG_FREQ_4MHZ      0x01                            
#define FLASH_CONFIG_FREQ_22_12MHZ  0x02                             

#define FLASH_TIMEOUT_INIT         (0xFFu)

#define FLASH_BYPASS()           do{M0P_FLASH->BYPASS = 0x5A5A;\
                                    M0P_FLASH->BYPASS = 0xA5A5; }while(0)

#define Flash_UnlockAll()        do{FLASH_BYPASS();\
                                    M0P_FLASH->SLOCK = 0xFFFFFFFFu;}while(0)

#define Flash_LockAll()          do{FLASH_BYPASS();\
                                    M0P_FLASH->SLOCK = 0;}while(0)
                                      
#define SystemClockFreq             22120000                          
#define SystemClockMHz              22.12                

                                      
BOOLEAN bsp_sto_configure(uint32_t u32CfgFreq)
{
    uint32_t    u32PrgTimer_22_12MHz[8] = {(uint32_t)(0.5+8*SystemClockMHz), (uint32_t)(0.5+5.75*SystemClockMHz), (uint32_t)(0.5+6.75*SystemClockMHz), (uint32_t)(0.5+4500*SystemClockMHz),\
                                           (uint32_t)(0.5+35000*SystemClockMHz), (uint32_t)(0.5+6*SystemClockMHz), (uint32_t)(0.5+60*SystemClockMHz), (uint32_t)(0.5+250*SystemClockMHz)}; 
    uint32_t    u32PrgTimer_4MHz[8] = {0x20, 0x17, 0x1B, 0x4650, 0x222E0, 0x18, 0xF0, 0x3E8};
    uint32_t   *pu32PrgTimer;
    volatile uint32_t  *pu32PrgTimerReg = (volatile uint32_t*)M0P_FLASH;
    volatile uint8_t    u8TimeOutCnt;      
    uint32_t            u32Index  = 0;      

    if (u32CfgFreq == FLASH_CONFIG_FREQ_4MHZ)  //flashӁдʱݤд4MHzƤ׃
    {
        pu32PrgTimer = u32PrgTimer_4MHz;
    }
    else                                      //flashӁдʱݤд22.12MHzƤ׃
    {
        pu32PrgTimer = u32PrgTimer_22_12MHz;
    }
    
    for(u32Index=0; u32Index<8; u32Index++)
    {
        u8TimeOutCnt = FLASH_TIMEOUT_INIT;
        while(pu32PrgTimerReg[u32Index]  != pu32PrgTimer[u32Index])
        {
            if(u8TimeOutCnt--)
            {
                FLASH_BYPASS();
                pu32PrgTimerReg[u32Index] = pu32PrgTimer[u32Index];
            }
            else
            {
                return BN_FALSE;
            }
        }
    }

    return BN_TRUE;
}

bool bsp_sto_pageErase(uint32_t add)
{
    uint32_t addLegalFlag = 0;
    addLegalFlag = add % 512;
    if(0 != addLegalFlag)  //页的首地址写入
    {
        return false;
    }
    volatile uint32_t       u32TimeOut;  
    u32TimeOut = FLASH_TIMEOUT_INIT;
    while(0x02 != M0P_FLASH->CR_f.OP)         //如果flash操作模式不是：页擦除
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = 0x02;        //配置flash操作为：页擦除
        }
        else
        {
            return false;                     //配置操作超时，错误返回
        }
    }        
    
    Flash_UnlockAll();                        //flash擦写 解锁    
    *((volatile uint32_t*)add) = 0; //向sector内写任意数据
    u32TimeOut = FLASH_TIMEOUT_INIT;
    while (TRUE == M0P_FLASH->CR_f.BUSY)      //查询是否擦写成功
    {
        if(0 == u32TimeOut--)
        {
            Flash_LockAll();                  //Flash 加锁
            return false;
        }
    }        
    Flash_LockAll();                          //Flash 加锁
    
    return (true);
}




void bsp_flash_write_oneBlock(INT16U blockNum,INT8U *ptDat)
{
    volatile uint32_t       u32TimeOut;
    uint32_t                i;    
    u32TimeOut = FLASH_TIMEOUT_INIT;
    
    INT32U addr = blockNum * 128;
    #ifndef NDEBUG
    volatile static uint32_t testAddress;
    testAddress = addr;
    #endif
    
    static uint32_t lastPage = 0;
           uint32_t currentPage = 0;
           
    currentPage = addr / 512;
    if(currentPage != lastPage)
    {
        lastPage = currentPage;
        bsp_sto_pageErase(lastPage * 512);
    }
     
    while(0x01 != M0P_FLASH->CR_f.OP)                    
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = 0x01;                   
        }
        else
        {
            return;                         
        }
    }
    
    Flash_UnlockAll();                                   
    for (i=0; i < 128; i++)
    {
        *((volatile uint8_t*)(addr + i)) = ptDat[i];              
        u32TimeOut = FLASH_TIMEOUT_INIT;
        while (TRUE == M0P_FLASH->CR_f.BUSY)             
        {
            if(0 == u32TimeOut--)
            {
                return;                     
            }
        }
    }    
    Flash_LockAll();                                        
}

void Prg_WriteFlashBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length)
{      
    if(128 >= _In_Length)   
    {    
        INT8U BlockBuff[128];
        INT8U i;  
        INT16U FlashBlockNumber;
        for(i = 0;i < _In_Length;i++)
        {
            BlockBuff[i] = _In_pWRData[i];
        }
        FlashBlockNumber = _In_FlashAddr / 128;
         
        bsp_flash_write_oneBlock(FlashBlockNumber,&BlockBuff[0]);        
    }
}

void Prg_WriteEepromBytes(INT8U* _In_pWRData,INT32U _In_FlashAddr,INT8U _In_Length)
{
    if(128 >= _In_Length)   
    {    
        INT8U BlockBuff[128];
        INT8U i;  
        INT16U FlashBlockNumber;
        for(i = 0;i < _In_Length;i++)
        {
            BlockBuff[i] = _In_pWRData[i];
        }
        FlashBlockNumber = _In_FlashAddr / 128;
        #ifndef NDEBUG
        volatile static uint16_t testFlashBlockNumber;
        volatile static uint8_t  testlength;
        testFlashBlockNumber = FlashBlockNumber;
        testlength = _In_Length;
        #endif
        bsp_flash_write_oneBlock(FlashBlockNumber,&BlockBuff[0]);        
    }
}

void Prg_ReadFlashBytes(INT8U* pRead,INT32U Flashaddr,INT8U Length)
{
    INT8U i;  
    for (i = 0; i < Length; i++)
    {
        pRead[i] = *((unsigned char *)(Flashaddr + i));
    }
}

#endif
