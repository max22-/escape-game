#include "light.h"

#include "freertos/task.h"

#define FREQ 1000

float clamp(const float val, const float min, const float max)
{
    if(val > max)
        return max;
    if(val < min)
        return min;
    return val;
}

void light_task(void *parameters)
{
    LightTaskParameters* light_task_parameters = (LightTaskParameters*)parameters;
    light_task_parameters->user_task_func(light_task_parameters->instance);
    while(true)
        delay(1000);
}

Light::Light(uint16_t offset, uint8_t pin, uint8_t channel, void (*user_task_func)(Light*)) : Coil(offset), channel(channel) { 
    ledcSetup(channel, FREQ, 8);
    ledcAttachPin(pin, channel);
    light_task_parameters = {user_task_func, this};
}

void Light::on_set(bool val)
{
    if(task_handle != nullptr)
        vTaskDelete(task_handle);
    xTaskCreate(light_task, "light_task", 3000, (void*)&light_task_parameters, 3, &task_handle);
}

void Light::set_level(float level)
{
    current_level = level;
    ledcWrite(channel, clamp(current_level, 0, 1) * 255);
}

void Light::ramp(float target_level, float time)
{
    float t = 0;
    const float inc_t = 10; // ms
    const float inc_l = inc_t * (target_level - current_level) / time;
    while(t < time) {
        set_level(current_level + inc_l);
        t += inc_t;
        delay(inc_t);
    }
}