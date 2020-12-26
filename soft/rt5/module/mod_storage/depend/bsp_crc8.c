#include "stdint.h"
#include <stdbool.h>



uint8_t bsp_makeCrc8Byte(uint8_t writeCnt,uint8_t len,uint8_t *ptDat)
{
    uint8_t checkByte = 0;;
    uint8_t i = 0;  
    checkByte = writeCnt;
    for(;i < len;i++)
    {
        checkByte += ptDat[i];
    }   
    return (checkByte ^ 0x45);
}

bool bsp_checkCrc8Byte(uint8_t len,uint8_t *ptDat)
{
    uint8_t checkByte = 0;
    checkByte = bsp_makeCrc8Byte(0,(len - 1),ptDat);
    if(checkByte == ptDat[len - 1])
    {
        return true;
    }
    return false;  
}
