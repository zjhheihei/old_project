//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_KEYBOARD_H
#define _BSP_KEYBOARD_H
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef  Data_Retyped
#include ".\DataTypedef.h"
#endif

#ifndef  _MOD_SYSTEMTICK_H
#include "..\..\Mod_Systemtick\Src\Mod_SystemTick.h"
#endif

//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************��ϼ��궨�����ã�����16Ϊ��ʽ�ĸ�8λ****************************
#define  KeyGroupBit0     0x0100   //��ϰ���
#define  KeyGroupBit1     0x0200   //��ϰ���
#define  KeyGroupBit2     0x0400   //��ϰ���
#define  KeyGroupBit3     0x0800   //��ϰ���
#define  NOCONTINUE       0xffff   //���������
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************���������궨�����ã�����16λ��ʽ�ĵ�8λ**************************
#define  KeyNull          0x0000
#define  KeyAloneBit0     0x0001    
#define  KeyAloneBit1     0x0002  
#define  KeyAloneBit2     0x0004  
#define  KeyAloneBit3     0x0008  
#define  KeyAloneBit4     0x0010
#define  KeyAloneBit5     0x0020  
#define  KeyAloneBit6     0x0040  
#define  KeyAloneBit7     0x0080  
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//����: ��ȡ�����˿�
//����: ��ȡIO״̬
//���: ��

//����: BN_TRUE�����м�����

BOOLEAN BSP_GetKey0(void);
BOOLEAN BSP_GetKey1(void); 
BOOLEAN BSP_GetKey2(void); 
BOOLEAN BSP_GetKey3(void);
BOOLEAN BSP_GetKey4(void);
BOOLEAN BSP_GetKey5(void);
BOOLEAN BSP_GetKey6(void);       
BOOLEAN BSP_GetKey7(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//����: ��������ϼ�����
//����: ���ð���IO�ں���ϼ�����ϼ�����ָ�뷵�أ����mod��bsp����֮������໥
//      ����
//���: ��

//����: INT16U* ��ָ�룬��ָ���Ǵ洢��ϼ����õ��׵�ַ

INT16U* BSP_KeyHardWares_Configure(void);
//------------------------------E N D-------------------------------------------
#endif

//-----------------------BSP_Keyboard.h--END------------------------------------
