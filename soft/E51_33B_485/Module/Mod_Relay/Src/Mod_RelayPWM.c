/*******************************************************************************
*
*	模块名称 : Mod继电器模块
*	文件名称 : Mod_RelayPWM.c
*	版    本 : V1.0
*	说    明 : 3路继电器模块逻辑关系固定，1开2关或全关。为风机的低中高
*                  关闭继电器无延时，开继电器延时2秒
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-027  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_RelayPWM.h"
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************继电器逻辑状态类型*************************************
typedef enum
{
    RelayAllClose      = 0x00,
    RelayOneOpen       = 0x01,
    RelayTwoOpen       = 0x02,
    RelayThreeOpen     = 0x03,
}ModRelayStatus_Def;  
//------------------------------E N D-------------------------------------------




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************PWM输出状态类型**************************************
enum
{
    GotoPWM_100Percent_Out    = 0x00,
    PWM_100Percent_Outing     = 0x01,
    GotoPWM_60Percent_Out     = 0x02,
    PWM_60Percent_Outing      = 0x03,
}PwmOutStatus;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************记录继电器开关变量**************************************
static ModRelayStatus_Def ModRelayStatus;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************继电器开关逻辑控制函数***********************************
void Mod_RealyLogicControl(ModRelayNumber_Def _In_RlyNumber)
{     
     switch(_In_RlyNumber)
     {
            case RelayNone:
            {
                 if(RelayAllClose != ModRelayStatus)  
                 {
                        //BSP_Relay1_OutStatus(BN_FALSE);
                        //BSP_Relay2_OutStatus(BN_FALSE);
                        //BSP_Relay3_OutStatus(BN_FALSE);
                        //产品有可能进入其他状态不扫描继电器任务，又需要立刻关闭继电器可使用上面已经屏蔽的代码。
                        ModRelayStatus = RelayAllClose;
                 }
                 break;
            }
            
            case RelayOne:
            {
                 if(RelayOneOpen != ModRelayStatus)
                 {
                        ModRelayStatus = RelayOneOpen;
                 }
                 break;
            }
            
            case RelayTwo:
            {
                 if(RelayTwoOpen != ModRelayStatus)
                 {
                        ModRelayStatus = RelayTwoOpen;
                 }
                 break;
            }
            
            case RelayThree:
            {
                 if(RelayThreeOpen != ModRelayStatus)
                 {
                        ModRelayStatus = RelayThreeOpen;
                 }
                 break;
            }
            default:
            {
            //while(1);
            break;
           }
     }
    
        
  
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************继电器开关延时控制函数***********************************
#define WaitOpen   0
#define Open       1
#define WaitClose  2
void Mod_RelayOpenDelay(void)
{
     static ModRelayStatus_Def LastStatus;
     static INT8U  OpenDelayStatus   = WaitOpen;
     static BOOLEAN  CloseDelayFinish  = BN_FALSE;
     static DelayTickClk_Def OpenDelayTime  = {Type_Second,0,0};
     static DelayTickClk_Def CloseDelayTime = {Type_Second,0,0}; 
     
     DelayTickClockRun(&OpenDelayTime);
     DelayTickClockRun(&CloseDelayTime);
     
     
     if(DelayTickClockFinished(&CloseDelayTime))
     {
          CloseDelayFinish = BN_TRUE;  
     } 
     
     if((LastStatus != ModRelayStatus) && (CloseDelayFinish)) //状态有改变
     {      
          BSP_Relay1_OutStatus(BN_FALSE);
          BSP_Relay2_OutStatus(BN_FALSE);
          BSP_Relay3_OutStatus(BN_FALSE);
          LastStatus = ModRelayStatus;
          OpenDelayStatus = WaitOpen;
     }
     
     if((RelayAllClose != ModRelayStatus)&&(WaitClose != OpenDelayStatus))
     {
          switch(OpenDelayStatus)
          {
               case WaitOpen: //设定延时时间
               {
                      ReloadDealyTickClock(&OpenDelayTime,2);
                      OpenDelayStatus = Open;
                      break;
               }
               
               case Open://开继电器时间到
               {
                      if(DelayTickClockFinished(&OpenDelayTime))
                      {
                            if(RelayOneOpen == ModRelayStatus)
                            {
                                 BSP_Relay1_OutStatus(BN_TRUE); 
                                 
                            }
                            else if(RelayTwoOpen == ModRelayStatus)
                            {
                                 BSP_Relay2_OutStatus(BN_TRUE); 
                            }
                            else if(RelayThreeOpen == ModRelayStatus)
                            {
                                 BSP_Relay3_OutStatus(BN_TRUE); 
                            }
                            PwmOutStatus = GotoPWM_100Percent_Out;
                            OpenDelayStatus = WaitClose;
                            ReloadDealyTickClock(&CloseDelayTime,1);
                            CloseDelayFinish  = BN_FALSE;
                      }
                      break;
               }
               
               default:
               {
                   //ModRelayStatus =  RelayAllClose;
                   //DelayStatus = 0;
                   //DelayTime.DelayClock = 0;
                   break;
               }
            
       
          }
          
     }
           
}


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************继电器扫描函数**************************************
void Mod_RelayPWMOutScanTask(void)
{
     static DelayTickClk_Def StatusTime={Type_Second,0,0};
     static INT8U TaskStatus = 0;
     DelayTickClockRun(&StatusTime);
     
     if(0 == TaskStatus)
     {
           BSP_RelayAndPWM_Configure();
           TaskStatus++;
     }
     else
     {
           Mod_RelayOpenDelay();
           switch(PwmOutStatus)
           {
                case GotoPWM_100Percent_Out:
                {
                      BSP_RelayPWM_Set_100Percent_Out();
                      StatusTime.DelayClock = Time_5_Second;
                      PwmOutStatus = PWM_100Percent_Outing;
                      break;
                }
                case PWM_100Percent_Outing:
                {
                      if(0 == StatusTime.DelayClock)
                      {
                          PwmOutStatus = GotoPWM_60Percent_Out;
                      }
                      break;
                }
                case GotoPWM_60Percent_Out:
                {
                      BSP_RelayPWM_Set_60Percent_Out();
                      StatusTime.DelayClock = Time_60_Second;
                      PwmOutStatus = PWM_60Percent_Outing;
                      break;
                }
                case PWM_60Percent_Outing:
                {
                      if(0 == StatusTime.DelayClock)
                      {
                          PwmOutStatus = GotoPWM_100Percent_Out;
                      }
                      break;
                }
                default:
                {
                      PwmOutStatus = GotoPWM_100Percent_Out;
                      TaskStatus = 0;
                      break;
                }
           }
     }
     
}
//------------------------------E N D-------------------------------------------



//-----------------------Mod_RelayPWM.c--END------------------------------------


