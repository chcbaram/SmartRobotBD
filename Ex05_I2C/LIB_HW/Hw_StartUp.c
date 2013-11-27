/******************************************************************************
 * @file      start_stm32f10x.c
 * @author    SmartRobot : e2g1234@naver.com
 * @version   V0.1
 * @date      04/15/2013
 *******************************************************************************/

//----- ������� ����
//
#define  HW_STARTUP_LOCAL

#include "Hw_StartUp.h"





//-- Private typedef
//
typedef void( *const intfunc )( void );


//-- init value for the stack pointer. defined in linker script 
//
extern unsigned long _estack;
extern unsigned long _sidata;    /*!< Start address for the initialization 
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */    
extern unsigned long _edata;     /*!< End address for the .data section       */    
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */      
extern void 	     _eram;      /*!< End address for ram                     */


//-- Private function prototypes
//
void Hw_StartUp_Init(void) __attribute__((__interrupt__));





__attribute__ ((section(".isr_vectorsflash")))    //linker scriptor���� ����
//-- vector table
//
void (* g_pfnVectors[])(void) =
{
    (intfunc)((unsigned long)&_estack),		// The stack pointer after relocation
    Hw_StartUp_Init,                      	// Reset Handler
};



extern int main();


/*---------------------------------------------------------------------------
     TITLE   : 	Hw_StartUp_Init
     WORK    :	 
     			���� ���½ÿ� ó�� ����Ǵ� �Լ�
     ARG     : 	void
     RET     : 	void
---------------------------------------------------------------------------*/
void Hw_StartUp_Init(void)
{
	
	u32 TimeOutCnt  = 0;
	u32 HSEStatus 	= 0;
	unsigned long *pulSrc, *pulDest;	
	
	
	
	//-- Initialize data and bss
	//  
	
	// Copy the data segment initializers from flash to SRAM
	pulSrc = &_sidata;

	for(pulDest = &_sdata; pulDest < &_edata; )
	{
		*(pulDest++) = *(pulSrc++);
	}
  
	/* Zero fill the bss segment.  This is done with inline assembly since this
	   will clear the value of pulDest if it is not kept in a register. */
	__asm("  ldr     r0, =_sbss\n"
          "  ldr     r1, =_ebss\n"
          "  mov     r2, #0\n"
          "  .thumb_func\n"
          "zero_loop:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop");
        	
	
	
    // HSI or HSE ����
            
    //-- CR_HSEON_Set
    //
	REG_RCC_CR |= (1<<16);	
		
	//-- ������ clock �� ���� �Ǿ����� Ȯ��
	//
	do
	{	
		HSEStatus = REG_RCC_CR & (1<<17);	// HSE �ΰ��
		TimeOutCnt++;		
	} while( (HSEStatus == 0) && (TimeOutCnt < 1280) );
  

	//-- FLASH Access Ÿ�̹� ����
	//
	REG_FLASH_ACR |=  (1<<4);	// Prefetch buffer enable
	REG_FLASH_ACR &= ~(3<<0);	// LATENCY Clear
	REG_FLASH_ACR |=  (2<<0);	// LATENCY = two wait


    main();

}
