#include <WiFiUdp.h>
#include <WiFi.h>
#include "configuration.h"
#include "syslog_utils.h"
#include "lora_utils.h"
#include "gps_utils.h"

extern Configuration    Config;

WiFiUDP udpClient;


namespace SYSLOG_Utils {

    void log(uint8_t type, ReceivedLoRaPacket rxPacket) {
        String syslogPacket = "<165>1 - " + Config.callsign + " CA2RXU_LoRa_iGate_1.3" + " - - - "; //RFC5424 The Syslog Protocol
        switch (type) {
            case 0:     // CRC
                syslogPacket += type + " / CRC-ERROR / " + rxPacket.packet;
                syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                break;
            case 1:     // RX
                if (rxPacket.packet.indexOf("::") > 10) {
                    syslogPacket += type + " / MESSAGE / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf("::")+2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":!") > 10 || rxPacket.packet.indexOf(":=") > 10) {
                    syslogPacket += type + " / GPS / " + rxPacket.packet.substring(3,rxPacket.packet.indexOf(">")) + " / ";
                    if (rxPacket.packet.indexOf("WIDE1-1") > 10) {
                        syslogPacket += rxPacket.packet.substring(rxPacket.packet.indexOf(">")+1,rxPacket.packet.indexOf(",")) + " / WIDE1-1 / ";
                    } else {
                        syslogPacket += rxPacket.packet.substring(rxPacket.packet.indexOf(">")+1,rxPacket.packet.indexOf(":")) + " / - / ";
                    }
                    syslogPacket += String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz / " +  GPS_Utils::getDistanceAndComment(rxPacket.packet);
                } else if (rxPacket.packet.indexOf(":>") > 10) {
                    syslogPacket += type + " / STATUS / " + rxPacket.packet.substring(3,rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":>")+2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":`") > 10) {
                    syslogPacket += type + " / MIC-E / " + rxPacket.packet.substring(3,rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":`")+2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":T#") >= 10 && rxPacket.packet.indexOf(":=/") == -1) {
                    syslogPacket += type + " / TELEMETRY / " + rxPacket.packet.substring(3,rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":T#")+3);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":;") > 10) {
                    syslogPacket += type + " / OBJECT / " + rxPacket.packet.substring(3,rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":;")+2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else {
                    syslogPacket += type + " / " + rxPacket.packet;
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                }
                break;
            case 2:     // APRSIS TX
                if (rxPacket.packet.indexOf(":>") > 10) {
                    syslogPacket += type + " / StartUp_Status / " + rxPacket.packet.substring(rxPacket.packet.indexOf(":>")+2);
                } else {
                    syslogPacket += type + " / QUERY / " + rxPacket.packet;
                }
                break;
            case 3:     // TX
                if (rxPacket.packet.indexOf("RFONLY") > 10) {
                    syslogPacket += type + " / RFONLY / " + rxPacket.packet;
                } else if (rxPacket.packet.indexOf("::") > 10) {
                    syslogPacket += type + " / MESSAGE / " + rxPacket.packet.substring(0,rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf("::")+2);
                } else {
                    syslogPacket += type + " / " + rxPacket.packet;
                }
                break;                
            default:
                syslogPacket = "<165>1 - ERROR LoRa - - - ERROR / Error in Syslog Packet"; //RFC5424 The Syslog Protocol
                break;
        }
        udpClient.beginPacket(Config.syslog.server.c_str(), Config.syslog.port);
        udpClient.write((const uint8_t*)syslogPacket.c_str(), syslogPacket.length());
        udpClient.endPacket();
    }

    void setup() {
        if (Config.syslog.active && WiFi.status() == WL_CONNECTED) {
            udpClient.begin(WiFi.localIP(), 0);
            Serial.println("init : Syslog Server  ...     done!    (at " + Config.syslog.server + ")");
        }
    }

}