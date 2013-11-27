//----------------------------------------------------------------------------
//    프로그램명 	: UartMenu 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_UartMenu.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _AP_UARTMENU_H
#define  _AP_UARTMENU_H



#include "Ap.h"



#ifdef   AP_UARTMENU_LOCAL
#define  EXT_AP_UARTMENU_DEF
#else
#define  EXT_AP_UARTMENU_DEF     extern
#endif


EXT_AP_UARTMENU_DEF int Menu_ExeCmd(void);

#endif
                                                                                                 
                                                                                                 

