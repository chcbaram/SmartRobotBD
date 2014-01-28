//----------------------------------------------------------------------------
//    프로그램명 	: SmartRobot BD
//
//    만든이     	: Cho Han Cheol (Baram)
//
//    날  짜     	: 2013. 8.20.
//
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Main.c
//----------------------------------------------------------------------------
/*
	130820 
		- 초기 프로젝트 작업 시작
	130823 
		- Uart 인터럽트 구현
	131216
		- MPU6050 구현
	140115
		- 초음파 센서 구현  	
*/




//----- 헤더파일 열기
//
#define  MAIN_LOCAL

#include "main.h"




//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//
static void Main_Init( void );



void LED_Tick( void )
{
	Hw_Led_Toggle(0);
}



/*---------------------------------------------------------------------------
     TITLE   :	main          
     WORK    : 	
     			메인 함수
     ARG     : 	void
     RET     : 	
     			int
---------------------------------------------------------------------------*/
int main(void)
{
	int key;
	u32 TimeStart;
	u32 TimeEnd;
	int raw_values[11];


	Main_Init();

	//-- 명령어 처리
	//  
	Ap_GLcdMenu_ExeCmd();    


    while(1)
    {
    	key = get_byte();

    	if( key == 'r' )
    	{
    		Hw_IMU_FreeIMU_getRawValues(raw_values);

    		Lb_printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n", raw_values[0], raw_values[1], raw_values[2], raw_values[3], raw_values[4], raw_values[5], raw_values[6], raw_values[7], raw_values[8], raw_values[9], raw_values[10]);
    	}

    	if( key == 'v' )
    	{
    		Hw_IMU_FreeIMU_getRawValues(raw_values);

    		Lb_printf("Baram \n");
    	}    

    	if( key == 'b' )
    	{
    		uint8_t count = get_byte();

    		//int count = 50;

      		for(uint8_t i=0; i<count; i++) 
      		{
          		Hw_IMU_FreeIMU_getRawValues(raw_values);

          		for( int j=0; j<9; j++ )
          		{
          			Hw_Uart_Putch(HW_UART_COM1, (u8)((raw_values[j] >> 0) & 0xFF) );
          			Hw_Uart_Putch(HW_UART_COM1, (u8)((raw_values[j] >> 8) & 0xFF) );
          			//print_byte( i >> 0 );
          			//print_byte( i >> 8 );

          			//print_byte( raw_values[i] >> 16 );
          			//print_byte( raw_values[i] >> 24 );          			
          		}
        		//Lb_printf("\r\n");
        		Hw_Uart_Putch(HW_UART_COM1, 0x01 );
        		Hw_Uart_Putch(HW_UART_COM1, 0x01 );

        		Hw_Wait_ms(10);
        	}
        }	
    }
    
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



