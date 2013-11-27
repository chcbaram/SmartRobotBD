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
//    ���ϸ�     	: Hw_Uart.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _HW_UART_H
#define  _HW_UART_H



#include "Hw.h"



#ifdef   HW_UART_LOCAL
#define  EXT_HW_UART_DEF
#else
#define  EXT_HW_UART_DEF     extern
#endif


EXT_HW_UART_DEF void Hw_Uart_Init( void );

EXT_HW_UART_DEF void Hw_Uart_Printf( char *fmt, ... );


EXT_HW_UART_DEF char get_byte(void);                                            

#endif
                                                                                                 
                                                                                                 

