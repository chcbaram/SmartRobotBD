//----------------------------------------------------------------------------
//    ���α׷��� 	:
//
//    ������     	: Cho Han Cheol
//
//    ��  ¥     	:
//    
//    ���� ����  	:
//
//    MPU_Type	:
//
//    ���ϸ�     	: Ap.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __AP_H__
#define __AP_H__


#ifdef   AP_LOCAL
#define  EXT_AP_DEF
#else
#define  EXT_AP_DEF     extern
#endif





#include "Hw.h"
#include "Ap_Define.h"
#include "Lb_Util.h"

#include "Ap_UartMenu.h"





EXT_AP_DEF void Ap_Init( void );



#endif
