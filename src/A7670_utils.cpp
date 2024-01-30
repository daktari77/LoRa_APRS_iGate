#include "A7670_utils.h"
/*#include "configuration.h"
#include "station_utils.h"
#include "aprs_is_utils.h"
#include "lora_utils.h"
#include "digi_utils.h"
#include "wifi_utils.h"
#include "gps_utils.h"
#include "display.h"
#include "utils.h"

extern Configuration    Config;
extern int              stationMode;
extern uint32_t         lastScreenOn;
extern int              lastStationModeState;
extern String           iGateBeaconPacket;
extern String           firstLine;
extern String           secondLine;
extern String           thirdLine;
extern String           fourthLine;
extern String           fifthLine;
extern String           sixthLine;
extern String           seventhLine;*/

namespace A7670_Utils {

    void setup() {
        Serial.println("Setup Modem");
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

}