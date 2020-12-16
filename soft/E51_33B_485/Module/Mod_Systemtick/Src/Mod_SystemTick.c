//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//Version  1.0 
//Data     2016-10-20
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//虚拟组件不主动访问其他组件。
//虚拟运行节拍。
//编写4种类型的节拍,【系统最小周期节拍】,【1ms节拍】，【1s节拍】，【1m节拍】。
//-----------------------------------------------------------------------------
//Version   2.0
//Data      2017-11-20
//系统时间运行函数,从中断移出,采用独立函数的方式
//-----------------------------------------------------------------------------
#include ".\Mod_SystemTick.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static  INT16U SysTick_Millisecond;           //1ms系统延时
static  INT16U SysTick_Second;                //1秒系统延时
static  INT16U SysTick_Minute;                //一分钟延时
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SystickTimeRun(void)
{
    static INT8U TickCntBackup;
    INT8U ReadTickCount;
    INT8U GorunCount;
    
    static INT16U Second;
    static INT8U Mintue;
    
    ReadTickCount=Pull_SystickCount();
    GorunCount=ReadTickCount-TickCntBackup;
    TickCntBackup=ReadTickCount;
    while(GorunCount)
    {
        GorunCount--;
        SysTick_Millisecond++;;
        Second++;
        if(Second>999)
        {
            Second=0;
            SysTick_Second++;;
            Mintue++;
            if(Mintue>59)
            {
                Mintue=0;
                SysTick_Minute++;;
            }
        }      
    }
}
//-----------------------------------------------------------------------------
INT16U GetSysTickMillisecond(void)
{
    return(SysTick_Millisecond);
}
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//延时周期
//出口，是否延时完毕,BN_TURE,延时完毕
//-----------------------------------------------------------------------------
BOOLEAN DelayTickClockRun(DelayTickClk_Def* _In_Me)
{
    INT16U ReadCurrentTick;
    INT16U RunTick;
    
    SystickTimeRun();
    if(Type_Second==_In_Me->DelayType)
    {
        ReadCurrentTick=SysTick_Second;
    }
    else if(Type_Minute==_In_Me->DelayType)
    {
        ReadCurrentTick=SysTick_Minute;
    }
    else
    {
        while(1);
    }
    
    RunTick=ReadCurrentTick-_In_Me->KeepSysTick;
    _In_Me->KeepSysTick=ReadCurrentTick;
    
    if(Type_Second==_In_Me->DelayType)
    {
        if(RunTick>3)
        {
            _In_Me->DelayClock=0;
            return(BN_TRUE);
        }
    }
    else if(Type_Minute==_In_Me->DelayType)
    {
        if(RunTick>2)
        {
            _In_Me->DelayClock=0;
            return(BN_TRUE);
        }
    }

    while(RunTick)
    {
        if(0!=_In_Me->DelayClock)
        {
            _In_Me->DelayClock--;
            if(0==_In_Me->DelayClock)
            {
                return(BN_TRUE);
            }
        }
        else
        {
            return(BN_TRUE);
        }
        RunTick--;
    }
    return(BN_FALSE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//延时时间重新装载
//入口:延时类型，延时模块指针，装载数据
//-----------------------------------------------------------------------------
void ReloadDealyTickClock(DelayTickClk_Def* _In_Me,INT16U _In_ReloadData)
{
    DelayTickClockRun(_In_Me);   //update KeepSysTick
    _In_Me->DelayClock=_In_ReloadData;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//定时完成
//BN_TRUE:定时完成,BN_FALSE 定时中
//-----------------------------------------------------------------------------
BOOLEAN DelayTickClockFinished(DelayTickClk_Def* _In_Me)
{
    if(_In_Me->DelayClock==0)
    {
        return(BN_TRUE);
    }
    return(BN_FALSE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//延时周期
//出口，是否延时完毕,BN_TURE,延时完毕
//-----------------------------------------------------------------------------
BOOLEAN MillisecondTickClockRun(MilliscondTickClk_Def* _In_Me)
{
    INT16U ReadCurrentTick;
    INT16U RunTick;
    
    SystickTimeRun();
    ReadCurrentTick=SysTick_Millisecond;
    RunTick=ReadCurrentTick - _In_Me->KeepSysTick;
    _In_Me->KeepSysTick=ReadCurrentTick;
    if(RunTick>2000)  //错误判断
    {
        _In_Me->DelayClock=0;
        return(BN_TRUE);
    }
    while(RunTick)
    {
        if(0!=_In_Me->DelayClock)
        {
            _In_Me->DelayClock--;
            if(0==_In_Me->DelayClock)
            {
                return(BN_TRUE);
            }
        }
        else
        {
            return(BN_TRUE);
        }
        RunTick--;
    }
    return(BN_FALSE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//延时时间重新装载
//入口:延时类型，延时模块指针，装载数据
//-----------------------------------------------------------------------------
void ReloadMillisecondTickClock(MilliscondTickClk_Def* _In_Me,INT16U In_ReloadData)
{
    MillisecondTickClockRun(_In_Me);    //update KeepSysTick
    _In_Me->OnceTrigger=BN_TRUE;
    _In_Me->DelayClock=In_ReloadData;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//定时完成
//BN_TRUE:定时完成,BN_FALSE 定时中
//-----------------------------------------------------------------------------
BOOLEAN GetMillisecondTickClockFninshed(MilliscondTickClk_Def* _In_Me)
{
    if(_In_Me->DelayClock==0)
    {
        return(BN_TRUE);
    }
    return(BN_FALSE);
}
//-----------------------------------------------------------------------------
//一次触发读取
//-----------------------------------------------------------------------------
BOOLEAN GetMillsecondOnceTriggerFinished(MilliscondTickClk_Def* _In_Me)
{
    if(_In_Me->DelayClock==0)
    {
        if(_In_Me->OnceTrigger)
        {
            _In_Me->OnceTrigger=BN_FALSE;
            return(BN_TRUE);
        }
    }
    return(BN_FALSE);
}
//++++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++