/*********************************************************************************************************
*
*	模块名称 : 液晶模块
*	文件名称 : Mod_LCD.c
*	版    本 : V1.0
*	说    明 : 
                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-08-08   zhoujh  正式发布
*	Copyright (C), 2015-2020,   menredGroup
*
*********************************************************************************************************/

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\Mod_LCD.h"
//------------------------------E N D-------------------------------------------




INT8U LCDBuff[10]; 

const INT8U BcdCodeTable[16]=
{0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F,0x00,0x40,0x61,0x38,0X37,0X77};


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************风机扇叶刷新****************************************
void Mod_LCD_RefreshFanFalshSpeed(void)
{   
     if(Dis_FanSpeed_A_Add & Dis_FanSpeed_A_Dat)
     {
          Dis_FanSpeed_A_Ram = 1;
     } 
     else
     {
          Dis_FanSpeed_A_Ram = 0;
     }
        
     if(Dis_FanSpeed_B_Add & Dis_FanSpeed_B_Dat)
     {
          Dis_FanSpeed_B_Ram = 1;
     }
     else
     {
          Dis_FanSpeed_B_Ram = 0;
     }
      
     if(Dis_FanSpeed_C_Add & Dis_FanSpeed_C_Dat)
     {
          Dis_FanSpeed_C_Ram = 1;
     }
     else 
     {
          Dis_FanSpeed_C_Ram = 0;
     }
     
     if(Dis_FanRoom_Add & Dis_FanRoom_Dat)
     {
          Dis_FanRoom_Ram = 1;
     } 
     else
     {
          Dis_FanRoom_Ram = 0;
     }
}
//------------------------------E N D-------------------------------------------



