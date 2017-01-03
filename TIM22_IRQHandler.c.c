/**
 * @brief    obsluga przerwania od licznika TIM2 w trybie PWM
 */


#include <stm32l0xx.h> 
#include <TIM22_CH1_PWM_Lib.h>		
#include <user_STM32L053Disco.h>

volatile uint32_t pwm_time;					// chwilowa wartosc stanu LOW licznika
void TIM22_IRQHandler (void) {

	//zatrzymuje licznik TIM2
	TIM22_Stop();																		
	//skasowanie zacznika przerwania(CC1IF) ktore jest ustawiane w przerwaniu
	TIM2 -> SR &= ~(TIM_SR_CC1IF); 							
	if (pwm_time >= TimePWMLowMIN_us) {					
			pwm_time = pwm_time - TimePWMDelta;
	} else {
			pwm_time = TimePWMLowMAX_us;
	}
	TIM22_PWM_Reload_Start(pwm_time);							
}	
