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
#include ".\depend\bsp_lcd_ls3.h"
#include ".\mod_lcd_ls3.h"
//------------------------------E N D-------------------------------------------


#define __DIG1_ENABLE__
#define __DIG2_ENABLE__
#define __DIG3_ENABLE__
#define __DIG4_ENABLE__
#define __DIG5_ENABLE__
#define __DIG6_ENABLE__
#define __DIG7_ENABLE__


static INT8U lcdBuf[MAX_LCD_BUF_CONUT];

void mod_lcd_refresh(void)
{     
    bsp_lcd_refresh(lcdBuf,MAX_LCD_BUF_CONUT);
}

void mod_lcd_dig_addOrDat(INT8U add1,INT8U add2,INT8U add3,INT8U add4,\
  INT8U add5,INT8U add6,INT8U add7,INT8U *_out_addBuf)
{
    _out_addBuf[0] = add1;
    _out_addBuf[1] = add2;
    _out_addBuf[2] = add3;
    _out_addBuf[3] = add4;
    _out_addBuf[4] = add5;
    _out_addBuf[5] = add6;
    _out_addBuf[6] = add7;
}

void mod_lcd_dig_one(INT8U digBuf,INT8U *add,INT8U *dat)//,INT8U *_out_lcdBuf)
{
    INT8U i = 0;
    INT8U bit = 0x01;
    for(;i < 7;i++)
    {
        if(digBuf & (bit << i))
        {
            lcdBuf[add[i]] |= dat[i];
        }
    }
}

void mod_lcd_dig(INT8U *digBuf)
{  
    INT8U add[7];
    INT8U dat[7];
    #ifdef __DIG1_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG1_A_ADD,DIS_DIG1_B_ADD,DIS_DIG1_C_ADD,\
      DIS_DIG1_D_ADD,DIS_DIG1_E_ADD,DIS_DIG1_F_ADD,DIS_DIG1_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG1_A_DAT,DIS_DIG1_B_DAT,DIS_DIG1_C_DAT,\
      DIS_DIG1_D_DAT,DIS_DIG1_E_DAT,DIS_DIG1_F_DAT,DIS_DIG1_G_DAT,dat);
    mod_lcd_dig_one(digBuf[0],add,dat);//,_out_lcdBuf);
    #endif   
    
    
    #ifdef __DIG2_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG2_A_ADD,DIS_DIG2_B_ADD,DIS_DIG2_C_ADD,\
      DIS_DIG2_D_ADD,DIS_DIG2_E_ADD,DIS_DIG2_F_ADD,DIS_DIG2_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG2_A_DAT,DIS_DIG2_B_DAT,DIS_DIG2_C_DAT,\
      DIS_DIG2_D_DAT,DIS_DIG2_E_DAT,DIS_DIG2_F_DAT,DIS_DIG2_G_DAT,dat);
    mod_lcd_dig_one(digBuf[1],add,dat);//,_out_lcdBuf);
    #endif   
    
    
    #ifdef __DIG3_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG3_A_ADD,DIS_DIG3_B_ADD,DIS_DIG3_C_ADD,\
      DIS_DIG3_D_ADD,DIS_DIG3_E_ADD,DIS_DIG3_F_ADD,DIS_DIG3_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG3_A_DAT,DIS_DIG3_B_DAT,DIS_DIG3_C_DAT,\
      DIS_DIG3_D_DAT,DIS_DIG3_E_DAT,DIS_DIG3_F_DAT,DIS_DIG3_G_DAT,dat);
    mod_lcd_dig_one(digBuf[2],add,dat);//,_out_lcdBuf);
    #endif   
    
    
    #ifdef __DIG4_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG4_A_ADD,DIS_DIG4_B_ADD,DIS_DIG4_C_ADD,\
      DIS_DIG4_D_ADD,DIS_DIG4_E_ADD,DIS_DIG4_F_ADD,DIS_DIG4_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG4_A_DAT,DIS_DIG4_B_DAT,DIS_DIG4_C_DAT,\
      DIS_DIG4_D_DAT,DIS_DIG4_E_DAT,DIS_DIG4_F_DAT,DIS_DIG4_G_DAT,dat);
    mod_lcd_dig_one(digBuf[3],add,dat);//,_out_lcdBuf);
    #endif   
    
    
    #ifdef __DIG5_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG5_A_ADD,DIS_DIG5_B_ADD,DIS_DIG5_C_ADD,\
      DIS_DIG5_D_ADD,DIS_DIG5_E_ADD,DIS_DIG5_F_ADD,DIS_DIG5_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG5_A_DAT,DIS_DIG5_B_DAT,DIS_DIG5_C_DAT,\
      DIS_DIG5_D_DAT,DIS_DIG5_E_DAT,DIS_DIG5_F_DAT,DIS_DIG5_G_DAT,dat);
    mod_lcd_dig_one(digBuf[4],add,dat);//,_out_lcdBuf);
    #endif   
    
    
    #ifdef __DIG6_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG6_A_ADD,DIS_DIG6_B_ADD,DIS_DIG6_C_ADD,\
      DIS_DIG6_D_ADD,DIS_DIG6_E_ADD,DIS_DIG6_F_ADD,DIS_DIG6_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG6_A_DAT,DIS_DIG6_B_DAT,DIS_DIG6_C_DAT,\
      DIS_DIG6_D_DAT,DIS_DIG6_E_DAT,DIS_DIG6_F_DAT,DIS_DIG6_G_DAT,dat);
    mod_lcd_dig_one(digBuf[5],add,dat);//,_out_lcdBuf);
    #endif   
    
    
    #ifdef __DIG7_ENABLE__
    mod_lcd_dig_addOrDat(DIS_DIG7_A_ADD,DIS_DIG7_B_ADD,DIS_DIG7_C_ADD,\
      DIS_DIG7_D_ADD,DIS_DIG7_E_ADD,DIS_DIG7_F_ADD,DIS_DIG7_G_ADD,add);
    mod_lcd_dig_addOrDat(DIS_DIG7_A_DAT,DIS_DIG7_B_DAT,DIS_DIG7_C_DAT,\
      DIS_DIG7_D_DAT,DIS_DIG7_E_DAT,DIS_DIG7_F_DAT,DIS_DIG7_G_DAT,dat);
    mod_lcd_dig_one(digBuf[6],add,dat);//,_out_lcdBuf);
    #endif          
}

