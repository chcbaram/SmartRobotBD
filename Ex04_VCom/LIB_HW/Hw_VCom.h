//----------------------------------------------------------------------------
//    ���α׷��� 	: VCom ���� �Լ� ���
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type 		:
//
//    ���ϸ�     	: Hw_VCom.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _HW_VCOM_H
#define  _HW_VCOM_H



#include "Hw.h"


#ifdef   HW_VCOM_LOCAL
#define  EXT_HW_VCOM_DEF
#else
#define  EXT_HW_VCOM_DEF     extern
#endif




EXT_HW_VCOM_DEF void Hw_VCom_Init( void );

EXT_HW_VCOM_DEF u8   Hw_VCom_Getch( void );
EXT_HW_VCOM_DEF void Hw_VCom_Putch( char Uart_PutData );

#endif
                                                                                                 
                                                                                                 

