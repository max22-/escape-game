#include <Arduino.h>
#include "button.h"

Button::Button(uint8_t pin) : pin(pin) {}
void Button::begin() { pinMode(pin, INPUT_PULLUP); }
float Button::output() { return digitalRead(pin); }