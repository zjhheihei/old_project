//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_LCD_H
#define _MOD_LCD_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//------------------------------E N D-------------------------------------------
#include "intrinsics.h"
#define MAX_LCD_RAM  22
extern INT8U LCDBuff[MAX_LCD_RAM];
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************�Ǳ����ַ��궨��**************************************
#define BLACK   0
#define CHAR_0  0X3F
#define CHAR_1  0X06
#define CHAR_2  0X5B
#define CHAR_3  0X4F
#define CHAR_4  0X66
#define CHAR_5  0X6D
#define CHAR_6  0X7D
#define CHAR_7  0X07
#define CHAR_8  0X7F
#define CHAR_9  0X6F
#define CHAR_A  0X77
#define CHAR_B  0X7C
#define CHAR_C  0X39
#define CHAR_D  0X5E
#define CHAR_E  0X79
#define CHAR_F  0X71
#define CHAR_G  0X3D
#define CHAR_H  0X76
#define CHAR_I  0X04
#define CHAR_J  0X0E
#define CHAR_K  0X70
#define CHAR_L  0X38
#define CHAR_N  0X37
#define CHAR_R  0X50
#define CHAR_S  0X6D
#define CHAR_T  0X31
#define CHAR_O  0X3F
#define CHAR_o  0X5C
#define CHAR_P  0X73
#define CHAR_U  0X3E
//#define CHAR_K  0X07
#define CHAR_t  0x78
//#define CHAR_Y  0X72
#define CHAR_Y  0X6E
#define CHAR_NEGATIVE  0x40  //����
#define CHAR_UNIT_C  0X61    //�¶ȵ�СC
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************�����ַ��궨��****************************************
#define TableBlack         10
#define TableNegative      11
#define TableUnitC         12
#define TableCharL         13
#define TableCharN         14
#define TableCharA         15    
//------------------------------E N D-------------------------------------------

