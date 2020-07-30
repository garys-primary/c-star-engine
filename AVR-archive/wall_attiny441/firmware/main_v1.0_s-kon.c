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

		
#define r1 OCR1A
#define g1 OCR1B
#define b1 OCR0B
#define r2 OCR2B
#define g2 OCR2A
#define b2 OCR0A

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


volatile uint8_t hue1 = 180; //inv hue
volatile uint8_t hue2 = 20;
volatile uint8_t lig1 = 130;
volatile uint8_t lig2 = 130;
volatile uint8_t sat1 = 240;
volatile uint8_t sat2 = 240;

void initIO(){
	//DDR  in=0 out=1
	//PORT in=1 out=0
	
	DDRB  = 0b00000000;
	PORTB = 0b00001111;
	//inDDR = 0x00;
	//inPort = 0xFF; //pullup
	
	DDRA  = 0b01111110;
	PORTA = 0b10000001;
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
	
	_delay_ms(1); //debounce, yeah
	
	if(isPressedA(inEncBtn)){
		if(isPressedB(inEnc1l))
			bank =! bank;
			
		if(isPressedB(inEnc1r))
			mode =! mode;
	}		
	
	sei();
}

void initPWM(){
	
	//A1: g1 <- OC1B - TOCC0 01 
	//A2: r1 <- OC1A - TOCC1 01 
	//A3: b1 <- OC0B - TOCC2 00
	
	//A4: g2 <- OC2A - TOCC3 1X
	//A5: r2 <- OC2B - TOCC4 1x
	//A6: b2 <- OC0A - TOCC5 00
	
	TOCPMSA1 = 0b00000011; //p.115-116
	TOCPMSA0 = 0b11000101;
	
	//TOCPMCOE = (1<<TOCC0OE) | (1<<TOCC1OE) | (1<<TOCC2OE);
	TOCPMCOE = 0b00111111; //PWM outs
	
	//8-bit timer, mode 3, inv mode, no prescaler
	TCCR0A = 1<<COM0A0 | 1<<COM0A1 | 1<<COM0B0 | 1<<COM0B1 | 1<<WGM01 | 1<<WGM00;
	//TCCR0A = 0<<COM0A0 | 1<<COM0A1 | 1<<COM0B0 | 1<<COM0B1 | 1<<WGM01 | 1<<WGM00;
	TCCR0B = 1<<CS00;	
	
	//16-bit timers, mode 14, inv mode, no prescaler
	TCCR1A |= 1<<COM1A1 | 1<<COM1B1 | 1<<COM1A0 | 1<<COM1B0 | 1<<WGM10 | 1<<WGM11;	
	//TCCR1A |= 0<<COM1A1 | 1<<COM1B1 | 1<<COM1A0 | 1<<COM1B0 | 1<<WGM10 | 1<<WGM11;
	TCCR1B |= 1<<CS10 | 1<<WGM12;
	
	TCCR2A |= 1<<COM2A1 | 1<<COM2B1 | 1<<COM2A0 | 1<<COM2B0 | 1<<WGM20 | 1<<WGM21;	
	//TCCR2A |= 0<<COM2A1 | 1<<COM2B1 | 1<<COM2A0 | 1<<COM2B0 | 1<<WGM20 | 1<<WGM21;
	TCCR2B |= 1<<CS20 | 1<<WGM22;
	
	ICR1 = 0xFFFF;
	ICR2 = 0xFFFF;
	
	r1=0;
	g1=0;
	b1=0;
	
	r2=0;
	g2=0;
	b2=0;	
}

double hue2rgb(double p, double q, double t){
	double result = 0;

	if(t < 0) t += 1;
	if(t > 1) t -= 1;

	if(t < 1.0/6) {
		result = p+(q-p)*6*t;
	} else if(t < 1.0/2)  {
		result = q;
	} else if(t < 2.0/3) {
		result = p+(q-p)*(2.0/3-t)*6;
	} else {
		result = p;
	}

	return result;
}

