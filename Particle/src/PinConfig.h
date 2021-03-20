#pragma once


#define ERTC        1
#define SDCARD      1
#define LORA        0
#define COUNTERS    0
#define VOLTAGE     1
#define CURRENT     1
#define TEMPERATURE 1
#define FLOW        1
#define EN_GSM      1


/*
                    _________________________
                   |VIN                   3V3|o
                   |GND                   RST|o
                   |Tx                   VBAT|o
                  o|Rx                    GND|o
                  o|WKP    [ELECTRON]      D7|o
                  o|DAC                    D6|o
       SD(MOSI)-> *|A5:SPI(MOSI)           D5|o
       SD(MISO)-> *|A4:SPI(MISO)           D4|o
        SD(SCK)-> *|A3:SPI(SCK)            D3|o
         SD(SS)-> *|A2:SPI(SS)             D2|o
                  o|A1                     D1|o
                  o|A0                     D0|o
     Water Flow-> *|B5                     C5|o
    Temperature-> *|B4                     C4|o
        Current-> *|B3                     C3|o
        Voltage-> *|B2                     C2|o
                  o|B1                     C1|o
                  o|B0____________________ C0|o

(o): unused.
(*): used.
(x): cannot be used.
(?): WTF.
*/

#if PLATFORM_ID == PLATFORM_ELECTRON
#define BUILTIN_LED D7
#define VOLT_PIN    B2
#define CURRENT_PIN B3
#define TEMP_PIN    B4
#define FLOW_PIN    B5
#define SDCARD_SELECT_PIN SS
#define MAX_V 3.3
#endif


/*
                    ___________________________
                   |RST                      -|o
                   |3V3                      -|o
                   |MD                       -|o
                  o|GND                      -|o
        Voltage-> *|A0       [BORON]       Li+|o
        Current-> *|A1                      EN|o
    Temperature-> *|A2                    VBUS|o
     Water Flow-> *|A3                      D8|o
                  o|A4                      D7|o     
         SD(SS)-> *|A5:SPI(SS)              D6|o
        SD(SCK)-> *|SCK:SPI(SCK)            D5|o
       SD(MOSI)-> *|MO:SPI(MOSI)            D4|o
       SD(MISO)-> *|MI:SPI(MISO)            D3|o
                  o|RX                      D2|o
                  o|TX                     SCL|o
                  o|NC_____________________SDA|o

(o): unused.
(*): used.
(x): cannot be used.
(?): WTF.
*/

#if PLATFORM_ID == PLATFORM_BORON
#define BUILTIN_LED  D7
#define VOLT_PIN     A0
#define CURRENT_PIN  A1
#define TEMP_PIN     A2
#define FLOW_PIN     A3
#define SDCARD_SELECT_PIN  SCK
#define MAX_V 3.3
#endif
