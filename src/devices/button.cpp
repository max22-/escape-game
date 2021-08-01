#include "button.h"

Button::Button(uint16_t offset, uint8_t pin) : DiscreteInput(offset), pin(pin) { pinMode(pin, INPUT_PULLUP); }

void Button::task()
{
    setValue(digitalRead(pin) == LOW);
}