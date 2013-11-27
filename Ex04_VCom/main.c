//----------------------------------------------------------------------------
//    ���α׷��� 	: Radio
//
//    ������     	: Cho Han Cheol (Baram)
//
//    ��  ¥     	: 2013. 8.20.
//
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Main.c
//----------------------------------------------------------------------------
/*
	130820 
		- �ʱ� ������Ʈ �۾� ����
	130823 
		- Uart ���ͷ�Ʈ ����
		
*/




//----- ������� ����
//
#define  MAIN_LOCAL

#include "Main.h"




//-- ���� ����
//



//-- ���� ����
//



//-- ���� �Լ�
//
static void Main_Init( void );



void LED_Tick( void )
{
	Hw_Led_Toggle(0);
}



/*---------------------------------------------------------------------------
     TITLE   :	main          
     WORK    : 	
     			���� �Լ�
     ARG     : 	void
     RET     : 	
     			int
---------------------------------------------------------------------------*/
int main(void)
{
	int key;

	Main_Init();
	

    printf("Radio V0.1\n");


	//-- ��ɾ� ó��
	//
	Ap_RadioMenu_ExeCmd();    


    while(1);
    
    return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Main_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Main_Init( void )
{
	Hw_Init();
	Ap_Init();
	
	Hw_Timer_Set  ( HW_TIMER_CH_LED, 1000, LOOP_TIME, LED_Tick, NULL );
	Hw_Timer_Start( HW_TIMER_CH_LED );	
}

