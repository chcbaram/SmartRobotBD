//----------------------------------------------------------------------------
//    프로그램명 	: I2C 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_I2C_H__
#define __HW_I2C_H__


#ifdef   HW_I2C_LOCAL
#define  EXT_HW_I2C_DEF
#else
#define  EXT_HW_I2C_DEF     extern
#endif



#include "Hw.h"



EXT_HW_LED_DEF void  Hw_I2C_Init( void );


#endif
