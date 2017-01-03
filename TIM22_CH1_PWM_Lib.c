

#include <stm32l0xx.h>  
#include <TIM22_CH1_PWM_Lib.h>  

void TIM22_PWM_EdgeAligned (uint32_t TimePWMPeriod_us, uint32_t TimePWMLow_us) {

	//odblokowanie taktowania portu A
	RCC   -> IOPENR  |=  RCC_IOPENR_GPIOAEN;
	
	//ustawienie Alternate function mode dla portu PA5
	GPIOA -> MODER    = (GPIOA -> MODER & ~(GPIO_MODER_MODE5)) | 
										  (GPIO_MODER_MODE5_1);													
											
	//wybór typu funkcji alternatywnej, czyli TIM2_CH1 (AF5) dla PA5[tabelka 16] 
	GPIOA -> AFR[0]  |= 0x5 << (4 * 5);																
	
	//obslugujemy juz TIM2, PA juz jest wybrane. Wlaczenie licznika TIM2
  RCC   -> APB1ENR |= RCC_APB1ENR_TIM2EN;													
	
	//ustawienie taktowania TIM2 tak, zeby bylo 1MHz ( 2Mhz/2 ) => PSC=prescaler=dzielnik wstepny ustawiamy na wartosc 0, bo CK_CNT = fck_psc / PSC +1
	// na wejscie tego dzielnika otrzymujemy 1MHz(Ckpsc=1M) i to dzielimy przez 1 -> otrzymujemy 1MHz.
  TIM2 -> PSC      = 1 - 1; 																		  	
	
	//ustawienie okresu sygnalu - Period_us
  TIM2 -> ARR      = TimePWMPeriod_us; 														
	
	//ustawienie czasu trwania stanu niskiego
	TIM2 -> CCR1     = TimePWMLow_us; 																
	
	//ustawienie zeby licznik zliczal w gore, odblokowanie wyjscia komparataora CCR1,
	//ustawienie takiego dzialania, ze dopoki TIM2_CNT<TIM2_CCR1 to CH1 jest stanem niskim
  TIM2 -> CCMR1   |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | 
									    TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE; 					
											
	//podlaczenie wyjscia komparatora OC1 do wyjscia pinu PA5 - zebysmy mogli fizycznie zobaczyc efekty tego na diodzie
  TIM2 -> CCER    |= TIM_CCER_CC1E; 																
	
	//uruchomienie licznika TIMx -> w naszym przypadku TIM2
  TIM2 -> CR1     |= TIM_CR1_CEN; 																	
	
	/* DLA DIODY ZIELONEJ:
	
	 programowanie licznika TIM22 w tryb PWM:
		 [1] odblokowanie taktowania portu B 								[RM0367, str. 203]
		 [2] wybor funkcji alternatywnej dla linii PB4 			[RM0367, str. 248]
		 [3] wybor typu funkcji alternatywnej AF4: 					[RM0367, str. 252]
				 PB4/SPI1_MISO/LCD_SEG8/EVENTOUT/TSC_G5_IO2/TIM22_CH1	
																								 [STM32L053x6/x8, str. 46]
		 [4] odblokowanie taktowania licznika 							[RM0367, str. 207]
		 [5] stopien podzialu dzielnika wstepnego licznika	[RM0367, str. 519]
				 TIM22 rowny 1/1; czestotliwosc taktowania 
				 licznika TIM22 rowna 2,097/2 = 1,0485 MHz														
		 [6] ustalenie okresu generowanego sygnalu;					[RM0367, str. 519]
				 TIM22_ARR Auto-Reload Register									
		 [7] ustalenie czasu trwania stanu LOW sygnalu PWM;	[RM0367, str. 520]
				 TIM22_CCR1 Capture/Compare Register							
		 [8] ustalenie trybu 2 dla PWM:											[RM0367, str. 513]
				 - licznik zlicza w gore (upcounting)
				 - aktywne wyjscie komparatora CCR1
				 - kanal CH1 w stanie LOW dopoki TIM22_CNT < TIM22_CCR1
		 [9] wyjscie komparatora OC1 dolaczone do pinu PB4	[RM0367, str. 517]
		[10] start licznika 																[RM0367, str. 504] 
	RCC   -> IOPENR  |=  RCC_IOPENR_GPIOBEN; 													// [1]
	GPIOB -> MODER    = (GPIOB -> MODER & ~(GPIO_MODER_MODE4)) | 
										  (GPIO_MODER_MODE4_1);													// [2]
	GPIOB -> AFR[0]  |= 0x4 << (4 * 4);																// [3]
  RCC   -> APB2ENR |= RCC_APB2ENR_TIM22EN;													// [4]
  TIM22 -> PSC      = 1 - 1; 																		  	// [5]
  TIM22 -> ARR      = TimePWMPeriod_us; 														// [6]
	TIM22 -> CCR1     = TimePWMLow_us; 																// [7]
  TIM22 -> CCMR1   |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | 
									    TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE; 					// [8]	
  TIM22 -> CCER    |= TIM_CCER_CC1E; 																// [9]
  TIM22 -> CR1     |= TIM_CR1_CEN;
	*/
}

void TIM22_PWM_InterruptEnable (void) {

	//odblokowanie przerwan capture/compare 1 => pod CNT counterem, porownuje czas LOW z CNT i z jego pracy wynika przerwanie
	TIM2 -> DIER = TIM_DIER_CC1IE;					
	
	//odblokowanie przerwania dla TIM2
	NVIC_EnableIRQ(TIM2_IRQn);							
	//ustalenie najwyzszego priorytetu przerwania od licznika TIM2 => ustawione 0 w argumencie czyli najwyzszy priorytet
  NVIC_SetPriority(TIM2_IRQn, 0);	
	/* DLA DIODY ZIELONEJ
	
	TIM22 -> DIER = TIM_DIER_CC1IE;			
	NVIC_EnableIRQ(TIM22_IRQn);				
  NVIC_SetPriority(TIM22_IRQn, 0);		
	*/
}

void TIM22_Stop (void) {
//jako ze tylda neguje to jest to odwrotnosc do uruchamiania licznika TIM2 => czyli zeruje bit uruchomienia = zatrzymuje licznik TIM2
	TIM2 -> CR1 &= ~(TIM_CR1_CEN); 
}

void TIM22_PWM_Reload_Start (uint32_t TimePWMLow_us) {
	//modyfikacja czasu trwania LOW - tak aby ten czas byl zmienny
	TIM2 -> CCR1 = TimePWMLow_us;					
	//uruchomienie licznika po zaladowaniu czasu
	TIM2 -> CR1 |= TIM_CR1_CEN; 						
}
