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
	Lb_printf("                      GLcd V0.2                       \n");
	Lb_printf("*******************************************************\n");
	Lb_printf("* 1. LCD Clear                                        *\n");
	Lb_printf("* 2. Count ++                                         *\n");
	Lb_printf("* 3. Count --                                         *\n");
	Lb_printf("* 4. Move Circle Start                                *\n");
	Lb_printf("* 5. Move Circle End                                  *\n");
	Lb_printf("* 6. I2C HW TX                                        *\n");
	Lb_printf("* 7. I2C HW RX                                        *\n");
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
           		Lb_printf("Count ++ \n");
                     		
				Hw_N5110G_DrawPrint(0,0,"Smartrobot BD");								
				Hw_N5110G_DrawPrint(0,2,"Count : %4d", ++Count);						
           		Hw_N5110G_DrawRequest();
           		
				break;

           case '3':
           		Lb_printf("Count -- \n");
              
				Hw_N5110G_DrawPrint(0,0,"Smartrobot BD");								
				Hw_N5110G_DrawPrint(0,2,"Count : %4d", --Count);						
				Hw_N5110G_DrawRequest();
						
               break; 

           case '4':
				Hw_Timer_Start( HW_TIMER_CH_DEBUG1 );
				Hw_Timer_Start( HW_TIMER_CH_DEBUG2 );
               break;  

           case '5':
				Hw_Timer_Stop( HW_TIMER_CH_DEBUG1 );
				Hw_Timer_Stop( HW_TIMER_CH_DEBUG2 );
               break;  

           case '6':
           		I2C_Data[0] = 0x00;
           		Ret = Hw_I2C_IMU_MPU6050_WriteReg( 0x6B, 0x00 );
           		Lb_printf("I2C Write Ret : %d\n", Ret);

           		for( int i=0; i<=0x75; i++ )
           		{
           			//if( i%10 == 0 ) Lb_printf("\n %03d: ", i);

           			I2C_Data[0] = 0x00;
           			Ret = Hw_I2C_IMU_MPU6050_WriteReg( i, 0x00 );
           			//Lb_printf("%02x ", I2C_Data[0] );
           		}

               break;  

           case '7':
           		I2C_Data[0] = 0x00;
           		Ret = Hw_I2C_IMU_MPU6050_ReadReg( 0x75, I2C_Data );
           		Lb_printf("I2C Read Ret : %d 0x%x\n", Ret, I2C_Data[0] );

           		I2C_Data[0] = 0x00;
           		Ret = Hw_I2C_IMU_MPU6050_ReadReg( 0x1B, I2C_Data );
           		Lb_printf("I2C Read Ret : %d 0x%x\n", Ret, I2C_Data[0] );


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

           		I2C_Data[0] = 0x00;
           		Ret = Hw_I2C_IMU_MPU6050_ReadRegs( 0x3B, I2C_Data, 14 );
           		Lb_printf("AX %d ", (I2C_Data[0] << 8) | (I2C_Data[1] << 0) );
           		Lb_printf("AY %d ", (I2C_Data[2] << 8) | (I2C_Data[3] << 0) );
           		Lb_printf("AZ %d ", (I2C_Data[4] << 8) | (I2C_Data[5] << 0) );

           		Temperature = (s16)((I2C_Data[6] << 8) | (I2C_Data[7] << 0));
           		Temperature = Temperature/340 + 36;
           		Lb_printf("T %d ",  Temperature );

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
