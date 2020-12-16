/*******************************************************************************
*
*	模块名称 : 按键模块演示
*	文件名称 : ExMain.c
*	版    本 : V1.0
*	说    明 : 演示按键模块各个函数
                   的作用。用变量Date的值来检验按键模块。
                   Debug模式调试在，IAR的View菜单下打开Live Watch 添加Date,
                   操作按键Date 改变。
                   
                   菜单 01 +键02 -键04 电源键08 
                   模拟公司产品的高级模式，来检验按键的所有函数。
                   已經在E51.33B样机上测试通过
*                  
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-28  zhoujh  正式发布
*	Copyright (C), 2018-2022,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun)  
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\..\Mod_Keyboard\Src\Mod_Keyboard.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************按键名称宏定义***************************************
#define KeyMenu        KeyAloneBit1
#define KeyAdd         KeyAloneBit2
#define KeyPower       KeyAloneBit0
#define KeyDEC         KeyAloneBit3
#define KeyChildLock   KeyGroupBit0
#define KeySetTime     KeyGroupBit1
#define Delay1Second   1000
#define Delay3Second   3000
//------------------------------E N D-------------------------------------------

#define KeyMenuDown       0x01
#define KeyAddDown        0x02 
#define KeyDECDown        0x04
#define KeyPowerDown      0x08
#define KeyChildLockDown  0x10
#define KeySetTimeDown        0x20
#define ParameterMode     0x40
#define MaskMode          0x41



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************高级模式状态枚举**************************************
typedef enum
{
    GotoParameterModeDisable   = 0x00,  //
    GotoParameterModeEnable    = 0x01,  //使能进入高级模式
    GoingtoParameterMode       = 0x02,   //进入高级模式
} GotoParameterStatus_Def;

GotoParameterStatus_Def   GotoParameterStatus = GotoParameterModeDisable;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************值会随着按键变化*************************************
static INT8U Date = 0x01;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    KeyRetValue_Def CurrentKeyStatus;
   //++++++++++++++++++++++++++++++start++++++++++++++++++++++++++++++++++++++++
   //初始化芯片时钟，系统运行时钟和按键模块
    ITF_Clock_Configure();
    ITF_Systick_Configure();
   //------------------------------E N D----------------------------------------
    while(1)
    {
        //++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++
        //运行系统时钟，内部时钟校正和按键扫描函数
        SystickTimeRun(); 
        Mod_Key_ScanTask();
        HSICalibration_Task();
        //------------------------------E N D-----------------------------------
        
        
        //++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++
        //进入高级模式
        if(GoingtoParameterMode == GotoParameterStatus)    //模拟进入高级模式
        {
            Date = ParameterMode;                                   //高级模式状态
            GotoParameterStatus = GotoParameterModeDisable;//处理完清理状态
        }
        //------------------------------E N D-----------------------------------
        
        
        //if(Mod_Key_MaskReCaptureOperation(GetEvent,KeyNull)) 
        if(Mod_Key_GetEvent())
        {
                if(MaskMode == Date) //高级模式按菜单，进入，检验屏蔽函数的功能，组合键可以跳出
                {
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyMenu);
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyAdd);
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyDEC);
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyPower);                 
                }
                
                if(KeyPowerDown == Date) //关机模式
                {
                    if((KeyPushUp|KeyPushDown)&(CurrentKeyStatus = Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)))
                    {
                        if(KeyPushUp == CurrentKeyStatus)    //菜单按键抬起关闭进入高级使能，按下使能进入高级使能检验KeyPushUp状态
                        {
                            GotoParameterStatus = GotoParameterModeDisable;
                            Date &= ~ParameterMode;
                        }
                        else if(KeyPushDown == CurrentKeyStatus)
                        {
                            GotoParameterStatus = GotoParameterModeEnable;
                        }
                    }
               
                   if((KeyPushDown) & Key_ValueOperation(KeyPower,NOCONTINUE,NOCONTINUE))     //关机键按下
                   {
                        if(GotoParameterModeEnable == GotoParameterStatus)                      //高级模式使能进入高级模式，清零星期4显示
                        {
                            GotoParameterStatus = GoingtoParameterMode; 
                        }
                        Date &= ~KeyPowerDown;             
                   }
                }
                else
                {
                        if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyMenu,Delay3Second,Delay1Second))       //菜单键Date第0位取反，长按3秒触发长按功能，每1S进入一次长按操作
                        {
                            Date ^= KeyMenuDown;                                                   
                        }
                        else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyAdd,Delay3Second,Delay1Second))  //+键Date第1位取反，长按3秒触发长按功能，每1S进入一次长按操作
                        {
                            Date ^= KeyAddDown;
                           
                        }    
                        else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,Delay3Second,Delay1Second))  //+Date第2位取反，长按3秒触发长按功能，每1S进入一次长按操作
                        {
                            Date ^= KeyDECDown;
                            
                        } 
                        else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyPower,NOCONTINUE,NOCONTINUE))//按电源键Date第4位等于0x80，无长按操作
                        {
                            Date = KeyPowerDown;
                            Mod_Key_MaskReCaptureOperation(ReCapture,KeyMenu);//重读一次菜单键，如果不加此函数，按住菜单键关机开机无法进入高级模式
                                                                
                        }
                        else if((KeyContinueDown)&Key_ValueOperation(KeyChildLock,Delay3Second,NOCONTINUE))  //童锁键取反星期5显示，无长按连击功能
                        {
                            Date ^= KeyChildLockDown;
                            
                        } 
                        else if((KeyContinueDown)&Key_ValueOperation(KeySetTime,Delay3Second,NOCONTINUE))    //时间键取反星期6显示，无长按连击功能
                        {
                            Date ^= KeySetTimeDown;
                            
                        } 
                }
              
        }              

    }
}
//------------------------------E N D-------------------------------------------


//-----------------------ExMain.c--END------------------------------------------