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
    switch (state) {
        case CLR:
            TRISESET = 1 << (led - 1);
            PORTECLR = 1 << (led - 1);
            break;
        case SET:
            TRISECLR = 1 << (led - 1);
            PORTESET = 1 << (led - 1);
            break;
        case INV:
            if (PORTE & 1 << (led - 1))
                TRISESET = 1 << (led - 1);
            else
                TRISECLR = 1 << (led - 1);
            PORTEINV = 1 << (led - 1);
            break;
    }
}

/**
 * Sets state of button at specified position.
 * @param btn   Button number, 1 ... 4
 * @param state CLR, SET or INV
 */
void set_btn(int btn, int state) {
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
            TRISDCLR = 1 << (btn + 3);
            break;
        case SET:
            TRISDSET = 1 << (btn + 3);
            break;
        case INV:
            TRISDINV = 1 << (btn + 3);
            break;
    }
}

/**
 * Sets state of switch at specified position.
 * @param led   Switch number, 1 ... 4
 * @param state CLR, SET or INV
 */
void set_sw(int sw, int state) {
    switch (state) {
        case CLR:
            PORTDCLR = 1 << (sw + 7);
            break;
        case SET:
            PORTDSET = 1 << (sw + 7);
            break;
        case INV:
            PORTDINV = 1 << (sw + 7);
            break;
    }
}

/**
 * Returns the status of LED at specified position.
 * @param led   LED number, 1 ... 8
 * @return      1 if on, 0 if off.
 */
int get_led(int led) { return PORTE & (1 << (led - 1)); }

/**
 * Returns the status of switch at specified position.
 * @param led   Switch number, 1 ... 4
 * @return      1 if on, 0 if off.
 */
int get_sw(int sw) { return PORTD & (1 << (sw + 7)); }

/**
 * Returns the status of button at specified position.
 * @param led   Button number, 1 ... 4
 * @return      1 if on, 0 if off.
 */
int get_btn(int btn) {
    if (btn == 1)
        return PORTF & (1 << 1);
    else
        return PORTD & (1 << (btn + 3));
}