//#define DIS_KEY_DEC_1_ADD        LCDBuff[0]
//#define DIS_KEY_DEC_1_DAT        0x02
//#define DIS_KEY_DEC_6_ADD        LCDBuff[0]
//#define DIS_KEY_DEC_6_DAT        0x04
//#define DIS_MODE_AIR_WARM_ADD    LCDBuff[0]
//#define DIS_MODE_AIR_WARM_DAT    0x08
//#define DIS_MODE_FLOOR_WARM_ADD  LCDBuff[0]
//#define DIS_MODE_FLOOR_WARM_DAT  0x10
//#define DIS_FAN_AUTO_ADD         LCDBuff[0]
//#define DIS_FAN_AUTO_DAT         0x20
//#define DIS_FLOOR_OUTPUT_ADD     LCDBuff[0]
//#define DIS_FLOOR_OUTPUT_DAT     0x80
//#define DIS_FAN_OUTPUT_ADD       LCDBuff[0]
//#define DIS_FAN_OUTPUT_DAT       0x40
//
//           
//#define DIS_DIG7_C_ADD           LCDBuff[1]
//#define DIS_DIG7_C_DAT           0x01 
//#define DIS_DIG7_D_ADD           LCDBuff[1]
//#define DIS_DIG7_D_DAT           0x02
//#define DIS_DIG6_C_ADD           LCDBuff[1]
//#define DIS_DIG6_C_DAT           0x04 
//#define DIS_DIG6_D_ADD           LCDBuff[1]
//#define DIS_DIG6_D_DAT           0x08
//#define DIS_DIG5_C_ADD           LCDBuff[1]
//#define DIS_DIG5_C_DAT           0x10 
//#define DIS_DIG5_D_ADD           LCDBuff[1]
//#define DIS_DIG5_D_DAT           0x20
//#define DIS_DIG4_C_ADD           LCDBuff[1]
//#define DIS_DIG4_C_DAT           0x40 
//#define DIS_DIG4_D_ADD           LCDBuff[1]
//#define DIS_DIG4_D_DAT           0x80
//
//#define DIS_DIG3_C_ADD           LCDBuff[2]
//#define DIS_DIG3_C_DAT           0x01 
////#define DIS_DIG3_D_ADD           LCDBuff[2]
////#define DIS_DIG3_D_DAT           0x02
//#define DIS_LOCK_ADD             LCDBuff[2]   
//#define DIS_LOCK_DAT             0X04
//
//
//#define DIS_DIG2_C_ADD           LCDBuff[15]
//#define DIS_DIG2_C_DAT           0x01 
////#define DIS_DIG2_D_ADD           LCDBuff[15]
////#define DIS_DIG2_D_DAT           0x02 
//#define DIS_DIG1_C_ADD           LCDBuff[15]
//#define DIS_DIG1_C_DAT           0x04 
////#define DIS_DIG1_D_ADD           LCDBuff[15]
////#define DIS_DIG1_D_DAT           0x08 
//
//
//#define DIS_KEY_DEC_2_ADD        LCDBuff[3]
//#define DIS_KEY_DEC_2_DAT        0x20
//#define DIS_KEY_DEC_5_ADD        LCDBuff[3]
//#define DIS_KEY_DEC_5_DAT        0x40
//#define DIS_MODE_FLOOR_COLD_ADD  LCDBuff[3]
//#define DIS_MODE_FLOOR_COLD_DAT  0x80
//
//#define DIS_FAN_ADD              LCDBuff[4]
//#define DIS_FAN_DAT              0x01
//#define DIS_FAN_LOW_ADD          LCDBuff[4]
//#define DIS_FAN_LOW_DAT          0x02
//#define DIS_KEY_ADD_1_ADD        LCDBuff[4]
//#define DIS_KEY_ADD_1_DAT        0x04
//#define DIS_KEY_ADD_6_ADD        LCDBuff[4]
//#define DIS_KEY_ADD_6_DAT        0x08
//#define DIS_DIG7_G_ADD           LCDBuff[4]
//#define DIS_DIG7_G_DAT           0x10
//#define DIS_DIG7_E_ADD           LCDBuff[4]
//#define DIS_DIG7_E_DAT           0x20
//#define DIS_DIG6_G_ADD           LCDBuff[4]
//#define DIS_DIG6_G_DAT           0x40
//#define DIS_DIG6_E_ADD           LCDBuff[4]
//#define DIS_DIG6_E_DAT           0x80
//
//#define DIS_DIG5_G_ADD           LCDBuff[5]
//#define DIS_DIG5_G_DAT           0x01
//#define DIS_DIG5_E_ADD           LCDBuff[5]
//#define DIS_DIG5_E_DAT           0x02
//#define DIS_DIG4_G_ADD           LCDBuff[5]
//#define DIS_DIG4_G_DAT           0x04
//#define DIS_DIG4_E_ADD           LCDBuff[5]
//#define DIS_DIG4_E_DAT           0x08
//#define DIS_DIG3_G_ADD           LCDBuff[5]
//#define DIS_DIG3_G_DAT           0x10
////#define DIS_DIG3_E_ADD           LCDBuff[5]
////#define DIS_DIG3_E_DAT           0x20
//#define DIS_SET_TEMP_ADD         LCDBuff[5] 
//#define DIS_SET_TEMP_DAT         0x40
//
//#define DIS_DIG2_G_ADD           LCDBuff[17]
//#define DIS_DIG2_G_DAT           0x01
////#define DIS_DIG2_E_ADD           LCDBuff[17]
////#define DIS_DIG2_E_DAT           0x02
//#define DIS_DIG1_G_ADD           LCDBuff[17]
//#define DIS_DIG1_G_DAT           0x04
////#define DIS_DIG1_E_ADD           LCDBuff[17]
////#define DIS_DIG1_E_DAT           0x08
//
//
//#define DIS_KEY_DEC_3_ADD        LCDBuff[7]
//#define DIS_KEY_DEC_3_DAT        0x02
//#define DIS_KEY_DEC_4_ADD        LCDBuff[7]
//#define DIS_KEY_DEC_4_DAT        0x04
//#define DIS_MODE_AIR_COLD_ADD    LCDBuff[7]
//#define DIS_MODE_AIR_COLD_DAT    0x08
//#define DIS_KEY_FAN_ADD          LCDBuff[7]
//#define DIS_KEY_FAN_DAT          0x10
//#define DIS_FAN_MID_ADD          LCDBuff[7]
//#define DIS_FAN_MID_DAT          0x20
//#define DIS_KEY_ADD_2_ADD        LCDBuff[7]
//#define DIS_KEY_ADD_2_DAT        0x40
//#define DIS_KEY_ADD_5_ADD        LCDBuff[7]
//#define DIS_KEY_ADD_5_DAT        0x80
//
//#define DIS_DIG7_B_ADD           LCDBuff[8]
//#define DIS_DIG7_B_DAT           0x01
//#define DIS_DIG7_F_ADD           LCDBuff[8]
//#define DIS_DIG7_F_DAT           0x02
//#define DIS_DIG6_B_ADD           LCDBuff[8]
//#define DIS_DIG6_B_DAT           0x04
//#define DIS_DIG6_F_ADD           LCDBuff[8]
//#define DIS_DIG6_F_DAT           0x08
//#define DIS_DIG5_B_ADD           LCDBuff[8]
//#define DIS_DIG5_B_DAT           0x10
//#define DIS_DIG5_F_ADD           LCDBuff[8]
//#define DIS_DIG5_F_DAT           0x20
//#define DIS_DIG4_B_ADD           LCDBuff[8]
//#define DIS_DIG4_B_DAT           0x40
//#define DIS_DIG4_F_ADD           LCDBuff[8]
//#define DIS_DIG4_F_DAT           0x80
//
//#define DIS_DIG3_B_ADD           LCDBuff[9]
//#define DIS_DIG3_B_DAT           0x01
//#define DIS_DIG3_F_ADD           LCDBuff[9]
//#define DIS_DIG3_F_DAT           0x02
//#define DIS_MEAS_TEMP_ADD        LCDBuff[9]    
//#define DIS_MEAS_TEMP_DAT        0x04
//
//#define DIS_DIG2_B_ADD           LCDBuff[19]
//#define DIS_DIG2_B_DAT           0x01
//#define DIS_DIG2_F_ADD           LCDBuff[19]
//#define DIS_DIG2_F_DAT           0x02
//#define DIS_DIG1_B_ADD           LCDBuff[19]
//#define DIS_DIG1_B_DAT           0x04
//#define DIS_DIG1_F_ADD           LCDBuff[19]
//#define DIS_DIG1_F_DAT           0x08
//
//
//#define DIS_SLEEP_ADD            LCDBuff[10] 
//#define DIS_SLEEP_DAT            0x40
//#define DIS_KEY_MENU_ADD         LCDBuff[10]
//#define DIS_KEY_MENU_DAT         0x80
//
//#define DIS_KEY_POW_ADD          LCDBuff[11]
//#define DIS_KEY_POW_DAT          0x01
//#define DIS_FAN_HIGH_ADD         LCDBuff[11]
//#define DIS_FAN_HIGH_DAT         0x02
//#define DIS_KEY_ADD_3_ADD        LCDBuff[11]
//#define DIS_KEY_ADD_3_DAT        0x04
//#define DIS_KEY_ADD_4_ADD        LCDBuff[11]
//#define DIS_KEY_ADD_4_DAT        0x08
//#define DIS_MEAS_CHAR_ADD        LCDBuff[11]
//#define DIS_MEAS_CHAR_DAT        0x10
//#define DIS_DIG7_A_ADD           LCDBuff[11]
//#define DIS_DIG7_A_DAT           0x20
//#define DIS_DP_ADD               LCDBuff[11]
//#define DIS_DP_DAT               0x40
//#define DIS_DIG6_A_ADD           LCDBuff[11]
//#define DIS_DIG6_A_DAT           0x80
//
//#define DIS_DIG5_A_ADD           LCDBuff[12]
//#define DIS_DIG5_A_DAT           0x02
//#define DIS_TIMING_OFF_ADD       LCDBuff[12]
//#define DIS_TIMING_OFF_DAT       0x04
//#define DIS_DIG4_A_ADD           LCDBuff[12]
//#define DIS_DIG4_A_DAT           0x08
//#define DIS_TIMING_ON_ADD        LCDBuff[12]
//#define DIS_TIMING_ON_DAT        0x10
//#define DIS_DIG3_A_ADD           LCDBuff[12]
//#define DIS_DIG3_A_DAT           0x20
//#define DIS_WIFI_ADD             LCDBuff[12]
//#define DIS_WIFI_DAT             0x40
//
//#define DIS_COL_ADD              LCDBuff[21]
//#define DIS_COL_DAT              0x01
//#define DIS_DIG2_A_ADD           LCDBuff[21]
//#define DIS_DIG2_A_DAT           0x02
//#define DIS_LTP_ADD              LCDBuff[21]
//#define DIS_LTP_DAT              0x04
//#define DIS_DIG1_A_ADD           LCDBuff[21]
//#define DIS_DIG1_A_DAT           0x08



