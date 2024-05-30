#ifndef APRS_IS_UTILS_H_
#define APRS_IS_UTILS_H_

#include <Arduino.h>
#include "lora_utils.h"


namespace APRS_IS_Utils {

    void upload(const String& line);
    void connect();
    void checkStatus();
    String buildPacketToUpload(const String& packet);
    String buildPacketToTx(const String& aprsisPacket, uint8_t packetType);
    bool processReceivedLoRaMessage(const String& sender, const String& packet);
    void processLoRaPacket(ReceivedLoRaPacket rxPacket);
    void processAPRSISPacket(const String& packet);
    void listenAPRSIS();

}

#endif