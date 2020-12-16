//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MCU系列内置Flash存储模块
//采用Block的方式存储.
//8Bit操作模式
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_FlashStoBlockOf8Bits.h"
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
BlockFlashParamter_Def StoParaBlock;
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
    INT8U  Error;
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
void EEPFlash_Configure(INT8U _In_StoLength)
{
    StoParaBlock.BlockAddress=0x00001000;
    StoParaBlock.BlockSizeByte=128;
    SetStorageSizeOf8Bits(&StoParaBlock,_In_StoLength);
    StoParaBlock.ReadFalshBytes=EEP_ReadFalshBytes;
    StoParaBlock.WriteFlashBytes=EEP_WriteFlashBytes;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN ReadBlockOfEEPFlash(INT8U* _Out_pRamAddr)
{
    return(ReadBlockOfFlash(&StoParaBlock,_Out_pRamAddr));
}
//-----------------------------------------------------------------------------
void WriteBlockOfEEPFlash(INT8U* _In_pRamAddr)
{
    WriteBlockOfFlash(&StoParaBlock,_In_pRamAddr);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

