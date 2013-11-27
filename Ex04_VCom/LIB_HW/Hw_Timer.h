//----------------------------------------------------------------------------
//    ���α׷��� : Timer �Լ� ���
//
//    ������     : Cho Han Cheol
//
//    ��  ¥     : 
//    
//    ���� ����  : 
//
//    MPU_Type   : 
//
//    ���ϸ�     : Timer_Lib.h
//----------------------------------------------------------------------------



#ifndef __HW_TIMER_H__
#define __HW_TIMER_H__ 


#ifdef   HW_TIMER_LOCAL
#define  EXT_HW_TIMER_DEF 
#else
#define  EXT_HW_TIMER_DEF     extern
#endif



#include "Hw.h"






#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef NULL
#define NULL 0
#endif


#define TIMER_MAX    7

#define ONE_TIME     1
#define LOOP_TIME    2

#define TIMER_HANDLE  s16

typedef struct tmr 
{

	u8   Timer_En;                 // Ÿ�̸� �ο��̺� ��ȣ
	u8   Timer_Mode;               // Ÿ�̸� ���
	u16  Timer_Ctn;                // ������ Ÿ�̸� ��
	u16  Timer_Init;               // Ÿ�̸� �ʱ�ȭ�ɶ��� ī��Ʈ ��
	void (*TmrFnct)(void);         // ����ɶ� ����� �Լ�
	void *TmrFnctArg;              // �Լ��� ������ �μ���
} TMR;



EXT_HW_TIMER_DEF void Hw_Timer_Init( void );
EXT_HW_TIMER_DEF void Hw_Timer_Tick(void);
EXT_HW_TIMER_DEF void Hw_Timer_Set(u8 TmrNum, u16 TmrData, u8 TmrMode, void (*Fnct)(void),void *arg);
EXT_HW_TIMER_DEF void Hw_Timer_Start(u8 TmrNum);
EXT_HW_TIMER_DEF void Hw_Timer_Stop(u8 TmrNum);
EXT_HW_TIMER_DEF void Hw_Timer_Reset(u8 TmrNum);
EXT_HW_TIMER_DEF void Hw_Timer_Delay_ms( u16 DelayData );
EXT_HW_TIMER_DEF s16  Hw_Timer_Get_Handle( void );
EXT_HW_TIMER_DEF u32  Hw_Timer_Get_CountValue( void );


#endif