#define DIS_OUTPUT_RADIATION_ADD   LCDBuff[1]
#define DIS_OUTPUT_RADIATION_DAT   0X80
#define DIS_DIG3_D_ADD             LCDBuff[1]
#define DIS_DIG3_D_DAT             0X40
#define DIS_DIG3_E_ADD             LCDBuff[1]
#define DIS_DIG3_E_DAT             0X20
#define DIS_DIG2_D_ADD             LCDBuff[1]
#define DIS_DIG2_D_DAT             0X10
#define DIS_DIG2_E_ADD             LCDBuff[1]
#define DIS_DIG2_E_DAT             0X08
#define DIS_DIG1_D_ADD             LCDBuff[1]
#define DIS_DIG1_D_DAT             0X04
#define DIS_DIG1_E_ADD             LCDBuff[1]
#define DIS_DIG1_E_DAT             0X02

#define DIS_WIND_MARK_ADD          LCDBuff[0]
#define DIS_WIND_MARK_DAT          0X04
#define DIS_WIND_CLOSE_ADD         LCDBuff[0]
#define DIS_WIND_CLOSE_DAT         0X02

#define DIS_LOCK_ADD               LCDBuff[2]
#define DIS_LOCK_DAT               0X08
#define DIS_RH_MARK_ADD            LCDBuff[2]
#define DIS_RH_MARK_DAT            0X04
#define DIS_DIG4_A_ADD             LCDBuff[2]
#define DIS_DIG4_A_DAT             0X02
#define DIS_DIG4_F_ADD             LCDBuff[2]
#define DIS_DIG4_F_DAT             0X01

