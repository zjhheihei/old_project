/*******************************************************************************
*
*	模块名称 : APP485模块
*	文件名称 : APP.Rs485.c
*	版    本 : V1.0
*	说    明 : 1、传出485状态
*                  2、Rs485扫描任务
*       
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-23  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/

#include ".\APP_Configure.h"



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************公共寄存器1500****************************************
#define  RegAddr_DebugFac          0x1500
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************公共寄存器2000-200F***********************************
#define  RegAddr_DeviceType        0x2000       //设备类型
#define  RegAddr_Version           0x2001       //版本
#define  RegAddr_SecondWeek        0x2002
#define  RegAddr_HourMinute        0x2003
#define  RegAddr_MonthDay          0x2004  
#define  RegAddr_Year              0x2005
#define  RegAddr_SlaveAdrress      0x2006
#define  RegAddr_Baudrate          0x2007
#define  RegAddr_Parity            0x2008
#define  RegAddr_StopBits          0x2009
#define  RegAddr_Reserve2          0x200A
#define  RegAddr_Reserve3          0x200B
#define  RegAddr_Reserve4          0x200C
#define  RegAddr_Reserve5          0x200D
#define  RegAddr_Reserve6          0x200E
#define  RegAddr_OutFac            0x200F
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************公共寄存器2000-200F***********************************
#define  RegAddr_ContorlBits           0x2200       //设备操控字            W
#define  RegAddr_StatusBits            0x2201       //设备状态字            R
#define  RegAddr_FanSetValue           0x2203       //风速设定值            R/W
#define  RegAddr_FanStatusValue        0x2204       //风速状态值            R
#define  RegAddr_RoomTemp              0x2205       //室温                  R
#define  RegAddr_StrainerUsedTime      0x2209       //滤网使用时间          R
#define  RegAddr_StrainerChangeTime    0x220A       //滤网更换周期          R/W
#define  RegAddr_DayProStartHour       0x220B       //天编程时段开始        R/W
#define  RegAddr_DayProFanOn           0x220C       //天编程风机开          R
#define  RegAddr_DayProStopHour        0x220D       //天编程时段结束        R/W
#define  RegAddr_DayProFanOff          0x220E       //天编程风机关          R
#define  RegAddr_DayProSelect          0x221B       //天编程选择            R/W
#define  RegAddr_RunModeSelect         0x2236       //运行状态选择          R/W
//------------------------------E N D-------------------------------------------





//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************操控字*******************************************
enum
{
    Rs485_PowOn               = 0x0001,   //0  
    Rs485_PowOff              = 0x0002,   //1   
    Rs485_ReSetStrainerTime   = 0x0010,   //4
    Rs485_GoToAuto            = 0x0100,   //8
    Rs485_GoToHand            = 0x0200,   //9
};
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************操控字*******************************************
#define  FirstRegAddress   0x1500  //开始地址
#define  LastRegAddress    0x2236  //最后地址
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************存储只写地址和无效地址返回*******************************
#define DebugFacH       0
#define DebugFacL       1
#define OutFacH         2
#define OutFacL         3
#define ControlBitsH    4
#define ControlBitsL    5
#define NullAddressH    6
#define NullAddressL    7
#define SetFanH         8
#define SetFanL         9
static INT8U    OnlyWriteBuff[10];
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************传出Rs485通讯状态*************************************
typedef enum               //主运行模式
{
     Rs485Free       = 0x00,
     Rs485Busy       = 0x01, 
     Rs485WaitFinish = 0x02,
}APP_Rs485Status_Def;
static APP_Rs485Status_Def APP_Rs485Status;
void APP_Rs485_CheckRs485Status(void)
{
     static DelayTickClk_Def FinishTime = {Type_Second,0,0};
     DelayTickClockRun(&FinishTime);
     
     if(Rs485Busy == APP_Rs485Status)
     {
          ReloadDealyTickClock(&FinishTime,5);
          APP_Rs485Status = Rs485WaitFinish;
     }
     
     if(Rs485WaitFinish == APP_Rs485Status)
     {
          if(DelayTickClockFinished(&FinishTime))
          {
                APP_Rs485Status = Rs485Free;  
          }
     }
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************传出Rs485通讯状态*************************************
BOOLEAN APP_Rs485_PutOutStatus(void)
{
        if(Rs485Free != APP_Rs485Status)
        {
              return BN_TRUE;
        }
        return BN_FALSE;
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************写寄存器*****************************************
void APP_Rs485_WriteRegister(INT16U _In_RegAddr,INT8U _In_RegLength,INT8U* _In_pDataBuff)
{     
      INT8U     i = 0;
      INT16U    j = 0;
      BOOLEAN   WriteRTCEnable  = BN_FALSE;
      BOOLEAN   WriteStoEnable  = BN_FALSE;
      
      APP_RTC_Parameter_Def     *APP_Rs485RTC_Para;
      APP_StorageParameter_Def  *APP_Rs485Sto_Para;
         
      APP_Rs485RTC_Para = APP_RTC_PutOutPara();
      APP_Rs485Sto_Para = APP_Storage_PutOutParameter();
      
      if(_In_RegLength > 120)
      {
             _In_RegLength = 120; 
      }
      
      while(_In_RegLength)
      {
           APP_Rs485Status = Rs485Busy;
           switch(_In_RegAddr)
           {
                  //+++++++++++++++++++0x1500-0x200F++++++++++++++++++++++++++++
                  //********************公共寄存器******************************
                  case RegAddr_DebugFac:      //调试出厂0x1500 
                  {
                       if((0x25 == _In_pDataBuff[i]) && (0x91 == _In_pDataBuff[i+1]))
                       {
                              APP_Rs485Sto_Para->BacklightTimeWorkTime  = FAC_BACKLIGHT_WORK_TIME;
                              APP_Rs485Sto_Para->CleanPara.SetCleanDay  = 180;
                              APP_Rs485Sto_Para->DayProPara.StartHour   = 8;
                              APP_Rs485Sto_Para->DayProPara.StopHour    = 17;
                              APP_Rs485Sto_Para->DeviceStatusBits       = 0;
                              APP_Rs485Sto_Para->FanGearsSelect         = ThreeSpeed;
                              APP_Rs485Sto_Para->FanHandWorkSpeed       = SpeedMid;
                              APP_Rs485Sto_Para->Rs485Para.Baudrate     = 9600;
                              APP_Rs485Sto_Para->Rs485Para.SlaveAddress = 1;
                              APP_Rs485Sto_Para->Rs485Para.Parity       = NoneParity;                             
                              WriteStoEnable = BN_TRUE;
                              OnlyWriteBuff[DebugFacH] = _In_pDataBuff[i];
                              OnlyWriteBuff[DebugFacL] = _In_pDataBuff[i+1];
                       }
                       break;
                  }
                  /*case RegAddr_DeviceType:    //产品编号0x2000只读写入返回00
                  {
                       
                       break;
                  }*/
                  /*case RegAddr_Version:       //软件版本0x2001
                  {
                      
                       break;
                  }*/
                  case RegAddr_SecondWeek:    //秒星期0x2002 
                  {
                       APP_Rs485RTC_Para->RTC.Second     = _In_pDataBuff[i];
                       APP_Rs485RTC_Para->RTC.Week       = _In_pDataBuff[i+1];
                       APP_Rs485RTC_Para->RTCWriteEnable = BN_TRUE;
                       break;
                  }
                  case RegAddr_HourMinute:    //小时分钟0x2003 
                  {
                       APP_Rs485RTC_Para->RTC.Hour       = _In_pDataBuff[i];
                       APP_Rs485RTC_Para->RTC.Minite     = _In_pDataBuff[i+1];
                       APP_Rs485RTC_Para->RTCWriteEnable = BN_TRUE;
                       break;
                  }
                  case RegAddr_MonthDay:      //月天0x2004 
                  {
                       APP_Rs485RTC_Para->RTC.Month       = _In_pDataBuff[i];
                       APP_Rs485RTC_Para->RTC.Day         = _In_pDataBuff[i+1];
                       APP_Rs485RTC_Para->RTCWriteEnable  = BN_TRUE;
                       break;
                  }
                  case RegAddr_Year:          //年0x2005 
                  {
                       j =   _In_pDataBuff[i];
                       j <<= 8;
                       j |=  _In_pDataBuff[i+1];                       
                       APP_Rs485RTC_Para->RTC.Year  = (j-2000);
                       APP_Rs485RTC_Para->RTCWriteEnable = BN_TRUE;
                       break;
                  }
                  case RegAddr_SlaveAdrress:  //从机地址0x2006
                  {
                       if(0 == _In_pDataBuff[i])
                       {
                              if((_In_pDataBuff[i+1] > 0) && (_In_pDataBuff[i+1] < 32))
                              {
                                     APP_Rs485Sto_Para->Rs485Para.SlaveAddress = _In_pDataBuff[i+1];
                                     WriteStoEnable = BN_TRUE; 
                              }
                       }
  
                       break;
                  }
                  case RegAddr_Baudrate:      //从机波特率0x2007
                  {
                       j =   _In_pDataBuff[i];
                       j <<= 8;
                       j |=  _In_pDataBuff[i+1];
                       APP_Rs485Sto_Para->Rs485Para.Baudrate = j;
                       WriteStoEnable = BN_TRUE;
                       break;
                  }
                  case RegAddr_Parity:        //从机校验方式0x2008
                  {
                       if(0x00 == _In_pDataBuff[i+1])
                       {
                              APP_Rs485Sto_Para->Rs485Para.Parity = NoneParity;
                       }
                       else if(0x01 == _In_pDataBuff[i+1])
                       {
                              APP_Rs485Sto_Para->Rs485Para.Parity = OddParity;
                       }
                       else if(0x02 == _In_pDataBuff[i+1])
                       {
                              APP_Rs485Sto_Para->Rs485Para.Parity = EvenParity;
                       }
                       else
                       {
                              APP_Rs485Sto_Para->Rs485Para.Parity = NoneParity;
                       }
                       WriteStoEnable = BN_TRUE;
                       break;
                  }
                  case RegAddr_StopBits:        //停止位选择
                  {
                       if(0 == _In_pDataBuff[i])
                       {
                             if(0x00 == _In_pDataBuff[i+1])
                             {
                                    APP_Rs485Sto_Para->Rs485Para.StopBits = Bit_1_0;
                             }
                             else if(0x01 == _In_pDataBuff[i+1])
                             {
                                    APP_Rs485Sto_Para->Rs485Para.StopBits = Bit_1_5;
                             }
                             else if(0x02 == _In_pDataBuff[i+1])
                             {
                                    APP_Rs485Sto_Para->Rs485Para.StopBits = Bit_2_0;
                             }                          
                       }
                       
                       WriteStoEnable = BN_TRUE;
                       break;
                  }
                  case RegAddr_OutFac:        //出厂只写读为0
                  {
                       APP_Rs485Sto_Para->BacklightTimeWorkTime  = FAC_BACKLIGHT_WORK_TIME;
                       APP_Rs485Sto_Para->CleanPara.SetCleanDay  = 180;
                       APP_Rs485Sto_Para->DayProPara.StartHour   = 8;
                       APP_Rs485Sto_Para->DayProPara.StopHour    = 17;
                       APP_Rs485Sto_Para->DeviceStatusBits       = 0;
                       APP_Rs485Sto_Para->FanGearsSelect         = ThreeSpeed;
                       APP_Rs485Sto_Para->FanHandWorkSpeed       = SpeedMid;
                       WriteStoEnable = BN_TRUE;
                       OnlyWriteBuff[OutFacH] = _In_pDataBuff[i];
                       OnlyWriteBuff[OutFacL] = _In_pDataBuff[i+1];
                       break;
                  }
                  //-------------------------E N D------------------------------
                  
                  
                  //+++++++++++++++++++0x2200-0x2236++++++++++++++++++++++++++++
                  //********************私有寄存器******************************
                  case RegAddr_ContorlBits:       //操控字0x2200只写读为0
                  {           
                       j =   _In_pDataBuff[i];
                       j <<= 8;
                       j |=  _In_pDataBuff[i+1];
                       
                       if(Rs485_PowOn & j)
                       {
                              APP_Rs485Sto_Para->DeviceStatusBits &= ~StatusShutDown;
                       }
                       
                       if(Rs485_PowOff & j)
                       {
                              APP_Rs485Sto_Para->DeviceStatusBits |= StatusShutDown;
                       }
                       
                       if(Rs485_ReSetStrainerTime & j)
                       {
                              APP_Rs485Sto_Para->CleanPara.SetCleanDay = 180;
                       }
                       
                       if(Rs485_GoToAuto & j)
                       {
                              APP_Rs485Sto_Para->DeviceStatusBits |= StatusAuto;
                       }
                       
                       if(Rs485_GoToHand & j)
                       {
                              APP_Rs485Sto_Para->DeviceStatusBits &= ~StatusAuto;
                       }
                       
                       OnlyWriteBuff[ControlBitsH] = _In_pDataBuff[i];
                       OnlyWriteBuff[ControlBitsL] = _In_pDataBuff[i+1];
                       WriteStoEnable = BN_TRUE;
                       break;
                  }
                  /*case RegAddr_StatusBits:        //状态字0x2201
                  {
                       
                       break;
                  }*/
                  case RegAddr_FanSetValue:        //风速设定值读为0，0x2203
                  {
                       if(0 == _In_pDataBuff[i])
                       {
                              if((0 != _In_pDataBuff[i+1]) && (4 > _In_pDataBuff[i+1]))
                              {
                                     OnlyWriteBuff[SetFanH] = 0x00;
                                     OnlyWriteBuff[SetFanL] = 0x00;
                                       
                                     if(0x01 == _In_pDataBuff[i+1]) 
                                     {
                                            APP_Rs485Sto_Para->FanHandWorkSpeed = SpeedLow;
                                            OnlyWriteBuff[SetFanL] = 0x01;
                                     }
                                     else if(0x02 == _In_pDataBuff[i+1]) 
                                     {
                                            APP_Rs485Sto_Para->FanHandWorkSpeed = SpeedMid;
                                            OnlyWriteBuff[SetFanL] = 0x02;
                                     }
                                     else if(0x03 == _In_pDataBuff[i+1]) 
                                     {
                                            APP_Rs485Sto_Para->FanHandWorkSpeed = SpeedHigh;
                                            OnlyWriteBuff[SetFanL] = 0x03;
                                     }
                                     
                                     WriteStoEnable = BN_TRUE;
                              }   
                       } 
                       break;
                  }
                  /*case RegAddr_FanStatusValue:     //风速状态值，0x2204
                  {
                      
                       break;
                  }*/
                  /*case RegAddr_RoomTemp:           //室温0x2205
                  {
                       
                       break;
                  }*/
                  /*case RegAddr_StrainerUsedTime:   //滤网使用时间0x2209
                  {
                      
                       break;
                  }*/
                  case RegAddr_StrainerChangeTime:   //滤网使用时间0x220A
                  {
                       j =   _In_pDataBuff[i];
                       j <<= 8;
                       j |=  _In_pDataBuff[i+1];
                       
                       if(j <1000)
                       {
                              APP_Rs485Sto_Para->CleanPara.SetCleanDay = j;
                              WriteStoEnable = BN_TRUE;
                       }
                       break;
                  }
                  case RegAddr_DayProStartHour:   //天编程开始输出小时0x220B
                  {
                       //if(0 == _In_pDataBuff[i])
                       {
                              if(_In_pDataBuff[i] < APP_Rs485Sto_Para->DayProPara.StopHour)
                              {
                                    APP_Rs485Sto_Para->DayProPara.StartHour = _In_pDataBuff[i];
                                    WriteStoEnable = BN_TRUE;
                              }
                       }
                       break;
                  }
                  /*case RegAddr_DayProFanOn:      //天编程风机输出0x220C
                  {
                       
                       break;
                  }*/
                  case RegAddr_DayProStopHour:   //天编程停止时输出小时0x220D
                  {
                       //if(0 == _In_pDataBuff[i])
                       {
                              if((_In_pDataBuff[i] > APP_Rs485Sto_Para->DayProPara.StartHour) &&
                                 (_In_pDataBuff[i] < 25))
                              {
                                    APP_Rs485Sto_Para->DayProPara.StopHour = _In_pDataBuff[i];
                                    WriteStoEnable = BN_TRUE;
                              }
                       }
                       break;
                  }
                  /*case RegAddr_DayProFanOff:      //天编程风机不输出0x220E
                  {
                       
                       break;
                  }*/
                  case RegAddr_DayProSelect:      //天编程选择0x221B
                  {
                       if(0 == _In_pDataBuff[i])
                       {         
                              if(0x00 == _In_pDataBuff[i+1]) 
                              {
                                     APP_Rs485Sto_Para->DeviceStatusBits &= ~StatusDayProOn; 
                                     APP_Rs485Sto_Para->DeviceStatusBits &= ~StatusAuto;
                                     WriteStoEnable = BN_TRUE;
                              }
                              else
                              {
                                     if(4 > _In_pDataBuff[i+1])
                                     {
                                            APP_Rs485Sto_Para->DeviceStatusBits |= StatusDayProOn;
                                            WriteStoEnable = BN_TRUE;
                                     }
                              }
                       }                  
                       break;
                  }
                  case RegAddr_RunModeSelect:     //运行模式选择0x2236
                  {
                       if(0 == _In_pDataBuff[i])
                       {         
                              if(0x01 == _In_pDataBuff[i+1]) 
                              {
                                     APP_Rs485Sto_Para->DeviceStatusBits &= ~StatusAuto; 
                                     WriteStoEnable = BN_TRUE;
                              }
                              else if(0x04 == _In_pDataBuff[i+1]) 
                              {
                                     APP_Rs485Sto_Para->DeviceStatusBits |= StatusAuto; 
                                     WriteStoEnable = BN_TRUE;
                              }           
                       }                          
                       break;
                  }
                  //-------------------------E N D------------------------------
                  
                  default:
                  {
                       OnlyWriteBuff[NullAddressH] = _In_pDataBuff[i];
                       OnlyWriteBuff[NullAddressL] = _In_pDataBuff[i+1];
                       break;
                  }
           }
           
           _In_RegLength--;
           i+=2;
           _In_RegAddr++;
      }
      
      if(WriteRTCEnable)
      {
            APP_RTC_PutInPara(APP_Rs485RTC_Para);
      }
      if(WriteStoEnable)
      {
            APP_Storage_PutInPara();
      }
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************读寄存器*****************************************
void APP_Rs485_ReadRegister(INT16U _In_RegAddr,INT8U _In_RegLength,INT8U* _Out_pDataBuff)
{
      APP_RTC_Parameter_Def     *APP_Rs485RTC_Para;
      APP_StorageParameter_Def  *APP_Rs485Sto_Para;
      APP_3FanParameter_Def     *APP_Rs4853Fan_Para;
      INT16S                     APP_Rs485Temp_Para;
      INT8U i = 0;
      
      
      APP_Rs485RTC_Para  = APP_RTC_PutOutPara();
      APP_Rs485Sto_Para  = APP_Storage_PutOutParameter();
      APP_Rs4853Fan_Para = APP_3Fan_PutOutParameter();
      APP_Rs485Temp_Para = APP_Temp_GetTemperature(); 
      
      if(_In_RegLength > 120)
      {
             _In_RegLength = 120; 
      }
      
      while(_In_RegLength)
      {
           APP_Rs485Status = Rs485Busy;
           switch(_In_RegAddr)
           {
                  //+++++++++++++++++++0x1500-0x200F++++++++++++++++++++++++++++
                  //********************公共寄存器******************************
                  case RegAddr_DebugFac:      //调试出厂0x1500 
                  {
                       _Out_pDataBuff[i]   = OnlyWriteBuff[DebugFacH];
                       _Out_pDataBuff[i+1] = OnlyWriteBuff[DebugFacL];
                       
                       if((0x25 == OnlyWriteBuff[DebugFacH]) && (0x91 == OnlyWriteBuff[DebugFacH]))
                       {
                              OnlyWriteBuff[DebugFacH] = 0x00; 
                              OnlyWriteBuff[DebugFacL] = 0x00;
                       }
                       break;
                  }
                  case RegAddr_DeviceType:    //产品编号0x2000 
                  {
                       _Out_pDataBuff[i]   = (_DeviceType_ >> 8);
                       _Out_pDataBuff[i+1] = (_DeviceType_ & 0x00ff);
                       break;
                  }
                  case RegAddr_Version:       //软件版本0x2001
                  {
                       _Out_pDataBuff[i]   = (_Modified_ >> 8);
                       _Out_pDataBuff[i+1] = (_Modified_ & 0x00ff);
                       break;
                  }
                  case RegAddr_SecondWeek:    //秒星期0x2002 
                  {
                       _Out_pDataBuff[i]   = APP_Rs485RTC_Para->RTC.Second;
                       _Out_pDataBuff[i+1] = APP_Rs485RTC_Para->RTC.Week;
                       break;
                  }
                  case RegAddr_HourMinute:    //小时分钟0x2003 
                  {
                       _Out_pDataBuff[i]   = APP_Rs485RTC_Para->RTC.Hour;
                       _Out_pDataBuff[i+1] = APP_Rs485RTC_Para->RTC.Minite;
                       break;
                  }
                  case RegAddr_MonthDay:      //月天0x2004 
                  {
                       _Out_pDataBuff[i]   = APP_Rs485RTC_Para->RTC.Month;
                       _Out_pDataBuff[i+1] = APP_Rs485RTC_Para->RTC.Day;
                       break;
                  }
                  case RegAddr_Year:          //年0x2005 
                  {
                       _Out_pDataBuff[i]   = ((APP_Rs485RTC_Para->RTC.Year+2000) >> 8);
                       _Out_pDataBuff[i+1] = ((APP_Rs485RTC_Para->RTC.Year+2000) & 0x00ff);
                       break;
                  }
                  case RegAddr_SlaveAdrress:  //从机地址0x2006
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       _Out_pDataBuff[i+1] = APP_Rs485Sto_Para->Rs485Para.SlaveAddress;
                       break;
                  }
                  case RegAddr_Baudrate:      //从机波特率0x2007
                  {
                       _Out_pDataBuff[i]   = (APP_Rs485Sto_Para->Rs485Para.Baudrate >> 8);
                       _Out_pDataBuff[i+1] = (APP_Rs485Sto_Para->Rs485Para.Baudrate & 0x00ff);
                       break;
                  }
                  case RegAddr_Parity:        //从机校验方式0x2008
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       
                       if(NoneParity == APP_Rs485Sto_Para->Rs485Para.Parity)        //无校验方式
                       {
                              _Out_pDataBuff[i+1] = 0x00;
                       }
                       else if(EvenParity == APP_Rs485Sto_Para->Rs485Para.Parity)   //偶校验
                       {
                              _Out_pDataBuff[i+1] = 0x02;
                       }
                       else if(OddParity == APP_Rs485Sto_Para->Rs485Para.Parity)   //偶校验
                       {
                              _Out_pDataBuff[i+1] = 0x01;
                       }
                       break;
                  }
                  case RegAddr_StopBits:
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       _Out_pDataBuff[i+1] = APP_Rs485Sto_Para->Rs485Para.StopBits;
                       break;
                  }
                  case RegAddr_OutFac:        //出厂只写读为0
                  {
                       _Out_pDataBuff[i]   = OnlyWriteBuff[OutFacH];
                       _Out_pDataBuff[i+1] = OnlyWriteBuff[OutFacL];
                       
                       if((0x37 == OnlyWriteBuff[OutFacH]) && (0x50 == OnlyWriteBuff[OutFacL]))
                       {
                              OnlyWriteBuff[OutFacH] = 0x00; 
                              OnlyWriteBuff[OutFacL] = 0x00;
                       }
                       break;
                  }
                  //-------------------------E N D------------------------------
                  
                  
                  //+++++++++++++++++++0x2200-0x2236++++++++++++++++++++++++++++
                  //********************私有寄存器******************************
                  case RegAddr_ContorlBits:       //操控字0x2200只写读为0
                  {           
                       _Out_pDataBuff[i]   = OnlyWriteBuff[ControlBitsH];
                       _Out_pDataBuff[i+1] = OnlyWriteBuff[ControlBitsL];
                       
                       if(OnlyWriteBuff[ControlBitsH] || OnlyWriteBuff[ControlBitsL])
                       {
                              OnlyWriteBuff[ControlBitsH] = 0x00; 
                              OnlyWriteBuff[ControlBitsH] = 0x00;
                       }
                       break;
                  }
                  case RegAddr_StatusBits:        //状态字0x2201
                  {
                       _Out_pDataBuff[i]   = (APP_Rs485Sto_Para->DeviceStatusBits >> 8);
                       _Out_pDataBuff[i+1] = (APP_Rs485Sto_Para->DeviceStatusBits & 0x00ff);
                       break;
                  }
                  case RegAddr_FanSetValue:        //风速设定值读为0，0x2203
                  {
                       _Out_pDataBuff[i]   = OnlyWriteBuff[SetFanH];
                       _Out_pDataBuff[i+1] = OnlyWriteBuff[SetFanL];
                       
                       if(OnlyWriteBuff[SetFanH] || OnlyWriteBuff[SetFanL])
                       {
                              OnlyWriteBuff[SetFanH] = 0x00; 
                              OnlyWriteBuff[SetFanL] = 0x00;
                       }
                       break;
                  }
                  case RegAddr_FanStatusValue:     //风速状态值，0x2204
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       
                       if(SpeedNo == APP_Rs4853Fan_Para->FanSpeedStatus)
                       {
                              _Out_pDataBuff[i+1] = 0x00;
                       }
                       else if(SpeedLow == APP_Rs4853Fan_Para->FanSpeedStatus)
                       {
                              _Out_pDataBuff[i+1] = 0x01;
                       }
                       else if(SpeedMid == APP_Rs4853Fan_Para->FanSpeedStatus)
                       {
                              _Out_pDataBuff[i+1] = 0x02;
                       }
                       else if(SpeedHigh == APP_Rs4853Fan_Para->FanSpeedStatus)
                       {
                              _Out_pDataBuff[i+1] = 0x03;
                       }
                       break;
                  }
                  case RegAddr_RoomTemp:           //室温0x2205
                  {
                       _Out_pDataBuff[i]   = (APP_Rs485Temp_Para >> 8);
                       _Out_pDataBuff[i+1] = (APP_Rs485Temp_Para & 0x00ff);
                       break;
                  }
                  case RegAddr_StrainerUsedTime:   //滤网使用时间0x2209
                  {
                       _Out_pDataBuff[i]   = ((APP_Rs485Sto_Para->CleanPara.RunCleanMinite/1440) >> 8);
                       _Out_pDataBuff[i+1] = ((APP_Rs485Sto_Para->CleanPara.RunCleanMinite/1440) & 0x00ff);
                       break;
                  }
                  case RegAddr_StrainerChangeTime:   //滤网使用时间0x220A
                  {
                       _Out_pDataBuff[i]   = (APP_Rs485Sto_Para->CleanPara.SetCleanDay >> 8);
                       _Out_pDataBuff[i+1] = (APP_Rs485Sto_Para->CleanPara.SetCleanDay & 0x00ff);
                       break;
                  }
                  case RegAddr_DayProStartHour:   //天编程开始输出小时0x220B
                  {
                       _Out_pDataBuff[i]   = APP_Rs485Sto_Para->DayProPara.StartHour;
                       _Out_pDataBuff[i+1] = 0x00;
                       break;
                  }
                  case RegAddr_DayProFanOn:      //天编程风机输出0x220C
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       _Out_pDataBuff[i+1] = 0x01;
                       break;
                  }
                  case RegAddr_DayProStopHour:   //天编程停止时输出小时0x220D
                  {
                       _Out_pDataBuff[i]   = APP_Rs485Sto_Para->DayProPara.StopHour;
                       _Out_pDataBuff[i+1] = 0x00;
                       break;
                  }
                  case RegAddr_DayProFanOff:      //天编程风机不输出0x220E
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       _Out_pDataBuff[i+1] = 0x00;
                       break;
                  }
                  case RegAddr_DayProSelect:      //天编程选择0x221B
                  {
                       _Out_pDataBuff[i]   = 0x00;
                       _Out_pDataBuff[i+1] = 0x00;
                       
                       if(StatusDayProOn & APP_Rs485Sto_Para->DeviceStatusBits)
                       {
                             _Out_pDataBuff[i+1] = 0x01; 
                       }
                        
                       break;
                  }
                  case RegAddr_RunModeSelect:     //运行模式选择0x2236
                  {
                        _Out_pDataBuff[i]   = 0x00;
                        _Out_pDataBuff[i+1] = 0x00;
                        
                       if(StatusShutDown & APP_Rs485Sto_Para->DeviceStatusBits)
                       {
                              
                       }
                       else
                       {
                             if(StatusAuto & APP_Rs485Sto_Para->DeviceStatusBits)    //自动模式
                             {
                                    _Out_pDataBuff[i+1] = 0x04;
                             }
                             else
                             {
                                    _Out_pDataBuff[i+1] = 0x01;
                             }
                       }
                       break;
                  }
                  //-------------------------E N D------------------------------
                  
                  default:
                  {
                       _Out_pDataBuff[i]   = OnlyWriteBuff[NullAddressH];
                       _Out_pDataBuff[i+1] = OnlyWriteBuff[NullAddressL];
                       break;
                  }
           }
         
           _In_RegLength--;
           i+=2;
           _In_RegAddr++;
      }
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************检测窗口软件配置是否更新必须放在Task前面处理******************
void APP_Rs485_CheckConfigure(void)
{  
    static INT8U CheckStatus = FristInToTask;
    SlaveModbusMod_Parameter_Def         APP_Rs485_Para;
    APP_StorageParameter_Def            *APP_Rs485Sto_Para;
    static DelayTickClk_Def        CheckDelayTime= {Type_Second,0,0};
    DelayTickClockRun(&CheckDelayTime);
    
    if(DelayTickClockFinished(&CheckDelayTime))
    {
          ReloadDealyTickClock(&CheckDelayTime,2);
          APP_Rs485Sto_Para = APP_Storage_PutOutParameter();
          APP_Rs485_Para.Baudrate              = APP_Rs485Sto_Para->Rs485Para.Baudrate;
          APP_Rs485_Para.Parity                = APP_Rs485Sto_Para->Rs485Para.Parity;
          APP_Rs485_Para.CLK                   = Poll_HSIFrequency();
          APP_Rs485_Para.SlaveAddress          = APP_Rs485Sto_Para->Rs485Para.SlaveAddress;
          APP_Rs485_Para.StopBits              = APP_Rs485Sto_Para->Rs485Para.StopBits;
          if(FristInToTask == CheckStatus)
          {
              APP_Rs485_Para.WriteRegister         = APP_Rs485_WriteRegister;
              APP_Rs485_Para.ReadRegister          = APP_Rs485_ReadRegister; 
              CheckStatus++;
          }
          
          Mod_SlaveModbusUsartParameterConfigure(&APP_Rs485_Para);   
    }
     
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************APPModbus扫描任务*************************************
void APP_Rs485_ScanTask(void)
{
      APP_Rs485_CheckRs485Status();
      APP_Rs485_CheckConfigure(); //实际应用中定时1-3S调用，不需要实时调用
      Mod_SalveModbus_ScanTask();
}
//------------------------------E N D-------------------------------------------

//--------------------------APP.RTC.C--END--------------------------------------


