#ifndef DSP_H
#define DSP_H

#include <stdint.h>

class DSP {
public:
  virtual float output();
};

class Filter : public DSP {
public:
  Filter(DSP& source, int _delay, float b);
  float output() override;

private:
  DSP& source;
  const int _delay;
  const float b;
  float prev_micros, y;
  bool started = false;
};

#endif