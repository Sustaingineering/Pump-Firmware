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
    int m_syncWord;
public:
    LoRaTransceiver(int, int, int, int);
    void initialize();
    packet request(int);
    void respond(packet);
    void send(packet);
    packet receive(int);
};

LoRaTransceiver::LoRaTransceiver(int ss, int rst, int dio0, int syncWord = -1):
 m_hspi(HSPI)
,mk_ss(ss)
,mk_rst(rst)
,mk_dio0(dio0)
,m_syncWord(syncWord) 
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
  LoRa.setSyncWord(m_syncWord);
  Serial.println("LoRa Initializing OK!");
}

packet LoRaTransceiver::request(int syncWord = -1)
{
    if (syncWord != -1)
    {
        m_syncWord = syncWord;
        LoRa.setSyncWord(m_syncWord);
    }
    else if (m_syncWord == -1)
    {
        Serial.print("Error: Sync Word was NEVER Specified!");
    }

    send({'r', 0});
    //Receive
    packet received;
    received =  receive(1000);
    return received;
}

void LoRaTransceiver::respond(packet toSend)
{
    packet request;
    request = receive(1000);
    if (request.type == 'r')
    {
        //send
        delay(500);
        send(toSend);
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

packet LoRaTransceiver::receive(int timeout)
{
    String LoRaData = "e0";
    bool isReceived = 0;
    Serial.println ("Waiting to receive...");
    int start = millis();
    while (!isReceived & ((millis()-start) <= timeout))
    {
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
        isReceived = 1;
        }
    }
    packet received;
    received.type = LoRaData.charAt(0);
    LoRaData.setCharAt(0, ' ');
    received.data = LoRaData.toInt();
    return received;
}