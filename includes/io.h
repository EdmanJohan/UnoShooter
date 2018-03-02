#ifndef _IO_H_
#define _IO_H_

#define CLR 0
#define SET 1
#define INV 2


void init_pin();

void init_input();
/**
 * Sets state of LED light at specified position.
 * Will also modify whether input/output.
 *
 * @param led   LED number, 1 ... 8
 * @param state CLR, SET or INV
 */
void set_led(int led, int state);

/**
 * Sets state of button at specified position.
 * @param btn   Button number, 1 ... 4
 * @param state CLR, SET or INV
 */
void set_btn(int btn, int state);

/**
 * Sets state of switch at specified position.
 * @param led   Switch number, 1 ... 4
 * @param state CLR, SET or INV
 */
void set_sw(int sw, int state);

/**
 * Returns the status of LED at specified position.
 * @param led 	LED number, 1 ... 8
 * @return  	1 if on, 0 if off.
 */
int get_led(int led);

/**
 * Returns the status of switch at specified position.
 * @param led 	Switch number, 1 ... 4
 * @return  	1 if on, 0 if off.
 */
int get_sw(int sw);

/**
 * Returns the status of button at specified position.
 * @param led 	Button number, 1 ... 4
 * @return  	1 if on, 0 if off.
 */
int get_btn(int btn);

#endif  // _IO_H_
