#ifndef _MEMORY_H_
#define _MEMORY_H_

typedef unsigned char byte;

/* Wait for I2C bus to become idle */
void i2c_idle();

/* Send one byte on I2C bus, return ack/nack status of transaction */
int i2c_send(byte data);

/* Receive one byte from I2C bus */
byte i2c_recv();

/* Send acknowledge conditon on the bus */
void i2c_ack();

/* Send not-acknowledge conditon on the bus */
void i2c_nack();

/* Send start conditon on the bus */
void i2c_start();

/* Send restart conditon on the bus */
void i2c_restart();

/* Send stop conditon on the bus */
void i2c_stop();

/**
 * Writes an integer to the EEPROM.
 * @param address The 16-bit address to send to the EEPROM.
 * @param data    The integer to send.
 */
void write_int(short address, int data);

/**
 * Reads an integer from the EEPROM.
 * @param  address The 16-bit address to read from the EEPROM.
 * @return         The integer.
 */
int read_int(short address);

/**
 * Writes a character to the EEPROM.
 * @param address The 16-bit address to send to the EEPROM.
 * @param data    The character to send.
 */
void write_char(short address, char* data, int len);

/**
 * Reads a character from the EEPROM.
 * @param address   The 16-bit address to read from the EEPROM.
 * @return          The character.
 */
char* read_char(short address, int len);

/* Set up i2c */
void i2c_init();

#endif  // _MEMORY_H_