#if defined(SALLE_HIVER)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"

static void light_task(void *params) {
  Light.set_level(80);
  Light.set_level(50, 180);
  Light.set_level(70, 180);
  Light.set_level(40, 180);
  Light.set_level(80, 180);
  Light.set_level(30, 180);

  while (true)
    delay(1000);
}

void room_init() {
  pinMode(DOOR_RELAY, OUTPUT);
  pinMode(HOT_PLATE, OUTPUT);
  pinMode(SPOT, OUTPUT);
  Sensors.begin();
  Light.begin();
  Light.set_level(80);
  Profilab.rx(0, [](bool val) {
    if (val)
      Light.run_task(light_task);
  });
  Profilab.rx(6, [](bool val) { digitalWrite(SPOT, val ? HIGH : LOW); });
  Profilab.rx(7, [](bool val) { digitalWrite(DOOR_RELAY, val ? HIGH : LOW); });
  Profilab.rx(8, [](bool val) { digitalWrite(HOT_PLATE, val ? HIGH : LOW); });
}

void room_handle() {
  for (int i = 0; i < 7; i++)
    Profilab.tx(i, Sensors.read(i) > Config.threshold());
}

#endif