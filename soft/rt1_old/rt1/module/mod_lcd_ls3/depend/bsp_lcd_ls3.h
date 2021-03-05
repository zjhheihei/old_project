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

#if defined(__STM8L052R8__)
#define MAX_LCD_BUF_CONUT             22
#define DIS_TEMP_ADJ_ADD              0
#define DIS_TEMP_ADJ_DAT              0X02
#define DIS_POW_OFF_MARK_ADD          0
#define DIS_POW_OFF_MARK_DAT          0X04
#define DIS_DIG6_G_ADD                0
#define DIS_DIG6_G_DAT                0X08
#define DIS_DIG6_E_ADD                0
#define DIS_DIG6_E_DAT                0X10
#define DIS_DIG5_G_ADD                0
#define DIS_DIG5_G_DAT                0X20
#define DIS_DIG5_E_ADD                0
#define DIS_DIG5_E_DAT                0X40
#define DIS_DIG4_G_ADD                0
#define DIS_DIG4_G_DAT                0X80

#define DIS_DIG4_E_ADD                1
#define DIS_DIG4_E_DAT                0X01
#define DIS_MAX_SET_TEMP_MARK_ADD     1
#define DIS_MAX_SET_TEMP_MARK_DAT     0X02
#define DIS_TIME_CONROTL_ADD          1
#define DIS_TIME_CONROTL_DAT          0X04
#define DIS_MODE_AI_ADD               1
#define DIS_MODE_AI_DAT               0X08



#define DIS_BLUE_TOOTH_ADD            1
#define DIS_BLUE_TOOTH_DAT            0X80

#define DIS_DIG3_B_ADD                2
#define DIS_DIG3_B_DAT                0X01
#define DIS_DIG1_F_ADD                2
#define DIS_DIG1_F_DAT                0X04

#define DIS_OUTPUT_ENABLE_MARK_ADD    2
#define DIS_OUTPUT_ENABLE_MARK_DAT    0X08
#define DIS_INSIDE_SONSOR_MARK_ADD    2
#define DIS_INSIDE_SONSOR_MARK_DAT    0X10
#define DIS_MODE_SAVE_ADD             2
#define DIS_MODE_SAVE_DAT             0X20

#define DIS_SET_TEMP_CHAR_ADD         3
#define DIS_SET_TEMP_CHAR_DAT         0X20
#define DIS_POW_ON_ADD                3
#define DIS_POW_ON_DAT                0X40
#define DIS_DIG6_B_ADD                3
#define DIS_DIG6_B_DAT                0X80


#define DIS_DIG6_F_ADD                4
#define DIS_DIG6_F_DAT                0X01
#define DIS_DIG5_B_ADD                4
#define DIS_DIG5_B_DAT                0X02
#define DIS_DIG5_F_ADD                4
#define DIS_DIG5_F_DAT                0X04
#define DIS_DIG4_B_ADD                4
#define DIS_DIG4_B_DAT                0X08
#define DIS_DIG4_F_ADD                4
#define DIS_DIG4_F_DAT                0X10
#define DIS_LEFT_BOX_MARK_ADD         4
#define DIS_LEFT_BOX_MARK_DAT         0X20
#define DIS_OUTPUT_HEAT_ADD           4
#define DIS_OUTPUT_HEAT_DAT           0X40
#define DIS_LTP_ADD                   4
#define DIS_LTP_DAT                   0X80

#define DIS_WIFI_ADD                  5
#define DIS_WIFI_DAT                  0X08
#define DIS_DIG3_G_ADD                5
#define DIS_DIG3_G_DAT                0X10
#define DIS_DIG1_E_ADD                5
#define DIS_DIG1_E_DAT                0X40
#define DIS_OUTPUT_STOP_MARK_ADD      5
#define DIS_OUTPUT_STOP_MARK_DAT      0X80

#define DIS_OUTSIDE_SONSOR_MARK_ADD   6
#define DIS_OUTSIDE_SONSOR_MARK_DAT   0x01
#define DIS_MOD_COMFORTABLE_ADD       6
#define DIS_MOD_COMFORTABLE_DAT       0x02


#define DIS_LOCK_ADD                  7
#define DIS_LOCK_DAT                  0x02
#define DIS_FAC_SET_ADD               7
#define DIS_FAC_SET_DAT               0x08
#define DIS_DIG6_A_ADD                7
#define DIS_DIG6_A_DAT                0X10
#define DIS_DP2_ADD                   7
#define DIS_DP2_DAT                   0X20
#define DIS_DIG5_A_ADD                7
#define DIS_DIG5_A_DAT                0X40
#define DIS_SET_TEMP_MARK_ADD         7
#define DIS_SET_TEMP_MARK_DAT         0X80


