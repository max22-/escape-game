#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include "rooms/rooms.h"
#include "profilab.h"

WiFiServer wifiServer(30000);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;
  bool c = wifiManager.autoConnect(WIFI_SSID, WIFI_PASSWORD);
  if(!c) {
    while(true) {
      Serial.println("Connection failed");
      delay(5000);
    }
  }
  Serial.printf("\nIP : %s\n", WiFi.localIP().toString().c_str());
  wifiServer.begin();
  room_init();
}

void loop() {
  uint8_t buffer[12];
  WiFiClient client = wifiServer.available();
  if(client) {
    printf("Client connected : %s\n", client.remoteIP().toString().c_str());
    while(client.connected()) {
      while(client.available() >= 12) {
        client.readBytes(buffer, sizeof(buffer));
        profilab_data_t data = profilab_decode(buffer);
        room_receive(data);
      }
      static profilab_data_t old_data = {0, 0};
      profilab_data_t data = room_send();
      if(old_data.n != data.n || old_data.channel != data.channel) {
        profilab_encode(data, &buffer);
        client.write(buffer, sizeof(buffer));
        printf("old_data = {%d, %lf}\t data = {%d, %lf}\n", old_data.channel, old_data.n, data.channel, data.n);
        printf("sending %lf on channel %d\n", data.n, data.channel);
        old_data = data;
      }
      delay(10);
    }
    client.stop();
    printf("client disconnected\n");
  }
  delay(10);
}