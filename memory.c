#include "includes/memory.h"
#include "includes/registers.h"

#define WRITE 0xA0
#define READ 0xA1

/* Wait for I2C bus to become idle */
void i2c_idle() {
    while (I2C1CON & 0x1F || I2C1STAT & (1 << 14));  // TRSTAT
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

void write_int(short address, int data) {
    do {
        i2c_start();
    } while (!i2c_send(WRITE));

    byte seg[] = {(data & 0xFF), ((data >> 8) & 0xFF), ((data >> 16) & 0xFF),
                  ((data >> 24) & 0xFF)};

    i2c_send(address >> 8);
    i2c_send(address);

    int i;
    for (i = 0; i < 4; i++) i2c_send(seg[i]);

    i2c_stop();
}

void read_int(short address, int* recv) {
    *recv = 0;

    do {
        i2c_start();
    } while (!i2c_send(WRITE));

    i2c_send(address >> 8);
    i2c_send(address);

    i2c_start();
    i2c_send(READ);

    int i;
    for (i = 0; i < 4; i++) {
        *recv |= i2c_recv();
        i2c_ack();
    }

    i2c_nack();
    i2c_stop();
}

void write_char(short address, char* data, int len) {
    do {
        i2c_start();
    } while (!i2c_send(WRITE));

    i2c_send(address >> 8);
    i2c_send(address);

    while (len > 0) {
        i2c_send(*data);
        data++;
        len--;
    }

    i2c_stop();
}

void read_char(short address, char* recv, int len) {
    do {
        i2c_start();
    } while (!i2c_send(WRITE));

    i2c_send(address >> 8);
    i2c_send(address);

    i2c_start();
    i2c_send(READ);

    while (len + 1 > 0) {
        *recv = i2c_recv();
        i2c_idle();
        i2c_ack();

        recv++;
        len--;
    }

    *recv = i2c_recv();
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
