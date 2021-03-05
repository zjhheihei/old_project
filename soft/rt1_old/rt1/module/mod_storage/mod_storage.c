//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\BSP_FlashMemory.h"
#include ".\depend\SnailMacro.h"
#include ".\mod_storage.h"
#include ".\depend\bsp_crc8.h"
////////////////////////////////////////////////////////////////////////////////
static INT8U eepromValidLength = 0;
#define EERPOM_MIAN_LENGTH  (eepromValidLength + 2) 

#define NO_BLOCK_ERR         0
#define ONE_BLOCK_ERR        1
#define TWO_BLOCK_ERR        2

void mod_sto_configure(INT8U eepromLength)
{
    eepromValidLength = eepromLength;
    bsp_sto_configure(FLASH_CONFIG_FREQ_4MHZ);
}

BOOLEAN mod_sto_readEepromOneBlock(INT8U blockNum,INT8U *_out_dat)
{
    if((0 == blockNum) || (1 == blockNum))
    {
        INT32U addr = 0;
        addr = (blockNum * EPPROM_ONE_BLOCK_SIZE) + EEPROM_BLOCK_ONE_START_ADDR;
        Prg_ReadFlashBytes(&_out_dat[0],addr,EERPOM_MIAN_LENGTH);
        if(bsp_checkCrc8Byte(EERPOM_MIAN_LENGTH,_out_dat))
        {
            return BN_TRUE;
        }
    }
    return BN_FALSE;
}

INT8U mod_sto_readEeprom(INT8U *_out_block1,INT8U *_out_block2)
{
    INT8U errStatus = NO_BLOCK_ERR;
    if(BN_FALSE == mod_sto_readEepromOneBlock(0,&_out_block1[0]))
    {
        errStatus |= ONE_BLOCK_ERR;
    }
    if(BN_FALSE == mod_sto_readEepromOneBlock(1,&_out_block2[0]))
    {
        errStatus |= TWO_BLOCK_ERR;
    }
    return errStatus;
}

BOOLEAN mod_sto_readStoPara(INT8U *_out_dat)
{
    INT8U block1Buf[128];
    INT8U block2Buf[128];
    INT8U errStatus = NO_BLOCK_ERR;
    errStatus = mod_sto_readEeprom(block1Buf,block2Buf);    
    if((ONE_BLOCK_ERR & errStatus) && (TWO_BLOCK_ERR & errStatus))
    {
        return BN_TRUE;
    }
    else
    {
        INT8U *ptRightBuf;
        INT8U  i = 0;
        if(ONE_BLOCK_ERR & errStatus)
        {
            ptRightBuf = block2Buf;           
        }
        else if(TWO_BLOCK_ERR & errStatus)
        {
            ptRightBuf = block1Buf;
        }
        else
        {
            ptRightBuf = block1Buf; 
        }
        for(; i < eepromValidLength;i++)
        {
            _out_dat[i] = ptRightBuf[i + 1];
        }
        return BN_FALSE;
    }
}

void mod_sto_writeStoPara(INT8U *dat)
{
    INT8U block1Buf[128];
    INT8U block2Buf[128];
    INT8U errStatus = NO_BLOCK_ERR;
    INT8U nextBlcok1WriteCnt = 0;
    INT8U nextBlcok2WriteCnt = 1;
    INT8U block1CheckByte,block2CheckByte;
    errStatus = mod_sto_readEeprom(block1Buf,block2Buf);  
    if((ONE_BLOCK_ERR & errStatus) || (TWO_BLOCK_ERR & errStatus))
    {
        if(ONE_BLOCK_ERR & errStatus)
        {
            nextBlcok1WriteCnt = block2Buf[0] + 1;
            nextBlcok2WriteCnt = nextBlcok1WriteCnt + 1;
        }
        else if(TWO_BLOCK_ERR & errStatus)
        {
            nextBlcok2WriteCnt = block2Buf[0] + 1; 
            nextBlcok1WriteCnt = nextBlcok2WriteCnt + 1;
        }
    }
    else
    {
        nextBlcok1WriteCnt = block1Buf[0] + 2;
        nextBlcok2WriteCnt = block2Buf[0] + 2;        
    }  
    block1CheckByte = bsp_makeCrc8Byte(nextBlcok1WriteCnt,eepromValidLength,dat);    
    block2CheckByte = bsp_makeCrc8Byte(nextBlcok2WriteCnt,eepromValidLength,dat);
    INT8U wirteBuf[128];
    INT8U i = 0;
    for(; i < eepromValidLength;i++)
    {
        wirteBuf[i + 1] = dat[i];
    }
    wirteBuf[0] = nextBlcok1WriteCnt;
    wirteBuf[eepromValidLength + 1] = block1CheckByte;
    Prg_WriteEepromBytes(&wirteBuf[0],EEPROM_BLOCK_ONE_START_ADDR,EERPOM_MIAN_LENGTH);   
    wirteBuf[0] = nextBlcok2WriteCnt;
    wirteBuf[eepromValidLength + 1] = block2CheckByte;  
    Prg_WriteEepromBytes(&wirteBuf[0],(EEPROM_BLOCK_ONE_START_ADDR + EPPROM_ONE_BLOCK_SIZE),EERPOM_MIAN_LENGTH);
}


