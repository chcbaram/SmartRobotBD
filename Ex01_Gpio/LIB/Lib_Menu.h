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




                                                                                                 
#ifndef  _LIB_MENU_H
#define  _LIB_MENU_H



#include "Define.h"

#include "Lib_Uart.h"
#include "Lib_Util.h"

#ifdef   LIB_MENU_LOCAL
#define  EXT_LIB_MENU_DEF
#else
#define  EXT_LIB_MENU_DEF     extern
#endif



EXT_LIB_MENU_DEF int main_menu(void);
EXT_LIB_MENU_DEF int Menu_ExeCmd(void);

#endif
                                                                                                 
                                                                                                 

