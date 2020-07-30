.include "../../include/tn461def.inc"

;------TESTS----
sbi DDRA,	0
sbi DDRA,	1
sbi DDRA,	2
sbi DDRB,	0
cbi	DDRB,	0


sbi	PORTA,	0
sbi	PORTA,	1
sbi	PORTA,	2
sbi	PORTB,	0

main:



rjmp main