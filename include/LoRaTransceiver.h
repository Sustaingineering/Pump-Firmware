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
#include "config.h"

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
#define NUMBER_OF_PACKETS 6


struct packet
{
    char type; //'r': request packet, 'v': voltage, 'i': current, 't': temperature, 'f': flow, 'c': count.
    float data;
};

class LoRaTransceiver
{
private:
    LoRaClass m_LoRa;
    SPIClass m_hspi;
    const float mk_id;
    const int mk_ss;
    const int mk_rst;
    const int mk_dio0;
    int m_syncWord;
    void send(packet*, int);
    void receive(packet*, int, int);
public:
    LoRaTransceiver(int, int, int, int, float);
    void initialize();
    bool request(float, packet*, int);
    bool respond(packet*, int);
};

LoRaTransceiver::LoRaTransceiver(int ss, int rst, int dio0, int syncWord, float id = -1.0):
 m_LoRa()
,m_hspi(LORA_SPI_INTERFACE)
,mk_id(id)
,mk_ss(ss)
,mk_rst(rst)
,mk_dio0(dio0)
,m_syncWord(syncWord) 
{}

void LoRaTransceiver::initialize()
{   
  //setup LoRa transceiver module
  m_LoRa.setPins(mk_ss, mk_rst, mk_dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  m_LoRa.setSPI(m_hspi);
  while (!m_LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  m_LoRa.setSyncWord(m_syncWord);
  Serial.println("LoRa Initializing OK!");
}

bool LoRaTransceiver::request(float id, packet *received, int amount)
{
    digitalWrite(BUILTIN_LED, LOW);
    m_LoRa.begin(915E6);
    packet request = {'r', id};
    send(&request, 1);
    //Receive
    receive(received, amount, REQUEST_TIMEOUT);
    m_LoRa.end();
    digitalWrite(BUILTIN_LED, HIGH);
    for (int i = 0; i < amount; i++)
    {
        if (received[i].type != 'e')
            return true;
    }
    return false;
}

bool LoRaTransceiver::respond(packet *toSend, int amount)
{
    digitalWrite(BUILTIN_LED, LOW);
    m_LoRa.begin(915E6);
    packet request;
    receive(&request, 1, RESPOND_TIMEOUT);
    if (request.type == 'r')
    {
        //send
        if (request.data == mk_id)
        {
            delay(SEND_DELAY);
            send(toSend, amount);
        }
        m_LoRa.end();
        digitalWrite(BUILTIN_LED, HIGH);
        return true;
    }
    else
    {
        Serial.print("Invalid Request! '");
        Serial.print(request.type);
        Serial.print(request.data);
        Serial.println("'");
        m_LoRa.end();
        digitalWrite(BUILTIN_LED, HIGH);
        return false;
    }
}

void LoRaTransceiver::send(packet* toSend, int amount)
{
    //Send LoRa packet to receiver
    for (int i = 0; i < amount; i++)
    {
        delay(1); // for stability: sender must be behind receiver to guarantee the transaction.
        m_LoRa.beginPacket();
        m_LoRa.print(toSend[i].type);
        m_LoRa.print(toSend[i].data);
        m_LoRa.endPacket();
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
            int packetSize = m_LoRa.parsePacket();
            if (packetSize)
            {
                // received a packet
                Serial.print("Received packet '");

                // read packet
                while (m_LoRa.available())
                {
                    LoRaData = m_LoRa.readString();
                    Serial.print(LoRaData);
                }

                // print RSSI of packet
                Serial.print("' with RSSI ");
                Serial.println(m_LoRa.packetRssi());
                isReceived = 1;
            }
        }
        received[i].type = LoRaData.charAt(0);
        LoRaData.setCharAt(0, ' ');
        received[i].data = LoRaData.toFloat();
    }
}
