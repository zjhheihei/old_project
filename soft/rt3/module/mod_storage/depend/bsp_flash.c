#include ".\SnailDataTypedef.h"
#include ".\SnailMacro.h"
#include "intrinsics.h"
#include "DeviceSelect.h"

#if defined(__STM8L052R8__)
#include ".\iostm8l052r8.h"

#define FLASH_START_ADDR       0X008000
#define BlockSize             128
INT8U FlashAddr[3]; 
#define Addr_H FlashAddr[0] //FlashAddr[0] --Add_H
#define Addr_M FlashAddr[1] //FlashAddr[1] --Add_M
#define Addr_L FlashAddr[2] //FlashAddr[2] --Add_L
INT8U ReadData;
__ramfunc void bsp_flash_writeBlock(INT16U blockNumber,INT8U* ptDat)
{   
    asm("SIM");    
    INT8U i;
    
    Addr_L = (INT8U)(FLASH_START_ADDR & 0x0000FF);
    Addr_M = (INT8U)((PrgFlashStartAddr>>8) & 0x0000FF);
    Addr_H = 0x00;
    while(_In_BlockNumber)
    {
        Addr_L += BlockSize;
        if(Addr_L < BlockSize)
        {
            Addr_M += 1;   //进位
            if(Addr_M < 1)
            {
                Addr_H + =1;
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

#elif defined(__HDSC32L136__)
#endif
