#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "configuration.h"
#include "display.h"


void Configuration::check() {
    if (reload) {
        show_display("------- UPDATE ------", "config is old", "device will update", "and then reboot", 1000);
        
        writeFile();

        ESP.restart();
    }
}

void Configuration::writeFile() {
    Serial.println("Saving config..");

    StaticJsonDocument<2048> data;
    File configFile = SPIFFS.open("/igate_conf.json", "w");

    if (wifiAPs[0].ssid != "") { // We don't want to save Auto AP empty SSID
        for (int i = 0; i < wifiAPs.size(); i++) {
            data["wifi"]["AP"][i]["ssid"] = wifiAPs[i].ssid;
            data["wifi"]["AP"][i]["password"] = wifiAPs[i].password;
        }
    }

    data["wifi"]["autoAP"]["password"]      = wifiAutoAP.password;
    data["wifi"]["autoAP"]["powerOff"]      = wifiAutoAP.powerOff;

    data["callsign"]                        = callsign;
    data["other"]["rememberStationTime"]    = rememberStationTime;

    data["digi"]["mode"]                    = digi.mode;

    data["tnc"]["enableServer"]             = tnc.enableServer;
    data["tnc"]["enableSerial"]             = tnc.enableSerial;
    data["tnc"]["acceptOwn"]                = tnc.acceptOwn;

    data["aprs_is"]["active"]               = aprs_is.active;
    data["aprs_is"]["passcode"]             = aprs_is.passcode;
    data["aprs_is"]["server"]               = aprs_is.server;
    data["aprs_is"]["port"]                 = aprs_is.port;
    data["aprs_is"]["filter"]               = aprs_is.filter;
    data["aprs_is"]["messagesToRF"]         = aprs_is.messagesToRF;
    data["aprs_is"]["objectsToRF"]          = aprs_is.objectsToRF;
    

    data["beacon"]["comment"]               = beacon.comment;
    data["beacon"]["interval"]              = beacon.interval;
    data["beacon"]["latitude"]              = beacon.latitude;
    data["beacon"]["longitude"]             = beacon.longitude;
    data["beacon"]["overlay"]               = beacon.overlay;
    data["beacon"]["symbol"]                = beacon.symbol;
    data["beacon"]["sendViaAPRSIS"]         = beacon.sendViaAPRSIS;
    data["beacon"]["sendViaRF"]             = beacon.sendViaRF;
    data["beacon"]["path"]                  = beacon.path;

    data["lora"]["rxFreq"]                  = lora.rxFreq;
    data["lora"]["txFreq"]                  = lora.txFreq;
    data["lora"]["spreadingFactor"]         = lora.spreadingFactor;
    data["lora"]["signalBandwidth"]         = lora.signalBandwidth;
    data["lora"]["codingRate4"]             = lora.codingRate4;
    data["lora"]["power"]                   = lora.power;
    data["lora"]["txActive"]                = lora.txActive;
    data["lora"]["rxActive"]                = lora.rxActive;

    data["display"]["alwaysOn"]             = display.alwaysOn;
    data["display"]["timeout"]              = display.timeout;
    data["display"]["turn180"]              = display.turn180;

    data["syslog"]["active"]                = syslog.active;
    data["syslog"]["server"]                = syslog.server;
    data["syslog"]["port"]                  = syslog.port;

    data["bme"]["active"]                   = bme.active;
    data["bme"]["heightCorrection"]         = bme.heightCorrection;
    data["bme"]["temperatureCorrection"]    = bme.temperatureCorrection;

    data["ota"]["username"]                 = ota.username;
    data["ota"]["password"]                 = ota.password;

    data["other"]["lowPowerMode"]           = lowPowerMode;
    data["other"]["lowVoltageCutOff"]       = lowVoltageCutOff;

    data["other"]["backupDigiMode"]         = backupDigiMode;

    data["other"]["rebootMode"]             = rebootMode;
    data["other"]["rebootModeTime"]         = rebootModeTime;

    data["battery"]["sendInternalVoltage"]  = battery.sendInternalVoltage;
    data["battery"]["sendExternalVoltage"]  = battery.sendExternalVoltage;
    data["battery"]["externalVoltagePin"]   = battery.externalVoltagePin;
    data["battery"]["internalMonitor"]      = battery.internalMonitor;
    data["battery"]["internalSleepVoltage"] = battery.internalSleepVoltage;
    data["battery"]["externalMonitor"]      = battery.externalMonitor;
    data["battery"]["externalSleepVoltage"] = battery.externalSleepVoltage;

    serializeJson(data, configFile);

    configFile.close();

    Serial.println("Config saved");
}

