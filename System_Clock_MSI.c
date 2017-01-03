/**
 * @file     		System_Clock_MSI.c 
 * @Author   		A. Stepien
 * @version  		2.1
 * @date     		24-10-2016
 * @brief    		programowanie taktowania procesora STM32L053
 * @literature	[1] RM0367 Reference manual. STMicroelectronics. 
 *										 		 February 2016, Rev.4
 *							[2] STM32L053Cx/Rx. Ultra-low-power 32-bit MCU ARM®-based
 *										 		 Cortex®-M0+, up to 64KB Flash, 8KB SRAM, 
 *												 2KB EEPROM, LCD, USB, ADC, DAC.
 *										 		 Datasheet. STMicroelectronics. October 2017, Rev.7
*/

#include "stm32l0xx.h"							// plik naglowkowy typu procesora

/**
 * @name				SystemClock_Config_MSI
 * @param [in]	brak
 * @retval			brak
 * @brief				programowanie taktowania procesora:
 *							- taktowanie wewnetrznym generatorem MSI o czestotliwosci 2,097 MHz
 *							- czestotliwosc taktowania rdzenia SYSCLK = 2,097 MHz
 *							- czestotliwosc taktowania SysTick Timera rowna 2.097 MHz
 *							- czestotliwosc taktowania ukladow peryferyjnych PCLK1 = PCLK2 = 2,097/4 MHz
 *							- czestotliwosc taktowania licznikow APB1Timer = APB2Timer = 2,097/2 MHz
 */

void SystemClock_Config_MSI(void) {
	
	/* generator MSI jest wlaczony domyslnie (RESET); programowanie czestotliwosci
	taktowania: rdzenia procesora, ukladow peryferyjnych i licznikow
		 [1] odblokowanie taktowania ukladu sterowania zasilaniem 						[RM0367, str. 209]
		 [2] wybor zakresu zasilania 1,8 V (Range 1)													[RM0367, str. 166]
		 [3] generator MSI jest wlaczony domyslnie po RESET;									[RM0367, str. 186]
				 nie ma potrzeby czekania na ustabilizowanie drgan tego generatora
		 [4] wybor zrodla taktowania rdzenia procesora (MSI)									[RM0367, str. 191]
		 [5] brak dzielnika magistrali AHB, podzial 1:1; 											[RM0367, str. 191]
				 wartosc domyslna po RESET
		 [6] wlaczenie dzielnika 1:4 magistrali APB1 dla 											[RM0367, str. 191]
				 ukladow peryferyjnych sterowanych magistrala APB1 (sygnal PCLK1);
				 licznik sterowane magistala APB1 maja automatycznie wlaczony 
				 powielacz czestotliwosci 2x
		 [7] wlaczenie dzielnika 1:4 magistrali APB2 dla 											[RM0367, str. 191]
				 ukladow peryferyjnych sterowanych magistrala APB2 (sygnal PCLK2);
				 licznik sterowane magistala APB2 maja automatycznie wlaczony 
				 powielacz czestotliwosci 2x																											*/
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; 													// [1]
	PWR->CR 			= (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0; 	// [2]
  RCC->CR			 |= RCC_CR_MSION;																// [3] 
	RCC->CFGR		 |= RCC_CFGR_SW_MSI;														// [4] 
	RCC->CFGR		 |= RCC_CFGR_HPRE_DIV1;													// [5]
	RCC->CFGR 	 |= RCC_CFGR_PPRE1_DIV4;												// [6]
	RCC->CFGR 	 |= RCC_CFGR_PPRE2_DIV4;												// [7]
}

/**
 * @name				SystemCoreClockUpdate_MSI
 * @param [in]	brak
 * @retval			brak
 * @brief				aktualizacja wartosci zmiennej SystemCoreClock przy
 *							taktowaniu procesora wewnetrznym generatorem MSI o czestotliwosci 2,097 MHz
 *						
 *							podprogram jest fragmentem procedury void SystemCoreClockUpdate_MSI(void)
 *							autorstwa MCD Application Team, V1.2.0, 06-February-2015 
 * 							(CMSIS Cortex-M0+ Device Peripheral Access Layer System Source File)
 */

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
  #define MSI_VALUE    ((uint32_t)2000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */
   
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

void SystemCoreClockUpdate_MSI (void)  {
  uint32_t tmp = 0, msirange = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp) {
    case 0x00:  /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
    case 0x04:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x08:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
/*    case 0x0C:  // PLL used as system clock 
      // Get PLL clock source and multiplication factor --------------
      pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
      plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
      pllmul = PLLMulTable[(pllmul >> 18)];
      plldiv = (plldiv >> 22) + 1;
      
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      if (pllsource == 0x00)
      {
        // HSI oscillator clock selected as PLL clock entry 
        SystemCoreClock = (((HSI_VALUE) * pllmul) / plldiv);
      }
      else
      {
        // HSE selected as PLL clock entry 
        SystemCoreClock = (((HSE_VALUE) * pllmul) / plldiv);
      }
      break;
*/
    default: /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
  }
}
