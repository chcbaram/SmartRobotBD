//----------------------------------------------------------------------------
//    ���α׷��� 	: Timer ���� �Լ�
//
//    ������     	: Cho Han Cheol
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_Timer.c
//----------------------------------------------------------------------------



//----- ������� ����
//
#define  HW_TIMER_LOCAL  

#include "Hw_TImer.h"






//-- ���� ����
//







//-- ���� ����
//

static volatile u32 Hw_Timer_Counter  = 0;	

static volatile u16 Tmr_Ctn  = 0;	// Delay�� Ÿ�̸� ����.
static volatile u16 Tmr_HandleIndex = 0;

static TMR  Timer_Tbl[ TIMER_MAX ];    // Ÿ�̸� �迭 ����




           


//-- ���� �Լ�
//

void Hw_Timer_SetupISR( void );
void Hw_Timer_Setup( void );



void Hw_Timer_ISR(void)
{

	Hw_Timer_Tick();  

}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_Init( void )
{
	u8 i;
	static u8 Excute = 0;

	
	if( Excute == 1 ) return;  // �̹� �ѹ� �����ߴٸ� ����.
	
	
	// ����ü �ʱ�ȭ
	for(i=0; i<TIMER_MAX; i++)
	{
		Timer_Tbl[i].Timer_En   = OFF;
		Timer_Tbl[i].Timer_Ctn  = 0;
		Timer_Tbl[i].Timer_Init = 0;
		Timer_Tbl[i].TmrFnct    = NULL;
	}	                   
	
	
	Hw_Timer_SetupISR();
	Hw_Timer_Setup();	

	Excute = 1;

}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_SetupISR
     WORK    :
     	 	 	 Ÿ�̸� H/W ����� �ʱ�ȭ �Ѵ�.
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_SetupISR( void )
{	
	Hw_ISR_SetIRQFuncByAddr( 0x003C, (u32)Hw_Timer_ISR, 0 );	
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_Setup
     WORK    :
     	 	 	 Ÿ�̸� H/W ����� �ʱ�ȭ �Ѵ�.
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_Setup( void )
{

	REG_STK_LOAD = 72000 - 1;	// 72000000 / 72000 = 1000us = 1ms
	
	REG_STK_VAL  = 0;	
	
	SET_BIT( REG_STK_CTRL, 2 );	// CLKSOURCE = Processor Clock 72Mhz		
	SET_BIT( REG_STK_CTRL, 1 );	// ���ͷ�Ʈ Ȱ��ȭ		
	SET_BIT( REG_STK_CTRL, 0 );	// Counter Enable
	
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Tick
     WORK	:
			Ÿ�̸����ͷ�Ʈ �߻��ÿ� ����Ǵ� �Լ�(1ms Ȥ�� 10ms ������ ����)
     ARG	: void
     RET	: void
---------------------------------------------------------------------------*/
void Hw_Timer_Tick(void)
{
	u8 i;

	
	Hw_Timer_Counter++;


	if( Tmr_Ctn ) Tmr_Ctn--;
	
	
			   
	for(i=0; i<TIMER_MAX; i++)								// Ÿ�̸� ������ŭ
	{
		if( Timer_Tbl[i].Timer_En == ON)   					// Ÿ�̸Ӱ� Ȱ��ȭ ���?
		{
			Timer_Tbl[i].Timer_Ctn--;  						// Ÿ�̸Ӱ� ����

			if(Timer_Tbl[i].Timer_Ctn == 0) 				// Ÿ�̸� �����÷ξ�
			{
				if(Timer_Tbl[i].Timer_Mode == ONE_TIME)  	// �ѹ��� �����ϴ°Ÿ�

				Timer_Tbl[i].Timer_En = OFF;     			// Ÿ�̸� OFF �Ѵ�.

				Timer_Tbl[i].Timer_Ctn = Timer_Tbl[i].Timer_Init; // Ÿ�̸� �ʱ�ȭ

				(*Timer_Tbl[i].TmrFnct)();  				// ���޺��� ���� �Լ� ����
			}
		}
	}
	
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Set
     WORK
			����Ʈ Ÿ�̸ӿ��� �̺�Ʈ �߻��� ������ �Լ� ����
     ARG
     	 	U8 TmrNum 	: Ÿ�̸� ��ȣ
			U16 TmrData : Ÿ�̸� �ð���( ms ���� )
			U8 TmrMode 	: ONE_TIME : Ÿ�̸� ������ �ѹ��� ����
			LOOP_TIME 	: Ÿ�̸Ӱ� ������ �ֱ������� ���� ����.
			void (*Fnct)(void),void *arg : ������ Ÿ�̸� �Լ�.
     RET
     	 	void
---------------------------------------------------------------------------*/
void Hw_Timer_Set(u8 TmrNum, u16 TmrData, u8 TmrMode, void (*Fnct)(void),void *arg)
{
	Timer_Tbl[TmrNum].Timer_Mode = TmrMode;    // ��Ʈ����
	Timer_Tbl[TmrNum].TmrFnct    = Fnct;       // ������ �Լ�
	Timer_Tbl[TmrNum].TmrFnctArg = arg;        // �Ű�����
	Timer_Tbl[TmrNum].Timer_Ctn  = TmrData;
	Timer_Tbl[TmrNum].Timer_Init = TmrData;
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Start
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Start(u8 TmrNum)
{
	if(TmrNum < TIMER_MAX)
		Timer_Tbl[TmrNum].Timer_En = ON;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Stop
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Stop(u8 TmrNum)
{
	if(TmrNum < TIMER_MAX)
		Timer_Tbl[TmrNum].Timer_En = OFF;
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Reset
     WORK
     	 	 - Ÿ�̸Ӱ� �ʱ�ȭ
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Reset(u8 TmrNum)
{
	Timer_Tbl[TmrNum].Timer_En   = OFF;
	Timer_Tbl[TmrNum].Timer_Ctn  = Timer_Tbl[TmrNum].Timer_Init;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Delay_ms
     WORK
     	 	 ms ������ �����̸� �ش�.
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Delay_ms( u16 DelayData )
{
	Tmr_Ctn = DelayData;
	
	while( Tmr_Ctn );
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Get_Handle
     WORK
     	 	 ms ������ �����̸� �ش�.
     ARG
     RET
---------------------------------------------------------------------------*/
s16 Hw_Timer_Get_Handle( void )
{
	s16 TmrIndex = Tmr_HandleIndex;
	
	Tmr_HandleIndex++;
	
	return TmrIndex;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Get_CountValue
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
u32 Hw_Timer_Get_CountValue( void )
{
	return Hw_Timer_Counter;
}

