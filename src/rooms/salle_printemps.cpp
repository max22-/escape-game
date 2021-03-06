#if defined(SALLE_PRINTEMPS)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"
#include "button.h"

Button buttons[] = {
  Button(23), Button(35), Button(34),
  Button(39), Button(IO3), Button(IO4), Button(IO5)
};

Filter filtered_buttons[] = {
  Filter(buttons[0], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[1], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[2], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[3], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[4], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[5], BUTTON_FILTER_DELAY, FILTER_COEFF),
  Filter(buttons[6], BUTTON_FILTER_DELAY, FILTER_COEFF)
};

static void light_task_1(void *params) {
  Light.set_level(Config.night());
  while (true)
    delay(1000);
}

static void light_task_2(void *params) {
  Light.set_level(Config.night());
  Light.set_level(Config.day(), Config.delay1());
  while (true)
    delay(1000);
}

static void light_task_3(void *params) {
  Light.set_level(Config.day());
  Light.set_level(Config.night(), Config.delay2());
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
  pinMode(DOOR_RELAY, OUTPUT);
  pinMode(LEDS_RELAY, OUTPUT);
  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, LOW);
  Light.begin();
  Light.set_level(Config.night());
  Profilab.rx(0, [](bool val) {
    if (val)
      Light.run_task(light_task_1);
  });
  Profilab.rx(9, [](bool val) {
    if (val)
      Light.run_task(light_task_2);
  });
  Profilab.rx(10, [](bool val) {
    if (val)
      Light.run_task(light_task_3);
  });
  Profilab.rx(7, [](bool val) {
    if(val) { 
      if(door_task_handle != NULL)
        vTaskDelete(door_task_handle);
      xTaskCreate(door_task, "door_task", 3000, NULL, 1,
                  &door_task_handle);
     }
  });
  Profilab.rx(12, [](bool val) { digitalWrite(LEDS_RELAY, val ? HIGH : LOW); });
  Profilab.rx(14, [](bool val) { digitalWrite(LASER, val ? HIGH : LOW); });
}

void room_handle() {
  for (int i = 0; i < sizeof(buttons)/sizeof(buttons[0]); i++)
    Profilab.tx(1 + i, filtered_buttons[i].output() < 0.05);
}

#endif