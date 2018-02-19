#ifndef PIC32MX_DISP_H_
#include "pic32mx.h"

#define OLED_PAGE_MAX 4
typedef unsigned char byte;

char textbuffer[4][16];
void sleep(int ms);
byte spi_put_byte(byte data);
void oled_host_init();
void oled_disp_init();
void init_display();
void oled_update();
void print_string(int line, char *string);
void print_image(int line, const byte *data);
void remove_image(int line, const byte *data);

#endif