void mod_lcd_configure(void)
{
    bsp_lcd_configure();
    mod_lcd_show_all(BN_FALSE);
}
#define MAX_CHAR_NUN   35
const INT8U lcdCharAddr[MAX_CHAR_NUN][2] =
{   
    DIS_SET_ADD        , DIS_SET_DAT        , //1   
    DIS_WEEK_7_ADD     , DIS_WEEK_7_DAT     ,
    DIS_WEEK_1_ADD     , DIS_WEEK_1_DAT     ,
    DIS_WEEK_2_ADD     , DIS_WEEK_2_DAT     ,
    DIS_WEEK_3_ADD     , DIS_WEEK_3_DAT     , //5
    
    DIS_WEEK_4_ADD     , DIS_WEEK_4_DAT     ,
    DIS_WEEK_5_ADD     , DIS_WEEK_5_DAT     ,
    DIS_WEEK_6_ADD     , DIS_WEEK_6_DAT     ,   
    DIS_INTERVAL_1_ADD , DIS_INTERVAL_1_DAT ,
    DIS_INTERVAL_2_ADD , DIS_INTERVAL_2_DAT , // 10
    
    DIS_INTERVAL_3_ADD , DIS_INTERVAL_3_DAT ,
    DIS_INTERVAL_4_ADD , DIS_INTERVAL_4_DAT ,
    DIS_FLOOR_WARM_ADD , DIS_FLOOR_WARM_DAT ,
    DIS_LOCK_ADD       , DIS_LOCK_DAT       ,      
    DIS_FLOOR_OUT_ADD  , DIS_FLOOR_OUT_DAT  ,//15
    
    DIS_HIGH_SET_ADD   , DIS_HIGH_SET_DAT   ,
    DIS_DP2_ADD        , DIS_DP2_DAT        ,
    DIS_COL_ADD        , DIS_COL_DAT        ,
    DIS_ADD_DEC_ADD    , DIS_ADD_DEC_DAT    ,
    DIS_5S_ADD         , DIS_5S_DAT         ,//20
    
    DIS_BLUE_TOOTH_ADD , DIS_BLUE_TOOTH_DAT ,    
    DIS_WIFI_ADD       , DIS_WIFI_DAT       ,
    DIS_LTP_ADD        , DIS_LTP_DAT        ,
    DIS_FLOOR_VALID_ADD, DIS_FLOOR_VALID_DAT,
    DIS_SET_C_ADD      , DIS_SET_C_DAT      ,//25
    
    DIS_AI_ADD         , DIS_AI_DAT         ,
    DIS_MAN_ADD        , DIS_MAN_DAT        ,
    DIS_TEMPORARY_ADD  , DIS_TEMPORARY_DAT  ,
    DIS_DP1_ADD        , DIS_DP1_DAT        ,
    DIS_MEAS_C_ADD     , DIS_MEAS_C_DAT     ,
    
    DIS_PREHEAT_ADD    , DIS_PREHEAT_DAT    ,      
    DIS_WINDOW_ADD     , DIS_WINDOW_DAT     ,
    DIS_HOUSE_ADD      , DIS_HOUSE_DAT      ,
    DIS_MEAS_ADD       , DIS_MEAS_DAT       ,//34
};

void mod_lcd_char(rt1_lcd_char_number_t num,BOOLEAN newStatus)
{
    if((INT8U)num < MAX_CHAR_NUN)
    {
        if(newStatus)
        {
            lcdBuf[lcdCharAddr[(INT8U)num][0]] |= lcdCharAddr[(INT8U)num][1]; 
        }
        else
        {
            lcdBuf[lcdCharAddr[(INT8U)num][0]] &= ~lcdCharAddr[(INT8U)num][1]; 
        }   
    }
     
}

void mod_lcd_show_all(BOOLEAN newStatus)
{
    INT8U i = 0;
    INT8U dat = 0;
    for(i = 0; i < MAX_LCD_BUF_CONUT;i++)
    {
        if(newStatus)
        {
            dat = 0xff;
        }
        lcdBuf[i] = dat;
    }
}

#ifdef __LCD_STATUS_DEBUG__

static INT8U bufCnt = 12;
static INT8U movebit = 0;
volatile static INT8U moveStatus = 0;
void mod_lcd_debug_add_for_key(void)
{      
    if(movebit > 7)
    {
        movebit = 0;
        if(++bufCnt >= MAX_LCD_BUF_CONUT)
        {
           bufCnt = 0; 
        }
    }
    moveStatus = (0x01 << movebit);
    lcdBuf[bufCnt] |= (0x01 << movebit);
    movebit++;    
}
#endif
//------------------------------E N D-------------------------------------------



