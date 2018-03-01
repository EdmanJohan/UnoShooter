#include "includes/memory.h"
#include "includes/registers.h"

/* Wait for I2C bus to become idle */
void i2c_idle() {
    while (I2C1CON & 0x1F || I2C1STAT & (1 << 14))
        ;  // TRSTAT
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
int i2c_send(byte data) {
    i2c_idle();
    I2C1TRN = data & 0xFF;
    i2c_idle();
    return !(I2C1STAT & (1 << 15));  // ACKSTAT
}

/* Receive one byte from I2C bus */
byte i2c_recv() {
    i2c_idle();
    I2C1CONSET = 1 << 3;  // RCEN = 1
    i2c_idle();
    I2C1STATCLR = 1 << 6;  // I2COV = 0
    return I2C1RCV;
}

/* Send acknowledge conditon on the bus */
void i2c_ack() {
    i2c_idle();
    I2C1CONCLR = 1 << 5;  // ACKDT = 0
    I2C1CONSET = 1 << 4;  // ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
    i2c_idle();
    I2C1CONSET = 1 << 5;  // ACKDT = 1
    I2C1CONSET = 1 << 4;  // ACKEN = 1
}

/* Send start conditon on the bus */
void i2c_start() {
    i2c_idle();
    I2C1CONSET = 1;  // SEN
    i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
    i2c_idle();
    I2C1CONSET = 1 << 1;  // RSEN
    i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
    i2c_idle();
    I2C1CONSET = 1 << 2;  // PEN
    i2c_idle();
}

void write_data(unsigned short address, char* data, int i) {
    while (!i2c_send(0x50 << 1)) i2c_start();

    i2c_send(address >> 8);
    i2c_send(address & 0xFF);

    while (i > 0) {
        i2c_send(*data);
        data++;
        i--;
    }

    i2c_stop();
}

void read_data(unsigned short address, char* rec, int i) {
    while (!i2c_send(0x50 << 1)) i2c_start();

    i2c_send(address >> 8);
    i2c_send(address & 0xFF);

    i2c_start();
    i2c_send(0x50 << 1 | 1);

    while (i > 0) {
        *rec = i2c_recv();
        i2c_idle();

        rec++;
        i--;
    }

    i2c_nack();
    i2c_stop();
}

/* Set up i2c */
void i2c_init() {
    I2C1CON = 0x0;
    /* I2C Baud rate should be less than 400 kHz, is generated by dividing
    the 40 MHz peripheral bus clock down */
    I2C1BRG = 0x0C2;
    I2C1STAT = 0x0;
    I2C1CONSET = 1 << 13;  // SIDL = 1
    I2C1CONSET = 1 << 15;  // ON = 1

    // write_data(0x1234, 0, 1);
}