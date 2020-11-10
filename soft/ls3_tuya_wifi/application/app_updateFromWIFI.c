//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\APP_Cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//fireware update task via wifi
//-------------------------------------------------------------------------------------------------
typedef enum
{
    fwUpdate_status_waitFilesMap       = 0x00,
    fwUpdate_status_receiveFiles       = 0x01,
    fwUpdate_status_complete           = 0x02,
    fwUpdate_status_restart            = 0x03,
}fwUpdate_status_def;
//-------------------------------------------------------------------------------------------------
static fwUpdate_status_def fwUpdate_status = fwUpdate_status_waitFilesMap;
static INT16U fwBlock_number;
static MilliscondTickClk_Def timer_reset;
static DelayTickClk_Def timer_updateTimeout = {Type_Second,0,0};
//-------------------------------------------------------------------------------------------------
void app_updateViaWIFI_task(void)
{
    MillisecondTickClockRun(&timer_reset);
    DelayTickClockRun(&timer_updateTimeout);
    switch(fwUpdate_status)
    {
        case fwUpdate_status_waitFilesMap:
        {
            break;
        }
        case fwUpdate_status_receiveFiles:
        {
             
            ReloadMillisecondTickClock(&timer_reset,200);
            if(DelayTickClockFinished(&timer_updateTimeout))
            {
                fwUpdate_status = fwUpdate_status_waitFilesMap;
            }
            break;
        }
        case fwUpdate_status_complete:
        {
            if(GetMillisecondTickClockFninshed(&timer_reset))
            {
                fwUpdate_status = fwUpdate_status_restart;
            }
            break;
        }
        case fwUpdate_status_restart:
        {
            while(1); //wait watchdog timeout
            //break;
        }
        default:
        {
            break;
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#define KEYVAL  0x89,0x8E,0x3C,0x72,0xA7,0x19,0xF4,0x5D
//-------------------------------------------------------------------------------------------------
//名称: 文件解码
//功能: 传入密文，传出明文
//入口: 
//        _Mix_binf   文件入口指针                    <<---->>
//        _In_Length  文件长度,必须是128的倍数        <<----
//出口: 
//-------------------------------------------------------------------------------------------------
static void Decrypt(INT8U* _Mix_binf,INT8U _In_Length)
{
    INT8U Key[8]={(INT8U)0x89,(INT8U)0x8E,(INT8U)0x3C,(INT8U)0x72,(INT8U)0xA7,(INT8U)0x19,(INT8U)0xF4,(INT8U)0x5D,};
    INT8U i;
    INT8U Length,BaseIndex;

    if(_In_Length < 128)
    {
        return;
    }
    
    Length = _In_Length;
    BaseIndex = 0;
    while(0!=Length)
    {
        Key[0] = (INT8U)0x89;
        Key[1] = (INT8U)0x8E;
        Key[2] = (INT8U)0x3C;
        Key[3] = (INT8U)0x72;
        Key[4] = (INT8U)0xA7;
        Key[5] = (INT8U)0x19;
        Key[6] = (INT8U)0xF4;
        Key[7] = (INT8U)0x5D;

        for(i=0 ;i<16 ;i++)
        {
            _Mix_binf[0+i*8+BaseIndex] = _Mix_binf[0+i*8+BaseIndex] ^ Key[0];
            _Mix_binf[1+i*8+BaseIndex] = _Mix_binf[1+i*8+BaseIndex] ^ Key[1];
            _Mix_binf[2+i*8+BaseIndex] = _Mix_binf[2+i*8+BaseIndex] ^ Key[2];
            _Mix_binf[3+i*8+BaseIndex] = _Mix_binf[3+i*8+BaseIndex] ^ Key[3];
            _Mix_binf[4+i*8+BaseIndex] = _Mix_binf[4+i*8+BaseIndex] ^ Key[4];
            _Mix_binf[5+i*8+BaseIndex] = _Mix_binf[5+i*8+BaseIndex] ^ Key[5];
            _Mix_binf[6+i*8+BaseIndex] = _Mix_binf[6+i*8+BaseIndex] ^ Key[6];
            _Mix_binf[7+i*8+BaseIndex] = _Mix_binf[7+i*8+BaseIndex] ^ Key[7];

            char lastkeybit;
            if(Key[0] & 0x80)
            {
                lastkeybit = 0x01;
            }
            else
            {
                lastkeybit = 0x00;
            }
            unsigned char ics;
            for(ics=0;ics<7;ics++)
            {
                Key[ics] = Key[ics]<<1;
                if(Key[ics+1] & 0x80)
                {
                    Key[ics] |= 0x01;
                }
            }
            Key[ics] = Key[ics]<<1;
            Key[ics] |= lastkeybit;
        }
        if(Length < 128)
        {
            break;
        }
        Length = Length-128;
        BaseIndex += 128;
    }
}
//--------------------------------------------------------------------------------------------------
static INT32U makeData32u_bigEndian(INT8U* in_data)
{
    INT32U the_data;
    
    the_data = in_data[0];
    the_data = the_data<<8;
    the_data |= in_data[1];
    the_data = the_data<<8;
    the_data |= in_data[2];
    the_data = the_data<<8;
    the_data |= in_data[3];
    return(the_data);
}
//-------------------------------------------------------------------------------------------------
#define INF_STARTUPDATE   0x8D731A75//开始升级
#define INF_NEEDTOUPDATE  0x62ABF5A4//升级申请
#define INF_UPDATED       0x519AE493//升级完毕
//-------------------------------------------------------------------------------------------------
//推送一个块的数据到升级模块，128bytes的数据为一个块
//入口: 数据指针
//-------------------------------------------------------------------------------------------------
void app_pushBlock_toThisUpdate(INT8U*  in_blockData)
{
    INT8U buff_block[128];
    INT8U i;
    
    ReloadDealyTickClock(&timer_updateTimeout,10);    //10s 超时
    for(i = 0; i < 128; i++)
    {
        buff_block[i] = in_blockData[i];
    }
    
    switch(fwUpdate_status)
    {
        case fwUpdate_status_waitFilesMap:
        {
            INT32U checkSum;
            
            checkSum = makeData32u_bigEndian(&buff_block[124]);
            if(checkSum == MakeCheckSumText(&buff_block[0],124))
            {
                backup_updateInf_def the_backup_updateInf;
                
                fwUpdate_status = fwUpdate_status_receiveFiles;
                fwBlock_number = 0;
                the_backup_updateInf.buinf_update_sign = INF_STARTUPDATE;
                the_backup_updateInf.buinf_checkSum = makeData32u_bigEndian(&buff_block[108]); //108 109 110 111
                the_backup_updateInf.buinf_prgSize_byte = makeData32u_bigEndian(&buff_block[112]); //112 113 114 115,116 117 118 119 ,120 121 122 123 ,124 125 126 127 
                WriteFlash_InfUpdate(the_backup_updateInf);              
            }
            break;
        }
        case fwUpdate_status_receiveFiles:
        {
            Decrypt(&buff_block[0],128);
            WriteOneBlockOfUpdate(fwBlock_number, &buff_block[0]);
            fwBlock_number ++;
            
            backup_updateInf_def read_backup_updateInf;
            ReadFlash_InfUpdate(&read_backup_updateInf);
            if(fwBlock_number < (read_backup_updateInf.buinf_prgSize_byte / 128))
            {
            }
            else
            {
                read_backup_updateInf.buinf_update_sign = INF_NEEDTOUPDATE;
                WriteFlash_InfUpdate(read_backup_updateInf);
                fwUpdate_status = fwUpdate_status_complete;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++