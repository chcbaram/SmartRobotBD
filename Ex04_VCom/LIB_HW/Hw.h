//----------------------------------------------------------------------------
//    ���α׷��� 	: Hw ���� 
//
//    ������     	: 
//
//    ��  ¥     	:
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_H__
#define __HW_H__                                                                                                  


#ifdef   HW_LOCAL
#define  EXT_HW_DEF 
#else
#define  EXT_HW_DEF     extern
#endif


//-- ��� ����
//
#define HW_USE_USB_VCOM					1
#define HW_USE_USB_SMARTROBOT_BD		1	// ���� ���� ������ �ִ� ����� ����Ʈ�κ� ���尡 USB Enable ���� �ٸ�



//-- Ÿ�̸� ä�� ����
//
#define HW_TIMER_CH_LED					0
#define HW_TIMER_CH_DEBUG				1




#include "Hw_Regs.h"
#include "Hw_Define.h"

#include "Hw_PLL.h"
#include "Hw_Uart.h"
#include "Hw_Led.h"
#include "Hw_ISR.h"
#include "Hw_Timer.h"

#include "Hw_VCom.h"

#include "Lb_Printf.h"


EXT_HW_DEF void Hw_Init( void );

EXT_HW_DEF void Hw_Wait( u32 delay );
EXT_HW_DEF void Hw_Wait_Usec( u32 usec );

#endif
