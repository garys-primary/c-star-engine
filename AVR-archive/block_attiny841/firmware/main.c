#define F_CPU 800000UL
#define BAUD 19200
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define TRUE 1
#define FALSE 0

#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char data_in[8];
unsigned char command_in[8];

volatile unsigned char data_count;
volatile unsigned char command_ready;


void usart_putc (char send)
{
    // Do nothing for a bit if there is already
    // data waiting in the hardware to be sent
    while ((UCSR0A & (1 << UDRE0)) == 0) {};
    UDR0 = send;
}

void usart_puts (const char *send)
{
    // Cycle through each character individually
    while (*send) {
        usart_putc(*send++);
    }
}
void usart_ok()
{
    usart_puts("OK\r\n");
}


int main(void)
{
		DDRA = (1<<0);
		
		PINA = (1<<0);

    // Turn on USART hardware (RX, TX)
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // 8 bit char sizes
    UCSR0C |= (1 << UCSZ00) | (3 << UCSZ01);
    // Set baud rate
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    // Enable the USART Receive interrupt
    UCSR0B |= (1 << RXCIE0 );

    // Globally enable interrupts
    sei();

    while(1) {

        if (command_ready == TRUE) {
            PINA = (1<<0);
            _delay_ms(200);
            command_ready = FALSE;
        }
    }
}

ISR (USART0_RX_vect)
{
    // Get data from the USART in register
    data_in[data_count] = UDR0;

    // End of line!
    if (data_in[data_count] == '\n') {
        command_ready = TRUE;
        // Reset to 0, ready to go again
        data_count = 0;
    } else {
        data_count++;
    }
}