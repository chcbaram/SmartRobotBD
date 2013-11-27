/******************************************************************************
 * @file      start_stm32f10x.c
 * @author    SmartRobot : e2g1234@naver.com
 * @version   V0.1
 * @date      04/15/2013
 *******************************************************************************/


/* Private typedef -----------------------------------------------------------*/
typedef void( *const intfunc )( void );

/* init value for the stack pointer. defined in linker script */
extern unsigned long _estack;

/* Private function prototypes -----------------------------------------------*/
void Reset_Handler(void) __attribute__((__interrupt__));

__attribute__ ((section(".isr_vectorsflash")))    //linker scriptor���� ����
/* vector table*/
void (* g_pfnVectors[])(void) =
{
    (intfunc)((unsigned long)&_estack), /* The stack pointer after relocation */
    Reset_Handler,                      /* Reset Handler */
};



void Reset_Handler(void)
{
    unsigned long StartUpCounter = 0, HSEStatus = 0;
    // HSI or HSE ����
    // PLLSRC
    // 8MHz/2 x 18 = 72MHz
    // 12MHz x 6 = 72MHz
    
    
    ////HSI(0x1) or HSE ON(0x1<<16)
    //*(volatile unsigned long *) 0x40021000 |= 0x1;            //RCC_CR - HSION    Set HSION bit
    *(volatile unsigned long *) 0x40021000 |= 0x1<<16;        //RCC_CR  CR_HSEON_Set

    // ������ clock �� ���� �Ǿ����� Ȯ��
    do
    {
       //HSEStatus = (*(volatile unsigned long *) 0x40021000 & 0x1<<1);             // HSI �ΰ��
       HSEStatus = (*(volatile unsigned long *) 0x40021000 & 0x1<<17);             // HSE �ΰ��
       StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != 0x0500));
  
	#if 1
	*(volatile unsigned long *) 0x40022000 |= 0x10;      //FLASH->ACR &= ACR_LATENCY_Mask;
	*(volatile unsigned long *) 0x40022000 &= ~0x3;
	*(volatile unsigned long *) 0x40022000 |= 0x2;
	#endif

    /////// PLL ���� ���� �� ��� 36MHz �ܺ� �� ��� 72Mhz�� ������
    *(volatile unsigned long *) 0x40021004 &= ~(0xF<<18 | 0x1<<17 | 0x1<<16);                  //0x3F0000;      //RCC_CFGR
    //*(volatile unsigned long *) 0x40021004 |= (0x7<<18);              //RCC_CFGR  //���� 4MHz
    *(volatile unsigned long *) 0x40021004 |= (0x7<<18 | 0x1<<16);              //0x1D0000;       //RCC_CFGR  //8MHz
    //*(volatile unsigned long *) 0x40021004 |=  (0x4<<18 | 0x1<<16);              //0x110000;       //RCC_CFGR    //12MHz
    *(volatile unsigned long *) 0x40021000 |= 0x01 << 24/*0x1000000*/;                       //PLLON
    while( ((*(volatile unsigned long *) 0x40021000) & 0x01 << 25/*0x2000000*/) == 0);       //PLLRDY
    
    /////PLL�� �����Ѵ�.
    /////Bits 1:0 SW : System clock switch
    //00: HSI selected as system clock
    //01: HSE selected as system clock
    //10: PLL selected as system clock   <----------------- ���õ�
    //11: not allowed
    *(volatile unsigned long *) 0x40021004 &= ~0x3;
    *(volatile unsigned long *) 0x40021004 |= 0x2;
    while( ((*(volatile unsigned long *) 0x40021004) & 0xC) != 0x08);
    
    //APB2�� �ִ� UART �� �ֺ���ġ�� Clock �ҽ� ��������
    //UART�� ���۵Ǳ� ���ؼ��� GPIO�� Alternation Function IO�� ���ۻ��¿� �־�� ��
    *(volatile unsigned long *) 0x40021018 |= 0x1 << 14 | 0x1 << 4  | 0x1 << 3  | 0x1 << 2 | 0x1 << 0;         // UART/ IOPC EN / IOPB EN / IOPA EN / AFIO EN
   
    	/* Configure the GPIO ports */
	*(volatile unsigned long *) 0x40010800 = 0x33333333;            // PA0 - 7
	*(volatile unsigned long *) 0x40010804 = 0x333334B3;            // PA8 - 15    //PA9,10,11,12�� ���� UART, USB�� �Ҵ��
	*(volatile unsigned long *) 0x40010C00 = 0x33333333;            // PB0 - 7
	*(volatile unsigned long *) 0x40010C04 = 0x33333333;            // PB8 - 15
	*(volatile unsigned long *) 0x40011004 = 0x33444444;            // PC14, 15
    
    	/* USART1 configuration ------------------------------------------------------*/
    	/* USART1 configured as follow:
    	 - BaudRate = 19200 baud
    	 - Word Length = 8 Bits
    	 - One Stop Bit
    	 - No parity
    	 - Hardware flow control disabled (RTS and CTS signals)
    	 - Receive and transmit enabled
    	*/
    *(volatile unsigned long *) 0x40013810 	 = 0x0;           // 1 stop bit
    *(volatile unsigned long *) 0x4001380C 	 = 0x200C;        // 8bit no parity
    *(volatile unsigned long *) 0x40013814 	 = 0x0;
    *(volatile unsigned long *) 0x40013808 	 = 39 << 4 | 1;   // �ܺ� OSC 72Mhz�� ��� 115200bps�� ��� �ӵ� ����
    *(volatile unsigned long *) 0x4001380C 	|= 0x2000;

    main();
}

