#ifndef SYSLOG_H_
#define SYSLOG_H_

#include <Arduino.h>
#include "lora_utils.h"


namespace SYSLOG_Utils {

    void log(uint8_t type, ReceivedLoRaPacket rxPacket);
    void setup();

}

#endif