//----------------------------------------------------------------------------
//    프로그램명 	: GLcd Menu 관련 함수
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Ap_GLcdMenu.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  AP_GLCDMENU_LOCAL

#include "Ap_GLcdMenu.h"

  

//-- 내부 선언
//



//-- 내부 변수
//

  

//-- 내부 함수
//
void Ap_GLcdMenu_ShowMenu(void);


  

void Ap_GCldMenu_MoveCircleTick_1st( void )
{
	static s8 x   = 0;
	static s8 dir = 1;
	static u8 cnt = 0;
	
	Hw_N5110G_DrawClearRect( 0, 25, HW_N5110G_WIDTH, 35 );           						
	Hw_N5110G_DrawCircle( 10 + x, 30, 5, 0 ); 								
 	Hw_N5110G_DrawRequest();
 	
 	x += dir;
 	
 	if( x >= (HW_N5110G_WIDTH-15) ) 
 	{
 		dir = -1;
 		x += dir;
 	}

 	if( x < 0 ) 
 	{
 		dir = 1; 		
 		x  += dir;
 	}

	cnt++;
}

    
void Ap_GCldMenu_MoveCircleTick_2nd( void )
{
	static s8 x   = 0;
	static s8 dir = 1;
	static u8 cnt = 0;
	
	Hw_N5110G_DrawClearRect( 0, 35, HW_N5110G_WIDTH, 45 );           						
	Hw_N5110G_DrawCircle( 10 + x, 40, 3, 0 ); 							
 	Hw_N5110G_DrawRequest();
 	
 	x += dir;
 	
 	if( x >= (HW_N5110G_WIDTH-15) ) 
 	{
 		dir = -1;
 		x += dir;
 	}

 	if( x < 0 ) 
 	{
 		dir = 1; 		
 		x  += dir;
 	}

	cnt++;
}



/*---------------------------------------------------------------------------
     TITLE   : Ap_GLcdMenu_ShowMenu
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Ap_GLcdMenu_ShowMenu(void)
{
	u8 key;

	Lb_printf("\n\n");
	Lb_printf("*******************************************************\n");
	Lb_printf("                      MPU6050                          \n");
	Lb_printf("*******************************************************\n");
	Lb_printf("* 1. LCD Clear                                        *\n");
	Lb_printf("* 2. Show Acc                                         *\n");
	Lb_printf("* 3.                                                  *\n");
	Lb_printf("* 4.                                                  *\n");
	Lb_printf("* 5.                                                  *\n");
	Lb_printf("* 6.                                                  *\n");
	Lb_printf("* 7.                                                  *\n");
	Lb_printf("* 8.                                                  *\n");
	Lb_printf("* 9.                                                  *\n");
	Lb_printf("* m.  Menu                                            *\n");
	Lb_printf("*******************************************************\n");
	Lb_printf("\n");
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_GLcdMenu_GetCmd
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_GLcdMenu_GetCmd(void)
{
	u8  key;
	

	Lb_printf(">> ");

    key=get_byte();
	
	Lb_printf("\n");
	
    return key;
}




/*---------------------------------------------------------------------------
     TITLE   : Ap_GLcdMenu_ExeCmd
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_GLcdMenu_ExeCmd(void)
{
	u8 key;
	u8 Status;
	static u8 ExeFirst = TRUE;
	u16 Ret;
	static u8  Count    = 0;	
	static s8  CircleX  = 0;	
	u8 I2C_Data[20];	
	s16 Temperature;	
	s16 x;
    s16 y;

    HW_MPU6050_DATA_OBJ MPU6050_Data;


	if( ExeFirst == TRUE )
	{
		Ap_GLcdMenu_ShowMenu();	
		
		Hw_Timer_Set( HW_TIMER_CH_DEBUG1,  50, LOOP_TIME, Ap_GCldMenu_MoveCircleTick_1st, NULL );
		Hw_Timer_Set( HW_TIMER_CH_DEBUG2, 100, LOOP_TIME, Ap_GCldMenu_MoveCircleTick_2nd, NULL );		
	}

    while( (key = Ap_GLcdMenu_GetCmd()) != 0 )
    {
        switch(key)
        {
           case '1':
           		Lb_printf("LCD Clear \n");
           		Hw_N5110G_Clear();
               	break;

           case '2':
                
                while( get_byte_check() == 0 )
                {
                    Ret = Hw_MPU6050_GetData( &MPU6050_Data );

                    Lb_printf("AX %d ", MPU6050_Data.X_Acc );
                    Lb_printf("AY %d ", MPU6050_Data.Y_Acc );
                    Lb_printf("AZ %d ", MPU6050_Data.Z_Acc );
                    Lb_printf("T %d \n",  MPU6050_Data.Temp);

                    x = HW_N5110G_WIDTH/2 - MPU6050_Data.Y_Acc/200;
                    y = HW_N5110G_HEIGHT/2 - MPU6050_Data.X_Acc/200;

                    if( x > HW_N5110G_WIDTH-4) x = HW_N5110G_WIDTH-4;
                    if( x < 3 )                x = 3;
                    if( y > HW_N5110G_HEIGHT-4) y = HW_N5110G_HEIGHT-4;
                    if( y < 3 )                 y = 3;

                    Hw_N5110G_Clear();                                  
                    Hw_N5110G_DrawCircle(  x , y, 3, 0 );                           
                    Hw_N5110G_DrawRequest();
                    Hw_Wait_ms(100);
                }
           		
				break;

           case '3':						
               break; 

           case '4':
               break;  

           case '5':
               break;  

           case '6':
               break;  

           case '7':
               break;  

           case '8':
           		for( int i=0; i<=0x75; i++ )
           		{
           			if( i%10 == 0 ) Lb_printf("\n %03d: ", i);

           			I2C_Data[0] = 0x00;
           			Ret = Hw_I2C_IMU_MPU6050_ReadReg( i, I2C_Data );
           			Lb_printf("%02x ", I2C_Data[0]);
           		}
               break;

           case '9': 
               break;

           case '0':
               break;

			case 'm':
			case 'M':
				Ap_GLcdMenu_ShowMenu();
				break;
				
           default :
               break;
        }
    }
    
    return key;
}
