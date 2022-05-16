#if defined(DEBUG)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"

void room_init() {
  Sensors.begin();
}

void room_handle() { delay(10); }

#endif