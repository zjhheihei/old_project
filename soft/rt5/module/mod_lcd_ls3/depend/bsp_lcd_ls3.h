//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_LCD_LS3_H
#define _BSP_LCD_LS3_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef  __Snail_DataRetype__
#include ".\SnailDataTypedef.h"
#endif
#include "DeviceSelect.h"




//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//√˚≥∆:  “∫æßƒ£øÈ≥ı ºªØ
//π¶ƒ‹:  
//»Îø⁄:  Œﬁ
//≥ˆø⁄:  Œﬁ
void bsp_lcd_configure(void);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//func
//ÂêçÁß∞:  bsp_lcd_refresh
//ÂäüËÉΩ:  Âà∑Êñ∞ÊòæÁ§∫
//ÂÖ•Âè£:  Êó†  
//       Êó†   
//Âá∫Âè£:  Œﬁ
void bsp_lcd_refresh(INT8U *ptBuf,INT8U len);
//------------------------------E N D-------------------------------------------

#define MAX_LCD_BUF_CONUT             22
            
#define DIS_DIG4_F_ADD                0
#define DIS_DIG4_F_DAT                0X02
#define DIS_DIG4_G_ADD                0
#define DIS_DIG4_G_DAT                0X04
#define DIS_DIG5_F_ADD                0
#define DIS_DIG5_F_DAT                0X08
#define DIS_DIG5_G_ADD                0
#define DIS_DIG5_G_DAT                0X10
#define DIS_DIG6_F_ADD                0
#define DIS_DIG6_F_DAT                0X20
#define DIS_DIG6_G_ADD                0
#define DIS_DIG6_G_DAT                0X40
#define DIS_DIG7_F_ADD                0
#define DIS_DIG7_F_DAT                0X80

#define DIS_DIG7_G_ADD                1
#define DIS_DIG7_G_DAT                0X01
#define DIS_SET_ADD                   1
#define DIS_SET_DAT                   0X02
#define DIS_WEEK_2_ADD                1
#define DIS_WEEK_2_DAT                0X04
#define DIS_WEEK_3_ADD                1
#define DIS_WEEK_3_DAT                0X08
#define DIS_WEEK_1_ADD                1
#define DIS_WEEK_1_DAT                0X80


#define DIS_INTERVAL_2_ADD            2
#define DIS_INTERVAL_2_DAT            0X01
#define DIS_DIG2_C_ADD                2
#define DIS_DIG2_C_DAT                0X04
#define DIS_DIG3_E_ADD                2
#define DIS_DIG3_E_DAT                0X08
#define DIS_DIG3_C_ADD                2
#define DIS_DIG3_C_DAT                0X10
#define DIS_FLOOR_WARM_ADD            2
#define DIS_FLOOR_WARM_DAT            0X20



#define DIS_DIG4_E_ADD                3
#define DIS_DIG4_E_DAT                0X20
#define DIS_DIG4_C_ADD                3
#define DIS_DIG4_C_DAT                0X40
#define DIS_DIG5_E_ADD                3
#define DIS_DIG5_E_DAT                0X80


#define DIS_DIG5_C_ADD                4
#define DIS_DIG5_C_DAT                0X01
#define DIS_DIG6_E_ADD                4
#define DIS_DIG6_E_DAT                0X02
#define DIS_DIG6_C_ADD                4
#define DIS_DIG6_C_DAT                0X04
#define DIS_DIG7_E_ADD                4
#define DIS_DIG7_E_DAT                0X08
#define DIS_DIG7_C_ADD                4
#define DIS_DIG7_C_DAT                0X10
#define DIS_LOCK_ADD                  4
#define DIS_LOCK_DAT                  0X20
#define DIS_WEEK_5_ADD                4
#define DIS_WEEK_5_DAT                0X40
#define DIS_WEEK_6_ADD                4
#define DIS_WEEK_6_DAT                0X80


#define DIS_WEEK_4_ADD                5
#define DIS_WEEK_4_DAT                0X08
#define DIS_INTERVAL_3_ADD            5
#define DIS_INTERVAL_3_DAT            0X10
#define DIS_DIG2_G_ADD                5
#define DIS_DIG2_G_DAT                0X40
#define DIS_DIG3_F_ADD                5
#define DIS_DIG3_F_DAT                0X80

#define DIS_DIG3_G_ADD                6
#define DIS_DIG3_G_DAT                0X01
#define DIS_FLOOR_OUT_ADD             6
#define DIS_FLOOR_OUT_DAT             0X02


