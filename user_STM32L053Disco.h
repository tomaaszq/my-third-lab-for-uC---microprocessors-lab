
#include <stdint.h>             

#define PortButton					PortA		
#define PortRedLED					PortA	
#define PortGreenLED				PortB		

#define PinButton						0			
#define PinRedLED						5		
#define PinGreenLED					4			

#define PortIN							0			
#define PortOUT							1			

#define	ButtonPressed				1			
#define	ButtonNotPressed		0			

#define LEDOn								1			
#define LEDOff							0		

#define ONGreenLED					1			
#define OFFGreenLED					0			

#define TimePWMPeriod_us		1225		// okres sygnalu PWM w us
#define TimePWMLowMIN_us		1				// minimalny czas trwania stanu LOW w us
#define TimePWMLowMAX_us		1224		// maksymalny czas trwania sygnalu LOW w us
#define TimePWMDelta				1 			// szybkosc zmian sygnalu LOW w us
