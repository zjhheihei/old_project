/*******************************************************************************
*
*	模块名称 : 显示模块
*	文件名称 : APP.Storage.c
*	版    本 : V1.0
*	说    明 : 1传出压入数据
*                  2扫描任务
                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2018-08-21  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\APP_Configure.h"


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************存储模块参数****************************************
static APP_StorageParameter_Def  APP_Sto_Para;
static APP_StoDDR_Def            APP_Sto_DDR;
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************出厂设置c参数***************************************
void APP_Sto_FACParameter(void)
{
     APP_Sto_Para.DeviceStatusBits |= StatusDayProOn;
     APP_Sto_Para.CleanPara.SetCleanDay = 180;
     //APP_Sto_Para.BacklightTimeWorkTime = 10;
     APP_Sto_Para.BacklightTimeWorkTime = FAC_BACKLIGHT_WORK_TIME;
     APP_Sto_Para.FanHandWorkSpeed = SpeedMid;
     APP_Sto_Para.FanGearsSelect = ThreeSpeed;  
     APP_Sto_Para.DayProPara.StartHour = 8;
     APP_Sto_Para.DayProPara.StopHour = 17;
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************存储模块配置****************************************
void APP_Sto_Configure(void)
{
     EEPFlash_Configure(sizeof(APP_Sto_Para));
     if(ReadBlockOfEEPFlash(&APP_Sto_Para.data[0]))
     {    
         APP_Sto_FACParameter(); 
         APP_Sto_Para.CalibrateTemperature = 0;
         APP_Sto_Para.Rs485Para.Baudrate = 9600;
         APP_Sto_Para.Rs485Para.SlaveAddress = 0x01;
         APP_Sto_Para.Rs485Para.Parity = NoneParity;
         APP_Sto_Para.Rs485Para.StopBits = Bit_1_0;
         WriteBlockOfEEPFlash(&APP_Sto_Para.data[0]); 
     }           
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************存储模块扫描***************************************
void APP_Sto_ScanTask(void)
{
     static DelayTickClk_Def  APP_Sto_WriteDelay = {Type_Second,0,0};
     static BOOLEAN           APP_Sto_DelayEnable = BN_FALSE;
     
     DelayTickClockRun(&APP_Sto_WriteDelay);
      
     if(APP_Sto_DDR == StoWrite)
     {       
           ReloadDealyTickClock(&APP_Sto_WriteDelay,1);
           APP_Sto_DelayEnable = BN_TRUE;
           APP_Sto_DDR = StoFree; 
     }
     else if(StoRead == APP_Sto_DDR)
     {
           if(ReadBlockOfEEPFlash(&APP_Sto_Para.data[0]))
           {
                APP_Sto_FACParameter();
           }
           APP_Sto_DDR = StoFree; 
     }
     
     if(DelayTickClockFinished(&APP_Sto_WriteDelay))
     {
           ReloadDealyTickClock(&APP_Sto_WriteDelay,1);
           if(APP_Sto_DelayEnable)
           {
                 WriteBlockOfEEPFlash(&APP_Sto_Para.data[0]); 
                 APP_Sto_DelayEnable = BN_FALSE;
           }
     }   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************传出存储参数***************************************
APP_StorageParameter_Def *APP_Storage_PutOutParameter(void)
{
      return &APP_Sto_Para;
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************压入存储参数****************************************
void APP_Storage_PutInPara(void)
{
     /*APP_Sto_Para.DeviceStatusBits       = _In_me->DeviceStatusBits;
     APP_Sto_Para.CalibrateTemperature   = _In_me->CalibrateTemperature;
     
     APP_Sto_Para.BacklightTimeWorkTime  = _In_me->BacklightTimeWorkTime;
     APP_Sto_Para.FanHandWorkSpeed       = _In_me->FanHandWorkSpeed;
     APP_Sto_Para.FanGearsSelect         = _In_me->FanGearsSelect;
     
     APP_Sto_Para.Rs485Para.SlaveAddress = _In_me->Rs485Para.SlaveAddress;
     APP_Sto_Para.Rs485Para.Baudrate     = _In_me->Rs485Para.Baudrate;
     APP_Sto_Para.Rs485Para.Parity       = _In_me->Rs485Para.Parity;
     
     APP_Sto_Para.CleanPara.SetCleanDay  = _In_me->CleanPara.SetCleanDay;
 
     APP_Sto_Para.DayProPara.StartHour   = _In_me->DayProPara.StartHour;
     APP_Sto_Para.DayProPara.StopHour    = _In_me->DayProPara.StopHour;*/
    
     APP_Sto_DDR = StoWrite;
}
//------------------------------E N D-------------------------------------------


//--------------------------APP_Storage.c--END----------------------------------