#if defined(SALLE_ETE)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"
#include "button.h"

Button buttons[] = {
  Button(21), Button(35), Button(34), Button(39)
};

Filter filtered_buttons[] = {
  Filter(buttons[0], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[1], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[2], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[3], BUTTON_FILTER_DELAY, FILTER_COEFF)
};

Sensor sensors[] = {
  Sensor(0), Sensor(1), Sensor(2), Sensor(3),
  Sensor(4), Sensor(5), Sensor(6), Sensor(7)
};

Filter filtered_sensors[] = {
  Filter(sensors[0], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[1], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[2], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[3], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[4], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[5], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[6], SENSOR_FILTER_DELAY, FILTER_COEFF),
  Filter(sensors[7], SENSOR_FILTER_DELAY, FILTER_COEFF),
};

static void light_task_1(void *params) {
  Light.set_level(Config.night());
  Light.set_level(Config.day(), Config.delay1());
  while (true)
    delay(1000);
}

static void light_task_2(void *params) {
  Light.set_level(Config.day());
  Light.set_level(Config.night(), Config.delay2());
  while (true)
    delay(1000);
}

static void light_task_set_night(void *params) {
  Light.set_level(Config.night());
  while (true)
    delay(1000);
}

static xTaskHandle door_task_handle = NULL;

static void door_task(void *params) {
  digitalWrite(DOOR_RELAY, HIGH);
  delay(Config.door_delay() * 1000);
  digitalWrite(DOOR_RELAY, LOW);
  door_task_handle = NULL;
  vTaskDelete(NULL);
}

void room_init() {
  for (int i = 0; i < sizeof(buttons)/sizeof(buttons[0]); i++)
    buttons[i].begin();
  pinMode(BLACK_LIGHT, OUTPUT);
  pinMode(DOOR_RELAY, OUTPUT);
  Sensor::begin();

  Light.begin();
  Light.set_level(Config.night());
  Profilab.rx(4, [](bool val) {
    if (val)
      Light.run_task(light_task_set_night);
  });
  Profilab.rx(7, [](bool val) { 
    if(val) { 
      if(door_task_handle != NULL)
        vTaskDelete(door_task_handle);
      xTaskCreate(door_task, "door_task", 3000, NULL, 1,
                  &door_task_handle);
    }
  });
  Profilab.rx(8, [](bool val) { digitalWrite(BLACK_LIGHT, val ? HIGH : LOW); });
  Profilab.rx(9, [](bool val) {
    if (val)
      Light.run_task(light_task_1);
  });
  Profilab.rx(10, [](bool val) {
    if (val)
      Light.run_task(light_task_2);
  });
}

void room_handle() {
  for (int i = 0; i < 7; i++)
    Profilab.tx(i, filtered_sensors[i].output() > Config.thresholds(i));

  for (int i = 0; i < sizeof(buttons)/sizeof(buttons[0]); i++)
    Profilab.tx(10 + i, filtered_buttons[i].output() < 0.05);
}

#endif