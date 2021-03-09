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
#include ".\depend\bsp_lcd.h"
#include ".\mod_lcd.h"
//------------------------------E N D-------------------------------------------


#define MAX_LCD_RAM  22
INT8U LCDBuff[MAX_LCD_RAM];


void mod_lcd_refresh(void)
{  
    bsp_refresh_lcd(LCDBuff);
}

void Mod_LCD_SetAllDis(BOOLEAN _In_NewStatus)
{
     INT8U i = 0;
     if(_In_NewStatus)
     {
        for(i = 0;i < MAX_LCD_RAM;i++)
        {
            LCDBuff[i] = 0xff;
        }
     }
     else
     {
        for(i = 0;i < MAX_LCD_RAM;i++)
        {
            LCDBuff[i] = 0x00;
        }
     }
}

void mod_lcd_configure(void)
{
    bsp_lcd_configure();    
}


//void mod_lcd_dig(INT8U Index,INT8U Data)
//{   
//   switch(Index)
//   {
//        case 1:    //1、2、3
//        {                
//            if(Data & 0x01)
//            {
//                DIS_DIG1_A_ADD |= DIS_DIG1_A_DAT;
//            }
//            if(Data & 0x02)
//            {
//                DIS_DIG1_B_ADD |= DIS_DIG1_B_DAT;
//            }
//            if(Data & 0x04)
//            {
//                DIS_DIG1_C_ADD |= DIS_DIG1_C_DAT;
//            }
//            if(Data & 0x08)
//            {
//                DIS_DIG1_D_ADD |= DIS_DIG1_D_DAT;
//            }
//            if(Data & 0x10)
//            {
//                DIS_DIG1_E_ADD |= DIS_DIG1_E_DAT;
//            }
//            if(Data & 0x20)
//            {
//                DIS_DIG1_F_ADD |= DIS_DIG1_F_DAT;                  
//            }
//            if(Data & 0x40)
//            {
//                DIS_DIG1_G_ADD |= DIS_DIG1_G_DAT;
//            }
//            break; 
//        }                                  
//        case 2:      
//        {
//            if(Data & 0x01)
//            {
//                DIS_DIG2_A_ADD |= DIS_DIG2_A_DAT;
//            }
//            if(Data & 0x02)
//            {
//              DIS_DIG2_B_ADD |= DIS_DIG2_B_DAT;
//            }
//            if(Data & 0x04)
//            {
//              DIS_DIG2_C_ADD |= DIS_DIG2_C_DAT;
//            }
//            if(Data & 0x08)
//            {
//              DIS_DIG2_D_ADD |= DIS_DIG2_D_DAT;
//            }
//            if(Data & 0x10)
//            {
//              DIS_DIG2_E_ADD |= DIS_DIG2_E_DAT;
//            }
//            if(Data & 0x20)
//            {
//                DIS_DIG2_F_ADD |= DIS_DIG2_F_DAT;                  
//            }
//            if(Data & 0x40)
//            {
//                DIS_DIG2_G_ADD |= DIS_DIG2_G_DAT;
//            }
//            break; 
//        }           
//        case 3:
//        {
//            if(Data & 0x01)
//            {
//                DIS_DIG3_A_ADD |= DIS_DIG3_A_DAT;
//            }
//            if(Data & 0x02)
//            {
//                DIS_DIG3_B_ADD |= DIS_DIG3_B_DAT;
//            }
//            if(Data & 0x04)
//            {
//                DIS_DIG3_C_ADD |= DIS_DIG3_C_DAT;
//            }
//            if(Data & 0x08)
//            {
//                DIS_DIG3_D_ADD |= DIS_DIG3_D_DAT;
//            }
//            if(Data & 0x10)
//            {
//                DIS_DIG3_E_ADD |= DIS_DIG3_E_DAT;
//            }
//            if(Data & 0x20)
//            {
//                DIS_DIG3_F_ADD |= DIS_DIG3_F_DAT;                  
//            }
//            if(Data & 0x40)
//            {
//                DIS_DIG3_G_ADD |= DIS_DIG3_G_DAT;
//            }
//            break; 
//        }                                  
//        case 4://4、5、6、7
//        {           
//            if(Data & 0x01)
//            {
//                DIS_DIG4_A_ADD |= DIS_DIG4_A_DAT;
//            }
//            if(Data & 0x02)
//            {
//                DIS_DIG4_B_ADD |= DIS_DIG4_B_DAT;
//            }
//            if(Data & 0x04)
//            {
//                DIS_DIG4_C_ADD |= DIS_DIG4_C_DAT;
//            }
//            if(Data & 0x08)
//            {
//                DIS_DIG4_D_ADD |= DIS_DIG4_D_DAT;
//            }
//            if(Data & 0x10)
//            {
//                DIS_DIG4_E_ADD |= DIS_DIG4_E_DAT;
//            }
//            if(Data & 0x20)
//            {
//                DIS_DIG4_F_ADD |= DIS_DIG4_F_DAT;                  
//            }
//            if(Data & 0x40)
//            {
//                DIS_DIG4_G_ADD |= DIS_DIG4_G_DAT;
//            }
//            break; 
//        }     
//        case 5://led5
//        {           
//            if(Data & 0x01)
//            {
//                DIS_DIG5_A_ADD |= DIS_DIG5_A_DAT;
//            }
//            if(Data & 0x02)
//            {
//                DIS_DIG5_B_ADD |= DIS_DIG5_B_DAT;
//            }
//            if(Data & 0x04)
//            {
//                DIS_DIG5_C_ADD |= DIS_DIG5_C_DAT;
//            }
//            if(Data & 0x08)
//            {
//                DIS_DIG5_D_ADD |= DIS_DIG5_D_DAT;
//            }
//            if(Data & 0x10)
//            {
//                DIS_DIG5_E_ADD |= DIS_DIG5_E_DAT;
//            }
//            if(Data & 0x20)
//            {
//                DIS_DIG5_F_ADD |= DIS_DIG5_F_DAT;                  
//            }
//            if(Data & 0x40)
//            {
//                DIS_DIG5_G_ADD |= DIS_DIG5_G_DAT;
//            }
//            break; 
//        }     
//        case 6://led6
//        {           
//            if(Data & 0x01)
//            {   
//                DIS_DIG6_A_ADD |= DIS_DIG6_A_DAT;
//            }
//            if(Data & 0x02)
//            {
//                DIS_DIG6_B_ADD |= DIS_DIG6_B_DAT;
//            }
//            if(Data & 0x04)
//            {
//                DIS_DIG6_C_ADD |= DIS_DIG6_C_DAT;
//            }
//            if(Data & 0x08)
//            {
//                DIS_DIG6_D_ADD |= DIS_DIG6_D_DAT;
//            }
//            if(Data & 0x10)
//            {
//                DIS_DIG6_E_ADD |= DIS_DIG6_E_DAT;
//            }
//            if(Data & 0x20)
//            {
//                DIS_DIG6_F_ADD |= DIS_DIG6_F_DAT;                  
//            }
//            if(Data & 0x40)
//            {
//                DIS_DIG6_G_ADD |= DIS_DIG6_G_DAT;
//            }
//            break; 
//        }     
//        default: break;         
//    }        
//}


void mod_lcd_showDig(INT8U *buf)
{    
//    mod_lcd_dig(1,buf[0]);
//    mod_lcd_dig(2,buf[1]);
//    mod_lcd_dig(3,buf[2]); 
//    mod_lcd_dig(4,buf[3]);
//    mod_lcd_dig(5,buf[4]);
//    mod_lcd_dig(6,buf[5]); 
}

//------------------------------E N D-------------------------------------------



