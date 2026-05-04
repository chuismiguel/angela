#include "ota.h"
#include <WiFi.h>
#include <ArduinoOTA.h>

uint32_t OtaHandler::lastOtaTime = 0;

void OtaHandler::Begin(const char* ssid, const char* pwd) {

    Serial.println("Booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);
    
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    ArduinoOTA
        .onStart([]() {
            String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            if (millis() - lastOtaTime > 500) {
                Serial.printf("Progress: %u%%\n", progress / (total / 100));
                lastOtaTime = millis();
            }
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);

            switch (error) {
                case OTA_AUTH_ERROR:    Serial.println("Auth Failed"); break;
                case OTA_BEGIN_ERROR:   Serial.println("Begin Failed"); break;
                case OTA_CONNECT_ERROR: Serial.println("Connect Failed"); break;
                case OTA_RECEIVE_ERROR: Serial.println("Receive Failed"); break;
                case OTA_END_ERROR:     Serial.println("End Failed"); break;
            }
        });

    ArduinoOTA.begin();

    Serial.println("OTA Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void OtaHandler::Handle() {
    ArduinoOTA.handle();
}