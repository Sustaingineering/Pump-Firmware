/*
 * Content: class that controls LoRa module
 * Author : Tarek Al Ayoubi
 * Team   : Sustaingineering UBC
 */

/*
 * RESET -> Orange -> D27
 * NSS   -> Yellow -> D15
 * SCK   -> Grey   -> D14
 * MOSI  -> Green  -> D13
 * MISO  -> Blue   -> D12
 * DIO0  -> White  -> D26
 */

#pragma once
#include <SPI.h>
#include <LoRa.h>

// timeout for receiving any response after making a request
#define REQUEST_TIMEOUT 1000

// timeout for staying in receive mode before sampling the sensors
#define RESPOND_TIMEOUT 2000

/*
 * Delay between receiving request and sending the data 
 * to ensure that requester has switched to receiving mode
*/
#define SEND_DELAY 500

//Numer of Packets to exchange
#define NUMBER_OF_PACKETS 4


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
    void request(packet*, int,int);
    void respond(packet*, int);
    void send(packet*, int);
    void receive(packet*, int, int);
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

void LoRaTransceiver::request(packet *received, int amount, int syncWord = -1)
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
    packet request = {'r', 0};
    send(&request, 1);
    //Receive
    receive(received, amount, REQUEST_TIMEOUT);
}

void LoRaTransceiver::respond(packet *toSend, int amount)
{
    packet request;
    receive(&request, 1, RESPOND_TIMEOUT);
    if (request.type == 'r')
    {
        //send
        delay(SEND_DELAY);
        send(toSend, amount);
        return;
    }
    else
    {
        Serial.print("Invalid Request! '");
        Serial.print(request.type);
        Serial.print(request.data);
        Serial.println("'");
        return;
    }
}

void LoRaTransceiver::send(packet* toSend, int amount)
{
    //Send LoRa packet to receiver
    for (int i = 0; i < amount; i++)
    {
        LoRa.beginPacket();
        LoRa.print(toSend[i].type);
        LoRa.print(toSend[i].data);
        LoRa.endPacket();
        Serial.print("Sent: "); Serial.print(toSend[i].type); Serial.println(toSend[i].data);
    }
}

void LoRaTransceiver::receive(packet *received, int amount, int timeout)
{
    String LoRaData;
    bool isReceived;
    Serial.println ("Waiting to receive...");
    unsigned long start = millis();
    for (int i = 0; i < amount; i++)
    {
        LoRaData = "e0"; // HACK: Change to "r0" to simulate a request.
        isReceived = 0;
        while (!isReceived & ((millis()-start) <= timeout))
        {
            // try to parse packet
            int packetSize = LoRa.parsePacket();
            if (packetSize)
            {
                // received a packet
                Serial.print("Received packet '");

                // read packet
                while (LoRa.available())
                {
                    LoRaData = LoRa.readString();
                    Serial.print(LoRaData);
                }

                // print RSSI of packet
                Serial.print("' with RSSI ");
                Serial.println(LoRa.packetRssi());
                isReceived = 1;
            }
        }
        received[i].type = LoRaData.charAt(0);
        LoRaData.setCharAt(0, ' ');
        received[i].data = LoRaData.toInt();
    }
}