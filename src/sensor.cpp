#include "sensor.h"
#include "pin_config.h"

void SensorsClass::begin() {
  pinMode(SENSORS_TRIGGER, OUTPUT);
  pinMode(SENSORS_DATA, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
}

uint16_t SensorsClass::read(uint8_t sensor_number) {
  const uint8_t s0 = sensor_number & 1 ? HIGH : LOW;
  const uint8_t s1 = sensor_number & 2 ? HIGH : LOW;
  const uint8_t s2 = sensor_number & 4 ? HIGH : LOW;
  digitalWrite(S0, s0);
  digitalWrite(S1, s1);
  digitalWrite(S2, s2);

  digitalWrite(SENSORS_TRIGGER, LOW);
  delayMicroseconds(SENSORS_DELAY);
  int v1 = analogRead(SENSORS_DATA); // Mesure de la lumière infrarouge ambiante
                                     // (capteur éteint)
  digitalWrite(SENSORS_TRIGGER, HIGH);
  delayMicroseconds(SENSORS_DELAY);
  int v2 = analogRead(
      SENSORS_DATA); // Mesure de la lumière réfléchie avec le capteur allumé
  digitalWrite(SENSORS_TRIGGER, LOW);
  // Serial.printf("sensor %d : ambient %d, lit %d, return %d\n", sensor_number,
  // v1, v2, abs(v2-v1));
  return abs(v2 - v1);
}

SensorsClass::SensorsClass() {}

SensorsClass Sensors;