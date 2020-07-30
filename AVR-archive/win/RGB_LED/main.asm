.include "C:\asm\Appnotes\tn13def.inc"
.org	0
rjmp	begin
.org	4
.db	255,0,0,		0,255,0,		0,0,255,		255,127,80
.org	10
.db	0,255,127,	255,160,122,	0,255,255,	255,255,0
.org	16
.db	128,0,128,	218,165,32,	0,128,0,		255,127,80
.org	22
.db	127,255,212,	255,192,203,	320,230,250,	211,255,158

begin:
clr	r16
;bit	purpose
;1	OK (button released)

;			cycles	comment
sbi	PORTB, 	4		;Set BIt 2 in PORTB for input
sbi	DDRB,  	0 		;Red - output
sbi	DDRB,  	1 		;Green
sbi	DDRB,  	2 		;Blue

cbi	PORTB,	0		;turn off Red
cbi	PORTB,	1		;turn off Green
cbi	PORTB,	2		;turn off Blue

ldi	r23,	0		;start main PWM counter
clr	r20	;R actual
clr	r21	;G
clr	r22	;B
clr	r25	;R transitional
clr	r26	;G
clr	r27	;B

ldi	r17,	255	;transition delay

rcall	initIntDelay

clearColors:
clr	ZH
ldi	ZL,	8		;set start address of first color
ldi	r24,	0		;init color selector
rjmp	initColor

;============MAIN================
main:		; 1 pwm cycle = 12 cpu cycles minimum
inc 	r23		;1

cpi	r23,	255	;1
breq	init		;1 .. 2
retInit:

cp	r23,	r20	;1
breq 	offRed		;1 .. 2
retRed:

cp	r23,	r21	;1
breq	offGreen		;1 .. 2
retGreen:

cp	r23,	r22	;1
breq	offBlue		;1 .. 2
retBlue:



;==btn
sbic	PINB,	4	;1 .. 3	if button ever released
rcall	setFlag

sbrs	r16,	0	;skip if OK flag set
rjmp 	main		;else goto main

sbis	PINB,	4
rcall	nextColor

rjmp	main
;================================

;=====Set flag=====
setFlag:
dec	r28
cpi	r28,	0
breq	decFlagMS

decFlagMSret:
processEnd:
ret

decFlagMS:
dec	r29
cpi	r29,	0
sbic	PINB,	4
breq	processFlag
rjmp	decFlagMSret

processFlag:
ldi 	r16,	(1<<0)	;set OK flag in bit 0
rcall	initIntDelay
rjmp	processEnd
;=====end set flag=====


nextColor:
cpi	r24,	16	;total colors    <======
breq	clearColors

initColor:
inc	r24	;Color counter
clr	r25	;R
clr	r26	;G
clr	r27	;B

;		   init	clck1	clck2	clck3	clk4	clk5
lpm	r25,	Z+;0 	3 	6	9	12	15
lpm	r26,	Z+;1 	4 	7	10	13	?
lpm	r27,	Z+;2 	5 	8	11	14	?

ldi	r16,	(0<<0)	;clear OK flag
rjmp main

;==========COLOR WHEEL=============
init:
sbi	PORTB,	0	;2	turn on Red
sbi	PORTB,	1	;2	turn on Green
sbi	PORTB,	2	;2	turn on Blue

ldi	r23,	0	;1	reinit counter

;sbci	r17,	1
rjmp	transitionColors

rjmp	retInit

;=================================
offRed:
cbi	PORTB,	0	;2	turn off Red
rjmp	retRed

offGreen:
cbi	PORTB,	1	;2	turn off Green
rjmp	retGreen

offBlue:
cbi	PORTB,	2	;2	turn off Blue
rjmp	retBlue
;======

transitionColors:
cp	r20,	r25
brlo	incRed

cp	r25,	r20
brlo	decRed
endProcRed:

cp	r21,	r26
brlo	incGreen

cp	r26,	r21
brlo	decGreen
endProcGreen:

cp	r22,	r27
brlo	incBlue

cp	r27,	r22
brlo	decBlue
endProcBlue:

ldi	r17,	255
rjmp	retInit

;===

incRed:
inc	r20
rjmp	endProcRed
decRed:
dec	r20
rjmp	endProcRed

incGreen:
inc	r21
rjmp	endProcGreen
decGreen:
dec	r21
rjmp	endProcGreen

incBlue:
inc	r22
rjmp	endProcBlue
decBlue:
dec	r22
rjmp	endProcBlue
;=================================

initIntDelay:
ldi	r28,	255		;init internal delay
ldi	r29,	15		;init internal delay
ret