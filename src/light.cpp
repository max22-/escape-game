#include <Arduino.h>
#include "freertos/task.h"

#include "light.h"
#include "config.h"


static TaskHandle_t task_handle = nullptr;
static void (*light_task)(void*) = nullptr;
static float current_level = 0;

float clamp(const float val, const float min, const float max)
{
    if(val > max)
        return max;
    if(val < min)
        return min;
    return val;
}

void light_begin(void (*task)(void*)) 
{ 
    ledcSetup(0, PWM_FREQ, 16);
    ledcAttachPin(LIGHT, 0);
    light_task = task;
}

void light_trigger()
{
    if(task_handle != nullptr)
        vTaskDelete(task_handle);
    xTaskCreate(light_task, "light_task", 3000, nullptr, 3, &task_handle);
}

void light_set_level(float level)
{
    current_level = level;
    ledcWrite(0, pow(clamp(current_level, 0, 1), 4) * 65535);
}

void light_ramp(float target_level, float time)
{
    time *= 1000; // conversion to milliseconds
    float t = 0;
    const float inc_t = 10; // ms
    const float inc_l = inc_t * (target_level - current_level) / time;
    while(t < time) {
        light_set_level(current_level + inc_l);
        t += inc_t;
        delay(inc_t);
    }
}