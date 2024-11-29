#ifndef EEPROM_H

#define EECR        (*(volatile uint8_t*)0x3F)
#define EEARH       (*(volatile unsigned char*)0x42)  // EEPROM Address Register High Byte
#define EEARL       (*(volatile unsigned char*)0x41)  // EEPROM Address Register Low Byte
#define EEDR        (*(volatile unsigned char*)0x40)  // EEPROM Data Register
#define EECR        (*(volatile unsigned char*)0x3F)  // EEPROM Control Register

void eeprom_write_byte(uint16 address, uint8 data);
uint8 eeprom_read_byte(uint16 address);

#endif