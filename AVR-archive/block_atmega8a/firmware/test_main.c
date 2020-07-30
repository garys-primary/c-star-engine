#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#include <avr/io.h> 
#include <util/delay.h>


void initTests();
void pwmTest();
void ledBlink();
   
int main(void){
	DDRD |= (1 << PIND0); //PORTD pin0 as INPUT
	int UBRR_Value = 96;	//9600 baud

	UBRRH = (unsigned char) (UBRR_Value >> 8);
	UBRRL = (unsigned char) UBRR_Value;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << USBS) | (3 << UCSZ0);
	
	unsigned char receiveData;

	while(1){
		while (! (UCSRA & (1 << RXC)) ); 
			receiveData = UDR;
			//pwmTest();
			if (receiveData == 0xBE){
				pwmTest();
        //_delay_ms(220);
			}
	}
	
	//pwmTest();
	return 0;
}

void pwmTest(){
//This will set up and start rising pwm on PB1

  DDRB |= (1 << DDB1);
    // PB1 is now an output
  OCR1A = 0xFFFF;
    // set PWM for 50% duty cycle @ 10bit
  TCCR1A |= (1 << COM1A1);
    // set none-inverting mode
  TCCR1A |= (1 << WGM11) | (1 << WGM10);
    // set 10bit phase corrected PWM Mode
  TCCR1B |= (1 << CS11);
    // set prescaler to 8 and starts PWM

	int i;
  for(i=0; i<1001; i++){
    _delay_ms(0.1);
    if(OCR1A > 100)
      OCR1A--;
    else
			OCR1A = 0x1000;
  }
}

void ledBlink(){

}