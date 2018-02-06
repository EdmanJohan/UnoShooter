#include </opt/mcb32tools/include/pic32mx.h>
#include <stdint.h>

#define CLR 0
#define SET 1
#define INV 2

void en_led(int led, int state) {
  led -= 1;
  switch (state) {
  case CLR:
    PORTECLR = 1 << led;
    break;
  case SET:
    PORTESET = 1 << led;
    break;
  case INV:
    PORTEINV = 1 << led;
    break;
  }
}

void en_btn(int btn, int state) {
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

void en_sw(int sw, int state) {
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

int led_set(int led) {
  led -= 1;
  if (PORTE & (1 << led))
    return 1;
  return 0;
}

int sw_toggled(int sw) {
  int n = 7 + sw;

  if (PORTD & (1 << n))
    return 1;
  return 0;
}

int btn_pressed(int btn) {
  int n = 3 + btn;
  switch (btn) {
  case 1:
    if (PORTF & (1 << 1))
      return 1;
    break;
  case 2:
  case 3:
  case 4:
    if (PORTD & (1 << n))
      return 1;
    break;
  }
  return 0;
}
