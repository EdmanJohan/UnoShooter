#include "include/display.h"
#include "include/graphics.h"
#include "include/registers.h"

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
#define CLEAR 0xFF

#define SET 1
#define CLR 0

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define PIXEL_UNIT 8
#define HEIGHT 128
#define WIDTH 32
#define PAGES 4
#define TOTAL_PIXELS (HEIGHT * WIDTH / PIXEL_UNIT)

byte buffer[TOTAL_PIXELS];

/**
 * Simple delay function. TO BE REPLACED!
 * @param del Number of cycles to "stall".
 */
void delay(int del) {
    int i;
    for (i = 0; i < del; i++)
        ;
}

/**
 * Transmits buffer of SPI2BUF when SPI2TXB is empty. SPI2RBF receives
 * the buffer SPI2BUF if empty.
 * @param  data     Hexadecimal SPI command
 * @return          SPI buffer
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

    // COMMANDS TO INVERT DISPLAY. SETS DISPLAY ORIGIN TO UPPER LEFT CORNER
    spi_setbyte(SET_SEGMENT_REMAP);
    spi_setbyte(SET_COM_OUTPUT_SCAN_DIR);
    spi_setbyte(SET_COM_PINS_HW_CONFIG);
    spi_setbyte(SET_VCC);
    spi_setbyte(SET_DISPLAY_ON);
}

/**
 * Enables a single pixel on the buffer at a specific location within the
 * 128 x 32 resolution buffer.
 * @param x The x-value of the pixel.
 * @param y The y-value of the pixel.
 */
void set_pixel(int x, int y) {
    int i = y / PIXEL_UNIT;
    buffer[i * HEIGHT + x] |= 1 << (y - i * PIXEL_UNIT);
}

void unset_pixel(int x, int y) {
    int i = y / PIXEL_UNIT;
    buffer[i * HEIGHT + x] &= 0 << (y - i * PIXEL_UNIT);
}

/**
 * [set_object description]
 * @param o [description]
 * @param x [description]
 * @param y [description]
 */
void add(struct Objects o) {
    set_pixel(o.x, o.y);
}

void move(struct Objects o, int dir) {
    switch (dir) {
        case RIGHT:
            // unset_pixel(o.x, o.y);
            o.x++;
            set_pixel(o.x, o.y);
            break;
        case LEFT:
            // unset_pixel(o.x, o.y);
            o.x--;
            set_pixel(o.x, o.y);
            break;
        case UP:
            // unset_pixel(o.x, o.y);
            o.y++;
            set_pixel(o.x, o.y);
            break;
        case DOWN:
            // unset_pixel(o.x, o.y);
            o.y--;
            set_pixel(o.x, o.y);
            break;
    }
}

/**
 * Prints characters on the buffer. NON-FUNCTIONAL!
 * @param x    [description]
 * @param y    [description]
 * @param data [description]
 */
void print(int x, int y, const char* string) {
    int i, j;

    for (i = 0; i < 16; i++)
        for (j = 0; j < 8; i++) {
            if (string[i] & 0x80) continue;
            buffer[y * HEIGHT + x + PIXEL_UNIT * i] =
                font[string[i] * PIXEL_UNIT + j];
        }
}

/**
 * Pushes the buffer[] array to the SPI buffer to be rendered on the buffer,
 * page by page.
 */
void render() {
    int i, j;

    for (i = 0; i < PAGES; i++) {
        PORTFCLR = 1 << 4;  // CLEAR CMD/DATA

        spi_setbyte(0x22);  // SET PRE-CHARGE PERIOD
        spi_setbyte(i);     // SET MEM ADDRESSING MODE.
        spi_setbyte(0x10);  // SET HIGH COLUMN ADDRESS

        PORTFSET = 1 << 4;  // SET CMD/DATA

        for (j = 0; j < HEIGHT; j++) spi_setbyte(~buffer[i * HEIGHT + j]);
    }
}

/**
 * Fills the SPI buffer with 0xFF and the buffer array with 0.
 */
void clear() {
    int i, j;
    for (i = 0; i < PAGES; i++) {
        PORTFCLR = 1 << 4;  // CLEAR CMD/DATA

        spi_setbyte(0x22);  // SET PRE-CHARGE PERIOD
        spi_setbyte(i);     // SET MEM ADDRESSING MODE.
        spi_setbyte(0x10);  // SET HIGH COLUMN ADDRESS

        PORTFSET = 1 << 4;  // SET CMD/DATA

        for (j = 0; j < HEIGHT; j++) spi_setbyte(CLEAR);
    }

    for (i = 0; i < TOTAL_PIXELS; i++) buffer[i] = 0;
}