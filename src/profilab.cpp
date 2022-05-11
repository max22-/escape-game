#include "profilab.h"
#include "log.h"

static AsyncUDP udp;

void ProfilabClass::begin() {
  tx_mutex = xSemaphoreCreateMutex();
  rx_mutex = xSemaphoreCreateMutex();
  if (udp.listen(1234)) {
    udp.onPacket([this](AsyncUDPPacket packet) {
      flag = true;
      remoteIP = packet.remoteIP();
      remotePort = packet.remotePort();
      uint16_t data = *((unsigned short *)packet.data());
      Serial.print("UDP packet received from : ");
      Serial.print(remoteIP);
      Serial.print(":");
      Serial.print(remotePort);
      Serial.print(", Length: ");
      Serial.print(packet.length());
      Serial.print(", Data: ");
      Serial.printf("%d", data);
      Serial.println();
      uint16_t changed = input ^ data;
      if (changed) {
        for (int i = 0; i < 16; i++) {
          if ((changed & (1 << i)) && callbacks[i] != nullptr) {
            bool bitval = (data & (1 << i)) > 0;
            Serial.printf("rx[%d]=%d\n", i, bitval);
            LOG("rx[%d]=%d\n", i, bitval);
            callbacks[i](bitval);
          }
        }
      }
      input = data;
    });
  }
}

void ProfilabClass::tx(uint8_t pin, bool val) {
  xSemaphoreTake(tx_mutex, portMAX_DELAY);
  uint16_t old_output = output, mask = ~(1 << pin);
  output = (output & mask) | (val << pin);
  if (output != old_output && flag) {
    Serial.printf("tx[%d]=%d\n", pin, val);
    LOG("tx[%d]=%d\n", pin, val);
    udp.writeTo((const uint8_t *)&output, 2, remoteIP, remotePort);
  }
  xSemaphoreGive(tx_mutex);
}

void ProfilabClass::rx(uint8_t pin, void (*callback)(bool)) {
  xSemaphoreTake(rx_mutex, portMAX_DELAY);
  if (pin < 16)
    callbacks[pin] = callback;
  xSemaphoreGive(rx_mutex);
}

ProfilabClass::ProfilabClass() {}

ProfilabClass Profilab;