#ifndef SENSOR_H
#define SENSOR_H

#include "../dual_modbus.h"

class Sensor : InputRegister {
public:
    Sensor(uint16_t offset, uint8_t number);
    void task() override;
private:
    const uint8_t sensorNumber;
    static bool isInit;
};

#endif