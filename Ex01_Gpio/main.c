/******************************************************************************
 * @file      main.c
 * @author    SmartRobot : e2g1234@naver.com
 * @version   V0.1
 * @date      04/15/2013
 *******************************************************************************/


#include "main.h"









/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
    int key;

    printf("enter any key.......\n");

	//-- 명령어 처리
	//
	Menu_ExeCmd();    

    printf("The End\n");
    while(1);
    return 0;
}

