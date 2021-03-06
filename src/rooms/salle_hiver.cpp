#if defined(SALLE_HIVER)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"

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

static xTaskHandle door_task_handle = NULL;

static void door_task(void *params) {
  digitalWrite(DOOR_RELAY, HIGH);
  delay(Config.door_delay() * 1000);
  digitalWrite(DOOR_RELAY, LOW);
  door_task_handle = NULL;
  vTaskDelete(NULL);
}
  
void room_init() {
  pinMode(DOOR_RELAY, OUTPUT);
  pinMode(HOT_PLATE, OUTPUT);
  pinMode(SPOT, OUTPUT);
  Sensor::begin();
  Light.begin();
  Light.set_level(90);
  Profilab.rx(6, [](bool val) { digitalWrite(SPOT, val ? HIGH : LOW); });
  Profilab.rx(7, [](bool val) {
    if(val) { 
      if(door_task_handle != NULL)
        vTaskDelete(door_task_handle);
      xTaskCreate(door_task, "door_task", 3000, NULL, 1,
                  &door_task_handle);
    }
  });
  Profilab.rx(8, [](bool val) { digitalWrite(HOT_PLATE, val ? HIGH : LOW); });
}

void room_handle() {
  for (int i = 0; i < 7; i++)
    Profilab.tx(i, filtered_sensors[i].output() > Config.thresholds(i));
}

#endif