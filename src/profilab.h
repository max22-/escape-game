#ifndef PROFILAB_H
#define PROFILAB_H

#include <Arduino.h>
#include <AsyncUDP.h>

class ProfilabClass {

public:
  ProfilabClass();
  void begin();
  void tx(uint8_t pin, bool val);
  void rx(uint8_t pin, void (*callback)(bool));

private:
  ProfilabClass(const ProfilabClass &) = delete;
  ProfilabClass &operator=(const ProfilabClass &) = delete;

  void (*callbacks[16])(bool);
  uint16_t input = 0, output = 0;
  bool flag = false; // Signals when the first packet has been received
  IPAddress remoteIP;
  uint16_t remotePort;
  SemaphoreHandle_t tx_mutex, rx_mutex;
};

extern ProfilabClass Profilab;

#endif