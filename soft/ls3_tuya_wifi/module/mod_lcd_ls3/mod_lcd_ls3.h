//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_LCD_LS3_H
#define _MOD_LCD_LS3_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//------------------------------E N D-------------------------------------------
#include "intrinsics.h"
#define MAX_LCD_RAM  14 
#define MAX_DIG_RAM  7
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
#define TABLE_BLACK        10
#define TableNegative      11
#define TableUnitC         12
#define TableCharL         13
#define TableCharN         14
#define TableCharA         15    
//------------------------------E N D-------------------------------------------




//#define DIS_THURSDAY_ADD              LCDBuff[0]
//#define DIS_THURSDAY_DAT              0X02
//#define DIS_FRIDAY_ADD                LCDBuff[0]
//#define DIS_FRIDAY_DAT                0X04
//#define DIS_DIG1_D_ADD                LCDBuff[0]
//#define DIS_DIG1_D_DAT                0X08
//#define DIS_MEAS_TEMP_MARK_ADD             LCDBuff[0]
//#define DIS_MEAS_TEMP_MARK_DAT             0X10
//#define DIS_DIG2_D_ADD                LCDBuff[0]
//#define DIS_DIG2_D_DAT                0X20
//#define DIS_P_ADD                     LCDBuff[0]
//#define DIS_P_DAT                     0X40
//#define DIS_DIG3_D_ADD                LCDBuff[0]
//#define DIS_DIG3_D_DAT                0X80
//
//
//#define DIS_MOD_WARM_ADD              LCDBuff[1]
//#define DIS_MOD_WARM_DAT              0X02
//#define DIS_MOD_COLD_ADD              LCDBuff[1]
//#define DIS_MOD_COLD_DAT              0X04
//#define DIS_INTERVAL_3_ADD            LCDBuff[1]
//#define DIS_INTERVAL_3_DAT            0X08
//#define DIS_OUTPUT_VALVE_ADD          LCDBuff[1]
//#define DIS_OUTPUT_VALVE_DAT          0X10
//#define DIS_SET_TEMP_ADD              LCDBuff[1]
//#define DIS_SET_TEMP_DAT              0X20
//#define DIS_DIG4_A_ADD                LCDBuff[1]
//#define DIS_DIG4_A_DAT                0X40
//#define DIS_SET_TEMP_MARK_ADD         LCDBuff[1]
//#define DIS_SET_TEMP_MARK_DAT         0X80
//
//
//#define DIS_DIG5_A_ADD                LCDBuff[2]
//#define DIS_DIG5_A_DAT                0X01
//#define DIS_COL_ADD                   LCDBuff[2]
//#define DIS_COL_DAT                   0X02
//#define DIS_DIG6_A_ADD                LCDBuff[2]
//#define DIS_DIG6_A_DAT                0X04
//#define DIS_P2_ADD                    LCDBuff[2]
//#define DIS_P2_DAT                    0X08
//#define DIS_DIG7_A_ADD                LCDBuff[2]
//#define DIS_DIG7_A_DAT                0X10
//
//#define DIS_WEDNESDAY_ADD             LCDBuff[3]
//#define DIS_WEDNESDAY_DAT             0X20
//#define DIS_SATURDAY_ADD              LCDBuff[3]
//#define DIS_SATURDAY_DAT              0X40
//#define DIS_DIG1_E_ADD                LCDBuff[3]
//#define DIS_DIG1_E_DAT                0X80
//
//#define DIS_DIG1_C_ADD                LCDBuff[4]
//#define DIS_DIG1_C_DAT                0X01
//#define DIS_DIG2_E_ADD                LCDBuff[4]
//#define DIS_DIG2_E_DAT                0X02
//#define DIS_DIG2_C_ADD                LCDBuff[4]
//#define DIS_DIG2_C_DAT                0X04
//#define DIS_DIG3_E_ADD                LCDBuff[4]
//#define DIS_DIG3_E_DAT                0X08
//#define DIS_DIG3_C_ADD                LCDBuff[4]
//#define DIS_DIG3_C_DAT                0X10
//#define DIS_MOD_MAN_SET_ADD           LCDBuff[4]
//#define DIS_MOD_MAN_SET_DAT           0X20
//#define DIS_LTP_ADD                   LCDBuff[4]
//#define DIS_LTP_DAT                   0X40
//#define DIS_INTERVAL_2_ADD            LCDBuff[4]
//#define DIS_INTERVAL_2_DAT            0X80
//
//
//#define DIS_OUTPUT_FLOOR_ADD          LCDBuff[5]
//#define DIS_OUTPUT_FLOOR_DAT          0X01
//#define DIS_DIG4_B_ADD                LCDBuff[5]
//#define DIS_DIG4_B_DAT                0X02
//#define DIS_DIG4_F_ADD                LCDBuff[5]
//#define DIS_DIG4_F_DAT                0X04
//#define DIS_DIG5_B_ADD                LCDBuff[5]
//#define DIS_DIG5_B_DAT                0X08
//#define DIS_DIG5_F_ADD                LCDBuff[5]
//#define DIS_DIG5_F_DAT                0X10
//#define DIS_DIG6_B_ADD                LCDBuff[5]
//#define DIS_DIG6_B_DAT                0X20
//#define DIS_DIG6_F_ADD                LCDBuff[5]
//#define DIS_DIG6_F_DAT                0X40
//#define DIS_DIG7_B_ADD                LCDBuff[5]
//#define DIS_DIG7_B_DAT                0X80
//
//#define DIS_DIG7_F_ADD                LCDBuff[6]
//#define DIS_DIG7_F_DAT                0X01
//
//
//
//#define DIS_TUESDAY_ADD               LCDBuff[7]
//#define DIS_TUESDAY_DAT               0X02
//#define DIS_SUNDAY_ADD                LCDBuff[7]
//#define DIS_SUNDAY_DAT                0X04
//#define DIS_DIG1_F_ADD                LCDBuff[7]
//#define DIS_DIG1_F_DAT                0X08
//#define DIS_DIG1_G_ADD                LCDBuff[7]
//#define DIS_DIG1_G_DAT                0X10
//#define DIS_DIG2_F_ADD                LCDBuff[7]
//#define DIS_DIG2_F_DAT                0X20
//#define DIS_DIG2_G_ADD                LCDBuff[7]
//#define DIS_DIG2_G_DAT                0X40
//#define DIS_DIG3_F_ADD                LCDBuff[7]
//#define DIS_DIG3_F_DAT                0X80
//
//
//
//#define DIS_DIG3_G_ADD                LCDBuff[8]
//#define DIS_DIG3_G_DAT                0X01
//#define DIS_MOD_TIME_SET_ADD          LCDBuff[8]
//#define DIS_MOD_TIME_SET_DAT          0X02
//#define DIS_NET_ADD                   LCDBuff[8]
//#define DIS_NET_DAT                   0X04
//#define DIS_INTERVAL_1_ADD            LCDBuff[8]
//#define DIS_INTERVAL_1_DAT            0X08
//#define DIS_INTERVAL_4_ADD            LCDBuff[8]
//#define DIS_INTERVAL_4_DAT            0X10
//#define DIS_DIG4_G_ADD                LCDBuff[8]
//#define DIS_DIG4_G_DAT                0X20
//#define DIS_DIG4_E_ADD                LCDBuff[8]
//#define DIS_DIG4_E_DAT                0X40
//#define DIS_DIG5_G_ADD                LCDBuff[8]
//#define DIS_DIG5_G_DAT                0X80
//
//
//#define DIS_DIG5_E_ADD                LCDBuff[9]
//#define DIS_DIG5_E_DAT                0X01
//#define DIS_DIG6_G_ADD                LCDBuff[9]
//#define DIS_DIG6_G_DAT                0X02
//#define DIS_DIG6_E_ADD                LCDBuff[9]
//#define DIS_DIG6_E_DAT                0X04
//#define DIS_DIG7_G_ADD                LCDBuff[9]
//#define DIS_DIG7_G_DAT                0X08
//#define DIS_DIG7_E_ADD                LCDBuff[9]
//#define DIS_DIG7_E_DAT                0X10
//
//
//#define DIS_MONDAY_ADD                LCDBuff[10]
//#define DIS_MONDAY_DAT                0X20
//#define DIS_LOCK_ADD                  LCDBuff[10]
//#define DIS_LOCK_DAT                  0X40
//#define DIS_DIG1_A_ADD                LCDBuff[10]
//#define DIS_DIG1_A_DAT                0X80
//
//
//
//#define DIS_DIG1_B_ADD                LCDBuff[11]
//#define DIS_DIG1_B_DAT                0X01
//#define DIS_DIG2_A_ADD                LCDBuff[11]
//#define DIS_DIG2_A_DAT                0X02
//#define DIS_DIG2_B_ADD                LCDBuff[11]
//#define DIS_DIG2_B_DAT                0X04
//#define DIS_DIG3_A_ADD                LCDBuff[11]
//#define DIS_DIG3_A_DAT                0X08
//#define DIS_DIG3_B_ADD                LCDBuff[11]
//#define DIS_DIG3_B_DAT                0X10
//#define DIS_MEAS_TEMP_ADD             LCDBuff[11]
//#define DIS_MEAS_TEMP_DAT             0X20
//#define DIS_WIFI_ADD                  LCDBuff[11]
//#define DIS_WIFI_DAT                  0X40
//#define DIS_INTERVAL_0_ADD            LCDBuff[11]
//#define DIS_INTERVAL_0_DAT            0X80
//
//
//
//#define DIS_INTERVAL_5_ADD            LCDBuff[12]
//#define DIS_INTERVAL_5_DAT            0X01
//#define DIS_DIG4_C_ADD                LCDBuff[12]
//#define DIS_DIG4_C_DAT                0X02
//#define DIS_DIG4_D_ADD                LCDBuff[12]
//#define DIS_DIG4_D_DAT                0X04
//#define DIS_DIG5_C_ADD                LCDBuff[12]
//#define DIS_DIG5_C_DAT                0X08
//#define DIS_DIG5_D_ADD                LCDBuff[12]
//#define DIS_DIG5_D_DAT                0X10
//#define DIS_DIG6_C_ADD                LCDBuff[12]
//#define DIS_DIG6_C_DAT                0X20
//#define DIS_DIG6_D_ADD                LCDBuff[12]
//#define DIS_DIG6_D_DAT                0X40
//#define DIS_DIG7_C_ADD                LCDBuff[12]
//#define DIS_DIG7_C_DAT                0X80
//#define DIS_DIG7_D_ADD                LCDBuff[13]
//#define DIS_DIG7_D_DAT                0X01


