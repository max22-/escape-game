#if defined(SALLE_AUTOMNE)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"

const uint8_t button = 23;

static void light_task_1(void *params) {
  Light.set_level(Config.day());
  Light.set_level(Config.night(), Config.delay1());
  while (true)
    delay(1000);
}

static void light_task_2(void *params) {
  Light.set_level(Config.night());
  Light.set_level(Config.day(), Config.delay2());
  while (true)
    delay(1000);
}

void room_init() {
  pinMode(button, INPUT_PULLUP);
  pinMode(DOOR_RELAY, OUTPUT);
  pinMode(ACTUATOR, OUTPUT);
  Sensors.begin();
  Light.begin();
  Light.set_level(Config.day());
  Profilab.rx(7, [](bool val) { digitalWrite(DOOR_RELAY, val ? HIGH : LOW); });
  Profilab.rx(9, [](bool val) {
    if (val)
      Light.run_task(light_task_1);
  });
  Profilab.rx(10, [](bool val) {
    if (val)
      Light.run_task(light_task_2);
  });
  Profilab.rx(11, [](bool val) { digitalWrite(ACTUATOR, val ? HIGH : LOW); });
}

void room_handle() {
  Profilab.tx(0, Sensors.read(0) > Config.threshold());
  Profilab.tx(10, digitalRead(button) == LOW);
}

#endif