#define DIS_DIG4_A_ADD                8
#define DIS_DIG4_A_DAT                0X01
#define DIS_TEMP_KEEP_ADD             8
#define DIS_TEMP_KEEP_DAT             0X04
#define DIS_IN_TO_HIGH_SET_ADD        8
#define DIS_IN_TO_HIGH_SET_DAT        0X08
#define DIS_OUTPUT_MARK_ADD           8
#define DIS_OUTPUT_MARK_DAT           0X80
#define DIS_TEMP_KEEP_ADD             8
#define DIS_TEMP_KEEP_DAT             0X04

#define DIS_DIG3_C_ADD                9
#define DIS_DIG3_C_DAT                0X01
#define DIS_DIG1_D_ADD                9
#define DIS_DIG1_D_DAT                0X04
#define DIS_SENSOR_MARK_ADD           9
#define DIS_SENSOR_MARK_DAT           0X08
#define DIS_ALL_SENSOR_ADD            9
#define DIS_ALL_SENSOR_DAT            0X10
#define DIS_RIGHT_BOX_ADD             9
#define DIS_RIGHT_BOX_DAT             0X20


#define DIS_CONTORL_DIF_ADD           10
#define DIS_CONTORL_DIF_DAT           0X20
#define DIS_HOURS_LATER_ADD           10
#define DIS_HOURS_LATER_DAT           0X40
#define DIS_DIG6_C_ADD                10
#define DIS_DIG6_C_DAT                0X80

#define DIS_DIG6_D_ADD                11
#define DIS_DIG6_D_DAT                0X01
#define DIS_DIG5_C_ADD                11
#define DIS_DIG5_C_DAT                0X02
#define DIS_DIG5_D_ADD                11
#define DIS_DIG5_D_DAT                0X04
#define DIS_DIG4_C_ADD                11
#define DIS_DIG4_C_DAT                0X08
#define DIS_DIG4_D_ADD                11
#define DIS_DIG4_D_DAT                0X10
#define DIS_MIN_SET_TEMP_MARK_ADD     11
#define DIS_MIN_SET_TEMP_MARK_DAT     0X20
#define DIS_FLOOR_WARM_MARK_ADD       11
#define DIS_FLOOR_WARM_MARK_DAT       0X40
#define DIS_MEAS_TEMP_MARK_ADD        11
#define DIS_MEAS_TEMP_MARK_DAT        0X80


#define DIS_MEAS_TEMP_CHAR_ADD        12
#define DIS_MEAS_TEMP_CHAR_DAT        0X10
#define DIS_DIG1_A_ADD                12
#define DIS_DIG1_A_DAT                0X40
#define DIS_MINUTE_ADD                12
#define DIS_MINUTE_DAT                0X80

#define DIS_POW_ON_STATUS_ADD         13
#define DIS_POW_ON_STATUS_DAT         0X01
#define DIS_MODE_MARK_ADD             13
#define DIS_MODE_MARK_DAT             0X02

#define DIS_DIG3_F_ADD                15
#define DIS_DIG3_F_DAT                0X01
#define DIS_DIG2_B_ADD                15
#define DIS_DIG2_B_DAT                0X02
#define DIS_DIG2_F_ADD                15
#define DIS_DIG2_F_DAT                0X04
#define DIS_DIG1_B_ADD                15
#define DIS_DIG1_B_DAT                0X08


#define DIS_DIG3_E_ADD                17
#define DIS_DIG3_E_DAT                0X01
#define DIS_DIG2_G_ADD                17
#define DIS_DIG2_G_DAT                0X02
#define DIS_DIG2_E_ADD                17
#define DIS_DIG2_E_DAT                0X04
#define DIS_DIG1_G_ADD                17
#define DIS_DIG1_G_DAT                0X08


#define DIS_DIG3_D_ADD                19
#define DIS_DIG3_D_DAT                0X01
#define DIS_DIG2_C_ADD                19
#define DIS_DIG2_C_DAT                0X02
#define DIS_DIG2_D_ADD                19
#define DIS_DIG2_D_DAT                0X04
#define DIS_DIG1_C_ADD                19
#define DIS_DIG1_C_DAT                0X08


#define DIS_DIG3_A_ADD                21
#define DIS_DIG3_A_DAT                0X01
#define DIS_DP1_ADD                   21
#define DIS_DP1_DAT                   0X02
#define DIS_DIG2_A_ADD                21
#define DIS_DIG2_A_DAT                0X04
#define DIS_HIGH_SET_MARK_ADD         21
#define DIS_HIGH_SET_MARK_DAT         0X08


