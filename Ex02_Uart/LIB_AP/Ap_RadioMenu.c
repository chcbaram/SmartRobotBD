//----------------------------------------------------------------------------
//    프로그램명 	: Radio Menu 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Ap_UartMenu.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  AP_RADIOMENU_LOCAL

#include "Ap_RadioMenu.h"



//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//
void Ap_RadioMenu_ShowMenu(void);






/*---------------------------------------------------------------------------
     TITLE   : Ap_RadioMenu_ShowMenu
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Ap_RadioMenu_ShowMenu(void)
{
	u8 key;

	printf("\n\n");
	printf("*******************************************************\n");
	printf("                      Radio V0.1                       \n");
	printf("*******************************************************\n");
	printf("* 1.  Band Select                                     *\n");
	printf("* 2.  CH ++                                           *\n");
	printf("* 3.  CH --                                           *\n");
	printf("* 4.  Auto Search +                                   *\n");
	printf("* 5.  Auto Search -                                   *\n");
	printf("* 6.  Volume +                                        *\n");
	printf("* 7.  Volume -                                        *\n");
	printf("* 8.                                                  *\n");
	printf("* 9.                                                  *\n");
	printf("* m.  Menu                                            *\n");
	printf("*******************************************************\n");
	printf("\n");
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_RadioMenu_GetCmd
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_RadioMenu_GetCmd(void)
{
	u8 key;

	printf("select the command number : ");

    key=get_byte();
	
	printf("\n");
	
    return key;
}




/*---------------------------------------------------------------------------
     TITLE   : Ap_RadioMenu_ShowMenu
     WORK    : 
     ARG     : void
     RET     : 
				u8 : 눌려진 메뉴키 데이터 
---------------------------------------------------------------------------*/
u8 Ap_RadioMenu_ExeCmd(void)
{
	u8 key;
	static u8 ExeFirst = TRUE;
	
	if( ExeFirst == TRUE )
	{
		Ap_RadioMenu_ShowMenu();	
	}

    while( (key = Ap_RadioMenu_GetCmd()) != 0 )
    {
        switch(key)
        {
           case '1':
               printf("No.1\n");           
               break;

           case '2':
               printf("No.2\n");           
               break;

           case '3':
               printf("No.3\n");           
               break; 

           case '4':
               printf("No.4\n");           
               break;  

           case '5':
               printf("No.5\n");
               break;  

           case '6':
               printf("No.6\n");
               break;  

           case '7':
               printf("No.7\n");
               break;  

           case '8':
               printf("No.8\n");
               break;

           case '9':
               printf("No.9\n");
               break;

           case '0':
               printf("No.0\n");
               break;

			case 'm':
			case 'M':
				Ap_RadioMenu_ShowMenu();
				break;
				
           default :
               break;
        }
    }
    
    return key;
}
