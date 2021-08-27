#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

void sensors_init();
uint16_t sensor_read(uint8_t sensor_number);

#endif