/*******************************************************************************
*
*	模块名称 : Mod继电器模块
*	文件名称 : Mod_RelayPWM.c
*	版    本 : V1.0
*	说    明 : 3路继电器模块逻辑关系固定，1开2关或全关。为风机的低中高
*                  关闭继电器无延时，开继电器延时2秒
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-027  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*
*******************************************************************************/


#include ".\depend\bsp_relay.h"
#include ".\mod_relay.h"



typedef struct
{
    void(*relay_on)(void);
    void(*relay_off)(void);
}relay_drive_t;

typedef enum{
    RELAY_MODE_OFF  = (INT8U)0,
    RELAY_MODE_ON  ,
}relay_mode_t;

typedef enum
{
    RELAY_INIT = 0, 
    RELAY_RUN,
}relay_status_t;




typedef struct
{    
    void(*relay_on)(void);
    void(*relay_off)(void);
    relay_mode_t        mode;
    BOOLEAN         onEnableFlag;  
    INT16U        onDelay;
    INT16U        offDelay;
    INT16U        onEnableDelay;    
 }relay_t;

static relay_t floorRelay = 
{
    .relay_on = bsp_relay1_on,
    .relay_off = bsp_relay1_off,
    .mode = RELAY_MODE_OFF,
    .onEnableFlag = BN_TRUE,
    .onDelay = 0,
    .offDelay = 0,
    .onEnableDelay = 0,
    
};

static relay_t assistRelay = 
{
    .relay_on = bsp_relay2_on,
    .relay_off = bsp_relay2_off,
    .mode = RELAY_MODE_OFF,
    .onEnableFlag = BN_TRUE,
    .onDelay = 0,
    .offDelay = 0,
    .onEnableDelay = 0,
};

//static relay_t airRelay = {
//    .relay_on = BSP_Relay5_Enable,
//    .relay_off = BSP_Relay5_Disable,
//    .mode = RELAY_MODE_OFF,
//    .onEnableFlag = true,
//    .onDelay = NULL,
//    .offDelay = NULL,
//    .onEnableDelay = NULL,
//    
//};
//
//static relay_t fanHighRelay = {
//    .relay_on = BSP_Relay2_Enable,
//    .relay_off = BSP_Relay2_Disable,
//    .mode = RELAY_MODE_OFF,
//    .onEnableFlag = true,
//    .onDelay = NULL,
//    .offDelay = NULL,
//    .onEnableDelay = NULL,
//};
//
//static relay_t fanMidRelay = {
//    .relay_on = BSP_Relay3_Enable,
//    .relay_off = BSP_Relay3_Disable,
//    .mode = RELAY_MODE_OFF,
//    .onEnableFlag = true,
//    .onDelay = NULL,
//    .offDelay = NULL,
//    .onEnableDelay = NULL,
//    
//};
//
//static relay_t fanLowRelay = {
//    .relay_on = BSP_Relay4_Enable,
//    .relay_off = BSP_Relay4_Disable,
//    .mode = RELAY_MODE_OFF,
//    .onEnableFlag = true,
//    .onDelay = NULL,
//    .offDelay = NULL,
//    .onEnableDelay = NULL,
//};


void _relay_on(relay_t *me,INT16U _100milliscond)
{
    if(RELAY_MODE_ON == me->mode)
    {
        return;
    }
    me->mode = RELAY_MODE_ON; 
    me->onDelay = _100milliscond;
}

void _relay_off(relay_t*me,INT16U _100milliscond)
{
    if(RELAY_MODE_OFF == me->mode){
        return;
    }
    else
    {
        me->onEnableDelay = RELAY_RECLOSE_TIME;
        me->onEnableFlag = BN_FALSE;
    }
    me->mode = RELAY_MODE_OFF; 
    me->offDelay = _100milliscond; 
}


void _relay_run_handle(relay_t*me)
{  
    if(RELAY_MODE_ON == me->mode)
    {
        if(me->onDelay > 0)
        {
            me->onDelay--;
        }
        else
        {
            if(me->onEnableFlag)
            {
                me->relay_on();
            }
        }
    }
    else if(RELAY_MODE_OFF == me->mode)
    {
        if(me->offDelay > 0)
        {
            me->offDelay--;
        }
        else
        {
            me->relay_off();
        }
    }
    if(me->onEnableDelay > 0)
    {
        me->onEnableDelay--; 
    }
    else
    {
        me->onEnableFlag = BN_TRUE;
    }  
}

void mod_relay_init(void)
{
    bsp_relay_configure( );
}


void mod_relay_on(relay_number_t emNumber,INT16U _100milliscond)
{
    switch(emNumber)
    {
        case RELAY_FLOOR:
        {
            _relay_on(&floorRelay,_100milliscond);
            break;
        }
        case RELAY_ASSIST:
        {
            _relay_on(&assistRelay,_100milliscond);
            break;
        }  
        default:break;
    }
}

void mod_relay_off(relay_number_t emNumber,INT16U _100milliscond)
{
    switch(emNumber)
    {
        case RELAY_FLOOR:
        {
            _relay_off(&floorRelay,_100milliscond);
            break;
        }
        case RELAY_ASSIST:
        {
            _relay_off(&assistRelay,_100milliscond);
            break;
        } 
        default:break;
    }
}

void mod_relay_scanTask(INT16U milliscond)
{
    static relay_status_t emStatus = RELAY_INIT;
    static INT16U lastRelayTick = 0;
    INT16U timePassed=0;
		
    switch(emStatus)
    {
        case RELAY_INIT:
        {
            bsp_relay_configure();
            emStatus = RELAY_RUN;
            break;
        }
        case RELAY_RUN:
        {
            timePassed = (milliscond - lastRelayTick);
                       
            if(timePassed >= 100)
            {
                lastRelayTick = milliscond;
                _relay_run_handle(&floorRelay);			
                _relay_run_handle(&assistRelay);               
            }
            break;
        }
        default:break;
    }
}