bool Configuration::readFile() {
    Serial.println("Reading config..");

    File configFile = SPIFFS.open("/igate_conf.json", "r");

    if (configFile) {
        StaticJsonDocument<2048> data;

        DeserializationError error = deserializeJson(data, configFile);
        if (error) {
            Serial.println("Failed to read file, using default configuration");
        }

        JsonArray WiFiArray = data["wifi"]["AP"];
        for (int i = 0; i < WiFiArray.size(); i++) {
            WiFi_AP wifiap;
            wifiap.ssid                   = WiFiArray[i]["ssid"].as<String>();
            wifiap.password               = WiFiArray[i]["password"].as<String>();

            wifiAPs.push_back(wifiap);
        }

        wifiAutoAP.password             = data["wifi"]["autoAP"]["password"].as<String>();
        wifiAutoAP.powerOff             = data["wifi"]["autoAP"]["powerOff"].as<int>();

        callsign                        = data["callsign"].as<String>();
        rememberStationTime             = data["other"]["rememberStationTime"].as<int>();

        aprs_is.passcode                = data["aprs_is"]["passcode"].as<String>();
        aprs_is.server                  = data["aprs_is"]["server"].as<String>();
        aprs_is.port                    = data["aprs_is"]["port"].as<int>();

        lora.spreadingFactor            = data["lora"]["spreadingFactor"].as<int>();
        lora.signalBandwidth            = data["lora"]["signalBandwidth"].as<long>();
        lora.codingRate4                = data["lora"]["codingRate4"].as<int>();
        lora.power                      = data["lora"]["power"].as<int>();

        display.alwaysOn                = data["display"]["alwaysOn"].as<bool>();
        display.timeout                 = data["display"]["timeout"].as<int>();
        display.turn180                 = data["display"]["turn180"].as<bool>();

        syslog.active                   = data["syslog"]["active"].as<bool>();
        syslog.server                   = data["syslog"]["server"].as<String>();
        syslog.port                     = data["syslog"]["port"].as<int>();

        bme.active                      = data["bme"]["active"].as<bool>();
        bme.heightCorrection            = data["bme"]["heightCorrection"].as<int>();
        bme.temperatureCorrection       = data["bme"]["temperatureCorrection"].as<float>();

        ota.username                    = data["ota"]["username"].as<String>();
        ota.password                    = data["ota"]["password"].as<String>();

        tnc.enableServer                = data["tnc"]["enableServer"].as<bool>();
        tnc.enableSerial                = data["tnc"]["enableSerial"].as<bool>();
        tnc.acceptOwn                   = data["tnc"]["acceptOwn"].as<bool>();

        lowPowerMode                    = data["other"]["lowPowerMode"].as<bool>();
        lowVoltageCutOff                = data["other"]["lowVoltageCutOff"].as<double>();

        backupDigiMode                  = data["other"]["backupDigiMode"].as<bool>();

        rebootMode                      = data["other"]["rebootMode"].as<bool>();
        rebootModeTime                  = data["other"]["rebootModeTime"].as<int>();

        battery.sendInternalVoltage     = data["battery"]["sendInternalVoltage"].as<bool>();
        battery.sendExternalVoltage     = data["battery"]["sendExternalVoltage"].as<bool>();
        battery.externalVoltagePin      = data["battery"]["externalVoltagePin"].as<int>();
        battery.internalMonitor         = data["battery"]["internalMonitor"].as<bool>();
        battery.internalSleepVoltage    = data["battery"]["internalSleepVoltage"].as<float>();
        battery.externalMonitor         = data["battery"]["externalMonitor"].as<bool>();
        battery.externalSleepVoltage    = data["battery"]["externalSleepVoltage"].as<float>();

        int stationMode                 = data["stationMode"].as<int>(); // deprecated but need to specify config version

        if (stationMode == 0) {
            // Load new settings 

            beacon.latitude                   = data["beacon"]["latitude"].as<double>();
            beacon.longitude                  = data["beacon"]["longitude"].as<double>();
            beacon.comment                    = data["beacon"]["comment"].as<String>();
            beacon.overlay                    = data["beacon"]["overlay"].as<String>();
            beacon.symbol                     = data["beacon"]["symbol"].as<String>();
            beacon.interval                   = data["beacon"]["interval"].as<int>();
            beacon.sendViaAPRSIS              = data["beacon"]["sendViaAPRSIS"].as<bool>();
            beacon.sendViaRF                  = data["beacon"]["sendViaRF"].as<bool>();
            beacon.path                       = data["beacon"]["path"].as<String>();

            digi.mode                         = data["digi"]["mode"].as<int>();

            aprs_is.active                    = data["aprs_is"]["active"].as<bool>();
            aprs_is.filter                    = data["aprs_is"]["filter"].as<String>();
            aprs_is.messagesToRF              = data["aprs_is"]["messagesToRF"].as<bool>();
            aprs_is.objectsToRF               = data["aprs_is"]["objectsToRF"].as<bool>();

            lora.txFreq                       = data["lora"]["txFreq"].as<long>();
            lora.rxFreq                       = data["lora"]["rxFreq"].as<long>();
            lora.txActive                     = data["lora"]["txActive"].as<bool>();
            lora.rxActive                     = data["lora"]["rxActive"].as<bool>();
        } else {
            // Load old settings and put into new variables not actual config

            String iGateComment             = data["iGateComment"].as<String>();
            int beaconInterval              = data["other"]["beaconInterval"].as<int>();

            long iGateFreq                  = data["lora"]["iGateFreq"].as<long>();
            long digirepeaterTxFreq         = data["lora"]["digirepeaterTxFreq"].as<long>();
            long digirepeaterRxFreq         = data["lora"]["digirepeaterRxFreq"].as<long>();
        
            String digiComment              = data["digi"]["comment"].as<String>();
            double digiLatitude             = data["digi"]["latitude"].as<double>();
            double digiLongitude            = data["digi"]["longitude"].as<double>();

            beacon.latitude = digiLatitude;
            beacon.longitude = digiLongitude;
            beacon.interval = beaconInterval;

            lora.txFreq = digirepeaterTxFreq;
            lora.rxFreq = digirepeaterRxFreq;
            lora.rxActive = true;
            beacon.sendViaAPRSIS = true;
            beacon.sendViaRF = false;

            switch (stationMode) {
                case 1: // IGate only
                // aprs_is.active = true; // better don't do that automatically
                beacon.comment = iGateComment;
                lora.rxFreq = iGateFreq;
                break;
                case 5: // Digi + IGate
                case 2: // Digi + IGate
                // aprs_is.active = true; // better don't do that automatically
                // digi.mode = 2; // better don't do that automatically
                beacon.comment = digiComment;
                lora.rxFreq = iGateFreq;
                break;
                case 3: // Digi
                case 4: // Digi
                // digi.mode = 2; // better don't do that automatically
                beacon.comment = digiComment;
                break;
            }

            reload = true;
        }

        if (wifiAPs.size() == 0) { // If we don't have any WiFi's from config we need to add "empty" SSID for AUTO AP
            WiFi_AP wifiap;
            wifiap.ssid = "";
            wifiap.password = "";

            wifiAPs.push_back(wifiap);
        }
        configFile.close();
        Serial.println("Config read successfuly");
        return true;
    } else {
        Serial.println("Config file not found");
        return false;
    }
}
    