void WriteOneBlockOfUpdate(INT16U _In_UpdataNum,INT8U* _In_pData)
{
    if((DOWN_LOAD_AREA_FRIST_BLOCK_NUM + (_In_UpdataNum)) <= DOWN_LOAD_AREA_LAST_BLOCK_NUM)
    {
        bsp_flash_write_oneBlock(DOWN_LOAD_AREA_FRIST_BLOCK_NUM + _In_UpdataNum,_In_pData);
    }   
}

#define CheckSumWordEven          0x39ea2e76
#define CheckSumWordOdd           0x82b453c3

INT32U MakeOneCheckText(INT32U Data,INT32U Count)
{
    if(Count&0x00000001)
    {
        Data = Data^CheckSumWordOdd;
    }
    else
    {
        Data = Data^CheckSumWordEven;
    }

    return(Data);
}

INT32U MakeCheckSumText(INT8U* pData,INT32U Length)
{
    INT32U iWords,This_ids;
    INT32U Index;
    INT32U MakeSum,ReadData;
    iWords = (Length)/4;
    This_ids = 0;
    MakeSum = 0;
    Index = 0;
    while(iWords)
    {
        ReadData = (INT32U)pData[Index]&0x000000ff;
        ReadData = ReadData<<8;
        ReadData = ReadData|((INT32U)pData[Index+1]&0x000000ff);
        ReadData = ReadData<<8;
        ReadData = ReadData|((INT32U)pData[Index+2]&0x000000ff);
        ReadData = ReadData<<8;
        ReadData = ReadData|((INT32U)pData[Index+3]&0x000000ff);
        MakeSum+=MakeOneCheckText(ReadData,This_ids);
        Index = Index+4;
        iWords--;
        This_ids++;
    }
    return(MakeSum);
}

INT32U ReadFlash32Bits(INT32U StartAddr)
{
    INT8U ReadBuff[4];
    INT32U ReadFlashdata;
    Prg_ReadFlashBytes(&ReadBuff[0],StartAddr,4); 
    ReadFlashdata=ReadBuff[0];
    ReadFlashdata=ReadFlashdata<<8;
    ReadFlashdata|=ReadBuff[1];
    ReadFlashdata=ReadFlashdata<<8;
    ReadFlashdata|=ReadBuff[2];
    ReadFlashdata=ReadFlashdata<<8;
    ReadFlashdata|=ReadBuff[3];
    return(ReadFlashdata);    
}

void ReadFlash_InfUpdate(backup_updateInf_def* out_pInformation)
{
    out_pInformation->buinf_update_sign  = ReadFlash32Bits(DOWN_LOAD_AREA_INFORMATION_STATUS_ADDR);
    out_pInformation->buinf_checkSum     = ReadFlash32Bits(DOWN_LOAD_AREA_INFORMATION_CHECK_ADDR);
    out_pInformation->buinf_prgSize_byte = ReadFlash32Bits(DOWN_LOAD_AREA_INFORMATION_SIZE_ADDR);
}

void WriteFlash_InfUpdate(backup_updateInf_def in_information)
{
    INT8U buffer[128];   
    Prg_ReadFlashBytes(&buffer[0],DOWN_LOAD_AREA_INFORMATION_BLOCK_ADDR,128);
    buffer[116] = in_information.buinf_update_sign >> 24;
    buffer[117] = in_information.buinf_update_sign >> 16;
    buffer[118] = in_information.buinf_update_sign >> 8;
    buffer[119] = in_information.buinf_update_sign;
      
    buffer[120] = in_information.buinf_checkSum >> 24;
    buffer[121] = in_information.buinf_checkSum >> 16;
    buffer[122] = in_information.buinf_checkSum >> 8;
    buffer[123] = in_information.buinf_checkSum; 
      
    buffer[124] = in_information.buinf_prgSize_byte >> 24;
    buffer[125] = in_information.buinf_prgSize_byte >> 16;
    buffer[126] = in_information.buinf_prgSize_byte >> 8;
    buffer[127] = in_information.buinf_prgSize_byte;    
    Prg_WriteFlashBytes(&buffer[0],DOWN_LOAD_AREA_INFORMATION_BLOCK_ADDR,128);
}