#elif defined(__HDSC32L136__)
#define MAX_LCD_BUF_CONUT             16

#define DIS_DP2_ADD                   1
#define DIS_DP2_DAT                   0X01
#define DIS_DIG6_A_ADD                1
#define DIS_DIG6_A_DAT                0X02
#define DIS_FAC_SET_ADD               1
#define DIS_FAC_SET_DAT               0x04
#define DIS_LOCK_ADD                  1
#define DIS_LOCK_DAT                  0x10


#define DIS_DIG3_C_ADD                2
#define DIS_DIG3_C_DAT                0X01
#define DIS_OUTPUT_MARK_ADD           2
#define DIS_OUTPUT_MARK_DAT           0X02
#define DIS_IN_TO_HIGH_SET_ADD        2
#define DIS_IN_TO_HIGH_SET_DAT        0X04
#define DIS_TEMP_KEEP_ADD             2
#define DIS_TEMP_KEEP_DAT             0X08
#define DIS_DIG4_A_ADD                2
#define DIS_DIG4_A_DAT                0X20
#define DIS_SET_TEMP_MARK_ADD         2
#define DIS_SET_TEMP_MARK_DAT         0X40
#define DIS_DIG5_A_ADD                2
#define DIS_DIG5_A_DAT                0X80

#define DIS_RIGHT_BOX_ADD             3
#define DIS_RIGHT_BOX_DAT             0X01
#define DIS_ALL_SENSOR_ADD            3
#define DIS_ALL_SENSOR_DAT            0X02
#define DIS_SENSOR_MARK_ADD           3
#define DIS_SENSOR_MARK_DAT           0X04
#define DIS_DIG1_D_ADD                3
#define DIS_DIG1_D_DAT                0X08
#define DIS_DIG1_C_ADD                3
#define DIS_DIG1_C_DAT                0X10
#define DIS_DIG2_D_ADD                3
#define DIS_DIG2_D_DAT                0X20
#define DIS_DIG2_C_ADD                3
#define DIS_DIG2_C_DAT                0X40
#define DIS_DIG3_D_ADD                3
#define DIS_DIG3_D_DAT                0X80


#define DIS_DIG5_B_ADD                5
#define DIS_DIG5_B_DAT                0X01
#define DIS_DIG6_F_ADD                5
#define DIS_DIG6_F_DAT                0X02
#define DIS_DIG6_B_ADD                5
#define DIS_DIG6_B_DAT                0X04
#define DIS_POW_ON_ADD                5
#define DIS_POW_ON_DAT                0X08
#define DIS_SET_TEMP_CHAR_ADD         5
#define DIS_SET_TEMP_CHAR_DAT         0X10

#define DIS_DIG3_G_ADD                6
#define DIS_DIG3_G_DAT                0X01
#define DIS_WIFI_ADD                  6
#define DIS_WIFI_DAT                  0X02
#define DIS_LTP_ADD                   6
#define DIS_LTP_DAT                   0X04
#define DIS_OUTPUT_HEAT_ADD           6
#define DIS_OUTPUT_HEAT_DAT           0X08
#define DIS_LEFT_BOX_MARK_ADD         6
#define DIS_LEFT_BOX_MARK_DAT         0X10
#define DIS_DIG4_F_ADD                6
#define DIS_DIG4_F_DAT                0X20
#define DIS_DIG4_B_ADD                6
#define DIS_DIG4_B_DAT                0X40
#define DIS_DIG5_F_ADD                6
#define DIS_DIG5_F_DAT                0X80


#define DIS_MOD_COMFORTABLE_ADD       7
#define DIS_MOD_COMFORTABLE_DAT       0x01
#define DIS_OUTSIDE_SONSOR_MARK_ADD   7
#define DIS_OUTSIDE_SONSOR_MARK_DAT   0x02
#define DIS_OUTPUT_STOP_MARK_ADD      7
#define DIS_OUTPUT_STOP_MARK_DAT      0X04
#define DIS_DIG1_E_ADD                7
#define DIS_DIG1_E_DAT                0X08
#define DIS_DIG1_G_ADD                7
#define DIS_DIG1_G_DAT                0X10
#define DIS_DIG2_E_ADD                7
#define DIS_DIG2_E_DAT                0X20
#define DIS_DIG2_G_ADD                7
#define DIS_DIG2_G_DAT                0X40
#define DIS_DIG3_E_ADD                7
#define DIS_DIG3_E_DAT                0X80


