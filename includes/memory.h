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

void write_int(short address, int data);

void read_int(short address, int* recv);

void write_char(short address, char* data, int len);

void read_char(short address, char* recv, int len);

/* Set up i2c */
void i2c_init();

#endif  // _MEMORY_H_