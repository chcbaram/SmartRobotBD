//----------------------------------------------------------------------------
//    프로그램명 	: Menu 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Lib_Menu.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  LIB_MENU_LOCAL

#include "Lib_Menu.h"



//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//
int main_menu(void);





int main_menu(void)
{
    int key;

    printf("\n\n");
    printf("-------------------------------------------------\n");
    printf("                    MAIN MENU                    \n");
    printf("-------------------------------------------------\n");
    printf(" 0. Smart Robot B'D DIO 00                       \n");
    printf(" 1. Smart Robot B'D DIO 01                       \n");
    printf(" 2. Smart Robot B'D DIO 02                       \n");
    printf(" 3. Smart Robot B'D DIO 03                       \n");
    printf(" 4. Smart Robot B'D DIO 04                       \n");
    printf(" 5. Smart Robot B'D DIO 05                       \n");
    printf(" 6. Smart Robot B'D DIO 06                       \n");
    printf(" 7. Smart Robot B'D DIO 07                       \n");
    printf(" 8. Smart Robot B'D DIO 08                       \n");
    printf(" 9. Smart Robot B'D DIO 09                       \n");

    printf("-------------------------------------------------\n");
    printf(" q. Smart Robot B'D FirmwareQUIT                 \n");
    printf("-------------------------------------------------\n");
    printf("\n\n");
 
    printf("SELECT THE COMMAND NUMBER : ");

    key=get_byte();
//    key=getchar();
//    if (key == '\n') 
//        ;
//    else
//        getchar();

    return key;
}



int Menu_ExeCmd(void)
{
	int key;

    while((key=main_menu()) != 0)
    {
        switch(key)
        {
           case '1':
               printf("No.1\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 0;            // On   Set
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 0;            // Off   reset
               break;

           case '2':
               printf("No.2\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 1;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 1;            // Off
               break;

           case '3':
               printf("No.3\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 2;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 2;            // Off
               break; 

           case '4':
               printf("No.4\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 3;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 3;            // Off
               break;  

           case '5':
               printf("No.5\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 4;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 4;            // Off
               break;  

           case '6':
               printf("No.6\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 5;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 5;            // Off
               break;  

           case '7':
               printf("No.7\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 6;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 6;            // Off
               break;  

           case '8':
               printf("No.8\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 7;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 7;            // Off
               break;

           case '9':
               printf("No.9\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 8;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 8;            // Off
               break;

           case '0':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 13;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 13;            // Off
               break;






           case 'a':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 14;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 14;            // Off
               break;

           case 'b':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010810 |= 0x01 << 15;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010814 |= 0x01 << 15;            // Off
               break;

           case 'c':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 0;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 0;            // Off
               break;

           case 'd':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 1;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 1;            // Off
               break;

           case 'e':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 2;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 2;            // Off
               break;

           case 'f':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 3;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 3;            // Off
               break;

           case 'g':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 4;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 4;            // Off
               break;

           case 'h':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 5;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 5;            // Off
               break;

           case 'i':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 6;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 6;            // Off
               break;

           case 'j':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 7;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 7;            // Off
               break;

           case 'k':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 8;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 8;            // Off
               break;

           case 'l':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 9;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 9;            // Off
               break;

           case 'm':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 10;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 10;            // Off
               break;

           case 'n':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 11;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 12;            // Off
               break;

           case 'o':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 13;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 13;            // Off
               break;

           case 'p':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 14;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 14;            // Off
               break;

           case 'r':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40010C10 |= 0x01 << 15;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40010C14 |= 0x01 << 15;            // Off
               break;

           case 's':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40011010 |= 0x01 << 14;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40011014 |= 0x01 << 14;            // Off
               break;

           case 't':
               printf("No.0\n");
               *(volatile unsigned long *) 0x40011010 |= 0x01 << 15;            // On
               delay_second();               
               *(volatile unsigned long *) 0x40011014 |= 0x01 << 15;            // Off
               break;

//////////////////////////////////////////////




           case 'q':
               printf("exit\n");
               //serialClose(fd);
               //exit(0);
               break;

           default :
               //printf("Wrong key ..... try again\n");
               break;
        }
    }
    
    return key;
}
