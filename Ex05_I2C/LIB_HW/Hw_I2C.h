//----------------------------------------------------------------------------
//    ���α׷��� 	: I2C ���� ���
//
//    ������     	: 
//
//    ��  ¥     	:
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_I2C.h
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
