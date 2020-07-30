#define F_CPU 1000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define TRUE 1
#define FALSE 0

#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"

#include <avr/io.h>
#include <avr/interrupt.h>

// The inputted commands are never going to be
// more than 8 chars long.
// volatile so the ISR can alter them
volatile unsigned char data_in[8];
unsigned char command_in[8];

volatile unsigned char data_count;
volatile unsigned char command_ready;

void usart_putc (char send)
{
    // Do nothing for a bit if there is already
    // data waiting in the hardware to be sent
    while ((UCSRA & (1 << UDRE)) == 0) {};
    UDR = send;
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
		

    // Turn on USART hardware (RX, TX)
    UCSRB |= (1 << RXEN) | (1 << TXEN);
    // 8 bit char sizes
    UCSRC |= (1 << UCSZ0) | (1 << UCSZ1);
    // Set baud rate
    UBRRH = (BAUD_PRESCALE >> 8);
    UBRRL = BAUD_PRESCALE;
    // Enable the USART Receive interrupt
    UCSRB |= (1 << RXCIE );

    // Globally enable interrupts
    sei();

    while(1) {
        if (command_ready) {
            // Here is where we will copy
            // and parse the command.
            DDRB = (1<<1);
            command_ready = FALSE;
        }
    }
}

ISR (USART_RXC_vect)
{
    // Get data from the USART in register
    data_in[data_count] = UDR;

    // End of line!
    if (data_in[data_count] == '\n') {
        command_ready = TRUE;
        // Reset to 0, ready to go again
        data_count = 0;
    } else {
        data_count++;
    }
}