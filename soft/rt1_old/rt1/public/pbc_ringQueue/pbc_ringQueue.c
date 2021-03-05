//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//环形队列
//Version   1.0
//-----------------------------------------------------------------------------
#include ".\depend\data_typedef.h"
#include ".\pbc_ringQueue.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//入环形队列
//出口:BN_TRUE 入队列完成 BN_FALSE 进队列失败
//-----------------------------------------------------------------------------
BOOLEAN InputRonudQueue(ringQueueParameter_Def *Me,INT8U *Me_Buffer,INT8U Data)
{
    if(Me->BusyBytes < Me->FIFOSize)
    {
        Me_Buffer[Me->InputIndex] = Data;
        Me->BusyBytes++;
        Me->InputIndex++;
        if(Me->InputIndex<Me->FIFOSize)
        {
        }
        else
        {
            Me->InputIndex=0;
        }
        return(BN_TRUE);
    }
    else
    {
        return(BN_FALSE);     
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//出环形队列
//BN_TRUE  出队列完成,BN_FALSE 无数据出队列
//-----------------------------------------------------------------------------
BOOLEAN OutputRoundQueue(ringQueueParameter_Def *Me,INT8U *Me_Buffer,INT8U *pReadData)
{
    if(Me->BusyBytes!=0)
    {
        *pReadData=Me_Buffer[Me->OutputIndex];
        Me->BusyBytes--;
        Me->OutputIndex++;
        if(Me->OutputIndex<Me->FIFOSize)
        {   
        }
        else
        {
            Me->OutputIndex=0;
        }
        return(BN_TRUE);
    }
    else
    {
        return(BN_FALSE);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//获取队列字节数
//-----------------------------------------------------------------------------
INT16U GetQueueBusyBytes(ringQueueParameter_Def *Me)
{
    return(Me->BusyBytes);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//重启Queue
//-----------------------------------------------------------------------------
void RestartRoundQueue(ringQueueParameter_Def *Me)
{
    Me->BusyBytes=0;
    Me->InputIndex=0;
    Me->OutputIndex=0;
}
//+++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++