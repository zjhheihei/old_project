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

#define MAX_DIG_RAM  6

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
    DIS_TEMP_ADJ = 0x00,      //校温           0
    DIS_POW_OFF,              //关机           1
    DIS_MAX_SET_TEMP,        //最大设定温度    2
    DIS_TIME_CONROTL,         //时控           3
    DIS_MODE_AI,              //AI模式         4
    
    DIS_BLUE_TOOTH,           //蓝牙           5
    DIS_OUTPUT_WORK,         //时控开          6
    DIS_INSIDE_SONSOR,        //内控           7
    DIS_MODE_MOON,            //节能月亮       8
    DIS_SET_TEMP_C,           //设定温度符号℃ 9
    
    DIS_POW_ON,               //开机           10
    DIS_LEFT_BOX,             //左边的框       11
    DIS_OUTPUT_HEAT_CN,       //加热           12
    DIS_LTP_CN,               //防冻           13
    DIS_WIFI,                 //WIFI           14
    
    DIS_OUTPUT_REST,          //时控关         15
    DIS_OUTSIDE_SONSOR,       //外控           16
    DIS_SENSOR,               //传感器         24
    DIS_MOD_SUN,              //舒适太阳       17
    DIS_LOCK,                 //童锁           18
    
    DIS_FAC_SET,              //出厂           19    
    DIS_DP2,                  //第二个小数点   20
    DIS_SET_TEMP_CN,          //设定符号       21
    DIS_TEMP_KEEP,            //恒温           22
    DIS_OUTPUT_HEAT_CHAR,     //输出符号       23
    
    
    DIS_LIF,                  //外限           25
    DIS_RIGHT_BOX,            //右框           26
    DIS_DIF,                  //控制带宽       27
    DIS_HOURS_LATER,          //小时后         28
    DIS_MIN_SET_TEMP,         //最小设定       29
    
    DIS_MEAS_TEMP_CHAR,       //测量符号       30
    DIS_MEAS_TEMP_C,          //测量单位℃      31
    DIS_MINUTE,               //分钟           32
    DIS_POW_ON_STATUS,        //上电状态       33
    DIS_DP1,                  //小数1          34
    
    DIS_HIGH_SET,             //高级设置       35  
    DIS_FIVE_SECOND,          //5s             36
    DIS_ADD_AND_DEC,          // +  -          37
    DIS_ERR,                  //出错           38
    DIS_TEMP_CN,              //温 —— 中文     39
    DIS_OUTPUT_TYPE,          //输出方式       40
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
