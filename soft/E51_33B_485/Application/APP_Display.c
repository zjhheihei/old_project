/*******************************************************************************
*
*	模块名称 : 显示模块
*	文件名称 : APP.Display.c
*	版    本 : V1.0
*	说    明 : 1传出压入数据
*                  2扫描任务
                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//------------------------------E N D-------------------------------------------

#include ".\APP_Configure.h"



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************本模块使用的局部变量************************************
static APP_DisplayParameter_Def       APP_Dis_Para;              //显示参数
static APP_StorageParameter_Def      *APP_DisSto_Para;           //存储参数
static APP_RTC_Parameter_Def         *APP_DisRTC_Para;           //RTC参数
static INT16S                        APP_DisInsideTemperarure;  //温度参数
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************传出本模块参数***************************************
APP_DisplayParameter_Def *APP_Display_PutOutParameter(void)
{
      return &APP_Dis_Para;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************传入本模块参数***************************************
void APP_Display_PutInPara(APP_DisplayParameter_Def *_In_me)
{
     APP_Dis_Para.DisMainStatus.RunStatus = _In_me->DisMainStatus.RunStatus;
     APP_Dis_Para.DisMainStatus.FalshFlag.OnceTrigger = _In_me->DisMainStatus.FalshFlag.OnceTrigger;
     
     APP_Dis_Para.DisRTCSetPara.FalshFlag.Leave = _In_me->DisRTCSetPara.FalshFlag.Leave;
     APP_Dis_Para.DisRTCSetPara.FalshFlag.MiniteHour = _In_me->DisRTCSetPara.FalshFlag.MiniteHour;
     APP_Dis_Para.DisRTCSetPara.FalshFlag.SetTrigger = _In_me->DisRTCSetPara.FalshFlag.SetTrigger;
     APP_Dis_Para.DisRTCSetPara.FalshFlag.SelectTrigger = _In_me->DisRTCSetPara.FalshFlag.SelectTrigger;
     APP_Dis_Para.DisRTCSetPara.SetStatus = _In_me->DisRTCSetPara.SetStatus;
     
     APP_Dis_Para.DisParaSetPara.SetStatus = _In_me->DisParaSetPara.SetStatus;
     APP_Dis_Para.DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag = _In_me-> DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag;
     APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep = _In_me->DisParaSetPara.FalshFlag.FacFinishStep;
     
     APP_Dis_Para.DisDayProSetPara.SetStatus = _In_me->DisDayProSetPara.SetStatus;
     
     APP_Dis_Para.DisSoftStartPara.RunStatus = _In_me->DisSoftStartPara.RunStatus;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************获取其他函数参数*************************************
void APP_Display_GetParameter(void)
{
     APP_DisSto_Para = APP_Storage_PutOutParameter();
     APP_DisRTC_Para = APP_RTC_PutOutPara();
     APP_DisInsideTemperarure = APP_Temp_GetTemperature();   
     
     if(StatusDayProOn & APP_DisSto_Para->DeviceStatusBits)
     {
                    
     }
     else
     {
             if(DisAutoMode == APP_Dis_Para.DisMainStatus.RunStatus)
             {
                    APP_Dis_Para.DisMainStatus.RunStatus = DisHandMode;
             }
     }
     
     if(StatusShutDown & APP_DisSto_Para->DeviceStatusBits) //关机
     {
             APP_Dis_Para.DisMainStatus.RunStatus = DisShutDownMode;
     }
     else                                                  
     {       
             if((DisHandMode == APP_Dis_Para.DisMainStatus.RunStatus) ||
                (DisAutoMode == APP_Dis_Para.DisMainStatus.RunStatus))
             {
                     if(StatusAuto & APP_DisSto_Para->DeviceStatusBits)
                     {
                            APP_Dis_Para.DisMainStatus.RunStatus = DisAutoMode;
                     }
                     else
                     {
                            APP_Dis_Para.DisMainStatus.RunStatus = DisHandMode;
                     }
             }
             else
             {
                     if(DisShutDownMode == APP_Dis_Para.DisMainStatus.RunStatus)
                     {
                           APP_Dis_Para.DisMainStatus.RunStatus = DisSoftStartMode;
                     }
             }
             
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************通讯显示******************************************
void APP_Display_CommunicationDis(void)
{
     if(APP_Rs485_PutOutStatus())
     {
            Dis_Communication_Add |= Dis_Communication_Dat;
     }
     else
     {
            Dis_Communication_Add &= ~Dis_Communication_Dat;
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************清理滤网显示***************************************
void APP_Display_CleanDis(void)
{
     static MilliscondTickClk_Def     FalshTime;  
     static BOOLEAN                   FalshFlag = BN_FALSE;
     
     MillisecondTickClockRun(&FalshTime);
     
     if(GetMillisecondTickClockFninshed(&FalshTime))
     {
            ReloadMillisecondTickClock(&FalshTime,500);
            FalshFlag ^= BN_TRUE;        
     }
     
     if(StatusChangStrainer & APP_DisSto_Para->DeviceStatusBits)
     {
            if(FalshFlag)
            {
                  Dis_Clean_Add |=  Dis_Clean_Dat;
            }
            else
            {
                  Dis_Clean_Add &= ~Dis_Clean_Dat;
            }
     }
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************软启动函数****************************************

void APP_Display_SoftStartDis(void)
{
     static DelayTickClk_Def    SoftStartDelayTime= {Type_Second,3,0};
     INT8U Buff[4];
     
     DelayTickClockRun(&SoftStartDelayTime);
     
     switch(APP_Dis_Para.DisSoftStartPara.RunStatus)
     {
            case DisALL:
            {
                 ReloadDealyTickClock(&SoftStartDelayTime,1);
                 Mod_LCD_SetAll(BN_TRUE,AnyOther);
                 Mod_LCD_SetAll(BN_TRUE,FanLeaf);
                 Mod_LCD_RefreshRam(AnyOther);
                 Mod_LCD_RefreshRam(FanLeaf);
                 APP_Dis_Para.DisSoftStartPara.RunStatus = DisPV;
                 break;
            }
            case DisPV:
            {
                 if(DelayTickClockFinished(&SoftStartDelayTime))
                 {
                      ReloadDealyTickClock(&SoftStartDelayTime,1);
                      APP_Dis_Para.DisSoftStartPara.RunStatus = DisWait; 
                 }         
                 break;
            }
            case DisWait:
            {
                      Mod_LCD_SetAll(BN_FALSE,AnyOther);
                      Mod_LCD_SetAll(BN_FALSE,FanLeaf);
                      
                      Buff[0] = ((_DeviceType_ >> 12) & 0x000f);
                      Buff[1] = ((_DeviceType_ >>  8) & 0x000f);
                      Buff[2] = ((_DeviceType_ >>  4) & 0x000f);
                      Buff[3] = ((_DeviceType_      ) & 0x000f);
                      Mod_LCD_UpDateDigtal(Buff,DisDigtal);                
                      switch(_Modified_)
                      {                          
                             case 0x1807:
                             {
                                    Dis_0Clock_Add |= Dis_0Clock_Dat;
                                    Dis_1Clock_Add |= Dis_1Clock_Dat;
                                    Dis_2Clock_Add |= Dis_2Clock_Dat;
                                    Dis_3Clock_Add |= Dis_3Clock_Dat;
                                    Dis_4Clock_Add |= Dis_4Clock_Dat;
                                    Dis_5Clock_Add |= Dis_5Clock_Dat;
                                    Dis_6Clock_Add |= Dis_6Clock_Dat;
                                    break;
                             }   
                             case 0x1808:
                             {
                                    Dis_0Clock_Add |= Dis_0Clock_Dat;
                                    Dis_1Clock_Add |= Dis_1Clock_Dat;
                                    Dis_2Clock_Add |= Dis_2Clock_Dat;
                                    Dis_3Clock_Add |= Dis_3Clock_Dat;
                                    Dis_4Clock_Add |= Dis_4Clock_Dat;
                                    Dis_5Clock_Add |= Dis_5Clock_Dat;
                                    Dis_6Clock_Add |= Dis_6Clock_Dat;
                                    Dis_7Clock_Add |= Dis_7Clock_Dat;
                                    break;
                             } 
                             case 0x1909:
                             {
                                    Dis_0Clock_Add |= Dis_0Clock_Dat;
                                    Dis_1Clock_Add |= Dis_1Clock_Dat;
                                    Dis_2Clock_Add |= Dis_2Clock_Dat;
                                    Dis_3Clock_Add |= Dis_3Clock_Dat;
                                    Dis_4Clock_Add |= Dis_4Clock_Dat;
                                    Dis_5Clock_Add |= Dis_5Clock_Dat;
                                    Dis_6Clock_Add |= Dis_6Clock_Dat;
                                    Dis_7Clock_Add |= Dis_7Clock_Dat;                                    
                                    Dis_8Clock_Add |= Dis_8Clock_Dat;
                                  break;
                             }
                             case 0x2001:
                             {
                                  Dis_0Clock_Add |= Dis_0Clock_Dat;
                                    Dis_1Clock_Add |= Dis_1Clock_Dat;
                                    Dis_2Clock_Add |= Dis_2Clock_Dat;
                                    Dis_3Clock_Add |= Dis_3Clock_Dat;
                                    Dis_4Clock_Add |= Dis_4Clock_Dat;
                                    Dis_5Clock_Add |= Dis_5Clock_Dat;
                                    Dis_6Clock_Add |= Dis_6Clock_Dat;
                                    Dis_7Clock_Add |= Dis_7Clock_Dat;                                    
                                    Dis_8Clock_Add |= Dis_8Clock_Dat;
                                    Dis_9Clock_Add |= Dis_9Clock_Dat;
                                  break;
                             }
                             default :
                             {
                                    break;
                             }  
                      }
                      Mod_LCD_RefreshRam(AnyOther);
                      Mod_LCD_RefreshRam(FanLeaf);
                      if(DelayTickClockFinished(&SoftStartDelayTime))
                      {
                             APP_Dis_Para.DisSoftStartPara.RunStatus = LeaveMode;
                      }
                      break;
            }
            case LeaveMode:
            {
                     APP_Dis_Para.DisSoftStartPara.RunStatus = DisALL;
                     APP_Display_GetParameter();
                     APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 200; 
                     
                     if(StatusShutDown & APP_DisSto_Para->DeviceStatusBits) //关机
                     {
                              APP_Dis_Para.DisMainStatus.RunStatus = DisShutDownMode;
                     }
                     else
                     {
                              if(StatusAuto & APP_DisSto_Para->DeviceStatusBits) //自动模式
                              {
                                        APP_Dis_Para.DisMainStatus.RunStatus = DisAutoMode;
                              }
                              else
                              {
                                        APP_Dis_Para.DisMainStatus.RunStatus = DisHandMode;  //手动模式
                              }
                      } 
                     
                     APP_Dis_Para.DisMainStatus.FalshFlag.OnceTrigger = BN_TRUE;
                     break;
            }
      }    
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************天编程显示*****************************************
void APP_Display_DayProgramDis(void)
{
     INT8U i;
     INT8U Bits = 0x01;
     for(i = 0;i < 25;i++)
     {
          if(0 == Bits)
          { 
                Bits = 0x01;
          }
          
          if((i >= APP_DisSto_Para->DayProPara.StartHour ) &&(i <= APP_DisSto_Para->DayProPara.StopHour))
          {
                if(i < 8)
                {
                      Dis_0_7_Clock_Add |= Bits;
                      Bits <<= 1;               
                }
                
                if((i > 7) && (i < 16))
                {   
                      Dis_8_15_Clock_Add |= Bits;
                      Bits <<= 1; 
                }
                
                if((i >15) && (i < 24))
                {
                      Dis_16_23_Clock_Add |= Bits;
                      Bits <<= 1; 
                }
                if(24 == i)
                {
                      Dis_24Clock_Add |=  Dis_24Clock_Dat;
                }
          }
          else
          {
                if(i < 8)
                {
                      Bits <<= 1;  
                }
                 
                if((i > 7) && (i < 16))
                {
                      Bits <<= 1; 
                }
                 
                if((i >15) && (i < 24))
                {
                      Bits <<= 1; 
                }
          }
     }
     if((APP_DisRTC_Para->RTC.Hour >= APP_DisSto_Para->DayProPara.StartHour)&&
        (APP_DisRTC_Para->RTC.Hour < APP_DisSto_Para->DayProPara.StopHour))
     {
            switch(APP_DisSto_Para->FanHandWorkSpeed)
            {
                    case SpeedLow:
                    {
                          Dis_FanLow_Add |= Dis_FanLow_Dat;
                          APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 200;
                          break;
                    }
                    case SpeedMid:
                    {
                         Dis_FanMid_Add |= Dis_FanMid_Dat;
                         APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 150;
                         break;
                    }
                    case SpeedHigh:
                    {
                         Dis_FanHigh_Add |= Dis_FanHigh_Dat;
                         APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 100;
                         break;
                    } 
            }
            APP_Dis_Para.DisMainStatus.FalshFlag.AutoNoFanFlag = BN_FALSE;
     }
     else
     {
            Dis_FanSpeed_A_Add |= Dis_FanSpeed_A_Dat;
            Dis_FanSpeed_B_Add |= Dis_FanSpeed_B_Dat;
            Dis_FanSpeed_C_Add |= Dis_FanSpeed_C_Dat;
            APP_Dis_Para.DisMainStatus.FalshFlag.AutoNoFanFlag = BN_TRUE;
            Mod_LCD_RefreshRam(FanLeaf);
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************天编程设置显示**************************************
void APP_Display_DayProgramSetDis(void)
{
     INT8U   iCount = 0;
     INT8U   Bits = 0x01;
     INT8U   Buff[4];
     
     for(iCount = 0;iCount < 25;iCount++)
     {
          if(0 == Bits)
          { 
                Bits = 0x01;
          }
          
          if((iCount >= APP_DisSto_Para->DayProPara.StartHour) &&(iCount <= APP_DisSto_Para->DayProPara.StopHour))
          {           
                if(iCount < 8)
                {
                      Dis_0_7_Clock_Add |= Bits;
                      Bits <<= 1;          
                }
                
                if((iCount > 7) && (iCount < 16))
                {   
                      Dis_8_15_Clock_Add |= Bits;
                      Bits <<= 1; 
                }
                
                if((iCount >15) && (iCount < 24))
                {
                      Dis_16_23_Clock_Add |= Bits;
                      Bits <<= 1; 
                }
                
                if( iCount > 23)
                {
                      Dis_24Clock_Add |= Dis_24Clock_Dat;
                }
                
          }
          else
          {
                if(iCount < 8)
                {
                      Bits <<= 1;  
                }
                 
                if((iCount > 7) && (iCount < 16))
                {
                      Bits <<= 1; 
                }
                 
                if((iCount >15) && (iCount < 24))
                {
                      Bits <<= 1; 
                }
          }
     } 
     
     switch(APP_Dis_Para.DisDayProSetPara.SetStatus)
     {
            case SetStartHour:
            {
                 Buff[0] = APP_DisSto_Para->DayProPara.StartHour/10;
                 Buff[1] = APP_DisSto_Para->DayProPara.StartHour%10;
                 Buff[2] = TableBlack;
                 Buff[3] = TableBlack;
                 break;
            }
            case SetStopHour:
            {
                 Buff[2] = APP_DisSto_Para->DayProPara.StopHour/10;
                 Buff[3] = APP_DisSto_Para->DayProPara.StopHour%10;
                 Buff[0] = TableBlack;
                 Buff[1] = TableBlack;
                 break;
            }   
            default:
            {
                 break;
            }
     }
     Dis_Hour_Add |= Dis_Hour_Dat;
     Mod_LCD_UpDateDigtal(Buff,DisDigtal);
}



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************************关机********************************************
void App_Display_ShutDown(void)
{
     INT8U Buff[4];
     Buff[0] = BLACK;
     Buff[1] = CHAR_0;
     Buff[2] = CHAR_F;
     Buff[3] = CHAR_F;
     Mod_LCD_UpDateDigtal(Buff,DisChar);    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************时间设置******************************************
void App_Display_RtcSetDis(void)
{
     INT8U Buff[4];  
     static BOOLEAN               MH_Flash = BN_FALSE;
     static BOOLEAN               StopFlashTimeFinishEnable = BN_FALSE;   
     static MilliscondTickClk_Def MiniteSecondFalshTime;
     static DelayTickClk_Def      StopFalshTime = {Type_Second,0,0};
     static DelayTickClk_Def      LeaveTimeSetModeTime = {Type_Second,0,0};
     
     DelayTickClockRun(&StopFalshTime);
     DelayTickClockRun(&LeaveTimeSetModeTime);
     MillisecondTickClockRun(&MiniteSecondFalshTime);
     
     
     Dis_Col_Add |= Dis_Col_Dat;     
     Dis_SumClock_Add |= Dis_SumClock_Dat;  
     Buff[0] = APP_DisRTC_Para->RTC.Hour/10;
     Buff[1] = APP_DisRTC_Para->RTC.Hour%10;
     Buff[2] = APP_DisRTC_Para->RTC.Minite/10;
     Buff[3] = APP_DisRTC_Para->RTC.Minite%10;
     
     switch(APP_Dis_Para.DisRTCSetPara.SetStatus)
     {
            case InToRTCSetMode:                                       //进入时钟设置
            {
                  ReloadDealyTickClock(&LeaveTimeSetModeTime,30);      //设置无按键操作离开时间
                  APP_Dis_Para.DisRTCSetPara.SetStatus = SetMinite;   //调到分钟设置
                  break;
            }
            case  SetMinite:
            {
                  if(APP_Dis_Para.DisRTCSetPara.FalshFlag.MiniteHour)   //允许闪烁
                  {
                        if(BN_TRUE == MH_Flash)                         //闪烁为空的时候
                        {
                               Buff[2] = 10;
                               Buff[3] = 10;     
                        }
                  }
                  break;
            }
            case  SetHour:
            {
                  if(APP_Dis_Para.DisRTCSetPara.FalshFlag.MiniteHour)   //允许闪烁
                  {
                        if(BN_TRUE == MH_Flash)                         //闪烁为空的时候 
                        {
                               Buff[0] = 10;
                               Buff[1] = 10;     
                        }
                  }
                  break;
            }
            case  LeaveRTCSetMode:                                      //离开时间设定模块
            {
                  APP_Dis_Para.DisRTCSetPara.SetStatus = LeavedRTCSetMode;
                  APP_DisRTC_Para->RTCWriteEnable = BN_TRUE;
                  APP_DisRTC_Para->RTC.Second = 0;                        //秒针清零
                  APP_RTC_PutInPara(APP_DisRTC_Para);                   //数据压入RTC模块
                  break;
            }
     }
     
     if(APP_Dis_Para.DisRTCSetPara.FalshFlag.SetTrigger)                //停止闪烁
     {
            ReloadDealyTickClock(&StopFalshTime,3);            
            ReloadDealyTickClock(&LeaveTimeSetModeTime,30); 
            StopFlashTimeFinishEnable = BN_TRUE;
            APP_Dis_Para.DisRTCSetPara.FalshFlag.SetTrigger = BN_FALSE;
            APP_Dis_Para.DisRTCSetPara.FalshFlag.MiniteHour = BN_FALSE;
            
     }
     
     if(APP_Dis_Para.DisRTCSetPara.FalshFlag.SelectTrigger)                //停止闪烁
     {        
            StopFlashTimeFinishEnable = BN_FALSE;
            APP_Dis_Para.DisRTCSetPara.FalshFlag.MiniteHour = BN_TRUE;
            ReloadDealyTickClock(&LeaveTimeSetModeTime,30); 
            APP_Dis_Para.DisRTCSetPara.FalshFlag.SelectTrigger =BN_FALSE;
     }
          
     if(DelayTickClockFinished(&LeaveTimeSetModeTime)) 
     {
            ReloadDealyTickClock(&LeaveTimeSetModeTime,30); 
            APP_Dis_Para.DisRTCSetPara.SetStatus = LeaveRTCSetMode;
     }
     
     if(GetMillisecondTickClockFninshed(&MiniteSecondFalshTime))
     {
            ReloadMillisecondTickClock(&MiniteSecondFalshTime,500);
            if(MH_Flash) 
            {
                 MH_Flash = BN_FALSE;
            }
            else
            {
                 MH_Flash = BN_TRUE;
            }
     }
     
     if(DelayTickClockFinished(&StopFalshTime)) 
     {
            if(StopFlashTimeFinishEnable)
            {
                    APP_Dis_Para.DisRTCSetPara.FalshFlag.MiniteHour = BN_TRUE;
                    StopFlashTimeFinishEnable = BN_FALSE;
            }
     }
     
     Mod_LCD_UpDateDigtal(Buff,DisDigtal);            
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************时钟显示******************************************
void APP_Display_RTCDis(void)
{
     INT8U Buff[4];     

     if(APP_Dis_Para.DisMainStatus.FalshFlag.SecondEvent)
     {
          Dis_Col_Add |= Dis_Col_Dat;
     } 

     Dis_SumClock_Add |= Dis_SumClock_Dat;
     Buff[0] = APP_DisRTC_Para->RTC.Hour/10;
     Buff[1] = APP_DisRTC_Para->RTC.Hour%10;
     Buff[2] = APP_DisRTC_Para->RTC.Minite/10;
     Buff[3] = APP_DisRTC_Para->RTC.Minite%10;
     Mod_LCD_UpDateDigtal(Buff,DisDigtal);    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************温度显示******************************************
void APP_Display_TempDis(void)
{    
     INT8U Buff[4];
    
     if(0x7fff != APP_DisInsideTemperarure)
     {
           Dis_RoomTemp_Add |= Dis_RoomTemp_Dat;
           Dis_TempSymbol_P_Add |= Dis_TempSymbol_P_Dat;   
           Dis_Point_Add |= Dis_Point_Dat;
           if(APP_DisInsideTemperarure >= 0)
           {
               Buff[0] = APP_DisInsideTemperarure/100;
               Buff[1] = APP_DisInsideTemperarure%100/10;
               Buff[2] = APP_DisInsideTemperarure%10; 
               
               if(0 == Buff[0])
               {
                    Buff[0] = TableBlack;
               } 
           }
           
           if(APP_DisInsideTemperarure < 0)
           {
               Buff[0] = TableNegative;
               Buff[1] = (~APP_DisInsideTemperarure+1)/10;
               Buff[2] = (~APP_DisInsideTemperarure+1)%10; 
           }          
           Buff[3]  = TableUnitC;   
           Mod_LCD_UpDateDigtal(Buff,DisDigtal);  
     }
     else
     {
           
           Buff[0] = BLACK; 
           Buff[1] = CHAR_E;
           Buff[2] = CHAR_R;
           Buff[3] = CHAR_0;
           Mod_LCD_UpDateDigtal(Buff,DisChar);  
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************高级子函数和主函数************************************
void APP_Display_Para_AdjTempDis(void)
{
      INT8U Buff[4];
      static DelayTickClk_Def          GoBackToCurrentTempDelayTime= {Type_Second,0,0};
                       
      DelayTickClockRun(&GoBackToCurrentTempDelayTime);     
      Dis_TempSymbol_P_Add |= Dis_TempSymbol_P_Dat;      //温度符号
      Dis_Point_Add |= Dis_Point_Dat;                    //小数点
      
      if(APP_DisSto_Para->CalibrateTemperature >= 0)                  
      {
            Buff[0] = TableBlack;
            Buff[1] = APP_DisSto_Para->CalibrateTemperature/10;
            Buff[2] = APP_DisSto_Para->CalibrateTemperature%10; 
      }
      else
      {
             Buff[0] = TableNegative;
             Buff[1] = (~APP_DisSto_Para->CalibrateTemperature+1)/10;
             Buff[2] = (~APP_DisSto_Para->CalibrateTemperature+1)%10; 
      } 
      
      Buff[3]  = TableUnitC;   
      Mod_LCD_UpDateDigtal(Buff,DisDigtal); 
      
      if(APP_Dis_Para.DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag)
      {
            APP_Dis_Para.DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag = BN_FALSE;
            ReloadDealyTickClock(&GoBackToCurrentTempDelayTime,5);
      }
      
      if(DelayTickClockFinished(&GoBackToCurrentTempDelayTime))
      {     
            APP_Dis_Para.DisParaSetPara.SetStatus = CurrentTempDis;
      }  
}


void APP_Display_Para_FanGearsSetDis(void)
{
      INT8U Buff[4];
      
      Dis_FanSpeed_A_Add |= Dis_FanSpeed_A_Dat;
      Dis_FanSpeed_B_Add |= Dis_FanSpeed_B_Dat;
      Dis_FanSpeed_C_Add |= Dis_FanSpeed_C_Dat;
      Mod_LCD_RefreshRam(FanLeaf);
      Dis_FanLow_Add  |= Dis_FanLow_Dat;    
      Dis_FanHigh_Add |= Dis_FanHigh_Dat;
      
      if(ThreeSpeed == APP_DisSto_Para->FanGearsSelect)
      {
           Dis_FanMid_Add  |= Dis_FanMid_Dat; 
      }
      
      Buff[0] = CHAR_2;
      Buff[1] = CHAR_F;
      Buff[2] = CHAR_A; 
      Buff[3] = CHAR_N;
      Mod_LCD_UpDateDigtal(Buff,DisChar); 
}


void APP_Display_Para_CleanTimeSetDis(void)
{
     INT8U Buff[4];
     
     Dis_Clean_Add |= Dis_Clean_Dat;
     Buff[0] = 3;
     Buff[1] = APP_DisSto_Para->CleanPara.SetCleanDay/100;
     Buff[2] = APP_DisSto_Para->CleanPara.SetCleanDay%100/10;
     Buff[3] = APP_DisSto_Para->CleanPara.SetCleanDay%10;
     
     if(0 == Buff[1])
     {
         Buff[1] = TableBlack;
     }
     
     Mod_LCD_UpDateDigtal(Buff,DisDigtal);  
}


void APP_Display_Para_BackLightSetDis(void)
{ 
     INT8U Buff[4];
     
     Buff[0] = 4;
     Buff[1] = TableCharL;
     if(APP_DisSto_Para->BacklightTimeWorkTime > 60)
     {
          Buff[2] = 0;
          Buff[3] = TableCharN;
     }
     else
     {
          Buff[2] = APP_DisSto_Para->BacklightTimeWorkTime%100/10;
          Buff[3] = APP_DisSto_Para->BacklightTimeWorkTime%10;
     }
     
     Mod_LCD_UpDateDigtal(Buff,DisDigtal); 
}


void APP_Display_Para_SlaveAddressDis(void)
{
     INT8U Buff[4];
     
     Buff[0] = 5;
     Buff[1] = TableCharA;
     Buff[2] = APP_DisSto_Para->Rs485Para.SlaveAddress/10;
     Buff[3] = APP_DisSto_Para->Rs485Para.SlaveAddress%10;
     Mod_LCD_UpDateDigtal(Buff,DisDigtal); 
}


void APP_Display_Para_BaudrateDis(void)
{
     INT8U Buff[4];     
     Buff[0] = 6;
     Buff[1] = APP_DisSto_Para->Rs485Para.Baudrate / 10000;
     Buff[2] = APP_DisSto_Para->Rs485Para.Baudrate % 10000 / 1000;
     Buff[3] = APP_DisSto_Para->Rs485Para.Baudrate % 1000 / 100;
     if(0 == Buff[1])
     {
           Buff[1] = TableBlack;
     }
     Mod_LCD_UpDateDigtal(Buff,DisDigtal); 
}

void APP_Display_Para_ParityDis(void)
{
      INT8U Buff[4];
      
      Buff[0] = CHAR_7;
      if(NoneParity == APP_DisSto_Para->Rs485Para.Parity)
      {
            Buff[1] = CHAR_N;
            Buff[2] = CHAR_o;
            Buff[3] = CHAR_N;
      }
      else if(EvenParity == APP_DisSto_Para->Rs485Para.Parity)
      {
            Buff[1] = CHAR_E;
            Buff[2] = CHAR_E;
            Buff[3] = CHAR_N;
      }
      else if(OddParity == APP_DisSto_Para->Rs485Para.Parity)
      {
            Buff[1] = CHAR_O;
            Buff[2] = CHAR_D;
            Buff[3] = CHAR_D;
      }
      Mod_LCD_UpDateDigtal(Buff,DisChar); 
}


void APP_Display_Para_StopBitsDis(void)
{
     INT8U Buff[4]; 
     Buff[0] = CHAR_8;
     Buff[3] = BLACK;
     if(Bit_1_0 == APP_DisSto_Para->Rs485Para.StopBits)
     {           
           Buff[1] = BLACK;
           Buff[2] = CHAR_1;          
     }
     else if(Bit_1_5 == APP_DisSto_Para->Rs485Para.StopBits)
     {          
           Buff[1] = CHAR_1;
           Buff[2] = CHAR_5;          
           Dis_Point_Add |= Dis_Point_Dat;
     }
     else if(Bit_2_0 == APP_DisSto_Para->Rs485Para.StopBits)
     {
           Buff[1] = BLACK;
           Buff[2] = CHAR_2;  
     }
     Mod_LCD_UpDateDigtal(Buff,DisChar);  
}


void APP_Display_Para_DayProSelectDis(void)
{
      INT8U Buff[4];
      
      Buff[0] = CHAR_9;
      Buff[1] = CHAR_P;
      Buff[2] = CHAR_O;
      Buff[3] = CHAR_F;
      if(StatusDayProOn & APP_DisSto_Para->DeviceStatusBits)
      {
            Buff[2] = CHAR_O;
            Buff[3] = CHAR_N;
      }
      Mod_LCD_UpDateDigtal(Buff,DisChar); 
}


void APP_Display_Para_FacWaitDis(void)
{
      INT8U Buff[4]; 
      static  MilliscondTickClk_Def  FalshTime;
      static  BOOLEAN                FalshFlag = BN_FALSE;
      MillisecondTickClockRun(&FalshTime);
      
      Buff[0] = CHAR_A;
      Buff[1] = BLACK;
      Buff[2] = BLACK;
      Buff[3] = BLACK;
      
      if(GetMillisecondTickClockFninshed(&FalshTime))
      {
            FalshFlag ^= BN_TRUE;
      }
      
      if(BN_TRUE == FalshFlag)
      {
            Buff[1] = CHAR_NEGATIVE;
      }
      
      Mod_LCD_UpDateDigtal(Buff,DisChar);
}


void APP_Display_Para_FacFinishDis(void)
{
      INT8U Buff[4];      
      
      Buff[0] = CHAR_A;
      Buff[1] = CHAR_NEGATIVE;
     
      if(FinishFristStep == APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep)
      {        
            Buff[2] = BLACK;
            Buff[3] = BLACK; 
            APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep = FinishSecondStep;
      }
      else if(FinishSecondStep == APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep)
      {
            Buff[2] = CHAR_NEGATIVE;
            Buff[3] = BLACK;
            APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep = FinishThirdStep;
      }
      else if(FinishThirdStep == APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep)
      {
            Buff[2] = CHAR_NEGATIVE;
            Buff[3] = CHAR_NEGATIVE;
      }
      else
      {
            APP_Dis_Para.DisParaSetPara.FalshFlag.FacFinishStep = FinishFristStep;
      } 
      Mod_LCD_UpDateDigtal(Buff,DisChar);
}


void App_Display_ParameterSet(void)
{    
     switch(APP_Dis_Para.DisParaSetPara.SetStatus)
     {
          case CurrentTempDis:
          {           
               APP_Display_TempDis();
               break;
          }
          case AdjTempDis:
          {
               APP_Display_Para_AdjTempDis();
               break;
          }
          case FanGearsSetDis:
          {
               APP_Display_Para_FanGearsSetDis();
               break;
          }
          case CleanTimeSetDis:
          {
               APP_Display_Para_CleanTimeSetDis();
               break;
          }
          case BackLightSetDis:
          {
               APP_Display_Para_BackLightSetDis();
               break;
          }
          case SlaveAddressDis:
          {
               APP_Display_Para_SlaveAddressDis();
               break;
          }
          case BaudrateDis:
          {
               APP_Display_Para_BaudrateDis();
               break;
          }
          case ParityDis:
          {
               APP_Display_Para_ParityDis();
               break;
          }
          case StopBitsDis:
          {
               APP_Display_Para_StopBitsDis();
               break;
          }
          case DayProSelectDis:
          {
               APP_Display_Para_DayProSelectDis();
               break;
          }
          case FacWaitDis:
          {
               APP_Display_Para_FacWaitDis();
               break;
          }
          case FacFinishDis:
          {
               APP_Display_Para_FacFinishDis();
               break;
          }         
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************手动模式显示****************************************
void App_Display_HandModeDis(void)
{    
     Dis_HandWork_Add |= Dis_HandWork_Dat;  //手动
     Dis_RoomTemp_Add |= Dis_RoomTemp_Dat;  //室温
     Dis_FanRoom_Add  |= Dis_FanRoom_Dat;   //风机房子
     
     switch(APP_DisSto_Para->FanHandWorkSpeed)
     {
            case SpeedLow:
            {
                 Dis_FanLow_Add |= Dis_FanLow_Dat;
                 APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 200; 
                 break;
            }
            case SpeedMid:
            {
                 Dis_FanMid_Add |= Dis_FanMid_Dat;
                 APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 150;
                 break;
            }
            case SpeedHigh:
            {
                 Dis_FanHigh_Add |= Dis_FanHigh_Dat;
                 APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 100;
                 break;
            }   
            default:
            {
                 Dis_FanLow_Add |= Dis_FanLow_Dat;
                 APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime = 200;
                 break; 
            }
     }
     APP_Display_CommunicationDis();
     APP_Display_CleanDis();
     APP_Display_TempDis();   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************自动模式显示****************************************
void App_Display_AutoModeDis(void)
{
     static MilliscondTickClk_Def     SecondFalshTime;  
     static DelayTickClk_Def          TempRtcFalshTime= {Type_Second,0,0};
     MillisecondTickClockRun(&SecondFalshTime);
     DelayTickClockRun(&TempRtcFalshTime);
     
     if(DelayTickClockFinished(&TempRtcFalshTime))
     {
            ReloadDealyTickClock(&TempRtcFalshTime,3);
            if(APP_Dis_Para.DisMainStatus.FalshFlag.TempRtcEvent)         
            {
                  APP_Dis_Para.DisMainStatus.FalshFlag.TempRtcEvent = BN_FALSE;
            }
            else
            {
                  APP_Dis_Para.DisMainStatus.FalshFlag.TempRtcEvent = BN_TRUE;
            }
     }
       
     if(GetMillisecondTickClockFninshed(&SecondFalshTime))
     {
            ReloadMillisecondTickClock(&SecondFalshTime,500);
            if(APP_Dis_Para.DisMainStatus.FalshFlag.SecondEvent)
            {
                  APP_Dis_Para.DisMainStatus.FalshFlag.SecondEvent =BN_FALSE;
            }
            else
            {
                  APP_Dis_Para.DisMainStatus.FalshFlag.SecondEvent =BN_TRUE;
            }          
     }
     
     Dis_AutoWork_Add |= Dis_AutoWork_Dat;    // 自动标识符
     Dis_Hour_Add |= Dis_Hour_Dat;            // 小时显示
     Dis_FanRoom_Add  |= Dis_FanRoom_Dat;     //风机房子
     
     if(APP_Dis_Para.DisMainStatus.FalshFlag.TempRtcEvent)
     {
            APP_Display_RTCDis();
     }
     else
     {
            APP_Display_TempDis();
     }
     APP_Display_CommunicationDis();
     APP_Display_CleanDis();
     APP_Display_DayProgramDis();                //天编程显示
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************扇叶转动显示****************************************
typedef enum
{  
    RefreshSpeedA = 0x00,
    RefreshSpeedB = 0x01,
    RefreshSpeedC = 0x02,
}FanLeaf_Def;
void APP_Display_FanLeafModeDis(void)
{
     static FanLeaf_Def  FanLeaf = RefreshSpeedA;
     if(RefreshSpeedA == FanLeaf)
     {   
          Dis_FanSpeed_A_Add |= Dis_FanSpeed_A_Dat;
          Dis_FanSpeed_B_Add |= Dis_FanSpeed_B_Dat;
          FanLeaf = RefreshSpeedB;
     }
     else if(RefreshSpeedB == FanLeaf)
     {         
          Dis_FanSpeed_B_Add |= Dis_FanSpeed_B_Dat;     
          Dis_FanSpeed_C_Add |= Dis_FanSpeed_C_Dat;
          FanLeaf = RefreshSpeedC;
     }
     else if(RefreshSpeedC == FanLeaf)
     {
          Dis_FanSpeed_C_Add |= Dis_FanSpeed_C_Dat;
          Dis_FanSpeed_A_Add |= Dis_FanSpeed_A_Dat;
          FanLeaf = RefreshSpeedA;
     }   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************任务列表******************************************
void APP_Display_TaskList(void)
{
            APP_Display_GetParameter();              
            switch(APP_Dis_Para.DisMainStatus.RunStatus)
            {
                  case DisShutDownMode:
                  {
                        App_Display_ShutDown();
                        break;
                  }
                  case DisRTCSetMode:
                  {
                        App_Display_RtcSetDis();
                        break;
                  }
                  case DisParaSetMode:
                  {
                        App_Display_ParameterSet();
                        break;
                  }
                  case DisHandMode:
                  {
                        App_Display_HandModeDis();
                        break;
                  }
                  case DisAutoMode:
                  {
                        App_Display_AutoModeDis();
                        break;
                  }
                  case DisSoftStartMode:
                  {
                        APP_Display_SoftStartDis();
                        break;
                  }
                  case DisDayProMode:
                  {
                        APP_Display_DayProgramSetDis();
                        break;
                  }
            }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************显示扫描任务****************************************
void APP_Display_ScanTask(void)
{
      static MilliscondTickClk_Def     APP_LCD_GroupRefreshTime;
      static MilliscondTickClk_Def     APP_LCD_FanSpeedRefreshTime;
      
      MillisecondTickClockRun(&APP_LCD_GroupRefreshTime);
      MillisecondTickClockRun(&APP_LCD_FanSpeedRefreshTime);
      
      //////////离开设置状态后，扇叶和其他显示刷新不一致////////////
      if(LeavedRTCSetMode == APP_Dis_Para.DisRTCSetPara.SetStatus)
      {
            APP_Dis_Para.DisRTCSetPara.SetStatus = InToRTCSetMode;
            
            if(StatusAuto & APP_DisSto_Para->DeviceStatusBits)
            {
                  APP_Dis_Para.DisMainStatus.RunStatus = DisAutoMode;
            }
            else
            {
                  APP_Dis_Para.DisMainStatus.RunStatus = DisHandMode;
            }
            
            APP_Dis_Para.DisMainStatus.FalshFlag.OnceTrigger = BN_TRUE;
            
      }
      ///////////////////////////END///////////////////////////////
      if(LeaveDayProMode == APP_Dis_Para.DisDayProSetPara.SetStatus)
      {
            APP_Dis_Para.DisDayProSetPara.SetStatus = SetStartHour;
            
            if(StatusAuto & APP_DisSto_Para->DeviceStatusBits)
            {
                  APP_Dis_Para.DisMainStatus.RunStatus = DisAutoMode;
            }
            else
            {
                  APP_Dis_Para.DisMainStatus.RunStatus = DisHandMode;
            }
            
            APP_Dis_Para.DisMainStatus.FalshFlag.OnceTrigger = BN_TRUE;
            
      }
      ////////////////////500毫秒或单次触发刷新////////////////////
      if(GetMillisecondTickClockFninshed(&APP_LCD_GroupRefreshTime)||
        (APP_Dis_Para.DisMainStatus.FalshFlag.OnceTrigger))
      {        
            
            if(DisSoftStartMode != APP_Dis_Para.DisMainStatus.RunStatus)
            {
                  Mod_LCD_SetAll(BN_FALSE,AnyOther);
            }
            
            APP_Dis_Para.DisMainStatus.FalshFlag.OnceTrigger = BN_FALSE;        
            APP_Display_TaskList();
            ReloadMillisecondTickClock(&APP_LCD_GroupRefreshTime,500);         
            Mod_LCD_RefreshRam(AnyOther);          
      }
      ///////////////////////////END///////////////////////////////

      ////////////////////////扇叶刷新/////////////////////////////
      if((DisAutoMode == APP_Dis_Para.DisMainStatus.RunStatus)||
         (DisHandMode == APP_Dis_Para.DisMainStatus.RunStatus))
      {
            if(GetMillisecondTickClockFninshed(&APP_LCD_FanSpeedRefreshTime))
            {
                  
                  ReloadMillisecondTickClock(&APP_LCD_FanSpeedRefreshTime,APP_Dis_Para.DisMainStatus.FalshFlag.FanLeafTime); 
                  if((DisAutoMode == APP_Dis_Para.DisMainStatus.RunStatus) &&
                      (APP_Dis_Para.DisMainStatus.FalshFlag.AutoNoFanFlag))
                  {
                      
                  }
                  else
                  {
                      Mod_LCD_SetAll(BN_FALSE,FanLeaf);
                      APP_Display_FanLeafModeDis(); 
                      Mod_LCD_RefreshRam(FanLeaf); 
                  }                                               
            }
      }
      else
      {
            if(DisSoftStartMode == APP_Dis_Para.DisMainStatus.RunStatus)
            {
                             
            }
            else
            {
                  if(FanGearsSetDis != APP_Dis_Para.DisParaSetPara.SetStatus)
                  {
                         Mod_LCD_SetAll(BN_FALSE,FanLeaf);          
                         Mod_LCD_RefreshRam(FanLeaf); 
                  }   
            }         
      }
      ///////////////////////////END///////////////////////////////
}
//------------------------------E N D-------------------------------------------



//-----------------------Mod_Keyboard.c--END------------------------------------