#define DIS_DIG6_A_ADD             LCDBuff[15]
#define DIS_DIG6_A_DAT             0X08
#define DIS_DIG6_F_ADD             LCDBuff[15]
#define DIS_DIG6_F_DAT             0X04
#define DIS_DIG5_A_ADD             LCDBuff[15]
#define DIS_DIG5_A_DAT             0X02
#define DIS_DIG5_F_ADD             LCDBuff[15]
#define DIS_DIG5_F_DAT             0X01


#define DIS_TEMP_MARK_ADD          LCDBuff[5]
#define DIS_TEMP_MARK_DAT          0X08
#define DIS_DIG3_C_ADD             LCDBuff[5]
#define DIS_DIG3_C_DAT             0X04
#define DIS_DIG3_G_ADD             LCDBuff[5]
#define DIS_DIG3_G_DAT             0X02
#define DIS_DIG2_C_ADD             LCDBuff[5]
#define DIS_DIG2_C_DAT             0X01

#define DIS_DIG2_G_ADD             LCDBuff[4]
#define DIS_DIG2_G_DAT             0X80
#define DIS_DIG1_C_ADD             LCDBuff[4]
#define DIS_DIG1_C_DAT             0X40
#define DIS_DIG1_G_ADD             LCDBuff[4]
#define DIS_DIG1_G_DAT             0X20
#define DIS_SET_TEMP_ADD           LCDBuff[4]
#define DIS_SET_TEMP_DAT           0X10

#define DIS_WIND_LOW_1_ADD         LCDBuff[3]
#define DIS_WIND_LOW_1_DAT         0X20


#define DIS_WIND_HIGH_2_ADD        LCDBuff[5]
#define DIS_WIND_HIGH_2_DAT        0X80
#define DIS_PERCENT_ADD            LCDBuff[5]
#define DIS_PERCENT_DAT            0X40
#define DIS_DIG4_B_ADD             LCDBuff[5]
#define DIS_DIG4_B_DAT             0X20
#define DIS_DIG4_G_ADD             LCDBuff[5]
#define DIS_DIG4_G_DAT             0X10

#define DIS_DIG6_B_ADD             LCDBuff[17]
#define DIS_DIG6_B_DAT             0X08
#define DIS_DIG6_G_ADD             LCDBuff[17]
#define DIS_DIG6_G_DAT             0X04
#define DIS_DIG5_B_ADD             LCDBuff[17]
#define DIS_DIG5_B_DAT             0X02
#define DIS_DIG5_G_ADD             LCDBuff[17]
#define DIS_DIG5_G_DAT             0X01


