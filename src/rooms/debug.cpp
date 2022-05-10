#if defined(DEBUG)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"

const uint8_t button = 35;

void room_init() {
  Sensors.begin();

  /*for(int i = 0; i < 16; i++) {
    Profilab.rx(4, [i](bool val) {
     if (val)
      Serial.printf("RX%d->%d\n", i, val);
    });
  }*/
}

void room_handle() {
  delay(10);
}

#endif