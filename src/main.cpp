#include <Arduino.h>
#include <BluetoothSerial.h>
#include "config.h"
#if defined(USE_WIFI)
#include <WiFi.h>
#endif
#include "dual_modbus.h"
#include "rooms/rooms.h"


void setup() {
  Serial.begin(9600, SERIAL_8N1);
  #if defined(USE_WIFI)
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  #endif
  Modbus.begin();
  init_room();
}

void loop() {
  Modbus.task();
  delay(10);
}