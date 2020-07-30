.include "../../include/tn84Adef.inc"

;***** Pin definitions *****
.equ	UART_TxPort = PORTB
.equ	UART_TxDDR  = DDRB
.equ	UART_RxPin  = PINB
.equ	UART_RxDDR  = DDRB
.equ	RxD	= 2
.equ	TxD	= 3

;*****    Registers    *****
.def	bitcounter  = R24
.def	temp	    = R17
.def	UART_buffer = R23

;*****  Speed setting  *****
.equ	CRYSTAL_FREQ = 8000000
.equ	BAUD 		 = 9600

;*****       Etc       *****
.equ	stop_bits 	       = 1
; hand-picked for easier auto-baud calculations (no division)
.equ	cycles_per_bit 	   = 12 ;without delays + 4*nop
.equ	const_delay_cycles = 9	;rjmp + ret + 2*nop

; byte in ram that keeps speed
.dseg
DELAY_VALUE: .byte 1
.cseg

;***** Precalculations *****
;pure cycles necessary per bit
.equ	c = CRYSTAL_FREQ / BAUD			
;delay cycles
.equ	d = (c - cycles_per_bit - 2*const_delay_cycles)/2
;delay value	
.equ	b = d / 3						

; throw error if we're out of bounds
.if b > 255
	.error "Baud rate is too low!"
.elif b <= 0
	.error "Baud rate is too high!"
.endif

;****************************************************
;**********************CODE HERE*********************
;****************************************************
.org	0x00
rjmp	start
.org	0x0003
rjmp	get_byte_interrupt

start:
;-----interrupts
ldi r16, high(RAMEND)
out SPH, r16
ldi r16, low(RAMEND)
out SPL, r16

ldi	r16,	0b00100000	;pcie1
out	GIMSK,	r16

;ldi temp, (1<<RxD)
ldi	r16,	0b00000100
out PCMSK1, r16	;enable interrupt on PB2

;------TESTS----
sbi DDRA,	0
sbi DDRA,	1
sbi DDRA,	2
sbi DDRB,	0
sbi	DDRB,	1
	
sbi	PORTA,	0
sbi	PORTA,	1
sbi	PORTA,	2
sbi	PORTB,	0
sbi PORTB,	1

sbi	PORTB,	2	;is input

;------setings
clr	UART_buffer

cbi	 PORTB,	1 ;sdevice is ready

main:
	sei
	
	;sbic PINB, 	2
	;sbi	 PORTB,	0
	;sbi	PORTB,	0
	
	;cbi	PORTB,	0
	
	;rcall	UART_autocalibrate
	;rcall	UART_get_byte
	
	cpi	UART_buffer, 0x0
	breq	indicateZero
	
	cpi	UART_buffer, 0x10
	breq	indicateSeq
	
	cpi	UART_buffer, 0x0
	brne	indicateNonZero
			
rjmp main

indicateZero:
	cbi	PORTA,	0
	sbi	PORTA,	1
	sbi	PORTA,	2
rjmp main

indicateNonZero:
	sbi	PORTA,	0
	cbi	PORTA,	1
	sbi	PORTA,	2
rjmp main

indicateSeq:
	sbi	PORTA,	0
	sbi	PORTA,	1
	cbi	PORTA,	2
rjmp main

indicateTest:
	cbi	PORTB,	0
rjmp main

get_byte_interrupt:
	;cli
	cbi	PORTB,	0
	inc UART_buffer
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop

	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	
	
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop

	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	nop nop nop nop  nop nop nop nop
	
	sbi	PORTB,	0
	;rcall	UART_get_byte
	;rcall	UART_send_byte
	;rcall	UART_autocalibrate
;rjmp	main
reti


;****************************************************
;*****                 SEND BYTE                *****
;****************************************************
; UART_buffer -> UART
UART_send_byte:	
	ldi		bitcounter, 9 + stop_bits
	com		UART_buffer	
	sec		

; 12 cycles here (without delays)
uart_send_next_bit:	
	brcc	uart_send_1
	cbi		UART_TxPort, TxD
	rjmp	uart_wait

uart_send_1:	
	sbi		UART_TxPort, TxD
	nop

uart_wait:	
	rcall 	UART_delay	
	rcall 	UART_delay

	nop	nop nop nop
	lsr		UART_buffer
	dec		bitcounter		
	brne	uart_send_next_bit
					
	sbi		UART_TxPort, TxD
	ret		
		
;****************************************************
;*****                 GET BYTE                 *****
;****************************************************
; UART -> UART_buffer
UART_get_byte:	
	ldi 	bitcounter, 9

uart_wait_for_start:	
	sbic 	UART_RxPin, RxD	
	rjmp 	uart_wait_for_start
	;synchronize for 0.5 bit length
	rcall 	UART_delay	

; 10 cycles here (without delays)
uart_get_bit:	
	rcall 	UART_delay	
	rcall 	UART_delay		

	clc			
	sbic 	UART_RxPin, RxD	
	sec			
	
	nop	nop nop nop
	dec 	bitcounter		
	breq 	uart_bit_done	
				
	ror 	UART_buffer		
	rjmp 	uart_get_bit	

uart_bit_done:	
	ret

;****************************************************
;*****                UART DELAY                *****
;****************************************************
; 3*b + const_delay_cycles
UART_delay:	
	lds		temp, DELAY_VALUE
UART_delay1:	
	dec		temp
	brne	UART_delay1
	nop
	ret

;****************************************************
;*****              UART AUTO-BAUD              *****
;****************************************************
; calculates calc_b after receiving 0x55
; calc_b -> temp
; intermediate result is c/6
; sbis (2) + inc (1) + breq (2) + nop (1)

UART_autocalibrate:
	clr 	temp
	
uart_cal_wait_start:	
	sbic 	UART_RxPin, RxD	
	rjmp 	uart_cal_wait_start

uart_cal_wait_bit:	
	sbis 	UART_RxPin, RxD	
	rjmp 	uart_cal_wait_bit

uart_measure_bit:
	sbis 	UART_RxPin, RxD		
	rjmp 	uart_calibration_done
	nop
	inc		temp
	breq	uart_calibration_fail

uart_calibration_done:
	; b = c/6 - cycles_per_bit/6 - const_delay_cycles/3
	; temp = c/6
	cpi		temp, cycles_per_bit/6
	brlo	uart_calibration_fail
	subi	temp, cycles_per_bit/6

	cpi		temp, const_delay_cycles/3
	brlo	uart_calibration_fail
	subi	temp, const_delay_cycles/3

	sts		DELAY_VALUE, temp

uart_calibration_fail:
	ret

;****************************************************
;*****               UART REINIT                *****
;****************************************************
; just resets DELAY_VALUE to default
uart_reinit:
	in		temp, UART_TxDDR
	ori		temp, (1<<TxD)
	out		UART_TxDDR, temp
	sbi		UART_TxPort, TxD

	in		temp, UART_RxDDR
	andi	temp, ~(1<<RxD)
	out		UART_RxDDR, temp

	ldi		temp, b
	sts		DELAY_VALUE, temp
	ret
