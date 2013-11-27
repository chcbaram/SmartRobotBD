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
//    ���ϸ�     	: Hw_Uart_Q.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_UART_Q_H__
#define __HW_UART_Q_H__


#ifdef   HW_UART_Q_LOCAL
#define  EXT_HW_UART_Q_DEF
#else
#define  EXT_HW_UART_Q_DEF     extern
#endif



#include "Hw.h"



#define HW_UART_Q_CH_MAX       2
#define HW_UART_Q_BUFFER_MAX   10                //  ���� �ִ� ����


#define HW_UART_Q_EMPTY        1
#define HW_UART_Q_FULL         0
#define HW_UART_POP_SUCCESS    2


EXT_HW_UART_Q_DEF u32 HW_UART_Q_SIZE( u8 Ch );
EXT_HW_UART_Q_DEF u32 HW_UART_Q_VAILD( u8 Ch );

EXT_HW_UART_Q_DEF u8  Hw_Uart_Q_Buffer[HW_UART_Q_CH_MAX][ HW_UART_Q_BUFFER_MAX+1 ];


EXT_HW_UART_Q_DEF s16  Hw_Uart_Q_Size [HW_UART_Q_CH_MAX];                       // ������ ������ ����
EXT_HW_UART_Q_DEF s16  Hw_Uart_Q_Start[HW_UART_Q_CH_MAX];                      // ������ ���� ������
EXT_HW_UART_Q_DEF s16  Hw_Uart_Q_End  [HW_UART_Q_CH_MAX];                        // ������ �� ������


EXT_HW_UART_Q_DEF void Hw_Uart_Q_Init(void);
EXT_HW_UART_Q_DEF u32  Hw_Uart_Q_PushReady( u8 Ch );
EXT_HW_UART_Q_DEF u8   Hw_Uart_Q_Push( u8 Ch, u8 *PushData );
EXT_HW_UART_Q_DEF u8   Hw_Uart_Q_Pop( u8 Ch, u8 *pData );

#endif
