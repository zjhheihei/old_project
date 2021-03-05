//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _PBC_RINGQUEUE_H
#define _PBC_RINGQUEUE_H
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef struct
{
    volatile INT16U    InputIndex;     //输入索引
    volatile INT16U    OutputIndex;    //输出索引
    volatile INT16U    BusyBytes;      //字节数
    INT16U             FIFOSize;       //FIFO大小
}ringQueueParameter_Def;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//Task
//-----------------------------------------------------------------------------
//Event
//-----------------------------------------------------------------------------
//Status
//-----------------------------------------------------------------------------
//Function
BOOLEAN InputRonudQueue(ringQueueParameter_Def *Me,INT8U *Me_Buffer,INT8U Data);
BOOLEAN OutputRoundQueue(ringQueueParameter_Def *Me,INT8U *Me_Buffer,INT8U *pReadData);
INT16U GetQueueBusyBytes(ringQueueParameter_Def *Me);
void RestartRoundQueue(ringQueueParameter_Def *Me);
#endif
//++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++++