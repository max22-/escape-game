#include <Arduino.h>
#include "sensor.h"
#include "pin_config.h"

Sensor::Sensor(uint8_t number) : number(number) {}

void Sensor::begin() {
  pinMode(SENSORS_TRIGGER, OUTPUT);
  pinMode(SENSORS_DATA, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
}

float Sensor::output() {
  const uint8_t s0 = number & 1 ? HIGH : LOW;
  const uint8_t s1 = number & 2 ? HIGH : LOW;
  const uint8_t s2 = number & 4 ? HIGH : LOW;
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
