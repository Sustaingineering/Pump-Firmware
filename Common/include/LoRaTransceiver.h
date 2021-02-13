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
#include "PinConfig.h"
#include "FarmSensor.h"

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
