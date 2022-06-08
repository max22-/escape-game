#if defined(SALLE_AUTOMNE)

#include "config.h"
#include "light.h"
#include "pin_config.h"
#include "rooms.h"
#include "sensor.h"
#include "button.h"

Button button(35);
Filter filtered_button(button, BUTTON_FILTER_DELAY /* us */, FILTER_COEFF);

static SemaphoreHandle_t mutex;
static int sensor_value = 0;
#define SENSOR_MIN_DURATION 3000 // us


void sensor_task(void *params)
{
  uint16_t threshold = Config.thresholds(0);
  pinMode(SENSORS_DATA, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  digitalWrite(S0, 0);
  digitalWrite(S1, 0);
  digitalWrite(S2, 0);
  while(true) {
    if(analogRead(SENSORS_DATA) < threshold) {
      unsigned long timestamp = micros();
      while(analogRead(SENSORS_DATA) < threshold);
      if(micros() - timestamp >= SENSOR_MIN_DURATION) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        sensor_value = 1;
        xSemaphoreGive(mutex);
        delay(2000);
        xSemaphoreTake(mutex, portMAX_DELAY);
        sensor_value = 0;
        xSemaphoreGive(mutex);
      }

    }
    delay(1);
  }
}

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

static void light_task_set_day(void *params) {
  Light.set_level(Config.day());
  while (true)
    delay(1000);
}

static xTaskHandle chest_task_handle = NULL;

void open_chest() {
  digitalWrite(CHEST_RELAY_1, LOW);
  digitalWrite(CHEST_RELAY_2, HIGH);
}

void close_chest() {
  digitalWrite(CHEST_RELAY_1, HIGH);
  digitalWrite(CHEST_RELAY_2, LOW);
}

void stop_chest() {
  digitalWrite(CHEST_RELAY_1, LOW);
  digitalWrite(CHEST_RELAY_2, LOW);
}

static void chest_task(void *params) {
  Serial.printf("delay 1 = %d\n", (int)Config.chest_delay_1());
  Serial.printf("delay 2 = %d\n", (int)Config.chest_delay_2());
  Serial.println("open_chest");
  open_chest();
  delay(Config.chest_delay_1() * 1000);
  Serial.println("stop chest");
  stop_chest();
  delay(Config.chest_delay_2() * 1000);
  Serial.println("close chest");
  close_chest();
  delay(Config.chest_delay_1() * 1000);
  Serial.println("stop chest");
  stop_chest();
  chest_task_handle = NULL;
  vTaskDelete(NULL);
}

static void chest_manual_task(void *params) {
  close_chest();
  delay(Config.chest_delay_1() * 1000);
  stop_chest();
  chest_task_handle = NULL;
  vTaskDelete(NULL);
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
  button.begin();
  pinMode(DOOR_RELAY, OUTPUT);
  pinMode(CHEST_RELAY_1, OUTPUT);
  pinMode(CHEST_RELAY_2, OUTPUT);
  mutex = xSemaphoreCreateMutex();
  xTaskCreate(sensor_task, "sensor_task", 3000, NULL, 1, NULL);
  Light.begin();
  Light.set_level(Config.day());
  Profilab.rx(4, [](bool val) {
    if (val)
      Light.run_task(light_task_set_day);
  });
  Profilab.rx(7, [](bool val) {
    if(val) { 
      if(door_task_handle != NULL)
        vTaskDelete(door_task_handle);
      xTaskCreate(door_task, "door_task", 3000, NULL, 1,
                  &door_task_handle);
     }
  });
  Profilab.rx(8, [](bool val) {
    if (chest_task_handle != NULL)
      vTaskDelete(chest_task_handle);
    xTaskCreate(chest_manual_task, "chest_manual_task", 3000, NULL, 1,
                &chest_task_handle);
  });
  Profilab.rx(9, [](bool val) {
    if (val) {
      Serial.println("Starting light task 1");
      Light.run_task(light_task_1);
    }
  });
  Profilab.rx(10, [](bool val) {
    if (val) {
      Serial.println("Starting light task 2");
      Light.run_task(light_task_2);
    }
  });
  Profilab.rx(11, [](bool val) {
    if (chest_task_handle != NULL)
      vTaskDelete(chest_task_handle);
    xTaskCreate(chest_task, "chest_task", 3000, NULL, 1, &chest_task_handle);
  });
}

void room_handle() {
  xSemaphoreTake(mutex, 5 / portTICK_PERIOD_MS);
  Profilab.tx(0, sensor_value);
  xSemaphoreGive(mutex);
  Profilab.tx(10, filtered_button.output() < 0.05);
}

#endif