#include <SPI.h>
#include <LoRa.h>

enum dataType { voltage, current, temperature, flow, count};

struct packet
{
    char type;
    int data;
};

class LoRaTransceiver
{
private:
    const int mk_ss;
    const int mk_rst;
    const int mk_dio0;
    const int mk_syncWord;
public:
    LoRaTransceiver(int ss, int rst, int dio0, int syncWord);
    void initialize();
    void send(packet toSend);
    String receive();
};

LoRaTransceiver::LoRaTransceiver(int ss, int rst, int dio0, int syncWord): 
 mk_ss(ss)
,mk_rst(rst)
,mk_dio0(dio0)
,mk_syncWord(syncWord) 
{}

void LoRaTransceiver::initialize()
{
  //setup LoRa transceiver module
  LoRa.setPins(mk_ss, mk_rst, mk_dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(mk_syncWord);
  Serial.println("LoRa Initializing OK!");
}

void LoRaTransceiver::send(packet toSend)
{
    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print(String(toSend.type) + String(toSend.data, 10));
    LoRa.endPacket();
}

String LoRaTransceiver::receive()
{
    String LoRaData;
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
        LoRaData = LoRa.readString();
        Serial.print(LoRaData);
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    }
    return LoRaData;
}