#define DIS_DIG4_D_ADD                7
#define DIS_DIG4_D_DAT                0X02
#define DIS_HIGH_SET_ADD              7
#define DIS_HIGH_SET_DAT              0X04
#define DIS_DIG5_D_ADD                7
#define DIS_DIG5_D_DAT                0X08
#define DIS_DP2_ADD                   7
#define DIS_DP2_DAT                   0X10
#define DIS_DIG6_D_ADD                7
#define DIS_DIG6_D_DAT                0X20
#define DIS_COL_ADD                   7
#define DIS_COL_DAT                   0X40
#define DIS_DIG7_D_ADD                7
#define DIS_DIG7_D_DAT                0X80


#define DIS_ADD_DEC_ADD               8
#define DIS_ADD_DEC_DAT               0X01
#define DIS_5S_ADD                    8
#define DIS_5S_DAT                    0X02
#define DIS_BLUE_TOOTH_ADD            8
#define DIS_BLUE_TOOTH_DAT            0X04
#define DIS_WEEK_7_ADD                8
#define DIS_WEEK_7_DAT                0X08
#define DIS_WIFI_ADD                  8
#define DIS_WIFI_DAT                  0X80

#define DIS_LTP_ADD                   9
#define DIS_LTP_DAT                   0X01
#define DIS_DIG2_B_ADD                9
#define DIS_DIG2_B_DAT                0X04
#define DIS_DIG3_A_ADD                9
#define DIS_DIG3_A_DAT                0X08
#define DIS_DIG3_B_ADD                9
#define DIS_DIG3_B_DAT                0X10
#define DIS_FLOOR_VALID_ADD           9
#define DIS_FLOOR_VALID_DAT           0X20


#define DIS_DIG4_A_ADD                10
#define DIS_DIG4_A_DAT                0X20
#define DIS_DIG4_B_ADD                10
#define DIS_DIG4_B_DAT                0X40
#define DIS_DIG5_A_ADD                10
#define DIS_DIG5_A_DAT                0X80


#define DIS_DIG5_B_ADD                11
#define DIS_DIG5_B_DAT                0X01
#define DIS_DIG6_A_ADD                11
#define DIS_DIG6_A_DAT                0X02
#define DIS_DIG6_B_ADD                11
#define DIS_DIG6_B_DAT                0X04
#define DIS_DIG7_A_ADD                11
#define DIS_DIG7_A_DAT                0X08
#define DIS_DIG7_B_ADD                11
#define DIS_DIG7_B_DAT                0X10
#define DIS_SET_C_ADD                 11
#define DIS_SET_C_DAT                 0X20
#define DIS_AI_ADD                    11
#define DIS_AI_DAT                    0X40
#define DIS_MAN_ADD                   11
#define DIS_MAN_DAT                   0X80
#define DIS_TEMPORARY_ADD             11
#define DIS_TEMPORARY_DAT             0XC0

#define DIS_DP1_ADD                   12
#define DIS_DP1_DAT                   0X40
#define DIS_DIG3_D_ADD                12
#define DIS_DIG3_D_DAT                0X80

#define DIS_MEAS_C_ADD                13
#define DIS_MEAS_C_DAT                0X01
#define DIS_PREHEAT_ADD               13
#define DIS_PREHEAT_DAT               0X02

#define DIS_INTERVAL_1_ADD            15
#define DIS_INTERVAL_1_DAT            0X01
#define DIS_DIG1_E_ADD                15
#define DIS_DIG1_E_DAT                0X02
#define DIS_DIG1_C_ADD                15
#define DIS_DIG1_C_DAT                0X04
#define DIS_DIG2_E_ADD                15
#define DIS_DIG2_E_DAT                0X08


#define DIS_INTERVAL_4_ADD            17
#define DIS_INTERVAL_4_DAT            0X01
#define DIS_DIG1_F_ADD                17
#define DIS_DIG1_F_DAT                0X02
#define DIS_DIG1_G_ADD                17
#define DIS_DIG1_G_DAT                0X04
#define DIS_DIG2_F_ADD                17
#define DIS_DIG2_F_DAT                0X08

#define DIS_WINDOW_ADD                19
#define DIS_WINDOW_DAT                0X01
#define DIS_DIG1_A_ADD                19
#define DIS_DIG1_A_DAT                0X02
#define DIS_DIG1_B_ADD                19
#define DIS_DIG1_B_DAT                0X04
#define DIS_DIG2_A_ADD                19
#define DIS_DIG2_A_DAT                0X08

#define DIS_HOUSE_ADD                 21
#define DIS_HOUSE_DAT                 0X01
#define DIS_DIG1_D_ADD                21
#define DIS_DIG1_D_DAT                0X02
#define DIS_MEAS_ADD                  21
#define DIS_MEAS_DAT                  0X04
#define DIS_DIG2_D_ADD                21
#define DIS_DIG2_D_DAT                0X08



#endif




//-----------------------BSP_Keyboard.h--END------------------------------------