void setRGB(){
	uint16_t	b, r, g;
	double hueD, ligD, satD;
	
	if(bank){
		hueD = hue2/256.0;
		ligD = lig2/256.0;
		satD = sat2/256.0;
	} else {
		hueD = hue1/256.0;
		ligD = lig1/256.0;
		satD = sat1/256.0;
	}
	
	double q = ligD<0.5 ? ligD*(1.0 + satD) : ligD+satD-ligD*satD;
	double p = 2.0*ligD-q;

// + 0.00000001
	/* error color code
	if((hue2rgb(p, q, hueD + 1.0/3))<0.0)
		r=0;
	else
		r=256*4-1;
	
	if((hue2rgb(p, q, hueD))<0.0)		
		g=0;
	else	
		g=256*4-1;
	
	if((hue2rgb(p, q, hueD - 1.0/3))<0.0)	
		b=0;
	else
		b=255;
	*/

	r = (uint16_t) 256*4* sqrt(1.0 - hue2rgb(p, q, hueD + 1.0/3)) - 1;
	g = (uint16_t) 256*4* sqrt(1.0 - hue2rgb(p, q, hueD))				  - 1;
	b = (uint16_t) 256*   sqrt(1.0 - hue2rgb(p, q, hueD - 1.0/3)) - 1;
	
	if(bank){
		r2 = r; g2 = g; b2 = b;
	}else{
		r1 = r; g1 = g; b1 = b;
	}
	
	/*
	g1 = mode ? 256*4-1 : 0 ;//255*255; //low=true, high=false
	r1 = bank ? 256*4-1 : 0;//255*255;
	b1 = 0;//256-1;//255*255;
	g2 = 0;//256*4-1;//255*255;
	r2 = 0;//256*4-1;//255;
	b2 = 256-1;//255;
	*/
	
	//}
	/*uint8_t i=0;
	while(i<256){
	i++;
	hue=i;
	g1 = hue*4-1;//255*255;
	r1 = hue*4-1;//255*255;
	b1 = hue-1;//255*255;
	g2 = hue*4-1;//255*255;
	r2 = hue*4-1;//255;
	b2 = hue-1;//255;
	_delay_us(100);
	}*/
}

void initADC(){
	//PRR |= 0<<PRADC;//remove Power Reduction Bit 
	PRR = PRR &= ~(_BV(PRADC));//remove Power Reduction Bit 
	
	ADCSRA = ADCSRA |= ( 7 <<ADPS2 );//slow prescaler
	
	ADCSRA |= 1<<ADEN; //enable ADC
	//select ADC0 as input
	ADMUXA |= 0<<MUX0 | 0<<MUX1 | 0<<MUX2 | 0<<MUX3 | 0<<MUX4 | 0<<MUX5;
	ADCSRB |= 1<<ADLAR; //left adjusted result for 8bit precision (ADCH)
	ADCSRA |= 0<<ADATE; //no automatic triggering
}

		
		//ULN remark
		//cbi=off
		//sbi=on
		
		
