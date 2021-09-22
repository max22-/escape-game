#include <Arduino.h>
#include "config.h"
#include "wifi_config.h"
#include <WiFi.h>
#ifdef USE_OTA
#include <ArduinoOTA.h>
#endif
#include "rooms/rooms.h"
#include "profilab_tcp.h"

ProfilabTCP profilabTCP(30000);

#if defined(SALLE_ETE)
  IPAddress local_IP(192, 168, 0, 210);
#elif defined(SALLE_AUTOMNE)
  IPAddress local_IP(192, 168, 0, 202);
#elif defined(SALLE_HIVER)
  IPAddress local_IP(192, 168, 0, 230);
#elif defined(SALLE_PRINTEMPS)
  IPAddress local_IP(192, 168, 0, 240);
#elif defined(SALLE_CENTRALE)
  IPAddress local_IP(192, 168, 0, 250);
#endif

IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    while(true) {
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
  Serial.printf("\nIP : %s\n", WiFi.localIP().toString().c_str());
  #ifdef USE_OTA
  ArduinoOTA.begin();
  #endif
  profilabTCP.begin();
  room_init(profilabTCP);
}

void loop() {
  #ifdef USE_OTA
  ArduinoOTA.handle();
  #endif
  room_handle(profilabTCP);
  profilabTCP.handle();

  delay(10);
}