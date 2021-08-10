#ifndef LIGHT_H
#define LIGHT_H

#include "../dual_modbus.h"
#include <vector>

class Light;

struct LightTaskParameters {
    void (*user_task_func)(Light*);
    Light* instance;
};

class Light : public Coil {
public:
    Light(uint16_t offset, uint8_t pin, uint8_t channel, void (*task_func)(Light*));
    void on_set(bool val) override;
    void set_level(float level);
    void ramp(float target_level, float time);

private:
    const uint8_t channel;
    TaskHandle_t task_handle = nullptr;
    LightTaskParameters light_task_parameters;
    float current_level = 0;
};

#endif