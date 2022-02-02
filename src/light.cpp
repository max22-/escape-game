#include <Arduino.h>
#include "freertos/task.h"

#include "light.h"
#include "pin_config.h"

float clamp(const float val, const float min, const float max) {
  if (val > max)
    return max;
  if (val < min)
    return min;
  return val;
}

void LightClass::begin() {
  ledcSetup(0, PWM_FREQ, 16);
  ledcAttachPin(LIGHT, 0);
}

void LightClass::run_task(void (*task)(void *)) {
  if (task_handle != nullptr)
    vTaskDelete(task_handle);
  xTaskCreate(task, "light_task", 3000, nullptr, 3, &task_handle);
}

void LightClass::set_level(float level, float time) {
  level /= 100;
  if (time == 0) {
    current_level = level;
    ledcWrite(0, pow(clamp(current_level, 0, 1), 4) * 65535);
    return;
  }

  time *= 1000; // conversion to milliseconds
  float t = 0;
  const float inc_t = 10; // ms
  const float inc_l = inc_t * (level - current_level) / time;
  while (t < time) {
    current_level += inc_l;
    ledcWrite(0, pow(clamp(current_level, 0, 1), 4) * 65535);
    t += inc_t;
    delay(inc_t);
  }
}

LightClass::LightClass() {}

LightClass Light;