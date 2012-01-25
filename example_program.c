#include "system_utilities.c"

//Main program
int main(void) {
	
	//Variable Declerations
	//Int var for holding the data read from the UART buffer
	unsigned int adc_value;
	//Char array to hold adc value for usarting
	char adc_array[10];
	
	
	//Initialize the USART
	USARTinit();
	//Send successfully initalized message: USART
	TxString("USART Successfully Initalized...\r\n\0");

	//Initalize the ADC functionality.
	ADCinit();
	//Send successfully initalized message: ADC
	TxString("ADC Successfully Initalized...\r\n\0");
	
	//Initalize the PWM (Configured for Servos (50Hz, Prescaler 8, F_CPU=8000000
	ServoPWMinit();
	//Send successfully initalized message: PWM
	TxString("PWM Successfully Initalized...\r\n\0");
	
	//Activate the PWM Channels
	StartServoPWM();
	//Send successfully initalized message: PWM
	TxString("PWM Channels Successfully Activated...\r\n\0");
	
	//main Program Loop
	while(1) {
		
			
		/* Example Proggy:
		Read ADC, and transmit via USART, centre servos
		*/
		
		//Read ADC (chan 5)
		adc_value = GetADC(5);

		//Convert ADV Value int to char array
		utoa((unsigned)adc_value, adc_array, 10);
			
		//Echio data back our the Tx line 
		TxString(adc_array);
		TxString("\n\r\0");
		
		//Delay for a little bit
		_delay_ms(1000);
		
		//Center Servos by change pulse width of PWM to a 1.5ms pulse.
		ServoPWMPulseWidth(1, 1.5); //Channel 1 (OCR1A)
		ServoPWMPulseWidth(2, 1.5); //Channel 2 (OCR1B)

	
		/*End of Example*/
		
	}
	
	return 1;

}
