//----------------------------------------------------------------------------
//    ���α׷��� 	: Uart ���� �Լ� ���
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type 		:
//
//    ���ϸ�     	: Lib_Uart.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _HW_UART_H
#define  _HW_UART_H



#include "Define.h"



#ifdef   HW_UART_LOCAL
#define  EXT_HW_UART_DEF
#else
#define  EXT_HW_UART_DEF     extern
#endif



EXT_HW_UART_DEF void printf(char *fmt, ...);
                                            

#endif
                                                                                                 
                                                                                                 

