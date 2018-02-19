#ifndef PIC32MX_IO_H_
#define PIC32MX_IO_H_

#define CLR 0
#define SET 1
#define INV 2

void set_led(int led, int state);
void set_btn(int btn, int state);
void set_sw(int sw, int state);

int get_led(int led);
int get_sw(int sw);
int get_btn(int btn);

#endif //PIC32MX_IO_H_