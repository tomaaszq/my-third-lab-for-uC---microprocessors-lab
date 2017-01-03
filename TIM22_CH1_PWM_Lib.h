/**
* @file				user_STM32L053Disco.h
* @Author			A. Stepien
* @version  	01.10 
* @date				23-10-2016
* @brief			General-purpose timers (TIM22) interface header file 
*							for STM32L053-Discovery evaluation board
*	@literature	UM1775. User manual. Discovery kit for STM32L0 series 
*											with STM32L053C8 MCU.	STMicroelectronics, 
*											DocID026429 Rev 2, June 2014
*/

#include <stdint.h>					// Standard data type definition header
#include <stm32l053xx.h>		// Device header

extern void TIM22_PWM_EdgeAligned(uint32_t TimePWMPeriod_us, uint32_t TimePWMLow_us);
extern void TIM22_PWM_InterruptEnable (void);
extern void TIM22_Stop (void);
extern void TIM22_PWM_Reload_Start (uint32_t TimePWMLow_us) ;
