#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#ifdef USE_OTA
#include <ArduinoOTA.h>
#endif
#include "rooms/rooms.h"
#include "profilab.h"

#include "pin_config.h"

void heartbeat(void *params)
{
  while(true) {
    Profilab.tx(HEARTBEAT, 1);
    delay(500);
    Profilab.tx(HEARTBEAT, 0);
    delay(4500);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Access point\n");
  Serial.printf("SSID = \"%s\" \tpassword = \"%s\"\n", WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("IP : %s\n", WiFi.softAPIP().toString().c_str());
  #ifdef USE_OTA
  Serial.printf("OTA enabled\n");
  ArduinoOTA.begin();
  #endif
  Profilab.begin();
  xTaskCreate(heartbeat, "heartbeat", 4096, nullptr, 1, nullptr);
}

void loop() {
  #ifdef USE_OTA
  ArduinoOTA.handle();
  #endif
  room_handle();
  delay(10);
}