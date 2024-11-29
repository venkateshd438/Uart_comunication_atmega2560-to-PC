
#include "typedef.h"

#ifndef UART_Default_Configuration

#define F_CPU 16000000UL  // Clock frequency
#define BAUD 2400         // Desired baud rate
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)  // Baud rate formula

typedef struct 
{
   volatile uint8 UBRR1H;
   volatile uint8 UBRR1L;
}baud_rate_regdef;

#define baud_rate       ((baud_rate_regdef*)0xCC)
#define UCSR1B          (*(volatile uint8_t*)0xC9)

/*         reference  
    7:6	UMSEL1[1:0]	    USART Mode Select ( Asynchronous, Synchronous)
    5:4	UPM1[1:0]	    Parity Mode Select
    3	USBS1	        Stop Bit Select 
    2:1	UCSZ1[1:0]	    Character Size Select 
    0	UCPOL1	        Clock Polarity for Synchronous Mode
*/
#define UCSR1C          (*(volatile uint8_t*)0xCA)

#define UCSR1A          (*(volatile uint8_t*)0xC8)

#define UDR1            (*(volatile uint8_t*)0xCE)

// Define register pointers for timer 
#define TCCR0A          (*(volatile uint8_t*)0x44)
#define TCCR0B          (*(volatile uint8_t*)0x45)
#define OCR0A           (*(volatile uint8_t*)0x47)
#define TIMSK0          (*(volatile uint8_t*)0x6E)
#define TIFR0           (*(volatile uint8_t*)0x35)
#define SREG            (*(volatile uint8_t*)0x57)

#endif

