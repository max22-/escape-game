#if defined(SALLE_AUTOMNE)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"

const uint8_t button = 35;

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

xTaskHandle chest_task_handle = NULL;

static void chest_task(void *params) {
  int dir = (int)params;
  Serial.printf("delay 1 = %d\n", (int)Config.chest_delay_1());
  Serial.printf("delay 2 = %d\n", (int)Config.chest_delay_2());
  digitalWrite(CHEST_RELAY_1, dir ? HIGH : LOW);
  digitalWrite(CHEST_RELAY_2, dir ? LOW : HIGH);
  delay(Config.chest_delay_1()*1000);
  digitalWrite(CHEST_RELAY_1, LOW);
  digitalWrite(CHEST_RELAY_2, LOW);
  delay(Config.chest_delay_2()*1000);
  digitalWrite(CHEST_RELAY_1, dir ? LOW : HIGH);
  digitalWrite(CHEST_RELAY_2, dir ? HIGH : LOW); 
  delay(Config.chest_delay_1()*1000);
  digitalWrite(CHEST_RELAY_1, LOW);
  digitalWrite(CHEST_RELAY_2, LOW);
  chest_task_handle = NULL;
  vTaskDelete(NULL);
}

void room_init() {
  pinMode(button, INPUT_PULLUP);
  pinMode(DOOR_RELAY, OUTPUT);
  pinMode(CHEST_RELAY_1, OUTPUT);
  pinMode(CHEST_RELAY_2, OUTPUT);
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
  Profilab.rx(11, [](bool val) { 
    if(chest_task_handle != NULL)
      vTaskDelete(chest_task_handle);
    xTaskCreate(chest_task, "chest_task", 3000, (void*)val, 1, &chest_task_handle); }
  );
}

void room_handle() {
  Profilab.tx(0, Sensors.read(0) > Config.threshold());
  Profilab.tx(10, digitalRead(button) == LOW);
}

#endif