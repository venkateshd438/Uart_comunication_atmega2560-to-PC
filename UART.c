#include <avr/io.h>
#include <EEPROM.h>
#include <util/delay.h>
#include "typedef.h"
// Uart 
volatile uint32_t elapsed_time_us = 0;  // Microseconds elapsed
volatile uint8_t* ddrd = (uint8_t*)0x2A;

void UART1_Init(void) {
    // Set baud rate
    // Set PD3 (TX1) as output
    *ddrd |= (1 << 3);

    // Set PD2 (RX1) as input
    *ddrd &= ~(1 << 2);


    baud_rate->UBRR1H = (uint8_t)(UBRR_VALUE >> 8);
    baud_rate->UBRR1L = (uint8_t)UBRR_VALUE;

    // Enable transmitter and receiver
    UCSR1B = (1 << 4) | (1 << 3);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR1C = (1 << 2) | (1 << 1);
}
// Function to enable global interrupts manually
void EnableGlobalInterrupts(void) {
    SREG |= (1 << 7);  // Set the I-bit in the Status Register
}

// Timer0 initialization function
void Timer0_Init(void) {
    TCCR0A = (1 << 1);  // Set WGM01 (CTC Mode)
    TCCR0B = (1 << 0) | (1 << 1);  // Set CS01 and CS00 (Prescaler = 64)
    OCR0A = 250;  // Compare match value for 1ms interval at 16MHz
    TIMSK0 = (1 << 1);  // Enable Output Compare Match A Interrupt (OCIE0A)
    EnableGlobalInterrupts();  // Manually enable global interrupts
}

// Interrupt Service Routine for Timer0 Compare Match
void Timer0_CompareMatch_ISR(void) {
    if (TIFR0 & (1 << 1)) {  // Check if OCF0A flag is set
        elapsed_time_us += 1000;  // Increment elapsed time by 1ms
        TIFR0 |= (1 << 1);  // Clear the interrupt flag by writing 1
    }
}


void UART1_Transmit(char data) {
    while (!(UCSR1A & (1 << 5)));  // Wait until buffer is empty
    UDR1 = data;                       // Send data
}

char UART1_Receive(void) {
    while (!(UCSR1A & (1 << 7)));   // Wait for data to be received
    return UDR1;                    // Return received data
}

volatile uint32_t bit_count = 0;

void UpdateTransmissionSpeed(uint8_t bits_transferred) {
    bit_count += bits_transferred;
}

void PrintTransmissionSpeed(uint32_t byte_start_time, uint32_t byte_end_time) {
    uint32_t elapsed_time = byte_end_time - byte_start_time;  // Time for 1 byte
    if (elapsed_time > 0) {
        uint32_t speed_bps = (10 * 1000000) / elapsed_time;  // 10 bits per byte
        char speed_message[20];
        sprintf(speed_message, "Speed: %lu bps\n", speed_bps);
        for (char* p = speed_message; *p != '\0'; p++) {
            UART1_Transmit(*p);
        }
    }
}

int main(void) {
    char data;
    uint16_t eeprom_address = 0;//EEPROM starts from 0x0000 to 0x0FFF

    UART1_Init();  // Initialize UART1
    Timer0_Init(); // Initialize Timer0
    unsigned int byte_start_time, byte_end_time;

    // Step 1: Receive data from PC and store in EEPROM
    for (uint16 i = 0; i < 1000; i++) {
        byte_start_time = elapsed_time_us;
        data = UART1_Receive();             // Receive data from UART
        //UpdateTransmissionSpeed(10);        // Update bit count (8 data + 1 start + 1 stop bit)
        byte_end_time = elapsed_time_us;
        eeprom_write_byte(eeprom_address++, data);  // Store data in EEPROM
        PrintTransmissionSpeed(byte_start_time, byte_end_time);
    }

    // Reset EEPROM address
    eeprom_address = 0;

    // Step 2: Retrieve data from EEPROM and send it back to PC
    for (uint8 i = 0; i < 1000; i++) {
        byte_start_time = elapsed_time_us;
        data = eeprom_read_byte(eeprom_address++);  // Read data from EEPROM
        UART1_Transmit(data);                       // Transmit data back to PC
        //UpdateTransmissionSpeed(10);              // Update bit count
        byte_end_time = elapsed_time_us;
        PrintTransmissionSpeed(byte_start_time, byte_end_time);  // Print speed
    }

    while (1) {
        // Infinite loop
    }

    return 0;
}

