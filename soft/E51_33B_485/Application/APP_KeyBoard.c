/*******************************************************************************
*
*	模块名称 : APP按键模块
*	文件名称 : APP.KeyBoard.c
*	版    本 : V1.0
*	说    明 : 1、按键扫描任务
*                  
*                 
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*******************************************************************************/

#include ".\APP_Configure.h"


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************模块使用的变量**************************************
static APP_StorageParameter_Def     *APP_KeySto_Para;
static APP_DisplayParameter_Def     *APP_KeyDis_Para;
static APP_RTC_Parameter_Def        *APP_KeyRTC_Para;
static BOOLEAN                      APP_Key_Valid_Event = BN_FALSE;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************按键宏定义****************************************
#define KeyMenu        KeyAloneBit1
#define KeyAdd         KeyAloneBit2
#define KeyPower       KeyAloneBit0
#define KeyDEC         KeyAloneBit3
#define KeyRtcSet      KeyGroupBit0
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************进入高级模式变量************************************
typedef enum               
{
     GoToParaSetDisable = 0x00,
     GoToParaSetEnable  = 0x01,
     GoingToParaSet     = 0x02,
}GoToParaSetPara_Def;
static GoToParaSetPara_Def GoToParaSetPara;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************获取其他模块参数**************************************
void APP_Key_GetPara(void)
{
     APP_KeySto_Para = APP_Storage_PutOutParameter();
     APP_KeyDis_Para = APP_Display_PutOutParameter();
     APP_KeyRTC_Para = APP_RTC_PutOutPara();
}          
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************高级设置******************************************

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************当前温度******************************************
void APP_Key_ParaSetMode_CurretTemp(void)  
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = FanGearsSetDis;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = AdjTempDis;
            APP_KeyDis_Para->DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag = BN_TRUE;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyDEC,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = AdjTempDis;
            APP_KeyDis_Para->DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag = BN_TRUE;
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************************校温********************************************
void APP_Key_ParaSetMode_AdjTemp(void) 
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = FanGearsSetDis;
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyAdd,3000,200)) 
     {    
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag = BN_TRUE;
            if(++APP_KeySto_Para->CalibrateTemperature>99) //9.9校温最大值
            {
                 APP_KeySto_Para->CalibrateTemperature = 99; 
            }
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,3000,200)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.FalshFlag.GoBackToCurrentTempFlag = BN_TRUE;
            
            if(--APP_KeySto_Para->CalibrateTemperature<-99)//-9.9校温最大值
            {
                 APP_KeySto_Para->CalibrateTemperature = -99; 
            }
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************风机档数******************************************
void APP_Key_ParaSetMode_FanGearsSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = CleanTimeSetDis;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(ThreeSpeed == APP_KeySto_Para->FanGearsSelect) 
            {
                 APP_KeySto_Para->FanGearsSelect = TwoSpeed; 
            }
            else
            {
                 APP_KeySto_Para->FanGearsSelect = ThreeSpeed;  
            }
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyDEC,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(ThreeSpeed == APP_KeySto_Para->FanGearsSelect) 
            {
                 APP_KeySto_Para->FanGearsSelect = TwoSpeed; 
            }
            else
            {
                 APP_KeySto_Para->FanGearsSelect = ThreeSpeed;  
            }
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************滤网清理时间****************************************
void APP_Key_ParaSetMode_CleanTimeSet(void)
{
     KeyRetValue_Def CurrentKeyValue;
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = BackLightSetDis;
     }
     else if((KeyPushDown|KeyContinueDown)&(CurrentKeyValue = Key_ValueOperation(KeyAdd,3000,600))) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(CurrentKeyValue == KeyPushDown)
            {
                  APP_KeySto_Para->CleanPara.SetCleanDay += 1;
            }
            else
            {
                   if(APP_KeySto_Para->CleanPara.SetCleanDay <= 100)
                   {
                         APP_KeySto_Para->CleanPara.SetCleanDay += 10;
                   }
                   else
                   {
                         if(APP_KeySto_Para->CleanPara.SetCleanDay > 100) 
                         {
                               APP_KeySto_Para->CleanPara.SetCleanDay += 100;
                         }
                   }
            }
            
            if(APP_KeySto_Para->CleanPara.SetCleanDay > 999)
            {
                    APP_KeySto_Para->CleanPara.SetCleanDay = 999;
            }         
     }
     else if((KeyPushDown|KeyContinueDown)&(CurrentKeyValue = Key_ValueOperation(KeyDEC,3000,500))) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(CurrentKeyValue == KeyPushDown)
            {
                 APP_KeySto_Para->CleanPara.SetCleanDay -= 1;
            }
            else
            {
                 if(APP_KeySto_Para->CleanPara.SetCleanDay <= 100)
                 {
                       if(APP_KeySto_Para->CleanPara.SetCleanDay < 10)
                       {
                              APP_KeySto_Para->CleanPara.SetCleanDay = 1;
                       }
                       else
                       {
                              APP_KeySto_Para->CleanPara.SetCleanDay -= 10;
                       }          
                 }
                 else
                 {
                       if(APP_KeySto_Para->CleanPara.SetCleanDay > 100)
                       {
                              APP_KeySto_Para->CleanPara.SetCleanDay -= 100;
                       } 
                 }              
            }
            
            if(APP_KeySto_Para->CleanPara.SetCleanDay < 1)
            {
                  APP_KeySto_Para->CleanPara.SetCleanDay = 1;
            }         
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************背光工作时间****************************************
void APP_Key_ParaSetMode_BackLightSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = SlaveAddressDis;
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyAdd,3000,200)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(++APP_KeySto_Para->BacklightTimeWorkTime > BackLightWorkMaxSetTime)
            {
                 APP_KeySto_Para->BacklightTimeWorkTime = BackLightNormalOn;
            }      
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,3000,200)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(--APP_KeySto_Para->BacklightTimeWorkTime < BackLightWorkMinSetTime)
            {
                 APP_KeySto_Para->BacklightTimeWorkTime = BackLightWorkMinSetTime;
            }    
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************从机地址******************************************
void APP_Key_ParaSetMode_SlaveAddressSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = BaudrateDis;
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyAdd,3000,200)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(++APP_KeySto_Para->Rs485Para.SlaveAddress > MaxSlaveAddress)
            {
                 APP_KeySto_Para->Rs485Para.SlaveAddress = MaxSlaveAddress;
            }
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,3000,200)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(--APP_KeySto_Para->Rs485Para.SlaveAddress < MinSlaveAddress)
            {
                 APP_KeySto_Para->Rs485Para.SlaveAddress = MinSlaveAddress;
            }
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************波特率*******************************************
void APP_Key_ParaSetMode_BaudrateSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = ParityDis;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeySto_Para->Rs485Para.Baudrate = APP_KeySto_Para->Rs485Para.Baudrate * 2;
            
            if(APP_KeySto_Para->Rs485Para.Baudrate > 19200)
            {
                  APP_KeySto_Para->Rs485Para.Baudrate = 2400;
            }
            /*if(2400 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 4800;
            }
            else if(4800 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 9600;
            }
            else if(9600 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 19200;
            }
            else if(19200 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 2400; 
            }*/
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeySto_Para->Rs485Para.Baudrate = APP_KeySto_Para->Rs485Para.Baudrate / 2;
            
            if(2400 > APP_KeySto_Para->Rs485Para.Baudrate)
            {
                  APP_KeySto_Para->Rs485Para.Baudrate = 19200; 
            }
            
            /*if(2400 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 19200;
            }
            else if(19200 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 9600;
            }
            else if(9600 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 4800;
            }  
            else if(4800 == APP_KeySto_Para->Rs485Para.Baudrate)
            {
                 APP_KeySto_Para->Rs485Para.Baudrate = 2400; 
            }*/
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************校验位*******************************************
void APP_Key_ParaSetMode_ParitySet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = StopBitsDis;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(++APP_KeySto_Para->Rs485Para.Parity > OddParity)
            {
                  APP_KeySto_Para->Rs485Para.Parity = NoneParity; 
            }           
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyDEC,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(NoneParity == APP_KeySto_Para->Rs485Para.Parity)
            {
                 APP_KeySto_Para->Rs485Para.Parity = OddParity;
            }
            else
            {
                 APP_KeySto_Para->Rs485Para.Parity--;
            }           
     }
}
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************校验位*******************************************
void APP_Key_ParaSetMode_StopBitsSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = DayProSelectDis;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(++APP_KeySto_Para->Rs485Para.StopBits > Bit_2_0)
            {
                  APP_KeySto_Para->Rs485Para.StopBits = Bit_1_0;
            }            
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyDEC,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(Bit_1_0 == APP_KeySto_Para->Rs485Para.StopBits)
            {
                  APP_KeySto_Para->Rs485Para.StopBits = Bit_2_0;
            }
            else
            {
                  APP_KeySto_Para->Rs485Para.StopBits--;
            }
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************天编程选择*****************************************
void APP_Key_ParaSetMode_DayProSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = FacWaitDis;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(StatusDayProOn & APP_KeySto_Para->DeviceStatusBits)
            {
                 APP_KeySto_Para->DeviceStatusBits &= ~StatusDayProOn;
                 APP_KeySto_Para->DeviceStatusBits &= ~StatusAuto;
            }
            else 
            {
                 APP_KeySto_Para->DeviceStatusBits |= StatusDayProOn;
            }
           
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyDEC,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            
            if(StatusDayProOn & APP_KeySto_Para->DeviceStatusBits)
            {
                 APP_KeySto_Para->DeviceStatusBits &= ~StatusDayProOn;
                 APP_KeySto_Para->DeviceStatusBits &= ~StatusAuto;
            }
            else 
            {
                 APP_KeySto_Para->DeviceStatusBits |= StatusDayProOn;
            }
     }
}
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************恢复出厂数据***************************************
void APP_Key_ParaSetMode_ParaToFac(void)
{
     APP_KeySto_Para->DeviceStatusBits |= StatusDayProOn;
     APP_KeySto_Para->CleanPara.SetCleanDay = 180;
     APP_KeySto_Para->BacklightTimeWorkTime = FAC_BACKLIGHT_WORK_TIME;
     APP_KeySto_Para->FanHandWorkSpeed = SpeedMid;
     APP_KeySto_Para->FanGearsSelect = ThreeSpeed;   
     APP_KeySto_Para->DayProPara.StartHour = 8;
     APP_KeySto_Para->DayProPara.StopHour = 17;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************恢复出厂等待***************************************
void APP_Key_ParaSetMode_FacWaitSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = CurrentTempDis;
     }
     else
     {
            if((KeyContinueDown)&Key_ValueOperation(KeyAdd,5000,NOCONTINUE))
            {
                  APP_Key_Valid_Event = BN_TRUE;
                  APP_Key_ParaSetMode_ParaToFac();
                  APP_KeyDis_Para->DisParaSetPara.SetStatus = FacFinishDis;
            }
     }
}
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************恢复出完成*****************************************
void APP_Key_ParaSetMode_FacFinishSet(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)) 
     {
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisParaSetPara.SetStatus = CurrentTempDis;
            APP_KeyDis_Para->DisParaSetPara.FalshFlag.FacFinishStep = FinishFristStep;         
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************高级设置模块***************************************
void APP_Key_ParaSetMode(void)
{
     switch(APP_KeyDis_Para->DisParaSetPara.SetStatus)
     {
            case CurrentTempDis:
            {
                 APP_Key_ParaSetMode_CurretTemp();
                 break;
            }
            case AdjTempDis:
            {
                 APP_Key_ParaSetMode_AdjTemp();
                 break;
            }
            case FanGearsSetDis:
            {
                 APP_Key_ParaSetMode_FanGearsSet();
                 break;
            }
            case CleanTimeSetDis:
            {
                 APP_Key_ParaSetMode_CleanTimeSet();
                 break;
            }
            case BackLightSetDis:
            {
                 APP_Key_ParaSetMode_BackLightSet();
                 break;
            }
            case SlaveAddressDis:
            {
                 APP_Key_ParaSetMode_SlaveAddressSet();
                 break;
            }
            case BaudrateDis:
            {
                 APP_Key_ParaSetMode_BaudrateSet();
                 break;
            }
            case ParityDis:
            {
                 APP_Key_ParaSetMode_ParitySet();
                 break;
            }
            case StopBitsDis:
            {
                 APP_Key_ParaSetMode_StopBitsSet();
                 break;
            }
            case DayProSelectDis:
            {
                 APP_Key_ParaSetMode_DayProSet();
                 break;
            }
            case FacWaitDis:
            {
                 APP_Key_ParaSetMode_FacWaitSet();
                 break;
            }
            case FacFinishDis:
            {
                 APP_Key_ParaSetMode_FacFinishSet();
                 break;
            }    
     }
     
     if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
     {
           APP_Key_Valid_Event = BN_TRUE;
           APP_KeyDis_Para->DisMainStatus.RunStatus = DisSoftStartMode;       
           /*if(FacFinishDis == APP_KeyDis_Para->DisParaSetPara.SetStatus)
           {
                  APP_Key_ParaSetMode_ParaToFac();
           } */ 
           APP_KeyDis_Para->DisParaSetPara.SetStatus = CurrentTempDis;
     }      
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************时钟设置******************************************
void APP_Key_RTCSetMode(void)
{    
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,0xffff,0xffff))
     {
           Mod_Key_MaskReCaptureOperation(MaskOnce,KeyMenu); 
           APP_Key_Valid_Event = BN_TRUE;
           APP_KeyDis_Para->DisRTCSetPara.FalshFlag.SelectTrigger = BN_TRUE;
           if(SetMinite == APP_KeyDis_Para->DisRTCSetPara.SetStatus)
           {
                 APP_KeyDis_Para->DisRTCSetPara.SetStatus = SetHour;
           }
           else
           {
                 APP_KeyDis_Para->DisRTCSetPara.SetStatus = SetMinite;
           }
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyAdd,3000,200))
     {
           if(SetMinite == APP_KeyDis_Para->DisRTCSetPara.SetStatus)
           {
                  if(++APP_KeyRTC_Para->RTC.Minite > 59)
                  {
                       APP_KeyRTC_Para->RTC.Minite = 0;
                  }
           }
           else
           {
                  if(++APP_KeyRTC_Para->RTC.Hour > 23)
                  {
                       APP_KeyRTC_Para->RTC.Hour = 0;
                  }
           } 
           APP_Key_Valid_Event = BN_TRUE;
           APP_KeyDis_Para->DisRTCSetPara.FalshFlag.SetTrigger = BN_TRUE;
     }
     else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,3000,200))
     {
           if(SetMinite == APP_KeyDis_Para->DisRTCSetPara.SetStatus)
           {
                  if(APP_KeyRTC_Para->RTC.Minite==0)
                  {
                       APP_KeyRTC_Para->RTC.Minite = 59; 
                  }
                  else
                  {
                       APP_KeyRTC_Para->RTC.Minite--; 
                  }
           }
           else
           {
                  if(APP_KeyRTC_Para->RTC.Hour == 0)
                  {
                       APP_KeyRTC_Para->RTC.Hour = 23; 
                  }
                  else
                  {
                       APP_KeyRTC_Para->RTC.Hour--; 
                  }
           } 
           APP_Key_Valid_Event = BN_TRUE;
           APP_KeyDis_Para->DisRTCSetPara.FalshFlag.SetTrigger = BN_TRUE;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
     {
           Mod_Key_MaskReCaptureOperation(MaskOnce,KeyPower); 
           APP_Key_Valid_Event = BN_TRUE;
           APP_KeyDis_Para->DisRTCSetPara.SetStatus = LeaveRTCSetMode;
     }
     
     APP_RTC_PutInPara(APP_KeyRTC_Para);
        
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************************关机********************************************
void APP_KeyShunDownMode(void)
{
     if(GoToParaSetDisable == GoToParaSetPara)
     {
          if((KeyPushDown)&Key_ValueOperation(KeyMenu,0xffff,0xffff))
          {
               GoToParaSetPara = GoToParaSetEnable;          
          }
     }
     
     if((KeyPushUp)&Key_ValueOperation(KeyMenu,0xffff,0xffff))
     {
           GoToParaSetPara = GoToParaSetDisable;          
     }
     
     if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
     {
           Mod_Key_MaskReCaptureOperation(MaskOnce,KeyPower); 
           APP_Key_Valid_Event = BN_TRUE;   
           APP_KeyDis_Para->DisMainStatus.RunStatus = DisSoftStartMode;
           APP_KeySto_Para->DeviceStatusBits &= ~StatusShutDown;
           
           if(GoToParaSetEnable == GoToParaSetPara)
           {
                 GoToParaSetPara = GoingToParaSet;  
           }
     }   
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************手动设置******************************************
void APP_Key_HandMode(void)
{
     KeyRetValue_Def  CurrentKeyValue;
     
     if((KeyPushDown|KeyContinueDown)&(CurrentKeyValue = Key_ValueOperation(KeyMenu,3000,0xffff)))
     {
           if(StatusDayProOn&APP_KeySto_Para->DeviceStatusBits)
           {
                 APP_Key_Valid_Event = BN_TRUE;
                 if(KeyPushDown == CurrentKeyValue)
                 {              
                        APP_KeyDis_Para->DisMainStatus.RunStatus = DisAutoMode;
                        APP_KeySto_Para->DeviceStatusBits |= StatusAuto;     
                 }
                 else
                 {
                        APP_KeyDis_Para->DisMainStatus.RunStatus = DisDayProMode;
                 }
                   
           }            
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,0xffff,0xffff))
     {
           Mod_Key_MaskReCaptureOperation(MaskOnce,KeyAdd); 
           APP_Key_Valid_Event = BN_TRUE;
           
           if(TwoSpeed == APP_KeySto_Para->FanGearsSelect)
           {
                   if(SpeedLow == APP_KeySto_Para->FanHandWorkSpeed)
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedHigh;
                   }
                   else if(SpeedHigh == APP_KeySto_Para->FanHandWorkSpeed)
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedLow;
                   }
                   else
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedLow;
                   } 
           }
           else
           {
                   if(SpeedLow == APP_KeySto_Para->FanHandWorkSpeed)
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedMid;
                   }
                   else if(SpeedMid == APP_KeySto_Para->FanHandWorkSpeed)
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedHigh;
                   }
                   else if(SpeedHigh == APP_KeySto_Para->FanHandWorkSpeed)
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedLow;
                   }
                   else
                   {
                          APP_KeySto_Para->FanHandWorkSpeed = SpeedLow;
                   }
           }
           
     }
     else if((KeyContinueDown)&Key_ValueOperation(KeyDEC,3000,0xffff)) 
     {
           if(StatusChangStrainer & APP_KeySto_Para->DeviceStatusBits)
           {
                  APP_KeySto_Para->DeviceStatusBits &= ~StatusChangStrainer;
                  APP_KeySto_Para->CleanPara.RunCleanMinite = 0;
                  APP_Key_Valid_Event = BN_TRUE;
           }
           
     }
     else if((KeyContinueDown)&Key_ValueOperation(KeyRtcSet,3000,0xffff)) //时钟设置模式
     {
            Mod_Key_MaskReCaptureOperation(MaskOnce,KeyRtcSet); 
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisRTCSetPara.FalshFlag.MiniteHour =BN_TRUE;                                  
            APP_KeyDis_Para->DisMainStatus.RunStatus = DisRTCSetMode;       
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
     {
           
           APP_Key_Valid_Event = BN_TRUE;
           
           APP_KeyDis_Para->DisMainStatus.RunStatus = DisShutDownMode;       
           APP_KeySto_Para->DeviceStatusBits |= StatusShutDown;
           Mod_Key_MaskReCaptureOperation(ReCapture,KeyMenu);
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************自动设置******************************************
void APP_Key_AutoMode(void)
{
     KeyRetValue_Def CurrentKeyValue;
     
     if((KeyPushDown|KeyContinueDown) & (CurrentKeyValue = Key_ValueOperation(KeyMenu,3000,0xffff)))
     {   
            if(KeyPushDown == CurrentKeyValue)
            {
                  APP_KeyDis_Para->DisMainStatus.RunStatus = DisHandMode;
                  APP_KeySto_Para->DeviceStatusBits &= ~StatusAuto;
            }
            else
            {
                  if(StatusDayProOn & APP_KeySto_Para->DeviceStatusBits) //天编程开启
                  {
                        APP_KeyDis_Para->DisMainStatus.RunStatus = DisDayProMode;
                  }         
            }
            
            APP_Key_Valid_Event = BN_TRUE;  
     }
     else if((KeyContinueDown)&Key_ValueOperation(KeyDEC,3000,0xffff)) 
     {
           if(StatusChangStrainer & APP_KeySto_Para->DeviceStatusBits)
           {
                  APP_KeySto_Para->DeviceStatusBits &= ~StatusChangStrainer;
                  APP_KeySto_Para->CleanPara.RunCleanMinite = 0;
                  APP_Key_Valid_Event = BN_TRUE;
           }      
     }
     else if((KeyContinueDown)&Key_ValueOperation(KeyRtcSet,3000,0xffff)) //时钟设置模式
     {
            Mod_Key_MaskReCaptureOperation(MaskOnce,KeyRtcSet); 
            APP_Key_Valid_Event = BN_TRUE;
            APP_KeyDis_Para->DisRTCSetPara.FalshFlag.MiniteHour =BN_TRUE;                                  
            APP_KeyDis_Para->DisMainStatus.RunStatus = DisRTCSetMode;       
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
     { 
           APP_Key_Valid_Event = BN_TRUE;
           APP_KeyDis_Para->DisMainStatus.RunStatus = DisShutDownMode;       
           APP_KeySto_Para->DeviceStatusBits |= StatusShutDown;
           Mod_Key_MaskReCaptureOperation(ReCapture,KeyMenu);
     }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************软启动设置****************************************
void APP_Key_SoftStartMode(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,0xffff,0xffff))
     {
           GoToParaSetPara = GoingToParaSet;          
     }
     
     if(GoingToParaSet == GoToParaSetPara)
     {
           APP_KeyDis_Para->DisMainStatus.RunStatus = DisParaSetMode;
           //APP_KeyDis_Para->DisMainStatus.FalshFlag.SoftStartStatus  = FristInToTask;
           APP_KeyDis_Para->DisSoftStartPara.RunStatus = DisALL;
           GoToParaSetPara = GoToParaSetDisable;
           APP_Key_Valid_Event = BN_TRUE;
     }
}
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************天编程设置****************************************
void APP_Key_DayProMode(void)
{
     if((KeyPushDown)&Key_ValueOperation(KeyMenu,0xffff,0xffff))
     {
           if(SetStartHour == APP_KeyDis_Para->DisDayProSetPara.SetStatus)
           {
                  APP_KeyDis_Para->DisDayProSetPara.SetStatus = SetStopHour;
           }
           else
           {
                  APP_KeyDis_Para->DisDayProSetPara.SetStatus = SetStartHour;
           }
           
           APP_Key_Valid_Event = BN_TRUE;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyAdd,0xffff,0xffff)) 
     {
           if(SetStartHour == APP_KeyDis_Para->DisDayProSetPara.SetStatus)
           {
                  APP_KeySto_Para->DayProPara.StartHour++;
                  
                  if(APP_KeySto_Para->DayProPara.StartHour >= APP_KeySto_Para->DayProPara.StopHour)
                  {
                        APP_KeySto_Para->DayProPara.StartHour = (APP_KeySto_Para->DayProPara.StopHour-1);
                  }
           }
           else
           {
                  APP_KeySto_Para->DayProPara.StopHour++; 
                  
                  if(APP_KeySto_Para->DayProPara.StopHour > 24)
                  {
                        APP_KeySto_Para->DayProPara.StopHour = 24;
                  }
           }
           
           APP_Key_Valid_Event = BN_TRUE;                 
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyDEC,0xffff,0xffff)) 
     {
           if(SetStartHour == APP_KeyDis_Para->DisDayProSetPara.SetStatus)
           {          
                  if(0 != APP_KeySto_Para->DayProPara.StartHour)
                  {
                        APP_KeySto_Para->DayProPara.StartHour--; 
                  }                 
           }
           else
           {
                  APP_KeySto_Para->DayProPara.StopHour--; 
                  if(APP_KeySto_Para->DayProPara.StopHour <= APP_KeySto_Para->DayProPara.StartHour)
                  {
                        APP_KeySto_Para->DayProPara.StopHour = (APP_KeySto_Para->DayProPara.StartHour+1);
                  }
           }
           
           APP_Key_Valid_Event = BN_TRUE;
     }
     else if((KeyPushDown)&Key_ValueOperation(KeyPower,0xffff,0xffff))
     {
           APP_KeyDis_Para->DisDayProSetPara.SetStatus = LeaveDayProMode;
           APP_Key_Valid_Event = BN_TRUE;
     }
}

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************按键总任务*****************************************
void APP_Key_ScanTask(void)
{  
     static APP_BacklightParameter_Def *APP_KeyBL_Para;
     
     APP_KeyBL_Para = APP_Backlight_PutOutParameter();
     Mod_Key_ScanTask();   
     
     if(Mod_Key_GetEvent())
     {   
            if(BL_OFF == APP_KeyBL_Para->BacklightStatus)
            {
                  Mod_Key_MaskReCaptureOperation(MaskOnce,KeyMenu); 
                  Mod_Key_MaskReCaptureOperation(MaskOnce,KeyAdd); 
                  Mod_Key_MaskReCaptureOperation(MaskOnce,KeyPower); 
                  Mod_Key_MaskReCaptureOperation(MaskOnce,KeyDEC); 
            }
            APP_KeyBL_Para->BacklightStatus = BL_On;
            APP_Key_GetPara();
            switch(APP_KeyDis_Para->DisMainStatus.RunStatus)
            {                
                  case DisShutDownMode:
                  {
                        APP_KeyShunDownMode(); 
                        break;
                  }
                  case DisRTCSetMode:
                  {
                        APP_Key_RTCSetMode();
                        break;
                  }
                  case DisParaSetMode:
                  {
                        APP_Key_ParaSetMode();
                        break;
                  }
                  case DisHandMode:
                  {
                        APP_Key_HandMode();
                        break;
                  }
                  case DisAutoMode:
                  {
                        APP_Key_AutoMode();
                        break;
                  }
                  case DisDayProMode:
                  {
                        APP_Key_DayProMode();
                        break;
                  }
                  case DisSoftStartMode:
                  {
                        APP_Key_SoftStartMode();
                        break;
                  }
                  
            }
            
            if(APP_Key_Valid_Event)
            {
                  APP_KeyDis_Para->DisMainStatus.FalshFlag.OnceTrigger = BN_TRUE;
                  APP_Storage_PutInPara();
                  APP_Display_PutInPara(APP_KeyDis_Para);               
                  APP_Key_Valid_Event = BN_FALSE;
            }
     }
}
//------------------------------E N D-------------------------------------------


//-----------------------APP_KeyBoard.c--END------------------------------------


