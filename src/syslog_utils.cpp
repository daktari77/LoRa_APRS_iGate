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
                syslogPacket += "CRC / CRC-ERROR / " + rxPacket.packet;
                syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                break;
            case 1:     // RX
                syslogPacket += "RX / ";
                if (rxPacket.packet.indexOf("::") > 10) {
                    syslogPacket += "MESSAGE / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf("::") + 2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":!") > 10 || rxPacket.packet.indexOf(":=") > 10) {
                    syslogPacket += "GPS / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " / ";
                    if (rxPacket.packet.indexOf("WIDE1-1") > 10) {
                        syslogPacket += rxPacket.packet.substring(rxPacket.packet.indexOf(">") + 1, rxPacket.packet.indexOf(",")) + " / WIDE1-1 / ";
                    } else {
                        syslogPacket += rxPacket.packet.substring(rxPacket.packet.indexOf(">") + 1, rxPacket.packet.indexOf(":")) + " / - / ";
                    }
                    syslogPacket += String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz / " +  GPS_Utils::getDistanceAndComment(rxPacket.packet);
                } else if (rxPacket.packet.indexOf(":>") > 10) {
                    syslogPacket += "STATUS / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":>") + 2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":`") > 10) {
                    syslogPacket += "MIC-E / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":`") + 2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":T#") >= 10 && rxPacket.packet.indexOf(":=/") == -1) {
                    syslogPacket += "TELEMETRY / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":T#") + 3);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else if (rxPacket.packet.indexOf(":;") > 10) {
                    syslogPacket += "OBJECT / " + rxPacket.packet.substring(3, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf(":;") + 2);
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                } else {
                    syslogPacket += rxPacket.packet;
                    syslogPacket += " / " + String(rxPacket.rssi) + "dBm / " + String(rxPacket.snr) + "dB / " + String(rxPacket.freqError) + "Hz";
                }
                break;
            case 2:     // APRSIS TX
                if (rxPacket.packet.indexOf(":>") > 10) {
                    syslogPacket += "APRSIS TX / StartUp_Status / " + rxPacket.packet.substring(rxPacket.packet.indexOf(":>") + 2);
                } else {
                    syslogPacket += "APRSIS TX / QUERY / " + rxPacket.packet;
                }
                break;
            case 3:     // TX
                if (rxPacket.packet.indexOf("RFONLY") > 10) {
                    syslogPacket += "LORA TX / RFONLY / " + rxPacket.packet;
                } else if (rxPacket.packet.indexOf("::") > 10) {
                    syslogPacket += "LORA TX / MESSAGE / " + rxPacket.packet.substring(0, rxPacket.packet.indexOf(">")) + " ---> " + rxPacket.packet.substring(rxPacket.packet.indexOf("::") + 2);
                } else {
                    syslogPacket += "LORA TX / " + rxPacket.packet;
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