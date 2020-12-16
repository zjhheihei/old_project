//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_LCD_H
#define _MOD_LCD_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\Depend\BSP_LCD.h"
//------------------------------E N D-------------------------------------------
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
#define CHAR_L  0X38
#define CHAR_N  0X37
#define CHAR_R  0X50
#define CHAR_S  0X6D
#define CHAR_T  0X31
#define CHAR_O  0X3F
#define CHAR_P  0X73
#define CHAR_U  0X3E
#define CHAR_K  0X07
#define CHAR_o  0X5C
#define CHAR_t  0x78
#define CHAR_Y  0X72
#define CHAR_NEGATIVE  0x40  //符号
#define CHAR_UNIT_C  0X61    //温度的小C


#define TableBlack         10
#define TableNegative      11
#define TableUnitC         12
#define TableCharL         13
#define TableCharN         14
#define TableCharA         15

#define Dis_Dig1                 LCDBuff[6]
#define Dis_Dig1_G_Add           LCDBuff[6]
#define Dis_Dig1_G_Dat           0x40
#define Dis_Dig1_F_Add           LCDBuff[6]
#define Dis_Dig1_F_Dat           0x20
#define Dis_Dig1_E_Add           LCDBuff[6]
#define Dis_Dig1_E_Dat           0x10
#define Dis_Dig1_D_Add           LCDBuff[6]
#define Dis_Dig1_D_Dat           0x08
#define Dis_Dig1_C_Add           LCDBuff[6]
#define Dis_Dig1_C_Dat           0x04
#define Dis_Dig1_B_Add           LCDBuff[6]
#define Dis_Dig1_B_Dat           0x02
#define Dis_Dig1_A_Add           LCDBuff[6]
#define Dis_Dig1_A_Dat           0x01

#define Dis_Dig2                 LCDBuff[7]
#define Dis_Dig2_G_Add           LCDBuff[7]
#define Dis_Dig2_G_Dat           0x40
#define Dis_Dig2_F_Add           LCDBuff[7]
#define Dis_Dig2_F_Dat           0x20
#define Dis_Dig2_E_Add           LCDBuff[7]
#define Dis_Dig2_E_Dat           0x10
#define Dis_Dig2_D_Add           LCDBuff[7]
#define Dis_Dig2_D_Dat           0x08
#define Dis_Dig2_C_Add           LCDBuff[7]
#define Dis_Dig2_C_Dat           0x04
#define Dis_Dig2_B_Add           LCDBuff[7]
#define Dis_Dig2_B_Dat           0x02
#define Dis_Dig2_A_Add           LCDBuff[7]
#define Dis_Dig2_A_Dat           0x01

#define Dis_Dig3                 LCDBuff[8]
#define Dis_Dig3_G_Add           LCDBuff[8]
#define Dis_Dig3_G_Dat           0x40
#define Dis_Dig3_F_Add           LCDBuff[8]
#define Dis_Dig3_F_Dat           0x20
#define Dis_Dig3_E_Add           LCDBuff[8]
#define Dis_Dig3_E_Dat           0x10
#define Dis_Dig3_D_Add           LCDBuff[8]
#define Dis_Dig3_D_Dat           0x08
#define Dis_Dig3_C_Add           LCDBuff[8]
#define Dis_Dig3_C_Dat           0x04
#define Dis_Dig3_B_Add           LCDBuff[8]
#define Dis_Dig3_B_Dat           0x02
#define Dis_Dig3_A_Add           LCDBuff[8]
#define Dis_Dig3_A_Dat           0x01

