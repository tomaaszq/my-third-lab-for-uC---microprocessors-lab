/**
 * @file     TIM22_CH1_PWM_main.c 
 * @Author   A. Stepien
 * @version  2.1
 * @date     23-10-2016
 * @brief    modulacja jasnosci swiecenia zielonej diody LED (LD)3 / modul STM32L053DISCO
*/

/* pliki naglowkowe */
#include "stm32l0xx.h"							// typ procesora
#include <System_Clock.h>						// zasada taktowania procesora
#include <TIM22_CH1_PWM_Lib.h>			// biblioteki dla licznika TIM22_CH1 w trybie PWM	
#include <user_STM32L053Disco.h>		// parametry uzytkownika

int main(void) {
	//ustalenie czestotliwosci pracy zegara(Fcore=2,097) -> wejscie na dzielenie 2MHz/2
  SystemClock_Config_MSI();																			
	//aktualna wartosc zmiennej SystemCoreClock
	SystemCoreClockUpdate_MSI();																	
	
	//ustawienie diody na czerwona
	TIM22_PWM_EdgeAligned(TimePWMPeriod_us, TimePWMLowMIN_us);			
	//odblokowanie przerwan TIM2
	TIM22_PWM_InterruptEnable();																		
	
  while (1); 
}
