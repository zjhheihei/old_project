
/*********************************************************************************************************
*
*	模块名称 : Mod按键模块
*	文件名称 : Mod_Keyboard.c
*	版    本 : V1.0
*	说    明 : 1、模块调用BSP_Keyboar和Mod_SystemTick组件
*                  2、供其他组件调用
*                  3、模块有配置按键、按键处理、按键扫描函数、
*                     重新操作一次按键、屏蔽一次按键功能和获取按键事件。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-07-28  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*********************************************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_Keyboard.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************按键事件类型****************************************
enum
{
    NoneKeyOccur      = 0x00,  //无键盘发生
    KeyChangeOccur    = 0x01,  //状态改变键盘
    KeyContinueOccur   =0x02,  //持续按键键盘
}KeyEvent;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//********************静态变量定义，仅作用与本模块******************************
static INT16U   LastKeyWord;
static INT16U   KeyWordOfChange;                //当前改变值
static INT16U   KeyWordOfState;                 //当前状态值
static INT16U   MaskOnceKeyWord;                //屏蔽一次按键
static INT16U   KeyInContinue;                  //连续检测该键
static MilliscondTickClk_Def  KeyFLongDaley;    //首次长延时
static MilliscondTickClk_Def  KeyCShortDaley;   //连击短延时
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**组合键设置函数，ITF_KeyGroupAndAloneMap[4]存储BSP文件中配置好的组合键键值***
static INT16U ITF_KeyGroupAndAloneMap[4];
void Mod_Key_Configure(void)
{
  INT16U *Point;
  INT8U  i;
  Point = BSP_KeyHardWares_Configure(); 
  for(i=0;i<4;i++)
  {
    ITF_KeyGroupAndAloneMap[i] = *Point;
    Point++;
  }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************获取按键事件***************************************
BOOLEAN Mod_Key_GetEvent(void)
{
    if(NoneKeyOccur == KeyEvent)
    {
        return(BN_FALSE);
    }
    
    KeyEvent = NoneKeyOccur;
    return(BN_TRUE);
} 
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++


//***************屏蔽一次按键、重读一次按键或读取按键事件***********************
BOOLEAN Mod_Key_MaskReCaptureOperation(KeyOperation_Def _In_KeyOperation,INT16U _In_KeyInBit)
{
      if(MaskOnce == _In_KeyOperation)
      {
            MaskOnceKeyWord |= _In_KeyInBit;
      }
      else if(ReCapture == _In_KeyOperation)
      {
            LastKeyWord   &= ~_In_KeyInBit;
            KeyInContinue &= ~_In_KeyInBit; 
      }
     
      return BN_FALSE;
}
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************读取独立键盘字位,自动生成组合键盘***************************
INT16U ReadKeyAloneBits(void)
{
    INT16U ReadPinBits;
    INT16U ReadKeyWordBits;
    
    ReadPinBits = 0;
    ReadKeyWordBits = 0;
    
    if(BSP_GetKey0())
    {
        ReadPinBits|=KeyAloneBit0;
    }
    
    if(BSP_GetKey1())
    {
        ReadPinBits|=KeyAloneBit1;
    }
    
    if(BSP_GetKey2())
    {
        ReadPinBits|=KeyAloneBit2;
    }
    
    if(BSP_GetKey3())
    {
        ReadPinBits|=KeyAloneBit3;
    }
    
    if(BSP_GetKey4())
    {
        ReadPinBits|=KeyAloneBit4;
    }
    
    if(BSP_GetKey5())
    {
        ReadPinBits|=KeyAloneBit5;
    }
    
    if(BSP_GetKey6())
    {
        ReadPinBits|=KeyAloneBit6;
    }
    
    if(BSP_GetKey7())
    {
        ReadPinBits|=KeyAloneBit7;
    }
    
    ReadKeyWordBits = ReadPinBits;
    
    if((ReadPinBits == ITF_KeyGroupAndAloneMap[0])&&(0 != ITF_KeyGroupAndAloneMap[0]))
    {
        ReadPinBits |= KeyGroupBit0;
        ReadKeyWordBits |= KeyGroupBit0;
        ReadKeyWordBits &= ~ITF_KeyGroupAndAloneMap[0]; 
        MaskOnceKeyWord |= ITF_KeyGroupAndAloneMap[0];  
    }
    if((ReadPinBits == ITF_KeyGroupAndAloneMap[1])&&(0 != ITF_KeyGroupAndAloneMap[1]))
    {
        ReadPinBits |= KeyGroupBit1;
        ReadKeyWordBits |= KeyGroupBit1;
        ReadKeyWordBits &= ~ITF_KeyGroupAndAloneMap[1]; 
        MaskOnceKeyWord |= ITF_KeyGroupAndAloneMap[1];  
    }
    if((ReadPinBits == ITF_KeyGroupAndAloneMap[2])&&(0 != ITF_KeyGroupAndAloneMap[2]))
    {
        ReadPinBits |= KeyGroupBit2;
        ReadKeyWordBits |= KeyGroupBit2;
        ReadKeyWordBits &= ~ITF_KeyGroupAndAloneMap[2]; 
        MaskOnceKeyWord |= ITF_KeyGroupAndAloneMap[2];  
    }
    if((ReadPinBits == ITF_KeyGroupAndAloneMap[3])&&(0 != ITF_KeyGroupAndAloneMap[3]))
    {
        ReadPinBits |= KeyGroupBit3;
        ReadKeyWordBits |= KeyGroupBit3;
        ReadKeyWordBits &= ~ITF_KeyGroupAndAloneMap[3]; 
        MaskOnceKeyWord |= ITF_KeyGroupAndAloneMap[3];  
    }
    return(ReadKeyWordBits);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************键盘扫描任务***************************************
void Mod_Key_ScanTask(void)
{
    static INT8U FirstIntoTask = 0;
    static INT8U ChangeCnt;   //电平改变计数
    static INT8U StateCnt;    //电平状态值计数
    INT16U ReadKeyWord;
    
    if(0 == FirstIntoTask)
    {
        Mod_Key_Configure();
        FirstIntoTask = 1;
    }
    else
    {
        MillisecondTickClockRun(&KeyFLongDaley);
        MillisecondTickClockRun(&KeyCShortDaley);    
        ReadKeyWord = ReadKeyAloneBits();
        if(ReadKeyWord^LastKeyWord)
        {
            StateCnt=0;
            ChangeCnt++;
            if(ChangeCnt>5)
            {
                ChangeCnt=0;
                KeyWordOfChange=ReadKeyWord^LastKeyWord;
                LastKeyWord=ReadKeyWord;
                KeyWordOfState=ReadKeyWord;
                KeyEvent=KeyChangeOccur;
                return;
            }
        } 
        else
        {
            ChangeCnt=0;
            if(ReadKeyWord!=0)
            {
                StateCnt++;
                if(StateCnt>5)
                {
                    StateCnt = 0;
                    KeyWordOfState = ReadKeyWord;
                    KeyEvent = KeyContinueOccur;
                    return;
                }
            }
            else
            {
                ChangeCnt=0;
                StateCnt=0;
                KeyWordOfState=0;
                MaskOnceKeyWord=0;
                KeyInContinue=0;
            }
        }
        KeyEvent=NoneKeyOccur;
        }
    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************获取按键******************************************
KeyRetValue_Def Key_ValueOperation(INT16U _IN_KeyInBits,INT16U _IN_KeyFistDelay,INT16U _IN_KeyConDelay)
{
    if(MaskOnceKeyWord & _IN_KeyInBits)
    {
        return(KeyNonePush);
    }
    if(KeyInContinue & _IN_KeyInBits)  //处理该键的连续按键
    {
        if(KeyWordOfChange & _IN_KeyInBits)
        {
            KeyWordOfChange=0;
            if(KeyWordOfState & _IN_KeyInBits)
            {
            }
            else
            {
                return(KeyPushUp);
            }
        }
        if(KeyWordOfState & _IN_KeyInBits)
        {
            if((0==KeyFLongDaley.DelayClock)&&(0==KeyCShortDaley.DelayClock))
            {
                KeyCShortDaley.DelayClock = _IN_KeyConDelay;
                return(KeyContinueDown);
            }
        } 
    }
    else  //处理单击按键
    {
        if(KeyWordOfChange & _IN_KeyInBits)
        {
            KeyWordOfChange=0;
            if(MaskOnceKeyWord & _IN_KeyInBits)
            {
                MaskOnceKeyWord &= ~_IN_KeyInBits;
                return(KeyNonePush);
            }    
            else if(KeyWordOfState & _IN_KeyInBits)
            {
                if(_IN_KeyFistDelay == NOCONTINUE)
                {
                    KeyFLongDaley.DelayClock=0;
                    KeyCShortDaley.DelayClock=0;
                    return(KeyPushDown); 
                }
                else
                {
                    KeyInContinue |= _IN_KeyInBits;
                    KeyFLongDaley.DelayClock = _IN_KeyFistDelay;
                    KeyCShortDaley.DelayClock=0;
                    return(KeyPushDown);  
                }                  
            }
            else
            {
                return(KeyPushUp);
            }
        }        
    }
    return(KeyNonePush);
}
//------------------------------E N D-------------------------------------------


//-----------------------Mod_Keyboard.c--END------------------------------------