void Mod_LCD_ClrFanLeaf(void)
{
     Dis_FanSpeed_A_Ram = 0;
     Dis_FanSpeed_B_Ram = 0;
     Dis_FanSpeed_C_Ram = 0;
}


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************清理符号******************************************
void Mod_LCD_SetAll(BOOLEAN _In_Status,SelectType_Def _In_Type)
{
     if(_In_Status)
     {
         if(FanLeaf == _In_Type)
         {
               Dis_CharGroup3_Add |= 0x38;  
         }
         else if(AnyOther == _In_Type)
         {
               Dis_0_7_Clock_Add   = 0xff;
               Dis_8_15_Clock_Add  = 0xff;
               Dis_16_23_Clock_Add = 0xff;
               Dis_Dig1            = 0xff;
               Dis_Dig2            = 0xff;
               Dis_Dig3            = 0xff;
               Dis_Dig4            = 0xff;
               Dis_CharGroup1_Add  = 0xff;
               Dis_CharGroup2_Add  = 0xff;   
               Dis_CharGroup3_Add  |= 0x47;  
         }
             
     }
     else
     {
         if(FanLeaf == _In_Type)
         {
               Dis_CharGroup3_Add  &= ~0x38;  
         }
         else if(AnyOther == _In_Type)
         {
               Dis_0_7_Clock_Add   = 0x00;
               Dis_8_15_Clock_Add  = 0x00;
               Dis_16_23_Clock_Add = 0x00;
               Dis_Dig1            = 0x00;
               Dis_Dig2            = 0x00;
               Dis_Dig3            = 0x00;
               Dis_Dig4            = 0x00;
               Dis_CharGroup1_Add  = 0x00;
               Dis_CharGroup2_Add  = 0x00;
               Dis_CharGroup3_Add  &= 0x38;    
         }
           
     }
      
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************天编程数据刷新***************************************
void Mod_LCD_RefreshHourProgram(void)
{
      Dis_0Clock_Ram = 0;
      
      if(Dis_0Clock_Add & Dis_0Clock_Dat)
      {
          Dis_0Clock_Ram = 1;
      }
      
      Dis_1Clock_Ram = 0;
      
      if(Dis_1Clock_Add & Dis_1Clock_Dat)
      {
          Dis_1Clock_Ram = 1;
      }
      
      Dis_2Clock_Ram = 0;
      
      if(Dis_2Clock_Add & Dis_2Clock_Dat)
      {
          Dis_2Clock_Ram = 1;
      }
      
      Dis_3Clock_Ram = 0;
      
      if(Dis_3Clock_Add & Dis_3Clock_Dat)
      {
          Dis_3Clock_Ram = 1;
      }
      
      Dis_4Clock_Ram = 0;
      
      if(Dis_4Clock_Add & Dis_4Clock_Dat)
      {
          Dis_4Clock_Ram = 1;
      }
      
      Dis_5Clock_Ram = 0;
      
      if(Dis_5Clock_Add & Dis_5Clock_Dat)
      {
          Dis_5Clock_Ram = 1;
      }
      
      Dis_6Clock_Ram = 0;
      
      if(Dis_6Clock_Add & Dis_6Clock_Dat)
      {
          Dis_6Clock_Ram = 1;
      }
      
      Dis_7Clock_Ram = 0;
      
      if(Dis_7Clock_Add & Dis_7Clock_Dat)
      {
          Dis_7Clock_Ram = 1;
      }
      
      Dis_8Clock_Ram = 0;
      
      if(Dis_8Clock_Add & Dis_8Clock_Dat)
      {
          Dis_8Clock_Ram = 1;
      }
      
      Dis_9Clock_Ram = 0;
      
      if(Dis_9Clock_Add & Dis_9Clock_Dat)
      {
          Dis_9Clock_Ram = 1;
      }
      
      Dis_10Clock_Ram = 0;
      
      if(Dis_10Clock_Add & Dis_10Clock_Dat)
      {
          Dis_10Clock_Ram = 1;
      }
      
      Dis_11Clock_Ram = 0;
      
      if(Dis_11Clock_Add & Dis_11Clock_Dat)
      {
          Dis_11Clock_Ram = 1;
      }
      
      Dis_12Clock_Ram = 0;
      
      if(Dis_12Clock_Add & Dis_12Clock_Dat)
      {
          Dis_12Clock_Ram = 1;
      }
      
      Dis_13Clock_Ram = 0;
      
      if(Dis_13Clock_Add & Dis_13Clock_Dat)
      {
          Dis_13Clock_Ram = 1;
      }
      
      Dis_14Clock_Ram = 0;
      
      if(Dis_14Clock_Add & Dis_14Clock_Dat)
      {
          Dis_14Clock_Ram = 1;
      }
      
      Dis_15Clock_Ram = 0;
      
      if(Dis_15Clock_Add & Dis_15Clock_Dat)
      {
          Dis_15Clock_Ram = 1;
      }   
      
      Dis_16Clock_Ram = 0;
      
      if(Dis_16Clock_Add & Dis_16Clock_Dat)
      {
          Dis_16Clock_Ram = 1;
      }
      
      Dis_17Clock_Ram = 0;
      
      if(Dis_17Clock_Add & Dis_17Clock_Dat)
      {
          Dis_17Clock_Ram = 1;
      }
      
      Dis_18Clock_Ram = 0;
      
      if(Dis_18Clock_Add & Dis_18Clock_Dat)
      {
          Dis_18Clock_Ram = 1;
      }
      
      Dis_19Clock_Ram = 0;
      
      if(Dis_19Clock_Add & Dis_19Clock_Dat)
      {
          Dis_19Clock_Ram = 1;
      }
      
      Dis_20Clock_Ram = 0;
      
      if(Dis_20Clock_Add & Dis_20Clock_Dat)
      {
          Dis_20Clock_Ram = 1;
      }
      
      Dis_21Clock_Ram = 0;
      
      if(Dis_21Clock_Add & Dis_21Clock_Dat)
      {
          Dis_21Clock_Ram = 1;
      }
      
      Dis_22Clock_Ram = 0;
      
      if(Dis_22Clock_Add & Dis_22Clock_Dat)
      {
          Dis_22Clock_Ram = 1;
      }
      
      Dis_23Clock_Ram = 0;
      
      if(Dis_23Clock_Add & Dis_23Clock_Dat)
      {
          Dis_23Clock_Ram = 1;
      }
      
      Dis_24Clock_Ram = 0;
      
      if(Dis_24Clock_Add & Dis_24Clock_Dat)
      {
          Dis_24Clock_Ram = 1;
      }
      
      Dis_Hour_Ram = 0;
      
      if(Dis_Hour_Add & Dis_Hour_Dat)
      {
          Dis_Hour_Ram = 1;
      }      
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************字符数据刷新***************************************
void Mod_LCD_RefreshChar(void)
{
     Dis_Point_Ram = 0;
     
     if(Dis_Point_Add & Dis_Point_Dat)
     {
          Dis_Point_Ram = 1;
     }
     
     Dis_Col_Ram = 0;
     
     if(Dis_Col_Add & Dis_Col_Dat)
     {
          Dis_Col_Ram = 1;
     }
     
     Dis_TempSymbol_P_Ram = 0;
     
     if(Dis_TempSymbol_P_Add & Dis_TempSymbol_P_Dat)
     {
          Dis_TempSymbol_P_Ram = 1;
     }
     
     Dis_HandWork_Ram = 0;
     
     if(Dis_HandWork_Add & Dis_HandWork_Dat)
     {
          Dis_HandWork_Ram = 1;
     }
     
     Dis_AutoWork_Ram = 0;
     
     if(Dis_AutoWork_Add & Dis_AutoWork_Dat)
     {
          Dis_AutoWork_Ram = 1;
     }  
     
     Dis_Clean_Ram = 0;
     
     if(Dis_Clean_Add & Dis_Clean_Dat)
     {
          Dis_Clean_Ram = 1;
     } 
     
     Dis_SumClock_Ram = 0;
     
     if(Dis_SumClock_Add & Dis_SumClock_Dat)
     {
          Dis_SumClock_Ram = 1;
     } 
     
     Dis_Outside_Ram = 0;
     
     if(Dis_Outside_Add & Dis_Outside_Dat)
     {
          Dis_Outside_Ram = 1;
     } 
     
     Dis_RoomTemp_Ram = 0;
     
     if(Dis_RoomTemp_Add & Dis_RoomTemp_Dat)
     {
          Dis_RoomTemp_Ram = 1;
     } 
     
     Dis_Communication_Ram = 0;
     
     if(Dis_Communication_Add & Dis_Communication_Dat)
     {
          Dis_Communication_Ram = 1;
     } 
     
     Dis_RH_Ram = 0;
     
     if(Dis_RH_Add & Dis_RH_Dat)
     {
          Dis_RH_Ram = 1;
     } 
     
     Dis_FanLow_Ram = 0;
     
     if(Dis_FanLow_Add & Dis_FanLow_Dat)
     {
          Dis_FanLow_Ram = 1;
     } 
     
     Dis_FanMid_Ram = 0;
     
     if(Dis_FanMid_Add & Dis_FanMid_Dat)
     {
          Dis_FanMid_Ram = 1;
     } 
     
     Dis_FanHigh_Ram = 0;
     
     if(Dis_FanHigh_Add & Dis_FanHigh_Dat)
     {
          Dis_FanHigh_Ram = 1;
     } 
    
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************数字数据刷新***************************************
void Mod_LCD_RefreshDig(void)
{
     Dis_Dig1_G_Ram = 0;
     if(Dis_Dig1_G_Add & Dis_Dig1_G_Dat)
     {
          Dis_Dig1_G_Ram = 1;
     }
     
     Dis_Dig1_F_Ram = 0;
     if(Dis_Dig1_F_Add & Dis_Dig1_F_Dat)
     {
          Dis_Dig1_F_Ram = 1;
     }
     
     Dis_Dig1_E_Ram = 0;
     if(Dis_Dig1_E_Add & Dis_Dig1_E_Dat)
     {
          Dis_Dig1_E_Ram = 1;
     }
     
     Dis_Dig1_D_Ram = 0;
     if(Dis_Dig1_D_Add & Dis_Dig1_D_Dat)
     {
          Dis_Dig1_D_Ram = 1;
     }
     
     Dis_Dig1_C_Ram = 0;
     if(Dis_Dig1_C_Add & Dis_Dig1_C_Dat)
     {
          Dis_Dig1_C_Ram = 1;
     }
     
     Dis_Dig1_B_Ram = 0;
     if(Dis_Dig1_B_Add & Dis_Dig1_B_Dat)
     {
          Dis_Dig1_B_Ram = 1;
     }
     
     Dis_Dig1_A_Ram = 0;
     if(Dis_Dig1_A_Add & Dis_Dig1_A_Dat)
     {
          Dis_Dig1_A_Ram = 1;
     }
    
      Dis_Dig2_G_Ram = 0;
     if(Dis_Dig2_G_Add & Dis_Dig2_G_Dat)
     {
          Dis_Dig2_G_Ram = 1;
     }
     
     Dis_Dig2_F_Ram = 0;
     if(Dis_Dig2_F_Add & Dis_Dig2_F_Dat)
     {
          Dis_Dig2_F_Ram = 1;
     }
     
     Dis_Dig2_E_Ram = 0;
     if(Dis_Dig2_E_Add & Dis_Dig2_E_Dat)
     {
          Dis_Dig2_E_Ram = 1;
     }
     
     Dis_Dig2_D_Ram = 0;
     if(Dis_Dig2_D_Add & Dis_Dig2_D_Dat)
     {
          Dis_Dig2_D_Ram = 1;
     }
     
     Dis_Dig2_C_Ram = 0;
     if(Dis_Dig2_C_Add & Dis_Dig2_C_Dat)
     {
          Dis_Dig2_C_Ram = 1;
     }
     
     Dis_Dig2_B_Ram = 0;
     if(Dis_Dig2_B_Add & Dis_Dig2_B_Dat)
     {
          Dis_Dig2_B_Ram = 1;
     }
     
     Dis_Dig2_A_Ram = 0;
     if(Dis_Dig2_A_Add & Dis_Dig2_A_Dat)
     {
          Dis_Dig2_A_Ram = 1;
     }
     
         Dis_Dig3_G_Ram = 0;
     if(Dis_Dig3_G_Add & Dis_Dig3_G_Dat)
     {
          Dis_Dig3_G_Ram = 1;
     }
     
     Dis_Dig3_F_Ram = 0;
     if(Dis_Dig3_F_Add & Dis_Dig3_F_Dat)
     {
          Dis_Dig3_F_Ram = 1;
     }
     
     Dis_Dig3_E_Ram = 0;
     if(Dis_Dig3_E_Add & Dis_Dig3_E_Dat)
     {
          Dis_Dig3_E_Ram = 1;
     }
     
     Dis_Dig3_D_Ram = 0;
     if(Dis_Dig3_D_Add & Dis_Dig3_D_Dat)
     {
          Dis_Dig3_D_Ram = 1;
     }
     
     Dis_Dig3_C_Ram = 0;
     if(Dis_Dig3_C_Add & Dis_Dig3_C_Dat)
     {
          Dis_Dig3_C_Ram = 1;
     }
     
     Dis_Dig3_B_Ram = 0;
     if(Dis_Dig3_B_Add & Dis_Dig3_B_Dat)
     {
          Dis_Dig3_B_Ram = 1;
     }
     
     Dis_Dig3_A_Ram = 0;
     if(Dis_Dig3_A_Add & Dis_Dig3_A_Dat)
     {
          Dis_Dig3_A_Ram = 1;
     }

          Dis_Dig4_G_Ram = 0;
     if(Dis_Dig4_G_Add & Dis_Dig4_G_Dat)
     {
          Dis_Dig4_G_Ram = 1;
     }
     
     Dis_Dig4_F_Ram = 0;
     if(Dis_Dig4_F_Add & Dis_Dig4_F_Dat)
     {
          Dis_Dig4_F_Ram = 1;
     }
     
     Dis_Dig4_E_Ram = 0;
     if(Dis_Dig4_E_Add & Dis_Dig4_E_Dat)
     {
          Dis_Dig4_E_Ram = 1;
     }
     
     Dis_Dig4_D_Ram = 0;
     if(Dis_Dig4_D_Add & Dis_Dig4_D_Dat)
     {
          Dis_Dig4_D_Ram = 1;
     }
     
     Dis_Dig4_C_Ram = 0;
     if(Dis_Dig4_C_Add & Dis_Dig4_C_Dat)
     {
          Dis_Dig4_C_Ram = 1;
     }
     
     Dis_Dig4_B_Ram = 0;
     if(Dis_Dig4_B_Add & Dis_Dig4_B_Dat)
     {
          Dis_Dig4_B_Ram = 1;
     }
     
     Dis_Dig4_A_Ram = 0;
     if(Dis_Dig4_A_Add & Dis_Dig4_A_Dat)
     {
          Dis_Dig4_A_Ram = 1;
     }
}
//------------------------------E N D-------------------------------------------



void Mod_LCD_RefreshRam(SelectType_Def _In_Type)
{
     if(FanLeaf == _In_Type)
     {
           Mod_LCD_RefreshFanFalshSpeed(); 
     }
     else if(AnyOther == _In_Type)
     {
           Mod_LCD_RefreshHourProgram();
           Mod_LCD_RefreshChar();
           Mod_LCD_RefreshDig();
     }
}



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************加载数字数据****************************************
void Mod_LCD_UpDateDigtal(INT8U *_In_Buff,SelectDigtalType_Def _In_Type)
{
     if(DisChar == _In_Type)
     {
          Dis_Dig1 =  _In_Buff[0];
          Dis_Dig2 =  _In_Buff[1];
          Dis_Dig3 =  _In_Buff[2];
          Dis_Dig4 =  _In_Buff[3];
     }
     else
     {
          Dis_Dig1 =  BcdCodeTable[_In_Buff[0]];
          Dis_Dig2 =  BcdCodeTable[_In_Buff[1]];
          Dis_Dig3 =  BcdCodeTable[_In_Buff[2]];
          Dis_Dig4 =  BcdCodeTable[_In_Buff[3]];
     }
     
}
//------------------------------E N D-------------------------------------------

//-----------------------Mod_Keyboard.c--END------------------------------------