#define Dis_Dig4                 LCDBuff[9]
#define Dis_Dig4_G_Add           LCDBuff[9]
#define Dis_Dig4_G_Dat           0x40
#define Dis_Dig4_F_Add           LCDBuff[9]
#define Dis_Dig4_F_Dat           0x20
#define Dis_Dig4_E_Add           LCDBuff[9]
#define Dis_Dig4_E_Dat           0x10
#define Dis_Dig4_D_Add           LCDBuff[9]
#define Dis_Dig4_D_Dat           0x08
#define Dis_Dig4_C_Add           LCDBuff[9]
#define Dis_Dig4_C_Dat           0x04
#define Dis_Dig4_B_Add           LCDBuff[9]
#define Dis_Dig4_B_Dat           0x02
#define Dis_Dig4_A_Add           LCDBuff[9]
#define Dis_Dig4_A_Dat           0x01

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//************************天编程段宏定义*****************************************
#define Dis_0_7_Clock_Add        LCDBuff[0]
#define Dis_0Clock_Add           LCDBuff[0]
#define Dis_0Clock_Dat           0x01
#define Dis_1Clock_Add           LCDBuff[0]
#define Dis_1Clock_Dat           0x02
#define Dis_2Clock_Add           LCDBuff[0]
#define Dis_2Clock_Dat           0x04
#define Dis_3Clock_Add           LCDBuff[0]
#define Dis_3Clock_Dat           0x08
#define Dis_4Clock_Add           LCDBuff[0]
#define Dis_4Clock_Dat           0x10
#define Dis_5Clock_Add           LCDBuff[0]
#define Dis_5Clock_Dat           0x20
#define Dis_6Clock_Add           LCDBuff[0]
#define Dis_6Clock_Dat           0x40
#define Dis_7Clock_Add           LCDBuff[0]
#define Dis_7Clock_Dat           0x80

#define Dis_8_15_Clock_Add       LCDBuff[1]
#define Dis_8Clock_Add           LCDBuff[1]
#define Dis_8Clock_Dat           0x01
#define Dis_9Clock_Add           LCDBuff[1]
#define Dis_9Clock_Dat           0x02
#define Dis_10Clock_Add          LCDBuff[1]
#define Dis_10Clock_Dat          0x04
#define Dis_11Clock_Add          LCDBuff[1]
#define Dis_11Clock_Dat          0x08
#define Dis_12Clock_Add          LCDBuff[1]
#define Dis_12Clock_Dat          0x10
#define Dis_13Clock_Add          LCDBuff[1]
#define Dis_13Clock_Dat          0x20
#define Dis_14Clock_Add          LCDBuff[1]
#define Dis_14Clock_Dat          0x40
#define Dis_15Clock_Add          LCDBuff[1]
#define Dis_15Clock_Dat          0x80

#define Dis_16_23_Clock_Add      LCDBuff[2]
#define Dis_16Clock_Add          LCDBuff[2]
#define Dis_16Clock_Dat          0x01
#define Dis_17Clock_Add          LCDBuff[2]
#define Dis_17Clock_Dat          0x02
#define Dis_18Clock_Add          LCDBuff[2]
#define Dis_18Clock_Dat          0x04
#define Dis_19Clock_Add          LCDBuff[2]
#define Dis_19Clock_Dat          0x08
#define Dis_20Clock_Add          LCDBuff[2]
#define Dis_20Clock_Dat          0x10
#define Dis_21Clock_Add          LCDBuff[2]
#define Dis_21Clock_Dat          0x20
#define Dis_22Clock_Add          LCDBuff[2]
#define Dis_22Clock_Dat          0x40
#define Dis_23Clock_Add          LCDBuff[2]
#define Dis_23Clock_Dat          0x80

#define Dis_CharGroup1_Add       LCDBuff[3]
#define Dis_24Clock_Add          LCDBuff[3]
#define Dis_24Clock_Dat          0x01
#define Dis_Hour_Add             LCDBuff[3]
#define Dis_Hour_Dat             0x02
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//****************************字符宏定义****************************************
#define Dis_Point_Add             LCDBuff[3]   //小数点
#define Dis_Point_Dat             0x04
#define Dis_Col_Add               LCDBuff[3]   //秒符号
#define Dis_Col_Dat               0x08
#define Dis_TempSymbol_P_Add      LCDBuff[3]   //温度符号
#define Dis_TempSymbol_P_Dat      0x10

