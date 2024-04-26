#include <SPI.h>
#include "ethernet_utils.h"
#include "pins_config.h"

#ifdef ESP32_DIY_LoRa_Ethernet
#include <Ethernet.h>

extern EthernetClient client;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

namespace ETHERNET_Utils {

    void setup() {
        SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
        pinMode(Ethernet_CS, OUTPUT);
        pinMode(Ethernet_MOSI, OUTPUT);
        pinMode(Ethernet_MISO, INPUT);
        pinMode(Ethernet_SCK, OUTPUT);
        SPI.endTransaction();
        Ethernet.init(Ethernet_CS);
        Ethernet.begin(mac);
        delay(1000);
        Serial.println(Ethernet.localIP());
    }

}
#endif