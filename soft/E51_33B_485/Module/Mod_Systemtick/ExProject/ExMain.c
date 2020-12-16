//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MCU: STM8L052R8T6
//IDE: IARSTM8 2.10.4(EWSTM8-2104-Autorun)
//FCPU: HSI-RC-16MHZ
//-------------------------------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Begin++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\.\Src\Mod_SystemTick.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MilliscondTickClk_Def OneClockMs;
MilliscondTickClk_Def TwoClockMs;
MilliscondTickClk_Def ThreeClockMs;
DelayTickClk_Def  ExClockSecond={Type_Second,0,0};  //初始化必须配置延时类型
DelayTickClk_Def  ExClockMinute={Type_Minute,0,0};
//-------------------------------------------------------------------------------------------------
void main(void)
{
    ITF_Systick_Configure();
    while(1)
    {
        SystickTimeRun();
 //------------------------------------------------------------------------------------------------
        MillisecondTickClockRun(&OneClockMs);
        
        
        if(MillisecondTickClockRun(&OneClockMs))  //直接判断延时完成情况
        {
            ReloadMillisecondTickClock(&OneClockMs,200);  //200ms循环定时
        }
        
        MillisecondTickClockRun(&TwoClockMs);
        MillisecondTickClockRun(&ThreeClockMs);
        if(GetMillisecondTickClockFninshed(&TwoClockMs))  //采用Finish函数判断延时完成情况
        {
            //ReloadMillisecondTickClock(&OneClockMs,500); 
            ReloadMillisecondTickClock(&ThreeClockMs,30);
        }
        
        if(GetMillsecondOnceTriggerFinished(&ThreeClockMs))  //延时单次触发
        {
            ReloadMillisecondTickClock(&OneClockMs,1500); 
        }
//------------------------------------------------------------------------------------------------
        DelayTickClockRun(&ExClockSecond);
        DelayTickClockRun(&ExClockMinute);
        
        if(DelayTickClockFinished(&ExClockSecond))   
        {
            ReloadDealyTickClock(&ExClockSecond,10); //10s延时
        }
           
        if(DelayTickClockFinished(&ExClockMinute))
        {
            ReloadDealyTickClock(&ExClockMinute,5);  //5m延时
        }
    }
}
//End++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++