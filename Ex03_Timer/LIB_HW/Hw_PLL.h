//----------------------------------------------------------------------------
//    ���α׷��� 	: PLL ���� ���
//
//    ������     	: 
//
//    ��  ¥     	:
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_PLL.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_PLL_H__
#define __HW_PLL_H__


#ifdef   HW_PLL_LOCAL
#define  EXT_HW_PLL_DEF
#else
#define  EXT_HW_PLL_DEF     extern
#endif



#include "Hw.h"



EXT_HW_PLL_DEF void  Hw_PLL_Init( void );



#endif
