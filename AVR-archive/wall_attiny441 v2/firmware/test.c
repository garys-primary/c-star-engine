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


//Function-like Macros
#define sbi(port, pin) (port |= (1<<pin))
#define cbi(port, pin) (port &= ~(1<<pin))

#define isPressedA(bit) (PINA & (1<<bit))
#define isPressedB(bit) (PINB & (1<<bit))

//Port definitions
#define outPort PORTA
#define outDDR	DDRA
#define outPin	PINA
#define outR1 PA1
#define outG1 PA2
#define outB1 PA3
#define outR2 PA4
#define outG2 PA5
#define outB2 PA6

#define inPort PORTB
#define inDDR PORTB
#define inPin  PINB
#define inEnc1l PB0
#define inEnc1r PB1
#define inEnc2l PB2
#define inEnc2r PA7
#define inEncBtn PA0

		
#define r1 OCR1A
#define g1 OCR1B
#define b1 OCR0B
#define r2 OCR2B
#define g2 OCR2A
#define b2 OCR0A

//define bools
//TODO: optimization check
typedef struct { 
 unsigned f0:1; 
 unsigned f1:1; 
 unsigned f2:1; 
 unsigned f3:1; 
 unsigned f4:1; 
 unsigned f5:1; 
 unsigned f6:1; 
 unsigned f7:1; 
} Bool;

#define enc1l ( (volatile Bool*)(&GPIOR0) )->f7
#define enc1r ( (volatile Bool*)(&GPIOR0) )->f6
#define enc1done ( (volatile Bool*)(&GPIOR0) )->f5
#define enc1prog ( (volatile Bool*)(&GPIOR0) )->f4

int main(void){
	//uint8_t counter = 0;
	
	//init IO
	DDRA  = 0b01111110;
	PORTA = 0b10000001;
	//outDDR = 0xFF;
	//outPort = 0x00;	//set pins low
	
	while(1){
		_delay_ms(50);
		
		/*
		if(counter == 128 ){
			sbi(outPort, outR1);
			cbi(outPort, outG1);
		}
		
		if(counter == 0){
			cbi(outPort, outR1);
			sbi(outPort, outG1);
		}
		counter++;*/
		
		sbi(outPort, outR1);
		sbi(outPort, outR2);
		sbi(outPort, outG1);						
		sbi(outPort, outG2);
		sbi(outPort, outB1);
		sbi(outPort, outB2);
		
		_delay_ms(200);
		
		cbi(outPort, outR1);
		cbi(outPort, outR2);
		cbi(outPort, outG1);						
		cbi(outPort, outG2);
		cbi(outPort, outB1);
		cbi(outPort, outB2);
			
		//encoder determination routine
		
		/*
		//first encounter of pressed registers direction
		if(!enc1r && isPressed(inEnc1l) && !isPressed(inEnc1r)){
			enc1l=1;
		}else if(!enc1l && !isPressed(inEnc1l) && isPressed(inEnc1r)){
			enc1r=1;
		}
		
		//register transitional state
		if((enc1r || enc1l) &&
			(( isPressed(inEnc1l) && isPressed(inEnc1r) ))) { //both pressed
			enc1prog=1;	
		}
		
		//check if next signal is opposite (normal)
		if(enc1prog && (
			(enc1l && isPressed(inEnc1r) && !isPressed(inEnc1l)) ||
			(enc1r && !isPressed(inEnc1r) && isPressed(inEnc1l)) )
		) {
			enc1done = 1;
		}
		
		if(!isPressed(inEnc1l) && !isPressed(inEnc1r)){
			enc1l = 0;
			enc1r = 0;
			enc1done = 0;
			enc1prog = 0;
		}
		*/
		
		//attempt 2
		/*		
		enc1l = 1;
		
		if(1)
			enc1l = 1;
			
		if(1)
			enc1l = 0;
			
		enc1l = 0; 
		*/
	}
}