#define DIS_DIG5_G_ADD                9
#define DIS_DIG5_G_DAT                0X01
#define DIS_DIG6_E_ADD                9
#define DIS_DIG6_E_DAT                0X02
#define DIS_DIG6_G_ADD                9
#define DIS_DIG6_G_DAT                0X04
#define DIS_POW_OFF_MARK_ADD          9
#define DIS_POW_OFF_MARK_DAT          0X08
#define DIS_TEMP_ADJ_ADD              9
#define DIS_TEMP_ADJ_DAT              0X10

#define DIS_DIG3_B_ADD                10
#define DIS_DIG3_B_DAT                0X01
#define DIS_BLUE_TOOTH_ADD            10
#define DIS_BLUE_TOOTH_DAT            0X02
#define DIS_MODE_AI_ADD               10
#define DIS_MODE_AI_DAT               0X04
#define DIS_TIME_CONROTL_ADD          10
#define DIS_TIME_CONROTL_DAT          0X08
#define DIS_MAX_SET_TEMP_MARK_ADD     10
#define DIS_MAX_SET_TEMP_MARK_DAT     0X10
#define DIS_DIG4_E_ADD                10
#define DIS_DIG4_E_DAT                0X20
#define DIS_DIG4_G_ADD                10
#define DIS_DIG4_G_DAT                0X40
#define DIS_DIG5_E_ADD                10
#define DIS_DIG5_E_DAT                0X80


#define DIS_MODE_SAVE_ADD             11
#define DIS_MODE_SAVE_DAT             0X01
#define DIS_INSIDE_SONSOR_MARK_ADD    11
#define DIS_INSIDE_SONSOR_MARK_DAT    0X02
#define DIS_OUTPUT_ENABLE_MARK_ADD    11
#define DIS_OUTPUT_ENABLE_MARK_DAT    0X04
#define DIS_DIG1_F_ADD                11
#define DIS_DIG1_F_DAT                0X08
#define DIS_DIG1_B_ADD                11
#define DIS_DIG1_B_DAT                0X10
#define DIS_DIG2_F_ADD                11
#define DIS_DIG2_F_DAT                0X20
#define DIS_DIG2_B_ADD                11
#define DIS_DIG2_B_DAT                0X40
#define DIS_DIG3_F_ADD                11
#define DIS_DIG3_F_DAT                0X80


#define DIS_DIG5_C_ADD                13
#define DIS_DIG5_C_DAT                0X01
#define DIS_DIG6_D_ADD                13
#define DIS_DIG6_D_DAT                0X02
#define DIS_DIG6_C_ADD                13
#define DIS_DIG6_C_DAT                0X04
#define DIS_HOURS_LATER_ADD           13
#define DIS_HOURS_LATER_DAT           0X08
#define DIS_CONTORL_DIF_ADD           13
#define DIS_CONTORL_DIF_DAT           0X10

#define DIS_MEAS_TEMP_CHAR_ADD        14
#define DIS_MEAS_TEMP_CHAR_DAT        0X01
#define DIS_MEAS_TEMP_MARK_ADD        14
#define DIS_MEAS_TEMP_MARK_DAT        0X04
#define DIS_FLOOR_WARM_MARK_ADD       14
#define DIS_FLOOR_WARM_MARK_DAT       0X08
#define DIS_MIN_SET_TEMP_MARK_ADD     14
#define DIS_MIN_SET_TEMP_MARK_DAT     0X10
#define DIS_DIG4_D_ADD                14
#define DIS_DIG4_D_DAT                0X20
#define DIS_DIG4_C_ADD                14
#define DIS_DIG4_C_DAT                0X40
#define DIS_DIG5_D_ADD                14
#define DIS_DIG5_D_DAT                0X80


#define DIS_MODE_MARK_ADD             15
#define DIS_MODE_MARK_DAT             0X01
#define DIS_POW_ON_STATUS_ADD         15
#define DIS_POW_ON_STATUS_DAT         0X02
#define DIS_MINUTE_ADD                15
#define DIS_MINUTE_DAT                0X04
#define DIS_DIG1_A_ADD                15
#define DIS_DIG1_A_DAT                0X08
#define DIS_HIGH_SET_MARK_ADD         15
#define DIS_HIGH_SET_MARK_DAT         0X10
#define DIS_DIG2_A_ADD                15
#define DIS_DIG2_A_DAT                0X20
#define DIS_DP1_ADD                   15
#define DIS_DP1_DAT                   0X40
#define DIS_DIG3_A_ADD                15
#define DIS_DIG3_A_DAT                0X80


























































#endif


#endif




//-----------------------BSP_Keyboard.h--END------------------------------------