#define DIS_THURSDAY_ADD              0//LCDBuff[0]
#define DIS_THURSDAY_DAT              0X02
#define DIS_FRIDAY_ADD                0//LCDBuff[0]
#define DIS_FRIDAY_DAT                0X04
#define DIS_DIG1_D_ADD                0//LCDBuff[0]
#define DIS_DIG1_D_DAT                0X08
#define DIS_MEAS_TEMP_MARK_ADD             0//LCDBuff[0]
#define DIS_MEAS_TEMP_MARK_DAT             0X10
#define DIS_DIG2_D_ADD                0//LCDBuff[0]
#define DIS_DIG2_D_DAT                0X20
#define DIS_P_ADD                     0//LCDBuff[0]
#define DIS_P_DAT                     0X40
#define DIS_DIG3_D_ADD                0//LCDBuff[0]
#define DIS_DIG3_D_DAT                0X80


#define DIS_MOD_WARM_ADD              1//LCDBuff[1]
#define DIS_MOD_WARM_DAT              0X02
#define DIS_MOD_COLD_ADD              1//LCDBuff[1]
#define DIS_MOD_COLD_DAT              0X04
#define DIS_INTERVAL_3_ADD            1//LCDBuff[1]
#define DIS_INTERVAL_3_DAT            0X08
#define DIS_OUTPUT_VALVE_ADD          1//LCDBuff[1]
#define DIS_OUTPUT_VALVE_DAT          0X10
#define DIS_SET_TEMP_ADD              1//LCDBuff[1]
#define DIS_SET_TEMP_DAT              0X20
#define DIS_DIG4_A_ADD                1//LCDBuff[1]
#define DIS_DIG4_A_DAT                0X40
#define DIS_SET_TEMP_MARK_ADD         1//LCDBuff[1]
#define DIS_SET_TEMP_MARK_DAT         0X80


