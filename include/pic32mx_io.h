//
// Created by Johan on 06/02/2018.
//

#ifndef PIC32MX_IO_H_
#define PIC32MX_IO_H_

#define CLR 0
#define SET 1
#define INV 2


void en_led(int led, int state);
void en_btn(int btn, int state);
void en_sw(int sw, int state);

int led_set(int led);
int sw_toggled(int sw);
int btn_pressed(int btn);


#endif //UNOCLIMBER_PIC32MX_IO_H