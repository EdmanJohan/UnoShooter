#include "includes/display.h"
#include "includes/graphics.h"
#include "includes/registers.h"

#define SET_DISPLAY_OFF 0xAE
#define SET_CHARGE_PUMP_1 0x8D
#define SET_CHARGE_PUMP_2 0x14
#define SET_PRECHARGE_PERIOD_1 0xD9
#define SET_PRECHARGE_PERIOD_2 0xF1
#define SET_SEGMENT_REMAP 0xA1
#define SET_COM_OUTPUT_SCAN_DIR 0xC8
#define SET_COM_PINS_HW_CONFIG 0xDA
#define SET_VCC 0x20
#define SET_DISPLAY_ON 0xAF
#define CLEAR 0x00

static int counter = 0;

/**
 * Simple delay function.
 * @param del Number of cycles to "stall".
 * @author Alex Diaz
 */
void delay(int del) {
    int i;
    for (i = 0; i < del; i++);
}

/**
 * Enables a single pixel on the buffer at a specific location within the
 * 128 x 32 resolution buffer.
 * @param x The x-value of the pixel.
 * @param y The y-value of the pixel.
 * @author  Alex Diaz
 */
void set_pixel(int x, int y, int state) {
    int i = y / PIXEL_UNIT;

    if (state)
        buffer[i * HEIGHT + x] |= 1 << (y - i * PIXEL_UNIT);
    else
        buffer[i * HEIGHT + x] &= ~(1 << (y - i * PIXEL_UNIT));
}

/**
 * Initializes Timer2 with 1:256 pre-scaling, at 1 s.
 * Author: Alex Diaz, modified by Johan Edman
 */
void init_timer(int t) {
    switch (t) {
        case 2:
            T2CON = 0;    // STOP TIMER & CLEAR CONTROL REGISTERS
            PR2 = 0x100;  // PERIOD INITALIZED TO 31250, 0x7A12

            IPCSET(2) = 0xD;    // SET INTERRUPT PRIORITY 3-1
            IECSET(0) = 0x900;  // ENABLE INTERRUPTS

            T2CONSET = 0x70;  // 1:256 PRESCALE
            TMR2 = 0;         // CLEAR TIMER REGISTER
            break;
        case 3:
            T3CON = 0;
            PR3 = 312500;

            IPC(3) = 0x1B;       // IEC Bit 16
            IECSET(0) = 0x1000;  //

            T3CONSET = 0x70;
            TMR3 = 0;
            break;
    }
}

/**
 * Starts a Timer
 * Author: Alex Diaz, modified by Johan Edman
 */
void start_timer(int t) {
    switch (t) {
        case 2:
            T2CONSET = 0x8000;
            break;
        case 3:
            T3CONSET = 0x8000;
            break;
    }
}

/**
 * Stops a Timer.
 * Author: Alex Diaz, modified by Johan Edman
 */
void stop_timer(int t) {
    switch (t) {
        case 2:
            T2CONCLR = 0x8000;
            break;
        case 3:
            T3CONCLR = 0x8000;
            break;
    }
}

/**
 * Keeps the counter variable.
 * @return 1 is counter is full, else 0.
 * @author Alex Diaz
 */
int next_frame() {
    if (counter > 10) {
        counter = 0;
        return 1;
    }

    if (IFS(0) & 0x100) {
        counter++;
        IFSCLR(0) = 0x100;  // CLEAR TIMER INTERRUPT STATUS FLAG
        return 0;
    }

    return 0;
}

/**
 * Transmits buffer of SPI2BUF when SPI2TXB is empty. SPI2RBF receives
 * the buffer SPI2BUF if empty.
 * @param  data     Hexadecimal SPI command
 * @return          SPI buffer
 * @author          Fredrik Lundevall
 */
byte spi_setbyte(byte data) {
    while (!(SPI2STAT & 0x08))  // SPI2TXB
        ;
    SPI2BUF = data;
    while (!(SPI2STAT & 0x01))  // SPI2RBF
        ;
    return SPI2BUF;
}

/**
 * Initializes host SPI interface for the Basic I/O Shield.
 * Author: Fredrik Lundevall
 */
