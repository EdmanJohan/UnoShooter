#include "include/io.h"
#include "include/registers.h"

/**
 * Sets state of LED light at specified position.
 * Will also modify whether input/output.
 *
 * @param led   LED number, 1 ... 8
 * @param state CLR, SET or INV
 */
void set_led(int led, int state) {
    led -= 1;
    switch (state) {
        case CLR:
            TRISESET = 1 << led;
            PORTECLR = 1 << led;
            break;
        case SET:
            TRISECLR = 1 << led;
            PORTESET = 1 << led;
            break;
        case INV:
            if (PORTE & 1 << led)
                TRISESET = 1 << led;
            else
                TRISECLR = 1 << led;
            PORTEINV = 1 << led;
            break;
    }
}

/**
 * Sets state of button at specified position.
 * @param btn   Button number, 1 ... 4
 * @param state CLR, SET or INV
 */
void set_btn(int btn, int state) {
    int n = 3 + btn;
    if (btn == 1) {
        switch (state) {
            case CLR:
                TRISFCLR = 1 << 1;
                break;
            case SET:
                TRISFSET = 1 << 1;
                break;
            case INV:
                TRISFINV = 1 << 1;
                break;
        }
    }

    switch (state) {
        case CLR:
            TRISDCLR = 1 << n;
            break;
        case SET:
            TRISDSET = 1 << n;
            break;
        case INV:
            TRISDINV = 1 << n;
            break;
    }
}

/**
 * Sets state of switch at specified position.
 * @param led   Switch number, 1 ... 4
 * @param state CLR, SET or INV
 */
void set_sw(int sw, int state) {
    int n = 7 + sw;

    switch (state) {
        case CLR:
            PORTDCLR = 1 << n;
            break;
        case SET:
            PORTDSET = 1 << n;
            break;
        case INV:
            PORTDINV = 1 << n;
            break;
    }
}

/**
 * Returns the status of LED at specified position.
 * @param led   LED number, 1 ... 8
 * @return      1 if on, 0 if off.
 */
int get_led(int led) {
    led -= 1;
    if (PORTE & (1 << led)) return 1;
    return 0;
}

/**
 * Returns the status of switch at specified position.
 * @param led   Switch number, 1 ... 4
 * @return      1 if on, 0 if off.
 */
int get_sw(int sw) {
    int n = 7 + sw;

    if (PORTD & (1 << n)) return 1;
    return 0;
}

/**
 * Returns the status of button at specified position.
 * @param led   Button number, 1 ... 4
 * @return      1 if on, 0 if off.
 */
int get_btn(int btn) {
    int n = 3 + btn;
    switch (btn) {
        case 1:
            if (PORTF & (1 << 1)) return 1;
            break;
        case 2:
        case 3:
        case 4:
            if (PORTD & (1 << n)) return 1;
            break;
    }
    return 0;
}