void Configuration::init() {
    reload = false;

    WiFi_AP wifiap;
    wifiap.ssid                 = "";
    wifiap.password             = "";

    wifiAPs.push_back(wifiap);

    wifiAutoAP.password         = "1234567890";
    wifiAutoAP.powerOff         = 15;

    callsign                    = "N0CALL";

    beacon.comment              = "LoRa APRS";
    beacon.latitude             = 0.0;
    beacon.longitude            = 0.0;
    beacon.interval             = 15;
    beacon.overlay              = "L";
    beacon.symbol               = "#";
    beacon.sendViaAPRSIS        = true;
    beacon.sendViaRF            = false;
    beacon.path                 = "WIDE1-1";
    
    digi.mode = 0;

    tnc.enableServer            = false;
    tnc.enableSerial            = false;
    tnc.acceptOwn               = false;

    aprs_is.active              = false;
    aprs_is.passcode            = "XYZVW";
    aprs_is.server              = "rotate.aprs2.net";
    aprs_is.port                = 14580;
    aprs_is.filter              = "m/10";
    aprs_is.messagesToRF        = false;
    aprs_is.objectsToRF         = false;

    lora.txFreq           = 433775000;
    lora.rxFreq           = 433775000;
    lora.spreadingFactor  = 12;
    lora.signalBandwidth  = 125000;
    lora.codingRate4      = 5;
    lora.power            = 20;
    lora.txActive         = false;
    lora.rxActive         = true;

    display.alwaysOn            = true;
    display.timeout             = 4;
    display.turn180             = false;

    syslog.active               = false;
    syslog.server               = "192.168.0.100";
    syslog.port                 = 514;

    bme.active                  = false;
    bme.heightCorrection        = 0;
    bme.temperatureCorrection   = 0.0;

    ota.username                = "";
    ota.password                = "";

    rememberStationTime             = 30;

    lowPowerMode                    = false;
    lowVoltageCutOff                = 0;

    backupDigiMode                  = false;

    rebootMode                      = false;
    rebootModeTime                  = 0;

    battery.sendInternalVoltage     = false;
    battery.sendExternalVoltage     = false;
    battery.externalVoltagePin      = 34;
    battery.internalMonitor         = false;
    battery.internalSleepVoltage    = 0.0;
    battery.externalMonitor         = false;
    battery.externalSleepVoltage    = 0.0;

    Serial.println("All is Written!");
}

Configuration::Configuration() {
    if (!SPIFFS.begin(false)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    } else {
        Serial.println("SPIFFS Mounted");
    }

    bool exists = SPIFFS.exists("/igate_conf.json");
    if (!exists) {
        init();
        writeFile();
        ESP.restart();
    }

    readFile();
}