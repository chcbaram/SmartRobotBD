//----------------------------------------------------------------------------
//    프로그램명 	: Util 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: Lib_Util.h
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
                                                                                                 
                                                                                                 

