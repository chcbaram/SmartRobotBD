//----------------------------------------------------------------------------
//    ���α׷��� 	: UartMenu ���� �Լ� ���
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type 		:
//
//    ���ϸ�     	: AP_UartMenu.h
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
                                                                                                 
                                                                                                 

