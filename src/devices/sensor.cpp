#include "sensor.h"
#include "config.h"

Sensor::Sensor(uint16_t offset, uint8_t sensorNumber) : InputRegister(offset), sensorNumber(sensorNumber)
{
    if(!isInit) {
        pinMode(SENSORS_TRIGGER, OUTPUT);
        pinMode(SENSORS_DATA, INPUT);
        pinMode(S0, OUTPUT);
        pinMode(S1, OUTPUT);
        pinMode(S2, OUTPUT);
        isInit = true;
    }
}

void Sensor::task()
{
    const uint8_t s0 = sensorNumber & 1 ? HIGH : LOW;
    const uint8_t s1 = sensorNumber & 2 ? HIGH : LOW;
    const uint8_t s2 = sensorNumber & 4 ? HIGH : LOW;
    digitalWrite(S0, s0);
    digitalWrite(S1, s1);
    digitalWrite(S2, s2);

    digitalWrite(SENSORS_TRIGGER, LOW);
    delayMicroseconds(SENSORS_DELAY);
    int v1 = analogRead(SENSORS_DATA);    // Mesure de la lumière infrarouge ambiante (capteur éteint)
    digitalWrite(SENSORS_TRIGGER, HIGH);
    delayMicroseconds(SENSORS_DELAY);
    int v2 = analogRead(SENSORS_DATA);    // Mesure de la lumière réfléchie avec le capteur allumé
    digitalWrite(SENSORS_TRIGGER, LOW);
    //Serial.printf("sensor %d : ambient %d, lit %d, return %d\n", sensorNumber, v1, v2, abs(v2-v1));
    setValue(abs(v2 - v1));
}

bool Sensor::isInit = false;