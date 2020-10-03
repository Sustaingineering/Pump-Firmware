#pragma once

/*
                    _________________________
                   |VIN                   3V3|o
                   |GND                   RST|o
                   |Tx                   VBAT|o
                  o|Rx                    GND|o
                  o|WKP    [Particle]      D7|o
                  o|DAC                    D6|o
       SD(MOSI)-> *|A5:SPI:MOSI            D5|o
       SD(MISO)-> *|A4:SPI:MISO            D4|o
        SD(SCK)-> *|A3:SPI(SCK)            D3|o
         SD(SS)-> *|A2:SPI(SS)             D2|o
                  o|A1                     D1|o
                  o|A0                     D0|o
                  o|B5                     C5|o
    Temperature-> o|B4                     C4|o
        Current-> o|B3                     C3|o
        Voltage-> *|B2                     C2|o
                  o|B1                     C1|o
                  o|B0____________________ C0|o

(o): unused.
(*): used.
(x): cannot be used.
(?): WTF.
*/

#define electron
#define BUILTIN_LED D7
#define ERTC      1
#define SDCARD    1
#define LORA      0
#define COUNTERS  0
#define VOLTAGE   1
#define CURRENT   1
#define TEMP      1
#define FLOW      0

#define VOLT_PIN    B2
#define CURRENT_PIN B3
#define TEMP_PIN    B4
#define SDCARD_SELECT_PIN SS
#define MAX_V 3.3
