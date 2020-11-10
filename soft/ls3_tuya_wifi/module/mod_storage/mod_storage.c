//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\BSP_FlashMemory.h"
#include ".\depend\SnailMacro.h"
#include ".\mod_storage.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MCU系列内置Flash存储模块
//采用Block的方式存储.
//8Bit操作模式
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    INT32U  BlockAddress;
    INT8U   BlockSizeByte;
    INT8U   StorageLength;
    void(*ReadFalshBytes)(INT8U* pRead,INT32U Flashaddr,INT8U Length);       //Read many Bytes
    void(*WriteFlashBytes)(INT8U* WrData,INT32U FlashAddr,INT8U Length);     //Wrtie may Bytes  
}BlockFlashParamter_Def;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BlockFlashParamter_Def StoRunParameterBlock;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//1Byte写入次数+数据+1Byte的校验的方式
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称;产生数据校验和,8Bit
//入口:WrCnt   写入次数
//     pData   需要校验的指针
//     Length  数据长度
//出口:校验值
//-----------------------------------------------------------------------------
INT8U Make_CheckCs(INT8U WrCnt,INT8U* pData,INT8U Length)
{
    INT8U Cs;
    INT8U i;
   
    Cs=WrCnt;
    i=0;
    while(Length)
    {
        Cs+=pData[i];
        i++;
        Length--;
    }
    return(Cs^0x45);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数据块错误检查
//入口:数据长度
//出口:错误代码
//-----------------------------------------------------------------------------
#define  ErrOneBit      0x01     //1区错误
#define  ErrTwoBit      0x02     //2区错误
//-----------------------------------------------------------------------------
INT8U ChecBlockError(BlockFlashParamter_Def *Me)
{
    INT8U ReadCs;
    INT8U MakeCs;
    INT8U ErrorBit;
    INT8U ReadBuffer[128];
    
    if((Me->StorageLength)>(sizeof(ReadBuffer)))
    {
        while(1);
    }
    ErrorBit=0;
    Me->ReadFalshBytes(&ReadBuffer[0],(Me->BlockAddress),(Me->StorageLength+2));
    ReadCs=ReadBuffer[Me->StorageLength+1];
    
    MakeCs=Make_CheckCs(0,&ReadBuffer[0],(Me->StorageLength+1));  //入口次数为0,写成次数包含在首地址的两个字节
    
    if(MakeCs!=ReadCs)
    {
        ErrorBit|=ErrOneBit;
    }
    Me->ReadFalshBytes(&ReadBuffer[0],(Me->BlockAddress+Me->BlockSizeByte),(Me->StorageLength+2));
    ReadCs=ReadBuffer[Me->StorageLength+1];
    MakeCs=Make_CheckCs(0,&ReadBuffer[0],(Me->StorageLength+1));
    if(MakeCs!=ReadCs)
    {
        ErrorBit|=ErrTwoBit;
    }
    return(ErrorBit);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//读取数据块
//入口:数据块编号,Ram指针
//出口:数据是否错误,0xff--数据错误
//Example:
//    if(ReadBlockOfFlash(&StoParaBlock,&StoBootParamter.StoData[0]))
//    {//Error
//    }
//-----------------------------------------------------------------------------
BOOLEAN ReadBlockOfFlash(BlockFlashParamter_Def *Me,INT8U* pRamAddr)
{
    INT8U   Error;
    INT8U  ReadWrCnt1;
    INT8U  ReadWrCnt2;
    INT8U  j;
    INT8U ReadBuffer[128];
    
    Error=ChecBlockError(Me);
    if((Error&ErrOneBit)&&(Error&ErrTwoBit))
    {
        return(BN_TRUE);
    }
    else
    {
        if(Error&ErrOneBit)  //取2段
        {
            Me->ReadFalshBytes(&ReadBuffer[0],(Me->BlockAddress+Me->BlockSizeByte),(Me->StorageLength+2));
            
        }
        else if(Error&ErrTwoBit) //取1段
        {
            Me->ReadFalshBytes(&ReadBuffer[0],(Me->BlockAddress),(Me->StorageLength+2));
        }
        else
        {
            Me->ReadFalshBytes(&ReadWrCnt1,(Me->BlockAddress),1);
            Me->ReadFalshBytes(&ReadWrCnt2,(Me->BlockAddress+Me->BlockSizeByte),1);          
            
            while((ReadWrCnt1==0xff)||(ReadWrCnt2==0xff))
            {
                ReadWrCnt1=ReadWrCnt1+1;
                ReadWrCnt2=ReadWrCnt2+1;
            }
            if(ReadWrCnt1>ReadWrCnt2)
            {
                Me->ReadFalshBytes(&ReadBuffer[0],(Me->BlockAddress),(Me->StorageLength+2));
            }
            else
            {
                Me->ReadFalshBytes(&ReadBuffer[0],(Me->BlockAddress+Me->BlockSizeByte),(Me->StorageLength+2));
            }
        }
        
        for(j=0;j<(Me->StorageLength);j++)
        {
            *pRamAddr=ReadBuffer[j+1];
            pRamAddr++;
        }
    }
    return(BN_FALSE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//入口:Block参数指针 Me,写入数据的指针  pRamAddr.
//Example:WriteBlockOfFlash(&StoParaBlock,&StoBootParamter.StoData[0]);
//-----------------------------------------------------------------------------
void WriteBlockOfFlash(BlockFlashParamter_Def* Me,INT8U* pRamAddr)
{
    INT8U   Error;
    INT8U   ReadWrCnt1;
    INT8U   ReadWrCnt2;
    INT8U   NextWrCnt;
    INT8U   i;
    INT8U   MakeCs;
    INT8U   WriteBuffer[128];
    INT32U  WriteAddr;
    
    Error=ChecBlockError(Me);
    if((Error&ErrOneBit)&&(Error&ErrTwoBit))
    {
        WriteAddr=(Me->BlockAddress);
        NextWrCnt=0;
    }
    else
    {
        if(Error&ErrOneBit)  //取1段
        {
            WriteAddr=(Me->BlockAddress);
            Me->ReadFalshBytes(&NextWrCnt,(Me->BlockAddress+Me->BlockSizeByte),1);
            NextWrCnt+=1;
        }
        else if(Error&ErrTwoBit) //取2段
        {
            WriteAddr=(Me->BlockAddress+Me->BlockSizeByte);
            Me->ReadFalshBytes(&NextWrCnt,(Me->BlockAddress),1);
            NextWrCnt+=1;
        }
        else
        {
            Me->ReadFalshBytes(&ReadWrCnt1,(Me->BlockAddress),1);
            Me->ReadFalshBytes(&ReadWrCnt2,(Me->BlockAddress+Me->BlockSizeByte),1);
            INT8U Bak_WRCnt1,Bak_WRCnt2;
            
            Bak_WRCnt1 = ReadWrCnt1;
            Bak_WRCnt2 = ReadWrCnt2;
            while((ReadWrCnt1==0xff)||(ReadWrCnt2==0xff))
            {
                ReadWrCnt1=ReadWrCnt1+1;
                ReadWrCnt2=ReadWrCnt2+1;
            }
            if(ReadWrCnt1>ReadWrCnt2)
            {
                WriteAddr=(Me->BlockAddress+Me->BlockSizeByte);
                NextWrCnt=Bak_WRCnt1+1;
            }
            else
            {
                WriteAddr=(Me->BlockAddress);
                NextWrCnt=Bak_WRCnt2+1;
            }
        }
    }
    MakeCs=Make_CheckCs(NextWrCnt,pRamAddr,(Me->StorageLength));
    WriteBuffer[0]=NextWrCnt;
    for(i=0;i<Me->StorageLength;i++)
    {
        WriteBuffer[i+1]=pRamAddr[i];
    }
    WriteBuffer[(Me->StorageLength+1)]=MakeCs;
    
    Me->WriteFlashBytes(WriteBuffer,WriteAddr,(Me->StorageLength+2));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SetStorageSizeOf8Bits(BlockFlashParamter_Def *Me,INT8U SizeOfByte)
{
    if(SizeOfByte>126)
    {
        while(1);   //非法数据
    }
    else
    {
        Me->StorageLength=SizeOfByte;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//运行参赛存储和读取
//-----------------------------------------------------------------------------
void StoRunParameter_Configure(INT8U _In_StoLength)
{
    StoRunParameterBlock.BlockAddress=0x00001000;
    StoRunParameterBlock.BlockSizeByte=128;
    SetStorageSizeOf8Bits(&StoRunParameterBlock,_In_StoLength);
    StoRunParameterBlock.ReadFalshBytes=Prg_ReadFlashBytes;
    StoRunParameterBlock.WriteFlashBytes=Prg_WriteEepromBytes;
}
//-----------------------------------------------------------------------------
BOOLEAN ReadFlash_StoRunParameter(INT8U* _Out_pRamAddr)
{
    return(ReadBlockOfFlash(&StoRunParameterBlock,_Out_pRamAddr));
}
//-----------------------------------------------------------------------------
void WriteFlash_StoRunParameter(INT8U* _In_pRamAddr)
{
    INT8U i;
    for(i=0;i<2;i++)
    {
        WriteBlockOfFlash(&StoRunParameterBlock,_In_pRamAddr);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//backup         0x01 0400---0x01 FFFF  264-511

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define OFFSET_TPRG_SIZE       0x00007C00
#define BACKUP_STRADDR         0x00010400
#define OFFSET_INFADDR         (OFFSET_TPRG_SIZE-12)
#define OFFSET_SUMADDR         (OFFSET_TPRG_SIZE-8)
#define OFFSET_SIZEADDR        (OFFSET_TPRG_SIZE-4)

#define BACKUP_PRGINFADDR_START  0x00017F80
#define BACKUP_PRGINFADDR        0x00017FF4
#define BACKUP_PRGSUMADDR        0x00017FF8
#define BACKUP_PRGSIZEADDR       0x00017FFC
//-----------------------------------------------------------------------------
#define FirstBlockNumber   264
#define LastBlockNumber    511      //最后一个块
//#define ApplicationAddress     0x0000B000
//#define HardwareTypeAddr       ApplicationAddress+(0x000080)  //4 Block stern
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 写入Flash一个块的数据
//功能: 
//入口: _In_UpdataNum    升级序号 0...
//      _In_pData        升级数据
//出口: 无
//-----------------------------------------------------------------------------
void WriteOneBlockOfUpdate(INT16U _In_UpdataNum,INT8U* _In_pData)
{
    if((FirstBlockNumber+(_In_UpdataNum))>LastBlockNumber)
    {
    }
    else
    {
        ProgramFlash_WriteBlock(FirstBlockNumber+(_In_UpdataNum),_In_pData);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CheckSumWordEven          0x39ea2e76
#define CheckSumWordOdd           0x82b453c3
//----------------------------------------------------------------------------
INT32U MakeOneCheckText(INT32U Data,INT32U Count)
{
    if(Count&0x00000001)
    {
        Data=Data^CheckSumWordOdd;
    }
    else
    {
        Data=Data^CheckSumWordEven;
    }

    return(Data);
}
//--------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
INT32U ReadFlash32Bits(INT32U StartAddr)
{
    INT8U ReadBuff[4];
    INT32U ReadFlashdata;
    
    ReadBuff[0]=*((__far INT8U*)(StartAddr));
    ReadBuff[1]=*((__far INT8U*)(StartAddr+1));
    ReadBuff[2]=*((__far INT8U*)(StartAddr+2));
    ReadBuff[3]=*((__far INT8U*)(StartAddr+3));
    
    ReadFlashdata=ReadBuff[0];
    ReadFlashdata=ReadFlashdata<<8;
    ReadFlashdata|=ReadBuff[1];
    ReadFlashdata=ReadFlashdata<<8;
    ReadFlashdata|=ReadBuff[2];
    ReadFlashdata=ReadFlashdata<<8;
    ReadFlashdata|=ReadBuff[3];
    return(ReadFlashdata);    
}
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//检测程序代码的校验和是否合法
//BN_TRUE:合法
/*
//-----------------------------------------------------------------------------
BOOLEAN checkPrgDataLegalIsOk(void)
{
    INT32U FlashAddr;
    INT32U ReadFlashdata;
    INT32U MakeSum;
    INT32U iWords,iReadSum;
    INT32U i;

    FlashAddr = BACKUP_STRADDR;
    MakeSum=0;
    iWords = ReadFlash32Bits(BACKUP_STRADDR + (INT32U)OFFSET_SIZEADDR) / 4;//换算成4byts的word模式
    iReadSum = ReadFlash32Bits(BACKUP_STRADDR + (INT32U)OFFSET_SUMADDR);
    
    if((iWords < (OFFSET_TPRG_SIZE/4))&&(iWords > 0))
    {
        for(i=0;i<iWords;i++)
        {
            ReadFlashdata = ReadFlash32Bits(FlashAddr);
            MakeSum += MakeOneCheckText(ReadFlashdata,i);
            FlashAddr += 4;    
            #ifdef NDEBUG
            mod_watchdog_reload();
            #endif
        }
        if(MakeSum == iReadSum)
        {
            return(BN_TRUE);
        }
    }
    return(BN_FALSE);
}
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ReadFlash_InfUpdate(backup_updateInf_def* out_pInformation)
{
    out_pInformation->buinf_update_sign = ReadFlash32Bits(BACKUP_PRGINFADDR);
    out_pInformation->buinf_checkSum = ReadFlash32Bits(BACKUP_PRGSUMADDR);
    out_pInformation->buinf_prgSize_byte = ReadFlash32Bits(BACKUP_PRGSIZEADDR);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void WriteFlash_InfUpdate(backup_updateInf_def in_information)
{
    INT8U buffer[128];
    
    Prg_ReadFlashBytes(&buffer[0],BACKUP_PRGINFADDR_START,128);
    buffer[116] = in_information.buinf_update_sign>>24;
    buffer[117] = in_information.buinf_update_sign>>16;
    buffer[118] = in_information.buinf_update_sign>>8;
    buffer[119] = in_information.buinf_update_sign;
      
    buffer[120] = in_information.buinf_checkSum >> 24;
    buffer[121] = in_information.buinf_checkSum >> 16;
    buffer[122] = in_information.buinf_checkSum >> 8;
    buffer[123] = in_information.buinf_checkSum; 
      
    buffer[124] = in_information.buinf_prgSize_byte >> 24;
    buffer[125] = in_information.buinf_prgSize_byte >> 16;
    buffer[126] = in_information.buinf_prgSize_byte >> 8;
    buffer[127] = in_information.buinf_prgSize_byte;
      
    Prg_WriteFlashBytes(&buffer[0],BACKUP_PRGINFADDR_START,128);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
BOOLEAN CheckHardwareTypeIsFail(INT32U _In_Htype,INT16U _In_UpdataNum)
{
    if(_In_UpdataNum == 2)
    {
        if(_In_Htype != (ReadFlash32Bits(HardwareTypeAddr)))
        {
            return(BN_TRUE);
        }
    }
    return(BN_FALSE);
}*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++