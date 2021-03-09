//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//用户宏定义
//---------------------------------------------------------------------
#ifndef  SnailMacroDefed
#define  SnailMacroDefed
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//指针边界保护
//BN_TRUE 正常
//---------------------------------------------------------------------
#define _CheckPointerBoundary(_CurrentIndex,_MaxIndex,_NotFail) \
do\
{\
   if((_CurrentIndex)>(_MaxIndex))\
   {\
       _NotFail=BN_FALSE;\
   }\
   else\
   {\
       _NotFail=BN_TRUE;\
   }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数组清零
//BuffIndex=0;
//BuffSize=sizeof(UpdateLcdBuffer);
//_ClearArrayData(UpdateLcdBuffer[BuffIndex],BuffIndex,BuffSize);
//---------------------------------------------------------------------
#define _ClearArrayData(_ArrayName,_Index,_Size) \
do\
{\
   while(_Size)\
   {\
     _ArrayName=0;\
     _Index++;\
     _Size--;\
   }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数组填充
//---------------------------------------------------------------------
#define _FillArrayData(_ArrayName,_Index,_Size,NewData) \
do\
{\
   while(_Size)\
   {\
     _ArrayName=NewData;\
     _Index++;\
     _Size--;\
   }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//获取数组最大索引
//---------------------------------------------------------------------
#define _GetArrayMaxIndex(_ArrayName) (sizeof(_ArrayName)-1)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif	
//++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++