#define DIS_DIG5_A_ADD                2//LCDBuff[2]
#define DIS_DIG5_A_DAT                0X01
#define DIS_COL_ADD                   2//LCDBuff[2]
#define DIS_COL_DAT                   0X02
#define DIS_DIG6_A_ADD                2//LCDBuff[2]
#define DIS_DIG6_A_DAT                0X04
#define DIS_P2_ADD                    2//LCDBuff[2]
#define DIS_P2_DAT                    0X08
#define DIS_DIG7_A_ADD                2//LCDBuff[2]
#define DIS_DIG7_A_DAT                0X10

#define DIS_WEDNESDAY_ADD             3//LCDBuff[3]
#define DIS_WEDNESDAY_DAT             0X20
#define DIS_SATURDAY_ADD              3//LCDBuff[3]
#define DIS_SATURDAY_DAT              0X40
#define DIS_DIG1_E_ADD                3//LCDBuff[3]
#define DIS_DIG1_E_DAT                0X80

#define DIS_DIG1_C_ADD                4//LCDBuff[4]
#define DIS_DIG1_C_DAT                0X01
#define DIS_DIG2_E_ADD                4//LCDBuff[4]
#define DIS_DIG2_E_DAT                0X02
#define DIS_DIG2_C_ADD                4//LCDBuff[4]
#define DIS_DIG2_C_DAT                0X04
#define DIS_DIG3_E_ADD                4//LCDBuff[4]
#define DIS_DIG3_E_DAT                0X08
#define DIS_DIG3_C_ADD                4//LCDBuff[4]
#define DIS_DIG3_C_DAT                0X10
#define DIS_MOD_MAN_SET_ADD           4//LCDBuff[4]
#define DIS_MOD_MAN_SET_DAT           0X20
#define DIS_LTP_ADD                   4//LCDBuff[4]
#define DIS_LTP_DAT                   0X40
#define DIS_INTERVAL_2_ADD            4//LCDBuff[4]
#define DIS_INTERVAL_2_DAT            0X80


