
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
#include ".\depend\bsp_key.h"
#include ".\mod_key.h"
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
static INT16U                 LastKeyWord;
static INT16U                 KeyWordOfChange;                //当前改变值
static INT16U                 KeyWordOfState;                 //当前状态值
static INT16U                 MaskOnceKeyWord;                //屏蔽一次按键
static INT16U                 KeyInContinue;                  //连续检测该键


static INT16U                 fristLongHitTime;
static INT16U                 continueHitTime;
//------------------------------E N D-------------------------------------------


static INT16U keyGroupAndAloneMap[4];
void mod_key_configure(void)
{
    if(sizeof(keyGroupAndAloneMap) >= 4)
    {
        bsp_key_cofigure(&keyGroupAndAloneMap[0]);   
    }   
}


BOOLEAN mod_key_getEvent(void)
{
    if(NoneKeyOccur != KeyEvent)
    {
        KeyEvent = NoneKeyOccur;
        return BN_TRUE;
    }
    return BN_FALSE;
} 

void mod_key_maskOnceOperation(INT16U _In_KeyInBit)
{
    MaskOnceKeyWord |= _In_KeyInBit;
}

void mod_key_reCaptureOperation(INT16U _In_KeyInBit)
{
    LastKeyWord   &= ~_In_KeyInBit;
    KeyInContinue &= ~_In_KeyInBit; 
}

INT16U ReadKeyAloneBits(void)
{
    INT16U  ReadPinBits;
    INT16U  ReadKeyWordBits;       
    ReadPinBits = 0;
    ReadKeyWordBits = 0;
    
    INT8U i = 0;
    for(;i < 8;i++)
    {
        if(bsp_getKey(i))
        {
            ReadPinBits |= (1<<i);
        }
    }
   
    ReadKeyWordBits = ReadPinBits;
    
    if((ReadPinBits == keyGroupAndAloneMap[0])&&(0 != keyGroupAndAloneMap[0]))
    {
        ReadPinBits |= KeyGroupBit0;
        ReadKeyWordBits |= KeyGroupBit0;
        ReadKeyWordBits &= ~keyGroupAndAloneMap[0]; 
        MaskOnceKeyWord |= keyGroupAndAloneMap[0];  
    }
    if((ReadPinBits == keyGroupAndAloneMap[1])&&(0 != keyGroupAndAloneMap[1]))
    {
        ReadPinBits |= KeyGroupBit1;
        ReadKeyWordBits |= KeyGroupBit1;
        ReadKeyWordBits &= ~keyGroupAndAloneMap[1]; 
        MaskOnceKeyWord |= keyGroupAndAloneMap[1];  
    }
    if((ReadPinBits == keyGroupAndAloneMap[2])&&(0 != keyGroupAndAloneMap[2]))
    {
        ReadPinBits |= KeyGroupBit2;
        ReadKeyWordBits |= KeyGroupBit2;
        ReadKeyWordBits &= ~keyGroupAndAloneMap[2]; 
        MaskOnceKeyWord |= keyGroupAndAloneMap[2];  
    }
    if((ReadPinBits == keyGroupAndAloneMap[3])&&(0 != keyGroupAndAloneMap[3]))
    {
        ReadPinBits |= KeyGroupBit3;
        ReadKeyWordBits |= KeyGroupBit3;
        ReadKeyWordBits &= ~keyGroupAndAloneMap[3]; 
        MaskOnceKeyWord |= keyGroupAndAloneMap[3];  
    }
    return(ReadKeyWordBits);
}

void mod_key_runTimeHandle(INT16U milliscond)
{
    static INT16U lastTick = 0;
           INT16U diffTick = 0;
    diffTick = milliscond - lastTick;
    lastTick = milliscond;
    if(0 != fristLongHitTime)
    {
        if(fristLongHitTime >= diffTick)
        {
            fristLongHitTime -= diffTick; 
        }
        else
        {
            fristLongHitTime = 0;
        }
    }
    
    if(0 != continueHitTime)
    {
        if(continueHitTime >= diffTick)
        {
            continueHitTime -= diffTick; 
        }
        else
        {
            continueHitTime = 0;
        }
    }    
}

void mod_key_scanTask(INT16U milliscond)
{
    static BOOLEAN initFlag = BN_TRUE;
    static INT8U ChangeCnt;   //电平改变计数
    static INT8U StateCnt;    //电平状态值计数
    INT16U ReadKeyWord;
    if(initFlag)
    {
        mod_key_configure();
        initFlag = BN_FALSE;
    }
    else
    {       
        mod_key_runTimeHandle(milliscond);
        ReadKeyWord = ReadKeyAloneBits();
        if(ReadKeyWord^LastKeyWord)
        {
            StateCnt = 0;
            ChangeCnt++;
            if(ChangeCnt > 5)
            {
                ChangeCnt = 0;
                KeyWordOfChange = ReadKeyWord^LastKeyWord;
                LastKeyWord = ReadKeyWord;
                KeyWordOfState = ReadKeyWord;
                KeyEvent = KeyChangeOccur;
                return;
            }
        } 
        else
        {
            ChangeCnt=0;
            if(ReadKeyWord != 0)
            {
                StateCnt++;
                if(StateCnt > 5)
                {
                    StateCnt = 0;
                    KeyWordOfState = ReadKeyWord;
                    KeyEvent = KeyContinueOccur;
                    return;
                }
            }
            else
            {
                ChangeCnt = 0;
                StateCnt = 0;
                KeyWordOfState = 0;
                MaskOnceKeyWord = 0;
                KeyInContinue = 0;
            }
        }
        KeyEvent=NoneKeyOccur;
    }   
}

keyActionType_t mod_keyOperation(INT16U keyNum,INT16U fristValid_ms,INT16U continueBlink_ms)
{
    if(MaskOnceKeyWord & keyNum)
    {
        return(PUSH_NONE);
    }   
    if(KeyInContinue & keyNum)  //处理该键的连续按键
    {
        if(KeyWordOfChange & keyNum)
        {
            KeyWordOfChange = 0;
            if(KeyWordOfState & keyNum)
            {
            }
            else
            {
                return(PUSH_UP);
            }
        }
        if(KeyWordOfState & keyNum)
        {           
            if((0 == fristLongHitTime) && (0 == continueHitTime))
            {
                continueHitTime = continueBlink_ms;
                return(PUSH_CONTINUE);
            }
        } 
    }
    else  
    {
        if(KeyWordOfChange & keyNum)
        {
            KeyWordOfChange=0;
            if(MaskOnceKeyWord & keyNum)
            {
                MaskOnceKeyWord &= ~keyNum;
                return(PUSH_NONE);
            }    
            else if(KeyWordOfState & keyNum)
            {
                if(fristValid_ms == NOCONTINUE)
                {
                    if(KeyWordOfState == keyNum)
                    {                       
                        fristLongHitTime = 0;
                        continueHitTime = 0;
                    }    
                    return(PUSH_DOWN); 
                }
                else
                {
                    KeyInContinue |= keyNum;
                    fristLongHitTime = fristValid_ms;
                    continueHitTime = 0;                   
                    return(PUSH_DOWN);  
                }                  
            }
            else
            {
                return(PUSH_UP);
            }
        }        
    }
    return(PUSH_NONE);
}






