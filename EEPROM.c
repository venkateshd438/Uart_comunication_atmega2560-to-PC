#include "EEPROM.h"
#include "typedef.h"

void eeprom_write_byte(uint16 address, uint8 data) {
    // Ensure the address is within the valid EEPROM range
    if (address >= 0x1000) {
        return;  // Invalid address, do nothing
    }

    // Wait for completion of previous EEPROM write
    while (EECR & (1 << 2)) {
        // Wait until EEPE is cleared
    }

    // Set up the address and data registers
    EEARH = (address >> 8) & 0xFF;;  // Set the EEPROM Address Register
    EEARL = address & 0xFF;          //  low byte of the address to EEARL
    EEDR = data;                    // Set the EEPROM Data Register

    // Write logical 1 to the EEMPE bit (EEPROM Master Program Enable)
    EECR |= (1 << 2);

    // Start the EEPROM write by setting the EEPE bit
    EECR |= (1 << 1);
}

uint8 eeprom_read_byte(uint16 address) {
    // Ensure the address is within range
    if (address > 0x0FFF) {
        return 0xFF;  // Return default value for invalid address
    }

    // Wait for completion of any ongoing write
    while (EECR & (1 << 1));

    // Set up address registers
    EEARH = (uint8)(address >> 8);
    EEARL = (uint8)(address & 0xFF);

    // Start read operation
    EECR |= 1u;

    // Return data from EEPROM Data Register
    return EEDR;
}
