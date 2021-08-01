#ifndef BUTTON_H
#define BUTTON_H

#include "../dual_modbus.h"

class Button : DiscreteInput {
public:
    Button(uint16_t offset, uint8_t pin);
    void task() override;
private:
    const uint8_t pin;
};

#endif