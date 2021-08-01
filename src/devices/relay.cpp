#include "relay.h"

Relay::Relay(uint16_t offset, uint8_t pin) : Coil(offset), pin(pin) { pinMode(pin, OUTPUT); }

void Relay::on_set(bool val)
{
    if(val)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}