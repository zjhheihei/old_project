/*******************************************************************************
*
*	ģ������ : ����ģ����ʾ
*	�ļ����� : ExMain.c
*	��    �� : V1.0
*	˵    �� : ��ʾ����ģ���������
                   �����á��ñ���Date��ֵ�����鰴��ģ�顣
                   Debugģʽ�����ڣ�IAR��View�˵��´�Live Watch ���Date,
                   ��������Date �ı䡣
                   
                   �˵� 01 +��02 -��04 ��Դ��08 
                   ģ�⹫˾��Ʒ�ĸ߼�ģʽ�������鰴�������к�����
                   �ѽ���E51.33B�����ϲ���ͨ��
*                  
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0   2018-07-28  zhoujh  ��ʽ����
*	Copyright (C), 2018-2022,   menredGroup
*       ���뻷�� ��IAR FOR STM8 V2.10.4(EWSTM8-2104-Autorun)  
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#include "..\..\Mod_SysClock\Src\Mod_SysClock.h"
#include "..\..\Mod_Keyboard\Src\Mod_Keyboard.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************�������ƺ궨��***************************************
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
//*************************�߼�ģʽ״̬ö��**************************************
typedef enum
{
    GotoParameterModeDisable   = 0x00,  //
    GotoParameterModeEnable    = 0x01,  //ʹ�ܽ���߼�ģʽ
    GoingtoParameterMode       = 0x02,   //����߼�ģʽ
} GotoParameterStatus_Def;

GotoParameterStatus_Def   GotoParameterStatus = GotoParameterModeDisable;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************ֵ�����Ű����仯*************************************
static INT8U Date = 0x01;
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void main(void)
{
    KeyRetValue_Def CurrentKeyStatus;
   //++++++++++++++++++++++++++++++start++++++++++++++++++++++++++++++++++++++++
   //��ʼ��оƬʱ�ӣ�ϵͳ����ʱ�ӺͰ���ģ��
    ITF_Clock_Configure();
    ITF_Systick_Configure();
   //------------------------------E N D----------------------------------------
    while(1)
    {
        //++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++
        //����ϵͳʱ�ӣ��ڲ�ʱ��У���Ͱ���ɨ�躯��
        SystickTimeRun(); 
        Mod_Key_ScanTask();
        HSICalibration_Task();
        //------------------------------E N D-----------------------------------
        
        
        //++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++
        //����߼�ģʽ
        if(GoingtoParameterMode == GotoParameterStatus)    //ģ�����߼�ģʽ
        {
            Date = ParameterMode;                                   //�߼�ģʽ״̬
            GotoParameterStatus = GotoParameterModeDisable;//����������״̬
        }
        //------------------------------E N D-----------------------------------
        
        
        //if(Mod_Key_MaskReCaptureOperation(GetEvent,KeyNull)) 
        if(Mod_Key_GetEvent())
        {
                if(MaskMode == Date) //�߼�ģʽ���˵������룬�������κ����Ĺ��ܣ���ϼ���������
                {
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyMenu);
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyAdd);
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyDEC);
                    Mod_Key_MaskReCaptureOperation(MaskOnce,KeyPower);                 
                }
                
                if(KeyPowerDown == Date) //�ػ�ģʽ
                {
                    if((KeyPushUp|KeyPushDown)&(CurrentKeyStatus = Key_ValueOperation(KeyMenu,NOCONTINUE,NOCONTINUE)))
                    {
                        if(KeyPushUp == CurrentKeyStatus)    //�˵�����̧��رս���߼�ʹ�ܣ�����ʹ�ܽ���߼�ʹ�ܼ���KeyPushUp״̬
                        {
                            GotoParameterStatus = GotoParameterModeDisable;
                            Date &= ~ParameterMode;
                        }
                        else if(KeyPushDown == CurrentKeyStatus)
                        {
                            GotoParameterStatus = GotoParameterModeEnable;
                        }
                    }
               
                   if((KeyPushDown) & Key_ValueOperation(KeyPower,NOCONTINUE,NOCONTINUE))     //�ػ�������
                   {
                        if(GotoParameterModeEnable == GotoParameterStatus)                      //�߼�ģʽʹ�ܽ���߼�ģʽ����������4��ʾ
                        {
                            GotoParameterStatus = GoingtoParameterMode; 
                        }
                        Date &= ~KeyPowerDown;             
                   }
                }
                else
                {
                        if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyMenu,Delay3Second,Delay1Second))       //�˵���Date��0λȡ��������3�봥���������ܣ�ÿ1S����һ�γ�������
                        {
                            Date ^= KeyMenuDown;                                                   
                        }
                        else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyAdd,Delay3Second,Delay1Second))  //+��Date��1λȡ��������3�봥���������ܣ�ÿ1S����һ�γ�������
                        {
                            Date ^= KeyAddDown;
                           
                        }    
                        else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyDEC,Delay3Second,Delay1Second))  //+Date��2λȡ��������3�봥���������ܣ�ÿ1S����һ�γ�������
                        {
                            Date ^= KeyDECDown;
                            
                        } 
                        else if((KeyPushDown|KeyContinueDown)&Key_ValueOperation(KeyPower,NOCONTINUE,NOCONTINUE))//����Դ��Date��4λ����0x80���޳�������
                        {
                            Date = KeyPowerDown;
                            Mod_Key_MaskReCaptureOperation(ReCapture,KeyMenu);//�ض�һ�β˵�����������Ӵ˺�������ס�˵����ػ������޷�����߼�ģʽ
                                                                
                        }
                        else if((KeyContinueDown)&Key_ValueOperation(KeyChildLock,Delay3Second,NOCONTINUE))  //ͯ����ȡ������5��ʾ���޳�����������
                        {
                            Date ^= KeyChildLockDown;
                            
                        } 
                        else if((KeyContinueDown)&Key_ValueOperation(KeySetTime,Delay3Second,NOCONTINUE))    //ʱ���ȡ������6��ʾ���޳�����������
                        {
                            Date ^= KeySetTimeDown;
                            
                        } 
                }
              
        }              

    }
}
//------------------------------E N D-------------------------------------------


//-----------------------ExMain.c--END------------------------------------------