//----------------------------------------------------------------------------
//    ���α׷��� 	: Printf ���� �Լ� ���
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type 		:
//
//    ���ϸ�     	: Lb_Printf.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _LB_PRINTF_H
#define  _LB_PRINTF_H



#include "Hw.h"



#ifdef   LB_PRINTF_LOCAL
#define  EXT_LB_PRINTF_DEF
#else
#define  EXT_LB_PRINTF_DEF     extern
#endif


EXT_LB_PRINTF_DEF void printf( char *fmt, ... );                                            

#endif
                                                                                                 
                                                                                                 

