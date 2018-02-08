//
// Created by Johan on 06/02/2018.
//

#ifndef PIC32MX_IO_H_
#define PIC32MX_IO_H_

#define CLR 0
#define SET 1
#define INV 2

/* Definitions for Display PORTs. and operations. */
#define DISP_CMD_MD    (PORTFCLR = 0x10)    // PIN 39        RF4        | OLED DATA/CMD SELECT
#define DISP_DATA_MD   (PORTFSET = 0x10)   

#define DISP_RES_EN    (PORTGCLR = 0x200)   // PIN 10        RG9        | OLED RESET
#define DISP_RES_DIS   (PORTGSET = 0x200)

#define DISP_VDD_EN    (PORTFCLR = 0x40)    // PIN 38        RF6        | OLED VDD ENABLE
#define DISP_VDD_DIS   (PORTFSET = 0x40)

#define DISP_VBAT_EN   (PORTFCLR = 0x20)    // PIN 40        RF5        | OLED VBAT ENABLE
#define DISP_VBAT_DIS  (PORTFSET = 0x20)

/* #define DISP_SDIN                        // PIN 11        RG7/RG8    | OLED SERIAL DATA IN */
/* #define DISP_SCLK                        // PIN 13        RG6        | OLED SERIAL CLOCK   */


/* Forward declaration of ENABLE-functions for LEDs, Buttons and Switches */
void set_led(int led, int state);
void set_btn(int btn, int state);
void set_sw(int sw, int state);

/* Froward declaration of STATE-functions for LEDs, Buttons and Switches */
int led_state(int led);
int sw_state(int sw);
int btn_state(int btn);




#endif //UNOCLIMBER_PIC32MX_IO_H
