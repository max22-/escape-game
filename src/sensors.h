#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class Sensors {
public:
    void begin();
    uint16_t read(uint8_t sensorNumber);
};

#endif