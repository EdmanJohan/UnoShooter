#include "include/pic32mx_io.h"
#include </opt/mcb32tools/include/pic32mx.h>

int main(void) {
  TRISE = 0x0;
  PORTE = 0x0;
  en_btn(1, SET);

  while (1) {
    if (btn_pressed(1))
      en_led(5, SET);
    else
      en_led(5, CLR);
  }

  return 0;
}
