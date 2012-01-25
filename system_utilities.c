
//Define the clock speed of the MCU. In this case, 8MHZ
#define F_CPU 8000000UL

//Include the AVR I/O Lib
#include <avr/io.h>
//Include the delay library 
#include <util/delay.h>
//Standard Lib
#include <stdlib.h>

//Define the Baud Rate. This will be used in the USARTinit() function to calculate the baud prescaler
#define baud_rate 9600UL

//Function to configure the ports of the MCU
void PortConfig() {
	
	//Set PORT B to output :)
	DDRB = 255;
	
	
}

//Function to initalize the USART functions 
void USARTinit() {

	//Init ubrr_value variable
	int ubrr_value;

	//Calculate the baud prescaler using the chosen baud rate and the current clock freq
	ubrr_value = (F_CPU / (baud_rate * 16UL) ) - 1;
	
	//Put the prescaler into the UBRR register
	UBRRL = ubrr_value;
	UBRRH = (ubrr_value>>8);

	//Setup Frame: Async, No Parity, 1 stopbit, charsize 8 bits
	UCSRC=(1<<URSEL)|(3<<UCSZ0);

	//Enable The receiver and transmitter
	UCSRB=(1<<RXEN)|(1<<TXEN);


}


//Read char from UART. Will wait till data is available and then return it. 
char RxReadChar() {

   	//wait until some data action is up for grabs
	while(!(UCSRA & (1<<RXC)));
	
	//return data from buffer
	return UDR;
}


//Function to send a char to USART to be Tx'D
void TxChar(char write_char) {
   
	//Transmitter: Stand By For Action
	while(!(UCSRA & (1<<UDRE)));

	//Now write the data to USART
	UDR=write_char;
}


//Function to send an entire string via the UART.
void TxString(char write_str[]) {
	
	int x = 0;
	
	//Loop through the string, sending each char until we hit a null terminator
	while(write_str[x] !=  '\0')
	{
		TxChar(write_str[x]);
		x++;
	}
	
}

//Function to initalize the ADC
void ADCinit() {
	
	//Set prescaler. Prescaler is calculated using the below equation:
	//(F_CPU/prescaler) = SampleRate. We need a sample rate between 50khz and 200khz.
	//Using a prescaler value of 64 we get:
	//(8000000/64) = 125000
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); 
	
	//Set the reference voltage to the AVcc
	ADMUX |= (1 << REFS0); 
		
	//Change to 8-bit resolution. 
	ADMUX |= (1 << ADLAR);
	
	//Start ADC
	ADCSRA |= (1 << ADEN); 
	
	
}

//Return the value in the ADCH register :)
int GetADC(int ch) {
		
	//Perform AND to ensure the bottom bits are the only ones set
	ch=ch&0b00000111;
	
	//Clear bottom 3 bits of the reg
	ADMUX &= 0b11111000;
	
	//Add the #chan selection, innit :)
	ADMUX|=ch;
	
	//Start measurment
	ADCSRA|=(1<<ADSC);

	//Wait for measurment to complete
	 while(!(ADCSRA & (1<<ADIF)));
	
	//Clear bit
	 ADCSRA|=(1<<ADIF);

	//Return 8-bit result :)
	return(ADCH);
	
}

//Function to initalize the PWM functionality.
void ServoPWMinit() {
	
	//turn off PWM
	TCCR1A = 0;
	
	
	//Set frequency:  FreqTOP = (F_CPU / (Prescaler * PWM_Freq)) - 1
	//(8000000 / (8 * 50)) - 1 = 19999 (50Hz == 20ms)
	ICR1 = 19999;
	
	//Config Timer
	//Fast PWM, 8 Prescaler
	TCCR1A = (1<<WGM11);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11);
	
	//Set PWM Ports as outputs
	DDRB |= (1<<1); //OCR1A
	DDRB |= (1<<2); //OCR1B
	
		
}

//Activate the pwm channels
void StartServoPWM() {
	
	//Activate Channels
	TCCR1A |= (2<<6);
	TCCR1A |= (2<<4);
}

//Function to change the pulse width of the servo pulses
void ServoPWMPulseWidth(int channel, float pulsewidth) {
	
	//If channel one is selected
	if (channel == 1) {
		//Change the pulsewidth
		OCR1A = ICR1 * pulsewidth / 20;
	} 

	//If channel two is selected
	if (channel == 2) {
		//Change the pulsewidth
		OCR1B = ICR1 * pulsewidth / 20;
	}
	
}




