/*
*   Encoder-controlled 2-led PWM color selector 
*
*		ref. https://gcc.gnu.org/onlinedocs/cpp
*		Author garys
*/
#define	F_CPU		8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//Function-like Macros
#define sbi(port, pin) (port |= (1<<pin))
#define cbi(port, pin) (port &= ~(1<<pin))

#define isPressed(bit) ( inPin & (1<<bit))

//Port definitions
#define outPort PORTA
#define outDDR	DDRA
#define outPin	PINA
#define outR1 PA0
#define outG1 PA1
#define outB1 PA2
#define outR2 PA3
#define outG2 PA4
#define outB2 PA5

#define inPort PORTB
#define inDDR PORTB
#define inPin  PINB
#define inEnc1l PB0
#define inEnc1r PB1
#define inEnc2l PB2
#define inEnc2r PB7
#define inEnc1p PA6

//#define inEnc2p PB3 //reset


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


void initIO(){
	inDDR = 0x00;
	inPort = 0xFF; //pullup
	
	outDDR = 0xFF;
	outPort = 0x00;	//set pins low
}
void initInt(){
	PCMSK1 = 0b00001111;
	GIMSK |= (1<<PCIE1);
	
	PCMSK0 = 0b11000000;
	GIMSK |= (1<<PCIE0);
	sei();
}

//bank select interrupt
ISR(PCINT1_vect){
	sbi(outPort, outR1);
	_delay_ms(100);
	cbi(outPort, outR1);

}
//mode select interrupt
ISR(PCINT0_vect){
	sbi(outPort, outG1);
	_delay_ms(100);
	cbi(outPort, outG1);
}

void initPWM(){
	//listpwm channels - 1h
	
	//get source pin numbers
	//get output pin numbers
	
	//map all in pin mapping settings ->
	//mapping settings

	//TOCPMSA0 = (1<<TOCC0S0) | (1<<TOCC1S0) | (1<<TOCC2S1);
	//TOCPMSA1 = ();
	
	//A0: g1 <- TC1A 
	//A1: r1 <- TC1B
	//A2: b1 <- TC0A
	
	//A3: g2 <- TC2A
	//A4: r2 <- TC2B
	//A5: b2 <- TC0B
	
	TOCPMSA1 = 0b00001111; //p.115-116
	TOCPMSA0 = 0b00000101;
	
	//TOCPMCOE = (1<<TOCC0OE) | (1<<TOCC1OE) | (1<<TOCC2OE);
	TOCPMCOE = 0b00111111;
	
	TCCR0A = 0<<COM0A1 | 1<<COM0A0 | 1<<COM0B0 | 1<<WGM01 | 1<<WGM00;
	TCCR0B = 1<<WGM02 | 1<<CS00;	
	
	TCCR1A = 1<<COM1A1 | 1<<COM1B1 | 1<<WGM11 | 1<<WGM10;
	TCCR1B = 1<<WGM12 | 1<<CS10  |  1<<WGM13;
  
	TCCR2A = 1<<COM2A1 | 1<<COM2B1 | 1<<WGM21 | 1<<WGM20;
	TCCR2B = 1<<WGM22 | 1<<CS20  |  1<<WGM23;

}

int main(void){
	initIO();
	//initInt();
	initPWM();
	
	enc1l = 0;
	enc1r = 0;
	enc1prog = 0;
	
	uint8_t hue = 0;
	
	uint16_t r1, g1, b1;
	
	uint8_t counter = 0;
	uint16_t prescaler = 0;
	
	while(1){
		//PWM color set routine
		//r1=128;
		//g1=90;
		//b1=60;
		counter = hue;
		OCR0A = counter;
		OCR0B = counter;
		OCR1A = counter;//b1;
		OCR1B = counter;
		OCR2A = counter;
		OCR2B = counter;
		
		/*prescaler++;
		if(prescaler==255){
			counter++;
			if(counter==160)
				counter=0;
			prescaler=0;
		}*/
		
		
		//encoder determination routine
		
		//first encounter of pressed registers direction
		if(!enc1r && isPressed(inEnc1l) && !isPressed(inEnc1r)){
			enc1l=1;
		} else if (!enc1l && !isPressed(inEnc1l) && isPressed(inEnc1r)){
			enc1r=1;
		}
		
		//register transitional state
		if((enc1r || enc1l) &&
			(( isPressed(inEnc1l) && isPressed(inEnc1r) ))) { //both high
			enc1prog=1;	
		}
		
		//check if next signal is opposite (normal)
		if(enc1prog && (
			(enc1l && isPressed(inEnc1r) && !isPressed(inEnc1l)) ||
			(enc1r && !isPressed(inEnc1r) && isPressed(inEnc1l)) )) {
			
			//debounce 2
			_delay_us(20);
			
			if( (enc1l && isPressed(inEnc1r) && !isPressed(inEnc1l)) ||
					(enc1r && !isPressed(inEnc1r) && isPressed(inEnc1l)) ){
				
				if(enc1l)
					hue+=1;
				else if(enc1r)
					hue-=1;	
				
				enc1l = 0;
				enc1r = 0;
				enc1prog = 0;	
			}	
		}
		
		//idle state - reset
		if(!isPressed(inEnc1l) && !isPressed(inEnc1r)){
			enc1l = 0;
			enc1r = 0;
			enc1prog = 0;
		}
	
		//display values
		//outPort = hue;
		
		/*
		sbi(outPort, 0);
		sbi(outPort, 1);
		sbi(outPort, 2);
		sbi(outPort, 3);
		sbi(outPort, 4);
		sbi(outPort, 5);
		*/
		
		//display flags for encoder 1	
		/*
		if(enc1l)
			sbi(outPort, outR1);
		else
			cbi(outPort, outR1);
			
		if(enc1r)
			sbi(outPort, outG1);
		else
			cbi(outPort, outG1);
	
		if(enc1prog)
			sbi(outPort, outB1);		
		else
			cbi(outPort, outB1);
	*/
	}
}