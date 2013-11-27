//----------------------------------------------------------------------------
//    프로그램명 	: 하드웨어 레지스터 Define
//
//    만든이     	: 
//
//    날  짜     	:
//
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Regs.h
//----------------------------------------------------------------------------


#ifndef _HW_REGS_H_
#define _HW_REGS_H_



//-- RCC (reset and clock control )
//
#define REG_RCC_BASE					0x40021000
#define REG_RCC_CR						(*(volatile unsigned long *)(REG_RCC_BASE + 0x0000))
#define REG_RCC_CFGR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0004))
#define REG_RCC_CIR						(*(volatile unsigned long *)(REG_RCC_BASE + 0x0008))
#define REG_RCC_APB2RSTR				(*(volatile unsigned long *)(REG_RCC_BASE + 0x000C))
#define REG_RCC_APB2ENR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0018))



//-- FLASH Memory
//
#define REG_FLASH_BASE					0x40022000
#define REG_FLASH_ACR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0000))
#define REG_FLASH_KEYR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0004))
#define REG_FLASH_OPTKEYR				(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0008))
#define REG_FLASH_SR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x000C))
#define REG_FLASH_CR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0010))
#define REG_FLASH_AR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0014))
#define REG_FLASH_OBR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x001C))
#define REG_FLASH_WRPR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0020))



//-- USART
//
#define REG_USART1_BASE					0x40013800
#define REG_USART1_SR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0000))
#define REG_USART1_DR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0004))
#define REG_USART1_BRR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0008))
#define REG_USART1_CR1					(*(volatile unsigned long *)(REG_USART1_BASE + 0x000C))
#define REG_USART1_CR2					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0010))
#define REG_USART1_CR3					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0014))
#define REG_USART1_GTPR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0018))



//-- AFIO
//
#define REG_AFIO_BASE					0x40010000
#define REG_AFIO_EVCR					(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0000))
#define REG_AFIO_MAPR					(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0004))
#define REG_AFIO_EXTICR1				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0008))
#define REG_AFIO_EXTICR2				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x000C))
#define REG_AFIO_EXTICR3				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0010))
#define REG_AFIO_EXTICR4				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0014))
#define REG_AFIO_MAPR2					(*(volatile unsigned long *)(REG_AFIO_BASE + 0x001C))



//-- GPIO
//
#define REG_GPIOA_BASE					0x40010800
#define REG_GPIOA_CRL					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0000))
#define REG_GPIOA_CRH					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0004))
#define REG_GPIOA_IDR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0008))
#define REG_GPIOA_ODR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x000C))
#define REG_GPIOA_BSRR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0010))
#define REG_GPIOA_BRR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0014))
#define REG_GPIOA_LCKR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0018))


#define REG_GPIOB_BASE					0x40010C00
#define REG_GPIOB_CRL					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0000))
#define REG_GPIOB_CRH					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0004))
#define REG_GPIOB_IDR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0008))
#define REG_GPIOB_ODR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x000C))
#define REG_GPIOB_BSRR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0010))
#define REG_GPIOB_BRR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0014))
#define REG_GPIOB_LCKR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0018))


#define REG_GPIOC_BASE					0x40011000
#define REG_GPIOC_CRL					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0000))
#define REG_GPIOC_CRH					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0004))
#define REG_GPIOC_IDR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0008))
#define REG_GPIOC_ODR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x000C))
#define REG_GPIOC_BSRR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0010))
#define REG_GPIOC_BRR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0014))
#define REG_GPIOC_LCKR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0018))


#endif
