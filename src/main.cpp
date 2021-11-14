#include <Arduino.h>
#include "config.h"
#include "wifi_config.h"
#include <WiFi.h>
#ifdef USE_OTA
#include <ArduinoOTA.h>
#endif
#include "rooms/rooms.h"
#include "profilab.h"


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
  room_init();
}

void loop() {
  #ifdef USE_OTA
  ArduinoOTA.handle();
  #endif
  room_handle();
  delay(10);
}