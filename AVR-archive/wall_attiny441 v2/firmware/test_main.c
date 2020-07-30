#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#include <avr/io.h> 
#include <util/delay.h>


int main(void){

	DDRA = (1<<1);

	while(1){

	}
	
	return 0;
}