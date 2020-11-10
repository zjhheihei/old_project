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
DelayTickClk_Def  ExClockSecond={Type_Second,0,0};  //��ʼ������������ʱ����
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
        
        
        if(MillisecondTickClockRun(&OneClockMs))  //ֱ���ж���ʱ������
        {
            ReloadMillisecondTickClock(&OneClockMs,200);  //200msѭ����ʱ
        }
        
        MillisecondTickClockRun(&TwoClockMs);
        MillisecondTickClockRun(&ThreeClockMs);
        if(GetMillisecondTickClockFninshed(&TwoClockMs))  //����Finish�����ж���ʱ������
        {
            //ReloadMillisecondTickClock(&OneClockMs,500); 
            ReloadMillisecondTickClock(&ThreeClockMs,30);
        }
        
        if(GetMillsecondOnceTriggerFinished(&ThreeClockMs))  //��ʱ���δ���
        {
            ReloadMillisecondTickClock(&OneClockMs,1500); 
        }
//------------------------------------------------------------------------------------------------
        DelayTickClockRun(&ExClockSecond);
        DelayTickClockRun(&ExClockMinute);
        
        if(DelayTickClockFinished(&ExClockSecond))   
        {
            ReloadDealyTickClock(&ExClockSecond,10); //10s��ʱ
        }
           
        if(DelayTickClockFinished(&ExClockMinute))
        {
            ReloadDealyTickClock(&ExClockMinute,5);  //5m��ʱ
        }
    }
}
//End++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++