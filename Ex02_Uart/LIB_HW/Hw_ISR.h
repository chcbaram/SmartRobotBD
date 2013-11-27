//----------------------------------------------------------------------------
//    ���α׷��� 	: ISR ���� ���
//
//    ������     	: Cho Han Cheol
//
//    ��  ¥     	:
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_ISR.h
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