#define DIS_OUTPUT_FLOOR_ADD          5//LCDBuff[5]
#define DIS_OUTPUT_FLOOR_DAT          0X01
#define DIS_DIG4_B_ADD                5//LCDBuff[5]
#define DIS_DIG4_B_DAT                0X02
#define DIS_DIG4_F_ADD                5//LCDBuff[5]
#define DIS_DIG4_F_DAT                0X04
#define DIS_DIG5_B_ADD                5//LCDBuff[5]
#define DIS_DIG5_B_DAT                0X08
#define DIS_DIG5_F_ADD                5//LCDBuff[5]
#define DIS_DIG5_F_DAT                0X10
#define DIS_DIG6_B_ADD                5//LCDBuff[5]
#define DIS_DIG6_B_DAT                0X20
#define DIS_DIG6_F_ADD                5//LCDBuff[5]
#define DIS_DIG6_F_DAT                0X40
#define DIS_DIG7_B_ADD                5//LCDBuff[5]
#define DIS_DIG7_B_DAT                0X80

#define DIS_DIG7_F_ADD                6//LCDBuff[6]
#define DIS_DIG7_F_DAT                0X01



#define DIS_TUESDAY_ADD               7//LCDBuff[7]
#define DIS_TUESDAY_DAT               0X02
#define DIS_SUNDAY_ADD                7//LCDBuff[7]
#define DIS_SUNDAY_DAT                0X04
#define DIS_DIG1_F_ADD                7//LCDBuff[7]
#define DIS_DIG1_F_DAT                0X08
#define DIS_DIG1_G_ADD                7//LCDBuff[7]
#define DIS_DIG1_G_DAT                0X10
#define DIS_DIG2_F_ADD                7//LCDBuff[7]
#define DIS_DIG2_F_DAT                0X20
#define DIS_DIG2_G_ADD                7//LCDBuff[7]
#define DIS_DIG2_G_DAT                0X40
#define DIS_DIG3_F_ADD                7//LCDBuff[7]
#define DIS_DIG3_F_DAT                0X80



#define DIS_DIG3_G_ADD                8//LCDBuff[8]
#define DIS_DIG3_G_DAT                0X01
#define DIS_MOD_TIME_SET_ADD          8//LCDBuff[8]
#define DIS_MOD_TIME_SET_DAT          0X02
#define DIS_NET_ADD                   8//LCDBuff[8]
#define DIS_NET_DAT                   0X04
#define DIS_INTERVAL_1_ADD            8//LCDBuff[8]
#define DIS_INTERVAL_1_DAT            0X08
#define DIS_INTERVAL_4_ADD            8//LCDBuff[8]
#define DIS_INTERVAL_4_DAT            0X10
#define DIS_DIG4_G_ADD                8//LCDBuff[8]
#define DIS_DIG4_G_DAT                0X20
#define DIS_DIG4_E_ADD                8//LCDBuff[8]
#define DIS_DIG4_E_DAT                0X40
#define DIS_DIG5_G_ADD                8//LCDBuff[8]
#define DIS_DIG5_G_DAT                0X80


