#ifndef LORA_UTILS_H_
#define LORA_UTILS_H_

#include <Arduino.h>

struct ReceivedLoRaPacket {
    String  packet;
    int     rssi;
    float   snr;
    int     freqError;
};


namespace LoRa_Utils {

    void setup();
    void sendNewPacket(const String& newPacket);
    String packetSanitization(const String& packet);
    ReceivedLoRaPacket receivePacket();
    void changeFreqTx();
    void changeFreqRx();
    void startReceive(); // ???
    void sleepRadio();

}

#endif