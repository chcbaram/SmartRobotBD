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
//    파일명     	: Lib_Uart.h
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
                                                                                                 
                                                                                                 