void init_spi() {
    // SET PERIPHERAL BUS CLOCK
    OSCCONCLR = 1 << 19;
    OSCCONCLR = 1 << 20;
    OSCCONSET = 1 << 19;

    AD1PCFG = 0xFFFF;  // SET DIGITAL PIN

    // SET PINS FOR DISPLAY SIGNALS
    PORTF = 0xFFFF;
    ODCF = 0x0;         // SET NORMAL DIGITAL OUTPUT (ELSE OPEN DRAIN OUTPUT)
    ODCG = 0x0;         // SET NORMAL DIGITAL OUTPUT (ELSE OPEN DRAIN OUTPUT)
    TRISFCLR = 1 << 4;  // SET VDD TO OUTPUT
    TRISFCLR = 1 << 5;  // SET VBAT TO OUTPUT
    TRISFCLR = 1 << 6;  // SET DATA/CMD TO OUTPUT
    TRISGCLR = 1 << 9;  // SET RESET TO OUTPUT

    // SET SPI CONTROLLER
    SPI2CON = 0;           // CLEAR SPI CONTROLLER PORT 2
    SPI2BRG = 1 << 2;      // BAUD RATE DIVISOR 64
    SPI2STATCLR = 1 << 6;  // CLEAR SPIROV OVERFLOW FLAG
    SPI2CONSET = 1 << 5;   // SET CKP CLOCK POLARITY,
                           // STATE: ACTIVE LOW, IDLE HIGH
    SPI2CONSET = 1 << 6;   // SET MSTEN BIT, 1 = MASTER, ELSE SLAVE
    SPI2CONSET = 1 << 15;  // ENABLE SPI
}

/**
 * Initializes the SPI controller and the Basic I/O Shield OLED buffer.
 * Author: Fredrik Lundevall
 */
void init_display() {
    init_spi();

    PORTFCLR = 1 << 4;  // CLEAR DATA/CMD BIT
    delay(100);
    PORTFCLR = 1 << 6;  // TURN VDD ON
    delay(1000000);     // WAIT

    spi_setbyte(SET_DISPLAY_OFF);

    PORTGCLR = 1 << 9;
    delay(10);
    PORTGSET = 1 << 9;  // RESET THE RESET BIT
    delay(10);

    spi_setbyte(SET_CHARGE_PUMP_1);
    spi_setbyte(SET_CHARGE_PUMP_2);

    spi_setbyte(SET_PRECHARGE_PERIOD_1);
    spi_setbyte(SET_PRECHARGE_PERIOD_2);

    PORTFCLR = 1 << 5;  // TURN VCC ON
    delay(1000000);     // WAIT

    // COMMANDS TO FLIP DISPLAY. SETS DISPLAY ORIGIN TO UPPER LEFT CORNER
    spi_setbyte(SET_SEGMENT_REMAP);
    spi_setbyte(SET_COM_OUTPUT_SCAN_DIR);
    spi_setbyte(SET_COM_PINS_HW_CONFIG);
    spi_setbyte(SET_VCC);
    spi_setbyte(SET_DISPLAY_ON);
}

/**
 * Prints characters on the display.
 * @param x    Initial x-value position
 * @param line Row position, 0 ... 3
 * @param data The string to print
 * @author Fredrik Lundevall, modified by Alex Diaz
 */
void print(int x, int line, const char* string, const int len) {
    int i, j;

    for (i = 0; i < len; i++)
        for (j = 0; j < PIXEL_UNIT; j++)
            buffer[line * 128 + x + i * PIXEL_UNIT + j] =
                font[string[i] * PIXEL_UNIT + j];
}

/**
 * Pushes the buffer[] array to the SPI buffer to be rendered on the buffer,
 * page by page.
 * Author: Fredrik Lundevall, modified by Alex Diaz
 */
void render() {
    int i, j;

    for (i = 0; i < PAGES; i++) {
        PORTFCLR = 1 << 4;  // CLEAR CMD/DATA

        spi_setbyte(0x22);  // SET PRE-CHARGE PERIOD
        spi_setbyte(i);     // SET MEM ADDRESSING MODE.
        spi_setbyte(0x10);  // SET HIGH COLUMN ADDRESS

        PORTFSET = 1 << 4;  // SET CMD/DATA

        for (j = 0; j < HEIGHT; j++) spi_setbyte(buffer[i * HEIGHT + j]);
    }
}

/**
 * Fills the display array with 0s.
 * Author: Alex Diaz
 */
void clear() {
    int i;
    for (i = 0; i < TOTAL_PIXELS; i++) buffer[i] = 0;
}
