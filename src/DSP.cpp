#include "DSP.h"
#include <Arduino.h>

Filter::Filter(DSP& source, int _delay, float b) :
  source(source), _delay(_delay), b(b) {}
float Filter::output() {
    float x = source.output();
    if(!started) {
        y = x;
        prev_micros = micros();
        started = true;
    }
    y += (micros() - prev_micros) / _delay * b * (x - y);
    prev_micros = micros();
    return y;
}