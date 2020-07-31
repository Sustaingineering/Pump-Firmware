/*
                    _________________________
                   |EN          MOSI:VSPI:D23|* -> SD:MOSI
                  ?|D36:VP        SCL:I2C:D22|* -> RTC:SCL
                  ?|D39:VN   [ESP32]   TX0:D1|x
                  o|D34                RX0:D3|x
                  o|D35           SDA:I2C:D21|* -> RTC:SDA
                  o|D32         MISO:VSPI:D19|* -> SD:MISO
                  o|D33          CLK:VSPI:D18|* -> SD:SCK
                  o|D25            CS:VSPI:D5|* -> SD:SS
     LoRa:DIO0 <- *|D26               TX2:D17|o
    LoRa:RESET <- *|D27               RX2:D16|o
      LoRa:SCK <- *|D14:HSPI:CLK           D4|o
     LoRa:MISO <- *|D12:HSPI:MISO          D2|* -> BUILTIN_LED
     LoRa:MOSI <- *|D13:HSPI:MOSI CS:HSPI:D15|* -> LoRa:NSS
                  o|GND                   GND|*
                  o|VIN___________________3V3|*

(o): unused.
(*): used.
(x): cannot be used.
(?): WTF.
*/

// Hardware connected to ESP32 is true. Otherwise false.
#define RTC       1
#define SDCARD    1
#define LORA      1
#define GSM       0
#define COUNTERS  1
#define VOLTAGE   0
#define CURRENT   0
#define TEMP      0
#define FLOW      0