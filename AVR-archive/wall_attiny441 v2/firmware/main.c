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

#define pressed1 ( (volatile Bits*)(&GPIOR1) )->f7
#define pressed2 ( (volatile Bits*)(&GPIOR1) )->f6
#define colorUpdateDone ( (volatile Bits*)(&GPIOR1) )->f5
#define set1 ( (volatile Bits*)(&GPIOR1) )->f4
#define set2 ( (volatile Bits*)(&GPIOR1) )->f3

#define moveUp ( (volatile Bits*)(&GPIOR1) )->f2


volatile uint8_t hue1 = 20; //inv hue
volatile uint8_t hue2 = 180;
volatile uint8_t lig1 = 130;
volatile uint8_t lig2 = 130;
volatile uint8_t sat1 = 240;
volatile uint8_t sat2 = 240;

volatile uint8_t noise = 0;

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
	//cli();
	
	//this is hardware version 0.9 
	//where encoder presses were routed
	//to encoder rotation inputs
	
	//_delay_ms(1); //debounce, yeah
	/*
	if(isPressedA(inEncBtn)){
		if(isPressedB(inEnc1l))
			bank =! bank;
			
		if(isPressedB(inEnc1r))
			mode =! mode;
	}		
	*/
	//sei();
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

//color selecting math fx.
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

