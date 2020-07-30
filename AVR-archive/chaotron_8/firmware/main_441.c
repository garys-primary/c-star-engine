/*
*   Encoder-controlled 2-led PWM color selector 
*
*		ref. https://gcc.gnu.org/onlinedocs/cpp
*		Author garys
*/
#define	F_CPU		16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <libc/math.h>

//Function-like Macros
#define sbi(port, pin) (port |= (1<<pin))
#define cbi(port, pin) (port &= ~(1<<pin))

#define isPressedA(bit) (PINA & (1<<bit))
#define isPressedB(bit) (PINB & (1<<bit))
#define isHigh(reg, bit) (reg & (1<<bit))

//Port definitions
#define outPort PORTA
#define outDDR	DDRA
#define outPin	PINA
#define outG1 PA1
#define outR1 PA2
#define outB1 PA3
#define outG2 PA4
#define outR2 PA5
#define outB2 PA6

#define inPort PORTB
#define inDDR PORTB
#define inPin  PINB
#define inEnc1l PB0
#define inEnc1r PB1
#define inEnc2l PB2
#define inEnc2r PA7
#define inEncBtn PA0

//define bools
//TODO: optimization check
typedef struct { 
 uint8_t f0:1; 
 uint8_t f1:1; 
 uint8_t f2:1; 
 uint8_t f3:1; 
 uint8_t f4:1; 
 uint8_t f5:1; 
 uint8_t f6:1; 
 uint8_t f7:1; 
} Bits;

#define enc1l ( (volatile Bits*)(&GPIOR0) )->f7
#define enc1r ( (volatile Bits*)(&GPIOR0) )->f6
#define enc1prog ( (volatile Bits*)(&GPIOR0) )->f5
#define bank ( (volatile Bits*)(&GPIOR0) )->f4

#define enc2l ( (volatile Bits*)(&GPIOR0) )->f3
#define enc2r ( (volatile Bits*)(&GPIOR0) )->f2
#define enc2prog ( (volatile Bits*)(&GPIOR0) )->f1
#define mode ( (volatile Bits*)(&GPIOR0) )->f0

#define lPressed ( (volatile Bits*)(&GPIOR1) )->f7
#define rPressed ( (volatile Bits*)(&GPIOR1) )->f6



void initIO(){
	//DDR  in=0 out=1
	//PORT in=1 out=0
	
	//reg. bit numbering: 76543210
	
	DDRB  = 0b00000111;
	PORTB = 0b00000000;
	//inDDR = 0x00;
	//inPort = 0xFF; //pullup
	
	DDRA  = 0b11000000;
	PORTA = 0b00111111;
	//outDDR = 0xFF;
	//outPort = 0x00;	//set pins low
}

void initInt(){	
	PCMSK0 = 0b00000001;
	GIMSK |= (1<<PCIE0);
	sei();
}

//bank select interrupt
/*ISR(PCINT1_vect){
	sbi(outPort, outR1);
	_delay_ms(100);
	cbi(outPort, outR1);

}*/

//mode/bank select interrupt
ISR(PCINT0_vect){
	cli();
	sei();
}

void initADC(){
	//PRR |= 0<<PRADC;//remove Power Reduction Bit 
	PRR = PRR &= ~(_BV(PRADC));//remove Power Reduction Bit 
	
	//ADCSRA = ADCSRA |= ( 7 <<ADPS2 );//slow prescaler
	
	ADCSRA |= 1<<ADEN; //enable ADC
	//select ADC0 as input
	ADMUXA |= 0<<MUX0 | 0<<MUX1 | 0<<MUX2 | 0<<MUX3 | 0<<MUX4 | 0<<MUX5;
	ADCSRB |= 1<<ADLAR; //left adjusted result for 8bit precision (ADCH)
	ADCSRA |= 0<<ADATE; //no automatic triggering
}

		
		//ULN remark
		//cbi=off
		//sbi=on
float time = 0;
uint8_t outVal = 0;

uint8_t generateOutput(){
	return (uint8_t)fabs(sin(time*0.01*ADCH)*16)+15;
	
//max 31
}		

void outputToPins(uint8_t val){
	isHigh(val, 0) ? sbi(PORTB, PB0) : cbi(PORTB, PB0);
	isHigh(val, 1) ? sbi(PORTB, PB1) : cbi(PORTB, PB1);
	isHigh(val, 2) ? sbi(PORTB, PB2) : cbi(PORTB, PB2);
	isHigh(val, 3) ? sbi(PORTA, PA7) : cbi(PORTA, PA7);
	isHigh(val, 4) ? sbi(PORTA, PA6) : cbi(PORTA, PA6);
	//add more bits for higher bit depth
}
		
int main(void){
	initIO();
	//initInt();
	initADC();
	//sbi(PORTB, PB0);
	
	//uint16_t counter; //default counter for debounce
	
	//start ADC for reading buttons 
	ADCSRA |= 1<<ADSC;
	
	
	while(1){
		/*if(!(ADCSRA & (1<<ADSC))){	
			sbi(outPort, outB1); //alive
			if(ADCH < 0){
				sbi(outPort, outG2); //malfunction
			} else if(ADCH < 1){
				sbi(outPort, outR2);
			} else if(ADCH < 20){
				sbi(outPort, outR2);
				sbi(outPort, outG2);
			} else if(ADCH < 50){
				sbi(outPort, outB2);				
		  } else if(ADCH < 100){
				cbi(outPort, outR1);
				cbi(outPort, outG1);
			} else if(ADCH < 136){
				sbi(outPort, outR1);
				cbi(outPort, outG1);
			} else if(ADCH < 160){
				cbi(outPort, outR1);
				sbi(outPort, outG1);
			} else {
				sbi(outPort, outR1);
				sbi(outPort, outG1);
			}	
		}*/
		
		
		time+=0.05;
		//_delay_ms(10);
		
		//outVal = generateOutput();
		//outputToPins(outVal);
		outputToPins(generateOutput());
		isHigh(PORTB, PB0) ? cbi(PORTB, PB0) : sbi(PORTB, PB0);
		
		//if(!(ADCSRA & (1<<ADSC))){	
		//	outputToPins(ADCH);
		//}
	//blink all lights
	/*_delay_ms(50);
		
		sbi(PORTB, PB0);
		sbi(PORTB, PB1);
		sbi(PORTB, PB2);
		sbi(PORTA, PA7);
		sbi(PORTA, PA6);
		
		_delay_ms(50);
		
		cbi(PORTB, PB0);
		cbi(PORTB, PB1);
		cbi(PORTB, PB2);
		cbi(PORTA, PA7);
		cbi(PORTA, PA6);
		*/	
	}
	return 0;
}