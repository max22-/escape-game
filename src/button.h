#include "DSP.h"

class Button : public DSP {
public:
  Button(uint8_t pin);
  void begin();
  float output() override;
private:
  const uint8_t pin;
};