int main(void){
	initIO();
	//initInt();
	initPWM();
	initADC();
	
	enc1l = 0;
	enc1r = 0;
	enc1prog = 0;
	
	enc2l = 0;
	enc2r = 0;
	enc2prog = 0;

	//default preset init
	bank=1;
	setRGB();	
	bank=0;
	setRGB();
	mode=0;
	
	//uint8_t glow=0;
	
	//uint16_t counter; //default counter for debounce
	
	//start ADC for reading buttons 
	ADCSRA |= 1<<ADSC;
	
	while(1){
		if(!(ADCSRA & (1<<ADSC))){	
			/*sbi(outPort, outB1); //alive
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
			*/
			//register encoder presses
			if(ADCH < 100){
				rPressed=0;
				lPressed=0;
			} else if(ADCH < 136){ //right button
				if(!rPressed && !lPressed){
					mode = 0;
					bank = !bank;
					rPressed=1;	
				}
			} else if(ADCH < 170){ //left button
				if(!lPressed){
					mode = !mode;
					lPressed=1;	
				}
			} else { //both buttons
				if(!lPressed && !rPressed){ //make a sep. flag instead and resore mod/bank values
					//glow+=64;
					lPressed=1;	rPressed=1;	
				}
			}	
				
			ADCSRA |= 1<<ADSC;
		}
		
		
		//bank/mode testing
		/*if(isPressedA(inEncBtn))
			sbi(outPort, 1);
		else
			cbi(outPort, 1);
			
		if(bank)
			sbi(outPort, outR2);
		else
			cbi(outPort, outR2);
		
		if(mode)
			sbi(outPort, outG2);
		else
			cbi(outPort, outG2);
		*/
		
		//glowmode testing
		/*
		if(glow<=64){
			sbi(outPort, outB2);
			cbi(outPort, outR1);
			cbi(outPort, outG1);
			cbi(outPort, outB1);
		} else if(glow<=128){
			cbi(outPort, outB2);
			sbi(outPort, outR1);
			cbi(outPort, outG1);
			cbi(outPort, outB1);
		} else if(glow<=192){
			cbi(outPort, outB2);
			cbi(outPort, outR1);
			sbi(outPort, outG1);
			cbi(outPort, outB1);
		} else {
			cbi(outPort, outB2);
			cbi(outPort, outR1);
			cbi(outPort, outG1);
			sbi(outPort, outB1);
		}  
		*/
		
		//encoder determination routine
		//first encounter of pressed registers direction
		
		if(!enc1r && isPressedB(inEnc1l) && !isPressedB(inEnc1r)){
			enc1l=1;
		} else if (!enc1l && !isPressedB(inEnc1l) && isPressedB(inEnc1r)){
			enc1r=1;
		}
		if(!enc2r && isPressedB(inEnc2l) && !isPressedA(inEnc2r)){
			enc2l=1;
		} else if (!enc2l && !isPressedB(inEnc2l) && isPressedA(inEnc2r)){
			enc2r=1;
		}
		
		//register transitional state
		if((enc1r || enc1l) &&
			(( isPressedB(inEnc1l) && isPressedB(inEnc1r) ))) { //both high
			enc1prog=1;	
		}
		if((enc2r || enc2l) &&
			(( isPressedB(inEnc2l) && isPressedA(inEnc2r) ))) { //both high
			enc2prog=1;	
		}
		
		//check if next signal is opposite (normal)
		if(enc1prog && (
			(enc1l && isPressedB(inEnc1r) && !isPressedB(inEnc1l)) ||
			(enc1r && !isPressedB(inEnc1r) && isPressedB(inEnc1l)) )) {
			
			//debounce 2
			_delay_us(20);
			
			if( (enc1l && isPressedB(inEnc1r) && !isPressedB(inEnc1l)) ||
					(enc1r && !isPressedB(inEnc1r) && isPressedB(inEnc1l)) ){
				
				if(enc1l){
					if(bank)
						hue2+=2;
					else
						hue1+=2; 
				}else if(enc1r){
					if(bank)
						hue2-=2;
					else
						hue1-=2;
				}			
				
				setRGB();
				
				enc1l = 0;
				enc1r = 0;
				enc1prog = 0;	
			}	
		}
		if(enc2prog && (
			(enc2l && isPressedA(inEnc2r) && !isPressedB(inEnc2l)) ||
			(enc2r && !isPressedA(inEnc2r) && isPressedB(inEnc2l)) )) {
			
			//debounce 2
			_delay_us(20);
			
			if( (enc2l && isPressedA(inEnc2r) && !isPressedB(inEnc2l)) ||
					(enc2r && !isPressedA(inEnc2r) && isPressedB(inEnc2l)) ){
				
				if(enc2r){
					if(bank) {
						if(!mode)
							if(lig2>=247) lig2 = 255; else lig2+=4;
						else
							if(sat2>=247) sat2 = 255; else sat2+=4;
					} else {
						if(!mode)
							if(lig1>=247) lig1 = 255; else lig1+=4;
						else
							if(sat1>=247) sat1 = 255; else sat1+=4;
					}
				}else if(enc2l){
					if(bank) {
						if(!mode)
							if(lig2<=4) lig2 = 1; else lig2-=4;
						else
							if(sat2<=4) sat2 = 1; else sat2-=4;
					} else {
						if(!mode)
							if(lig1<=4) lig1 = 1; else lig1-=4;
						else
							if(sat1<=4) sat1 = 1; else sat1-=4;
					}
				}
				
				setRGB();
				
				enc2l = 0;
				enc2r = 0;
				enc2prog = 0;	
			}	
		}
		
		//idle state - reset
		if(!isPressedB(inEnc1l) && !isPressedB(inEnc1r)){
			enc1l = 0;
			enc1r = 0;
			enc1prog = 0;
		}
		if(!isPressedB(inEnc2l) && !isPressedA(inEnc2r)){
			enc2l = 0;
			enc2r = 0;
			enc2prog = 0;
		}
		//encoder determination end
		
		
		//display flags for encoder	
		/*
		if(enc2l)
			cbi(outPort, outR1);
		else
			sbi(outPort, outR1);
			
		if(enc2r)
			cbi(outPort, outG1);
		else
			sbi(outPort, outG1);
	
		if(enc2prog)
			cbi(outPort, outB1);		
		else
			cbi(outPort, outB1);
		*/
		
		
		//indicate encoder pressed
		/*if(isPressedB(inEnc1l))
			sbi(outPort, outR1);
		else
			cbi(outPort, outR1);
			
		if(isPressedB(inEnc1r))
			cbi(outPort, outG1);
		else
			sbi(outPort, outG1);

		if(isPressedB(inEnc2l))
			cbi(outPort, outB1);
		else
			sbi(outPort, outB1);
			
		if(isPressedA(inEnc2r))
			cbi(outPort, outR2);
		else
			sbi(outPort, outR2);
		*/
			
		//blink all lights
	/*_delay_ms(50);
		
		sbi(outPort, outR1);
		sbi(outPort, outR2);
		sbi(outPort, outG1);						
		sbi(outPort, outG2);
		sbi(outPort, outB1);
		sbi(outPort, outB2);
		
		_delay_ms(50);
		
		cbi(outPort, outR1);
		cbi(outPort, outR2);
		cbi(outPort, outG1);						
		cbi(outPort, outG2);
		cbi(outPort, outB1);
		cbi(outPort, outB2);
		*/	
	}
	return 0;
}