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
//    ���ϸ�     	: Lb_Util.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _LB_UTIL_H
#define  _LB_UTIL_H



#include "Hw.h"



#ifdef   LB_UTIL_LOCAL
#define  EXT_LB_UTIL_DEF
#else
#define  EXT_LB_UTIL_DEF     extern
#endif



EXT_LB_UTIL_DEF void delay(volatile unsigned int timeCount);
EXT_LB_UTIL_DEF void delay_second(void);
                                            

#endif
                                                                                                 
                                                                                                 

