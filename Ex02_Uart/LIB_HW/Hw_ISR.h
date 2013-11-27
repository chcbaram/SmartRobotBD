//----------------------------------------------------------------------------
//    프로그램명 	: ISR 관련 헤더
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_ISR.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_ISR_H__
#define __HW_ISR_H__


#ifdef   HW_ISR_LOCAL
#define  EXT_HW_ISR_DEF
#else
#define  EXT_HW_ISR_DEF     extern
#endif



#include "Hw.h"



EXT_HW_ISR_DEF void  Hw_ISR_Init( void );


EXT_HW_ISR_DEF void Hw_ISR_SetIRQFunc( u8 ISR_Num, u32 FuncAddress, u8 ISR_Priority );


#endif