#define DIS_WIFI_ADD               LCDBuff[8]
#define DIS_WIFI_DAT               0X80
#define DIS_DIG3_B_ADD             LCDBuff[8]
#define DIS_DIG3_B_DAT             0X40
#define DIS_DIG3_F_ADD             LCDBuff[8]
#define DIS_DIG3_F_DAT             0X20
#define DIS_DIG2_B_ADD             LCDBuff[8]
#define DIS_DIG2_B_DAT             0X10
#define DIS_DIG2_F_ADD             LCDBuff[8]
#define DIS_DIG2_F_DAT             0X08
#define DIS_DIG1_B_ADD             LCDBuff[8]
#define DIS_DIG1_B_DAT             0X04
#define DIS_DIG1_F_ADD             LCDBuff[8]
#define DIS_DIG1_F_DAT             0X02
#define DIS_MEAS_TEMP_ADD          LCDBuff[8]
#define DIS_MEAS_TEMP_DAT          0X01
#define DIS_MEAS_RH_ADD            LCDBuff[7]
#define DIS_MEAS_RH_DAT            0X08
#define DIS_WIND_LOW_2_ADD         LCDBuff[7]
#define DIS_WIND_LOW_2_DAT         0X02


#define DIS_WIND_HIGH_1_ADD        LCDBuff[9]
#define DIS_WIND_HIGH_1_DAT        0X08
#define DIS_DIG4_C_ADD             LCDBuff[9]
#define DIS_DIG4_C_DAT             0X02
#define DIS_DIG4_E_ADD             LCDBuff[9]
#define DIS_DIG4_E_DAT             0X01


#define DIS_DIG6_C_ADD             LCDBuff[19]
#define DIS_DIG6_C_DAT             0X08
#define DIS_DIG6_E_ADD             LCDBuff[19]
#define DIS_DIG6_E_DAT             0X04
#define DIS_DIG5_C_ADD             LCDBuff[19]
#define DIS_DIG5_C_DAT             0X02
#define DIS_DIG5_E_ADD             LCDBuff[19]
#define DIS_DIG5_E_DAT             0X01

#define DIS_OUTPUT_FAN_ADD         LCDBuff[12]
#define DIS_OUTPUT_FAN_DAT         0X08
#define DIS_DIG3_A_ADD             LCDBuff[12]
#define DIS_DIG3_A_DAT             0X04
#define DIS_SLEEP_ADD              LCDBuff[12]
#define DIS_SLEEP_DAT              0X02
#define DIS_DIG2_A_ADD             LCDBuff[12]
#define DIS_DIG2_A_DAT             0X01

#define DIS_MODE_AI_ADD            LCDBuff[11]
#define DIS_MODE_AI_DAT            0X80
#define DIS_DIG1_A_ADD             LCDBuff[11]
#define DIS_DIG1_A_DAT             0X40
#define DIS_MODE_RADIATION_ADD     LCDBuff[11]
#define DIS_MODE_RADIATION_DAT     0X20
#define DIS_MODE_FAN_ADD           LCDBuff[11]
#define DIS_MODE_FAN_DAT           0X10


#define DIS_LTP_ADD                LCDBuff[10]
#define DIS_LTP_DAT                0X80
#define DIS_WIND_NATURE_ADD        LCDBuff[10]
#define DIS_WIND_NATURE_DAT        0X40
#define DIS_WIND_MID_1_ADD         LCDBuff[7]
#define DIS_WIND_MID_1_DAT         0X02


#define DIS_WIND_MID_2_ADD         LCDBuff[12]
#define DIS_WIND_MID_2_DAT         0X80
#define DIS_DIG4_D_ADD             LCDBuff[11]
#define DIS_DIG4_D_DAT             0X20

#define DIS_DIG6_D_ADD             LCDBuff[21]
#define DIS_DIG6_D_DAT             0X20
#define DIS_DIG5_D_ADD             LCDBuff[21]
#define DIS_DIG5_D_DAT             0X20

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//名称: Mod_LCD_Configure
//功能: 初始化LCD模块
//入口: 无
//出口: 无 
void mod_lcd_configure(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称:    Mod_LCD_SetAllDis
//功能:    液晶全显全灭控制
//入口:    <-----------------传入_In_NewStatus,BN_TRUE全显 BN_FALSE全灭
//出口:    无
void Mod_LCD_SetAllDis(BOOLEAN _In_NewStatus);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称:    mod_lcd_showDig
//         
//功能:    7位数字输入
//入口:    <<-----------------------------指针buf
//出口:无  
void mod_lcd_showDig(INT8U *buf);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称: mod_lcd_refresh
//功能: 数据输入LCD寄存器
//入口: 无
//出口: 无  
void mod_lcd_refresh(void);
//------------------------------E N D-------------------------------------------


#endif


//-----------------------Mod_LCD.h--END------------------------------------
