#ifndef RELAY_H
#define RELAY_H

#include "../dual_modbus.h"

class Relay : public Coil {
public:
    Relay(uint16_t offset, uint8_t pin);
    void on_set(bool val) override;
private:
    const uint8_t pin;
};

#endif