#define DIS_DIG5_E_ADD                9//LCDBuff[9]
#define DIS_DIG5_E_DAT                0X01
#define DIS_DIG6_G_ADD                9//LCDBuff[9]
#define DIS_DIG6_G_DAT                0X02
#define DIS_DIG6_E_ADD                9//LCDBuff[9]
#define DIS_DIG6_E_DAT                0X04
#define DIS_DIG7_G_ADD                9//LCDBuff[9]
#define DIS_DIG7_G_DAT                0X08
#define DIS_DIG7_E_ADD                9//LCDBuff[9]
#define DIS_DIG7_E_DAT                0X10


#define DIS_MONDAY_ADD                10//LCDBuff[10]
#define DIS_MONDAY_DAT                0X20
#define DIS_LOCK_ADD                  10//LCDBuff[10]
#define DIS_LOCK_DAT                  0X40
#define DIS_DIG1_A_ADD                10//LCDBuff[10]
#define DIS_DIG1_A_DAT                0X80



#define DIS_DIG1_B_ADD                11//LCDBuff[11]
#define DIS_DIG1_B_DAT                0X01
#define DIS_DIG2_A_ADD                11//LCDBuff[11]
#define DIS_DIG2_A_DAT                0X02
#define DIS_DIG2_B_ADD                11//LCDBuff[11]
#define DIS_DIG2_B_DAT                0X04
#define DIS_DIG3_A_ADD                11//LCDBuff[11]
#define DIS_DIG3_A_DAT                0X08
#define DIS_DIG3_B_ADD                11//LCDBuff[11]
#define DIS_DIG3_B_DAT                0X10
#define DIS_MEAS_TEMP_ADD             11//LCDBuff[11]
#define DIS_MEAS_TEMP_DAT             0X20
#define DIS_WIFI_ADD                  11//LCDBuff[11]
#define DIS_WIFI_DAT                  0X40
#define DIS_INTERVAL_0_ADD            11//LCDBuff[11]
#define DIS_INTERVAL_0_DAT            0X80



#define DIS_INTERVAL_5_ADD            12//LCDBuff[12]
#define DIS_INTERVAL_5_DAT            0X01
#define DIS_DIG4_C_ADD                12//LCDBuff[12]
#define DIS_DIG4_C_DAT                0X02
#define DIS_DIG4_D_ADD                12//LCDBuff[12]
#define DIS_DIG4_D_DAT                0X04
#define DIS_DIG5_C_ADD                12//LCDBuff[12]
#define DIS_DIG5_C_DAT                0X08
#define DIS_DIG5_D_ADD                12//LCDBuff[12]
#define DIS_DIG5_D_DAT                0X10
#define DIS_DIG6_C_ADD                12//LCDBuff[12]
#define DIS_DIG6_C_DAT                0X20
#define DIS_DIG6_D_ADD                12//LCDBuff[12]
#define DIS_DIG6_D_DAT                0X40
#define DIS_DIG7_C_ADD                12//LCDBuff[12]
#define DIS_DIG7_C_DAT                0X80
#define DIS_DIG7_D_ADD                13//LCDBuff[13]
#define DIS_DIG7_D_DAT                0X01
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//名称: mod_lcd_configure
//功能: 初始化LCD模块
//入口: LCD寄存器指针赋值
//出口: 无 
void mod_lcd_configure(INT8U *lcdBuf);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称:    Mod_LCD_SetAllDis
//功能:    液晶全显全灭控制
//入口:    <-----------------传入_In_NewStatus,BN_TRUE全显 BN_FALSE全灭
//出口:    无
void mod_lcd_showAll(BOOLEAN newStatus);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称:    mod_lcd_showDig
//         
//功能:    7位数字输入
//入口:    <<-----------------------------指针buf
//出口:无  
void mod_lcd_dig(INT8U *digBuf,INT8U *_out_lcdBuf);
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
