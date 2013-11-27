//----------------------------------------------------------------------------
//    프로그램명 	: Uart 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: Hw_Uart.h
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
                                                                                                 
                                                                                                 

