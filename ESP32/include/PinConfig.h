/*
                    _________________________
                   |EN          MOSI:VSPI:D23|* -> SD:MOSI
                  ?|D36:VP        SCL:I2C:D22|* -> RTC:SCL
                  ?|D39:VN   [ESP32]   TX0:D1|x
    Water Flow <- *|D34                RX0:D3|x
       Current <- *|D35           SDA:I2C:D21|* -> RTC:SDA
       Voltage <- *|D32         MISO:VSPI:D19|* -> SD:MISO
                  o|D33          CLK:VSPI:D18|* -> SD:SCK
                  o|D25            CS:VSPI:D5|* -> SD:SS
     LoRa:DIO0 <- *|D26               TX2:D17|o
    LoRa:RESET <- *|D27               RX2:D16|o
      LoRa:SCK <- *|D14:HSPI:CLK           D4|* -> Temperature
     LoRa:MISO <- *|D12:HSPI:MISO          D2|* -> BUILTIN_LED
     LoRa:MOSI <- *|D13:HSPI:MOSI CS:HSPI:D15|* -> LoRa:NSS
                  o|GND                   GND|*
                  o|VIN___________________3V3|*

(o): unused.
(*): used.
(x): cannot be used.
(?): WTF.
*/

#pragma once

// Hardware connected to ESP32 is true. Otherwise false.
#define ERTC      1
#define SDCARD    1
#define LORA      0
#define COUNTERS  0
#define VOLTAGE   0
#define CURRENT   0
#define TEMP      0
#define FLOW      0

// Pins Definitions:
#define SDCARD_SPI_INTERFACE VSPI
#define SDCARD_SELECT_PIN 5

#define LORA_SPI_INTERFACE HSPI
#define LORA_SELECT_PIN 15
#define LORA_RST_PIN 27
#define LORA_DIO0_PIN 26
#define LORA_SECRET_WORD 0xF3

#define CURRENT_PIN 35

#define VOLT_PIN 32

#define TEMP_PIN 4

#define FLOW_PIN 34

#define MAX_V 3.9

#define LED_BUILTIN 2
