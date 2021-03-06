#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#ifdef USE_OTA
#include <ArduinoOTA.h>
#endif
#include "fe_repl.h"
#include "profilab.h"
#include "rooms/rooms.h"

#include "pin_config.h"

IPAddress local_IP(IP_1, IP_2, IP_3, IP_4);
IPAddress gateway(IP_1, IP_2, IP_3, 1);
IPAddress subnet(255, 255, 255, 0);

void heartbeat(void *params) {
  while (true) {
    Profilab.tx(HEARTBEAT, 1);
    delay(500);
    Profilab.tx(HEARTBEAT, 0);
    delay(4500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(5, INPUT); // it is in output high mode at boot !?
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(SOFT_AP_SSID, SOFT_AP_PASSWORD, 1, 1);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    while (true) {
      Serial.println("STA Failed to configure");
      delay(5000);
    }
  }
  Serial.printf("Connecting to %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  Serial.printf("\nIP : %s\n", WiFi.localIP().toString().c_str());
#ifdef USE_OTA
  Serial.printf("OTA enabled\n");
  ArduinoOTA.begin();
#endif
#ifdef USE_FE_REPL
  fe_begin();
#endif
  while (!Config.begin()) {
    Serial.println("Config error ! ");
    Serial.println(Config.dump().c_str());
    delay(5000);
  }
  Profilab.begin();
  xTaskCreate(heartbeat, "heartbeat", 4096, nullptr, 1, nullptr);

  room_init();
}

void loop() {
#ifdef USE_OTA
  ArduinoOTA.handle();
#endif
  room_handle();
  delay(10);
}