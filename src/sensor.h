#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class SensorsClass {
public:
  SensorsClass();
  void begin();
  uint16_t read(uint8_t sensor_number);

private:
  SensorsClass(const SensorsClass &) = delete;
  SensorsClass &operator=(const SensorsClass &) = delete;
};

extern SensorsClass Sensors;

#endif