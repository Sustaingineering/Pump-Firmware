/*
 * RESET -> Orange -> D27
 * NSS   -> Yellow -> D15
 * SCK   -> Grey   -> D14
 * MOSI  -> Green  -> D13
 * MISO  -> Blue   -> D12
 * DIO0  -> White  -> D26
 */

#include <SPI.h>
#include <LoRa.h>

//enum dataType { voltage, current, temperature, flow, count};

struct packet
{
    char type; //'r': request packet, 'v': voltage, 'i': current, 't': temperature, 'f': flow, 'c': count.
    int data;
};

class LoRaTransceiver
{
private:
    SPIClass m_hspi;
    const int mk_ss;
    const int mk_rst;
    const int mk_dio0;
    int mk_syncWord;
public:
    LoRaTransceiver(int, int, int, int);
    void initialize();
    void request(packet*, int);
    void respond(packet*);
    void send(packet);
    packet receive();
};

LoRaTransceiver::LoRaTransceiver(int ss, int rst, int dio0, int syncWord = -1):
 m_hspi(HSPI)
,mk_ss(ss)
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
  LoRa.setSPI(m_hspi);
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

void LoRaTransceiver::request(packet *packets, int syncWord = -1)
{
    if (syncWord != -1)
    {
        mk_syncWord = syncWord;
        LoRa.setSyncWord(mk_syncWord);
    }
    else if (mk_syncWord == -1)
    {
        Serial.print("Error: Sync Word was NEVER Specified!");
    }

    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print('r');
    LoRa.endPacket();
    //Receive
    packets[0] = receive();
}

void LoRaTransceiver::respond(packet *packets)
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
    if (LoRaData.charAt(0) == 'r')
    {
        //send
        send(packets[0]);
        return;
    }
    else
    {
        Serial.print("Invalid Request!\n");
        return;
    }
}

void LoRaTransceiver::send(packet toSend)
{
    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print(toSend.type);
    LoRa.print(toSend.data);
    LoRa.endPacket();
    Serial.print("Sent: "); Serial.print(toSend.type); Serial.println(toSend.data);
}

packet LoRaTransceiver::receive()
{
    String LoRaData;
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    bool isReceived = 0;
    while (!isReceived)
    {
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
        isReceived = 1;
        }
    }
    packet received;
    received.type = LoRaData.charAt(0);
    LoRaData.remove(0);
    received.data = LoRaData.toInt();
    return received;
}