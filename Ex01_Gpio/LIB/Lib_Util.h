//----------------------------------------------------------------------------
//    ���α׷��� 	: Util ���� �Լ� ���
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type 		:
//
//    ���ϸ�     	: Lib_Util.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _LIB_UTIL_H
#define  _LIB_UTIL_H



#include "Define.h"



#ifdef   LIB_UTIL_LOCAL
#define  EXT_LIB_UTIL_DEF
#else
#define  EXT_LIB_UTIL_DEF     extern
#endif



EXT_LIB_UTIL_DEF void delay(volatile unsigned int timeCount);
EXT_LIB_UTIL_DEF void delay_second(void);
                                            

#endif
                                                                                                 
                                                                                                 

