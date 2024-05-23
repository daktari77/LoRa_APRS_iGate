#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>
#include <vector>
#include <FS.h>

class WiFi_AP {
public:
    String  ssid;
    String  password;
};

class WiFi_Auto_AP {
public:
    String  password;
    int     powerOff;
};

class Beacon {
public:
    double  latitude;
    double  longitude;
    String  comment;
    int     interval;
    String  overlay;
    String  symbol;
    String  path;    
    bool    sendViaRF;
    bool    sendViaAPRSIS;
};

class APRS_IS {
public:
    bool    active;
    String  passcode;
    String  server;
    int     port;
    String  filter;
    bool    messagesToRF;
    bool    objectsToRF;
};

class DIGI {
public:
    int     mode;
};


class LoraModule {
public:
    long    txFreq;
    long    rxFreq;
    bool    txActive;
    bool    rxActive;
    int     spreadingFactor;
    long    signalBandwidth;
    int     codingRate4;
    int     power;
};

class Display {
public:
    bool    alwaysOn;
    int     timeout;
    bool    turn180;
};

class BME {
public:
    bool    active;
    int     heightCorrection;
    float   temperatureCorrection;
};

class SYSLOG {
public:
    bool    active;
    String  server;
    int     port;
};

class TNC {
public:
    bool    enableServer;
    bool    enableSerial;
    bool    acceptOwn;
};

class OTA {
public:
    String  username;
    String  password;
};

class Configuration {
public:
    bool                    reload;     // ?
    String                  callsign;
    int                     rememberStationTime;
    bool                    sendBatteryVoltage;
    bool                    externalVoltageMeasurement;
    int                     externalVoltagePin;
    bool                    lowPowerMode;
    double                  lowVoltageCutOff;
    bool                    backupDigiMode;
    bool                    rebootMode;
    int                     rebootModeTime;
    std::vector<WiFi_AP>    wifiAPs;
    WiFi_Auto_AP            wifiAutoAP;
    Beacon                  beacon;
    DIGI                    digi;
    TNC                     tnc;
    APRS_IS                 aprs_is;
    LoraModule              loramodule;
    Display                 display;
    SYSLOG                  syslog;
    BME                     bme;
    OTA                     ota;
  
    void init();
    void writeFile();
    void check();
    Configuration();

private:
    bool readFile();
    String _filePath;
};

#endif