//main color setting fx. 
void setRGB(){
	//colorUpdateDone = 0;

	double hueD, ligD, satD, q, p;

	hueD = hue1/256.0;
	ligD = lig1/256.0;
	satD = sat1/256.0;
	
	q = ligD<0.5 ? ligD*(1.0 + satD) : ligD+satD-ligD*satD;
	p = 2.0*ligD-q;

	r1 = (uint16_t) 256*4* sqrt(1.0 - hue2rgb(p, q, hueD + 1.0/3)) - 1;
	g1 = (uint16_t) 256*4* sqrt(1.0 - hue2rgb(p, q, hueD))				  - 1;
	b1 = (uint16_t) 256*   sqrt(1.0 - hue2rgb(p, q, hueD - 1.0/3)) - 1;

	hueD = hue2/256.0;
	ligD = lig2/256.0;
	satD = sat2/256.0;

	q = ligD<0.5 ? ligD*(1.0 + satD) : ligD+satD-ligD*satD;
	p = 2.0*ligD-q;

	r2 = (uint16_t) 256*4* sqrt(1.0 - hue2rgb(p, q, hueD + 1.0/3)) - 1;
	g2 = (uint16_t) 256*4* sqrt(1.0 - hue2rgb(p, q, hueD))				  - 1;
	b2 = (uint16_t) 256*   sqrt(1.0 - hue2rgb(p, q, hueD - 1.0/3)) - 1;
	
	//colorUpdateDone = 1;
	
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


	/* Old selective logic. Really, the MCU is capable of refreshing everything.
	
	if(bank){
		r2 = r; g2 = g; b2 = b;
	}else{
		r1 = r; g1 = g; b1 = b;
	}
	*/
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
	
	ADCSRA = ADCSRA |= 1<<ADPS2 | 0<<ADPS1 | 0<<ADPS0;//slow prescaler
	
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

	uint8_t prg = 0;
	uint8_t sbprg = 0;
	
	set1 = 1;
	set2 = 1;

	//default preset init
	//bank=1;
	//setRGB();	
	//bank=0;
	//setRGB();
	//mode=0;

	
	unsigned int counter = 0; //default counter
	unsigned int counter2 = 0;
	
	//v1.2 - simple rotary
	unsigned int slowness = 256;
	moveUp = 1;
	
	//program 0 variables
	//uint8_t hueDecP0 = 0;

	


	//uint8_t glow=0;
	
	// no debounce needed for ADC button reads
	//uint8_t l_press_count = 0; //debounce - left
	//uint8_t r_press_count = 0; //debounce - right
	
	//start ADC for reading buttons 
	ADCSRA |= 1<<ADSC;
	
	while(1){
	
		// --- encoder button press determination via ADC ---
		//                 v1.0    - author: gary sedletsky -
		
		if(!(ADCSRA & (1<<ADSC))){	

			//register encoder presses
			if(ADCH < 100){
				pressed1=0;
				pressed2=0;
			} else if(ADCH < 136){ //right button
				if(!pressed1 && !pressed2){
					//mode = 0;
					//bank = !bank;
					set1=!set1;
					pressed1=1;	
				}
			} else if(ADCH < 180){ //left button 180
				if(!pressed2){
					//mode = !mode;
					set2=!set2;
					pressed2=1;	
				}
			} else {
				//pressed1 = 0;
				//pressed2 = 0;
			}
			
			/*else { //both buttons
				if(!lPressed && !rPressed){ 
					//glow+=64;	
				}
			}	*/
		
				
			ADCSRA |= 1<<ADSC;
		}
		
		
		// --- encoder button press determination end ---
		
		
		
		
		// --- encoder rotation determination routine v1.0 ---
		//                 - author: gary sedletsky -
		
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
			//_delay_us(2);
			
			//if( (enc1l && isPressedB(inEnc1r) && !isPressedB(inEnc1l)) ||
			//		(enc1r && !isPressedB(inEnc1r) && isPressedB(inEnc1l)) ){
				
				//process:
				/*if(enc1l){
					if(bank)
						hue2+=2;
					else
						hue1+=2; 
				}else if(enc1r){
					if(bank)
						hue2-=2;
					else
						hue1-=2;
				}	*/
				
				if(enc1l){	
					if(slowness<=1024)
						slowness+=32;
				}else if(enc1r){
					if(slowness>=32)
						slowness-=32;		
				}
				//setRGB();
				
				enc1l = 0;
				enc1r = 0;
				enc1prog = 0;	
			//}	
		}
		if(enc2prog && (
			(enc2l && isPressedA(inEnc2r) && !isPressedB(inEnc2l)) ||
			(enc2r && !isPressedA(inEnc2r) && isPressedB(inEnc2l)) )) {
			
			//debounce 2
			//_delay_us(2);
			
			//if( (enc2l && isPressedA(inEnc2r) && !isPressedB(inEnc2l)) ||
			//		(enc2r && !isPressedA(inEnc2r) && isPressedB(inEnc2l)) ){
				
				//process:
				/*if(enc2r){
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
				}*/
				
				
				if(set2){
					if(enc2r){
						if(lig1<=253)
							lig1+=2;
						if(lig2<=253)
							lig2+=2;
					}else if(enc2l){
						if(lig1>2)
							lig1-=2;
						if(lig2>2)
							lig2-=2;
					}
				}else{
					if(enc2l)
						hue2+=8;
					else if(enc2r)
						hue2-=8;
				}
				setRGB();
				
				enc2l = 0;
				enc2r = 0;
				enc2prog = 0;	
			//}	
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
		
		
		
		// ---------------------------------------
		// ---------- magic starts here ----------
		
//         programs for version 2.0
//        here is how this will work:
//
//  prg | sbprg |enc1l?enc1r|enc2l?enc2r|  pressed1   |  pressed2
// ----------------------------------------------------------------
//  default program 0: rotate colors; def phase: 180
// ----------------------------------------------------------------
//   0  |   0   | lightness |   speed   |prg++;sbprg=0|   sbprg++
//   0  |   1   | lightness | sat A & B |prg++;sbprg=0|   sbprg++
//   0  |   2   | lightness |  phase B  |prg++;sbprg=0|   sbprg=0
// ----------------------------------------------------------------
//   program 1: sineglow on lightness, capped;	def pahse = 0
// ----------------------------------------------------------------
//   1  |   0	  | top light |  phase B  |prg++;sbprg=0|   sbprg++
//   1  |   1   | top light |   speed   |prg++;sbprg=0|   sbprg++
//   1  |   2   | top light | sat A & B |prg++;sbprg=0|   sbprg=0
// ----------------------------------------------------------------
//   program 2: individual color settings
// ----------------------------------------------------------------
//   2  |   0   |   hue A   | lightns A |prg=0;sbprg=0|   sbprg++
//   2  |   1   |   hue A   | saturat A |prg=0;sbprg=0|   sbprg++
//   2  |   2   |   hue B   | lightns B |prg=0;sbprg=0|   sbprg++
//   2  |   3   |   hue B   | saturat B |prg=0;sbprg=0|   sbprg=0
// ----------------------------------------------------------------

		/*
		if(pressed1){
			prg++;
			sbprg=0;
			
			if(prg==3)
				prg=0;
		}
		
		if(pressed2)
			sbprg++;
		
		
		switch(prg){
			case 0: 
				if(sbprg==3)
					sbprg=0;
				
				
				//if((counter++) >= hueDecP0){
				//hue1+=1 ;
				
				switch(sbprg){
					case 0:
						//testing prg/sbprg
						//hue1 = 100; hue2=100;
						
						break;
					case 1:
						//testing prg/sbprg
						//hue1 = 0; hue2=40;
						
						break;
					case 2:
						//testing prg/sbprg
						//hue1 = 0; hue2=80;
						
						break;
					
				}	
				
			case 1:
				if(sbprg==3)
					sbprg=0;
					
				switch(sbprg){
					case 0:
						//testing prg/sbprg
						//hue1 = 40; hue2=10;
						
						break;
					case 1:
						//testing prg/sbprg
						//hue1 = 40; hue2=50;
						
						break;
					case 2:
						//testing prg/sbprg
						//hue1 = 40; hue2=90;
						
						break;
				}		
			case 2:
				if(sbprg==4)
					sbprg=0;
					
				switch(sbprg){
					case 0:
						//testing prg/sbprg
						//hue1 = 80; hue2=20;
						
						break;
					case 1:
						//testing prg/sbprg
						//hue1 = 80; hue2=60;
						
						break;
					case 2:
						//testing prg/sbprg
						//hue1 = 80; hue2=100;
						
						break;
					case 3:
						//testing prg/sbprg
						//hue1 = 80; hue2=140;
						
						break;
				}		
		}
		
		*/
		
		//setRGB();
		
		//update frame
		
		if((counter++) >= slowness){
			
			if(set1){
				hue1+=1;
				hue2+=1;
			}
			
			setRGB();	
			counter = 0;
		}
		
		
		pressed1 = 0;
		pressed2 = 0;
		//enc1l = 0;
		//enc1r = 0;
		//enc2l = 0;
		//enc2r = 0;
		
		
		// -------- end business logic -----------
		// ---------------------------------------
		//              - author: gary sedletsky -
		
		
		
		// ----------- device testing -----------
		// all code below should be commented out
		// --------------------------------------
		
		
		
		//bank/mode testing (archaic)
		
		/*if(bank){
			hue1 = 40;
			//sbi(outPort, outR2);
		}else{
			hue1 = 200;
			//cbi(outPort, outR2);
		}
		if(mode){
			hue2 = 60;
			//sbi(outPort, outG2);
		}else{
			hue2 = 240;
			//cbi(outPort, outG2);
		}*/
		
			
		//ADC testing. Must be inserted inside ADC routine
		//in the beginning of main loop (if() is just for ref.)
		
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
		}
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