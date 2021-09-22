#ifndef PROFILAB_H
#define PROFILAB_H

#include <Arduino.h>
#include <WiFi.h>

class ProfilabTCP {
public:
  ProfilabTCP(uint16_t port);
  void begin();
  void handle();
  void tx(uint8_t pin, bool val);
  void rx(uint8_t pin, void (*callback)(bool));
private:
  ProfilabTCP(const ProfilabTCP&) = delete;
  ProfilabTCP& operator=(const ProfilabTCP&) = delete;
  WiFiClient wifiClient;
  WiFiServer wifiServer;
  uint16_t port;
  void (*callbacks[16])(bool);
  enum {WAITING, CLIENT_CONNECTED} state = WAITING;
  uint16_t tx_data = 0, rx_data = 0;
  unsigned long last_sent[16];
  uint8_t queue[16];
  uint8_t queue_size[16];

  bool queue_peek_back(uint8_t pin);
  bool queue_pop(uint8_t pin);
  void queue_push_back(uint8_t pin, bool val);
  void clear_queues();
};

typedef struct {
  unsigned char channel;
  double n;  
} profilab_data_t;

#endif