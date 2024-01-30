#include "A7670_utils.h"
#include "display.h"
/*#include "configuration.h"
#include "station_utils.h"
#include "aprs_is_utils.h"
#include "lora_utils.h"
#include "digi_utils.h"
#include "wifi_utils.h"
#include "gps_utils.h"

#include "utils.h"

extern Configuration    Config;
extern int              stationMode;
extern uint32_t         lastScreenOn;
extern int              lastStationModeState;
extern String           iGateBeaconPacket;*/

extern String           firstLine;
extern String           secondLine;
extern String           thirdLine;
extern String           fourthLine;
extern String           fifthLine;
extern String           sixthLine;
extern String           seventhLine;

namespace A7670_Utils {

    void setup() {
        // esto deberia ir en otro lado
        Serial.println("Setup Modem");
        //

        /*checkModemOn();
        if (modemReady) {
            delay(2000);
            // if gps active ->
            //setup_gps();
            APRS_IS_connect();
        } else {
            show_display(firstLine, "Starting Modem...", "---> Failed !!!", 0);
            Serial.println(F("*********** Failed to connect to the modem! ***********"));
        }*/
    }

    void checkStatus() {
        Serial.println("checkStatus");
    }

    void loop() {
        checkStatus();
        show_display(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0); 
        Serial.println("ESP32_DIY_LoRa_A7670");
        delay(3000);
        /*while (espClient.connected()) {
            Utils::checkDisplayInterval();
            Utils::checkBeaconInterval();
            processLoRaPacket(LoRa_Utils::receivePacket());            
            if (espClient.available()) {
                String aprsisPacket;
                aprsisPacket.concat(espClient.readStringUntil('\r'));
                processAPRSISPacket(aprsisPacket);
            }
        }*/
    }  
}