#define Dis_CharGroup2_Add        LCDBuff[4] 

#define Dis_AutoWork_Add          LCDBuff[4]   //自动  
#define Dis_AutoWork_Dat          0x02
#define Dis_HandWork_Add          LCDBuff[4]   //手动 
#define Dis_HandWork_Dat          0x01

#define Dis_Clean_Add             LCDBuff[4]   //清洁
#define Dis_Clean_Dat             0x04
#define Dis_SumClock_Add          LCDBuff[4]   //时钟
#define Dis_SumClock_Dat          0x08
#define Dis_Outside_Add           LCDBuff[4]   //户外
#define Dis_Outside_Dat           0x10
#define Dis_RoomTemp_Add          LCDBuff[4]   //室温
#define Dis_RoomTemp_Dat          0x20       
#define Dis_Communication_Add     LCDBuff[4]   //通讯
#define Dis_Communication_Dat     0x40
#define Dis_RH_Add                LCDBuff[4]   //湿度
#define Dis_RH_Dat                0x80

#define Dis_CharGroup3_Add       LCDBuff[5]
#define Dis_FanLow_Add           LCDBuff[5]    //低风
#define Dis_FanLow_Dat           0x01
#define Dis_FanMid_Add           LCDBuff[5]    //中风
#define Dis_FanMid_Dat           0x02
#define Dis_FanHigh_Add          LCDBuff[5]    //高风
#define Dis_FanHigh_Dat          0x04
#define Dis_FanSpeed_A_Add       LCDBuff[5]    //扇叶A
#define Dis_FanSpeed_A_Dat       0x08
#define Dis_FanSpeed_B_Add       LCDBuff[5]    //扇叶B
#define Dis_FanSpeed_B_Dat       0x10
#define Dis_FanSpeed_C_Add       LCDBuff[5]    //扇叶C
#define Dis_FanSpeed_C_Dat       0x20
#define Dis_FanRoom_Add          LCDBuff[5]    //风机小房子
#define Dis_FanRoom_Dat          0x40


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*****************************全局变量*****************************************
extern  INT8U LCDBuff[10]; 
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//名称: 初始化
//功能: 初始化液晶模块
//入口: 无
//出口: 无 
#define ITF_LCD_Configure        BSP_LCD_Configure
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    FanLeaf   = 0x00,
    AnyOther  = 0X01,
}SelectType_Def;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称：设置液晶模块
//功能：全显示或全灭
//入口： 
//状态：全显或全灭   _In_Status <-------------------------------------传入
//类型：扇叶或其他   _In_Type   <-------------------------------------传入

//出口:有按键状态发生改变返回BN_TURE,其他情况返回BN_FALSE  
void Mod_LCD_SetAll(BOOLEAN _In_Status,SelectType_Def _In_Type);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Task   
//名称: 液晶数据刷新
//功能: 
//入口
//类型: 扇叶或其他    _In_Type  <-------------------------------------传入  
//出口: 无
void Mod_LCD_RefreshRam(SelectType_Def _In_Type);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    DisChar   = 0x00,
    DisDigtal = 0X01,
}SelectDigtalType_Def;
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Funciton
//名称: 升级4位数字数据
//功能: 
//入口: 
//刷新的数据             *_In_Buff  <-------------------------------------传入
//需要刷新的类型          _In_Type   <-------------------------------------传入
//出口:有按键状态发生改变返回BN_TURE,其他情况返回BN_FALSE  
void Mod_LCD_UpDateDigtal(INT8U *_In_Buff,SelectDigtalType_Def _In_Type);
//------------------------------E N D-------------------------------------------





#endif



//-----------------------Mod_LCD.h--END------------------------------------
