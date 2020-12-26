//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_LCD_LS3_H
#define _MOD_LCD_LS3_H
//------------------------------E N D-------------------------------------------

#include "DeviceSelect.h"

//#define __LCD_STATUS_DEBUG__
#ifdef __LCD_STATUS_DEBUG__
void mod_lcd_debug_add_for_key(void);
#endif


#if defined(__STM8L052R8__)
#define MAX_LCD_RAM  22 
#elif defined(__HDSC32L136__)
#define MAX_LCD_RAM  16
#endif


#define MAX_DIG_RAM  7

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
#define CHAR_NEGATIVE  0x40  //符号
#define CHAR_UNIT_C  0X61    //温度的小C
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************表格字符宏定义****************************************
#define TABLE_BLACK        10
#define TableNegative      11
#define TableUnitC         12
#define TableCharL         13
#define TableCharN         14
#define TableCharA         15    
//------------------------------E N D-------------------------------------------


typedef enum
{
    DIS_SET  = 0,
    DIS_WEEK_7,
    DIS_WEEK_1,
    DIS_WEEK_2,
    DIS_WEEK_3,
    
    DIS_WEEK_4,
    DIS_WEEK_5,
    DIS_WEEK_6,  
    DIS_INTERVAL_1,
    DIS_INTERVAL_2,
    
    DIS_INTERVAL_3,
    DIS_INTERVAL_4,
    DIS_FLOOR_WARM,
    DIS_LOCK,      
    DIS_FLOOR_OUT,
    
    DIS_HIGH_SET,
    DIS_DP2,
    DIS_COL,
    DIS_ADD_DEC,
    DIS_5S,
    
    DIS_BLUE_TOOTH,    
    DIS_WIFI,
    DIS_LTP,
    DIS_FLOOR_VALID,
    DIS_SET_C,
    
    DIS_AI,
    DIS_MAN,
    DIS_TEMPORARY,
    DIS_DP1,
    DIS_MEAS_C,
    
    DIS_PREHEAT,    
    DIS_WINDOW,
    DIS_HOUSE,
    DIS_MEAS,
}rt1_lcd_char_number_t;




void mod_lcd_char(rt1_lcd_char_number_t num,BOOLEAN newStatus);
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//鍚嶇О: mod_lcd_configure
//鍔熻兘: 鍒濆鍖朙CD妯″潡
//鍏ュ彛: LCD瀵勫瓨鍣ㄦ寚閽堣祴鍊?
//鍑哄彛: 鏃?
void mod_lcd_configure(void);
//void mod_lcd_configure(INT8U *lcdBuf ,INT8U *_out_lcdBufLength);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称:    Mod_LCD_SetAllDis
//功能:    娑叉櫠鍏ㄦ樉鍏ㄧ伃鎺у埗
//入口:    <-----------------浼犲叆_In_NewStatus,BN_TRUE鍏ㄦ樉 BN_FALSE鍏ㄧ伃
//出口:    鏃?
void mod_lcd_showAll(BOOLEAN newStatus);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++




//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称: mod_lcd_dig         
//功能: 数字显示缓冲区加载到LCD缓冲区
//入口: digBuf数字缓冲区指针   <<-----------------------------传入
//      _out_lcdBuf缓冲区指针  ----------------------------->>传出
//出口: 无
void mod_lcd_dig(INT8U *digBuf);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称: mod_lcd_refresh
//功能: 加载LCD缓冲区到寄存器刷新显示
//入口: 无
//出口: 无 
void mod_lcd_refresh(void);
//------------------------------E N D-------------------------------------------
void mod_lcd_show_all(BOOLEAN newStatus);

#endif


//-----------------------Mod_LCD.h--END------------------------------------
