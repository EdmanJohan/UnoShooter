#### UNO32 PORTS

```
TRISx DEFINES INPUT(1)/OUTPUT(0) [Unused should be left as INPUT] 
```

|  I/O   |   #   |              PIN              |  PORT   |     RANGE     |    ENABLE(ALL)     |
| :---:  | :---: |             :---:             |  :---:  |     :---:     |       :----:       |
| LEDs   |   8   |     `26 27 28 29 30 31 32`    | `PORTE` |  `RE0 -> RE7` |     `TRISE = 0`    |
| BTNs   |   4   |              `4`              | `PORTF` |     `RF1`     |    `TRISF = 0x1`   |
|        |       |          `34 36 37`           | `PORTD` |  `RD5 -> RD7` |    `TRISD = 0xe0`  |
| SWT    |   4   |          `2 7 8 35`           | `PORTD` | `RD8 -> RD11` |   `TRISD = 0x8e0`  |

|  PORT  | # BITS  |
| :---:  |  :---:  |
|   B    |   16    |
|   D    |   12    |
|   C    |    4    |
|   E    |    8    |
|   F    |    7    |
|   G    |    6    |


Port Manipulation can be done with 
```
PORTxbits.Rx = PORTxbits.Rx;
```
```
PORTxSET = 0b [...];
PORTxCLR = 0b [...];
```
