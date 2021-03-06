//----------------------------------------------------------------------------
//    프로그램명 	: Hw 공용 
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_H__
#define __HW_H__                                                                                                  


#ifdef   HW_LOCAL
#define  EXT_HW_DEF 
#else
#define  EXT_HW_DEF     extern
#endif


#include "Hw_Regs.h"
#include "Hw_Define.h"

#include "Hw_PLL.h"
#include "Hw_Uart.h"
#include "Hw_Led.h"
#include "Hw_ISR.h"


#include "Lb_Printf.h"


EXT_HW_DEF void Hw_Init( void );

EXT_HW_DEF void Hw_Wait( u32 delay );
EXT_HW_DEF void Hw_Wait_Usec( u32 usec );

#endif
