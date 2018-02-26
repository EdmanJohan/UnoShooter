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
 * Simple delay function. TO BE REPLACED!
 * @param del Number of cycles to "stall".
 */
void delay(int del) {
        int i;
        for (i = 0; i < del; i++)
                ;
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
        buffer[i * HEIGHT + x] &= ~(1 << (y - i * PIXEL_UNIT));
}

/**
 * Initializes Timer2 with 1:256 pre-scaling, at 1 s.
 */
void init_timer() {
        T2CON = 0x0; // STOP TIMER & CLEAR CONTROL REGISTERS
        PR2 = 0x100; // PERIOD INITALIZED TO 31250, 0x7A12

        IPCSET(2) = 0xD; // SET INTERRUPT PRIORITY 3-1
        IECSET(0) = 0x900; // ENABLE INTERRUPTS

        T2CONSET = 0x70; // 1:256 PRESCALE
        TMR2 = 0x0;   // CLEAR TIMER REGISTER
}

/**
 * Starts Timer2.
 */
void start_timer() {
        T2CONSET = 0x8000;
}

/**
 * Stops Timer2.
 */
void stop_timer() {
        T2CONCLR = 0x8000;
}

int next_frame() {
        if (counter > 10) {
                counter = 0;
                return 1;
        }

        if (IFS(0) & 0x100) {
                counter++;
                IFSCLR(0) = 0x100; // CLEAR TIMER INTERRUPT STATUS FLAG
                return 0;
        }

        return 0;
}

/**
 * Transmits buffer of SPI2BUF when SPI2TXB is empty. SPI2RBF receives
 * the buffer SPI2BUF if empty.
 * @param  data     Hexadecimal SPI command
 * @return          SPI buffer
 */
byte spi_setbyte(byte data) {
        while (!(SPI2STAT & 0x08)) // SPI2TXB
                ;
        SPI2BUF = data;
        while (!(SPI2STAT & 0x01)) // SPI2RBF
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

        AD1PCFG = 0xFFFF; // SET DIGITAL PIN

        // SET PINS FOR DISPLAY SIGNALS
        PORTF = 0xFFFF;
        ODCF = 0x0;     // SET NORMAL DIGITAL OUTPUT (ELSE OPEN DRAIN OUTPUT)
        ODCG = 0x0;     // SET NORMAL DIGITAL OUTPUT (ELSE OPEN DRAIN OUTPUT)
        TRISFCLR = 1 << 4; // SET VDD TO OUTPUT
        TRISFCLR = 1 << 5; // SET VBAT TO OUTPUT
        TRISFCLR = 1 << 6; // SET DATA/CMD TO OUTPUT
        TRISGCLR = 1 << 9; // SET RESET TO OUTPUT

        // SET SPI CONTROLLER
        SPI2CON = 0;       // CLEAR SPI CONTROLLER PORT 2
        SPI2BRG = 1 << 2;  // BAUD RATE DIVISOR 64
        SPI2STATCLR = 1 << 6; // CLEAR SPIROV OVERFLOW FLAG
        SPI2CONSET = 1 << 5; // SET CKP CLOCK POLARITY,
                             // STATE: ACTIVE LOW, IDLE HIGH
        SPI2CONSET = 1 << 6; // SET MSTEN BIT, 1 = MASTER, ELSE SLAVE
        SPI2CONSET = 1 << 15; // ENABLE SPI
}

/**
 * Initializes the SPI controller and the Basic I/O Shield OLED buffer.
 */
void init_display() {
        init_spi();

        PORTFCLR = 1 << 4; // CLEAR DATA/CMD BIT
        delay(100);
        PORTFCLR = 1 << 6; // TURN VDD ON
        delay(1000000); // WAIT

        spi_setbyte(SET_DISPLAY_OFF);

        PORTGCLR = 1 << 9;
        delay(10);
        PORTGSET = 1 << 9; // RESET THE RESET BIT
        delay(10);

        spi_setbyte(SET_CHARGE_PUMP_1);
        spi_setbyte(SET_CHARGE_PUMP_2);

        spi_setbyte(SET_PRECHARGE_PERIOD_1);
        spi_setbyte(SET_PRECHARGE_PERIOD_2);

        PORTFCLR = 1 << 5; // TURN VCC ON
        delay(1000000); // WAIT

        // COMMANDS TO FLIP DISPLAY. SETS DISPLAY ORIGIN TO UPPER LEFT CORNER
        spi_setbyte(SET_SEGMENT_REMAP);
        spi_setbyte(SET_COM_OUTPUT_SCAN_DIR);
        spi_setbyte(SET_COM_PINS_HW_CONFIG);
        spi_setbyte(SET_VCC);
        spi_setbyte(SET_DISPLAY_ON);
}

/**
 * Prints characters on the buffer. NON-FUNCTIONAL!
 * @param x    [description]
 * @param y    [description]
 * @param data [description]
 */
void print(int x, int y, const char* string) {
        int i, j;

        for (i = 0; i < sizeof(string) / sizeof(char); i++) {
                for (j = 0; j < PIXEL_UNIT; j++) {
                        buffer[y * 128 + x + i * PIXEL_UNIT + j] =
                                font[string[i] * PIXEL_UNIT + j];
                }
        }
}

/**
 * Pushes the buffer[] array to the SPI buffer to be rendered on the buffer,
 * page by page.
 */
void render() {
        int i, j;

        for (i = 0; i < PAGES; i++) {
                PORTFCLR = 1 << 4; // CLEAR CMD/DATA

                spi_setbyte(0x22); // SET PRE-CHARGE PERIOD
                spi_setbyte(i); // SET MEM ADDRESSING MODE.
                spi_setbyte(0x10); // SET HIGH COLUMN ADDRESS

                PORTFSET = 1 << 4; // SET CMD/DATA

                for (j = 0; j < HEIGHT; j++) spi_setbyte(buffer[i * HEIGHT + j]);
        }
}

/**
 * Fills the SPI buffer with 0xFF and the buffer array with 0.
 */
void clear() {
        int i, j;

        for (i = 0; i < PAGES; i++) {
                PORTFCLR = 1 << 4; // CLEAR CMD/DATA

                spi_setbyte(0x22); // SET PRE-CHARGE PERIOD
                spi_setbyte(i); // SET MEM ADDRESSING MODE.
                spi_setbyte(0x10); // SET HIGH COLUMN ADDRESS

                PORTFSET = 1 << 4; // SET CMD/DATA

                for (j = 0; j < HEIGHT; j++) spi_setbyte(CLEAR);
        }

        for (i = 0; i < TOTAL_PIXELS; i++) buffer[i] = 0;
}
