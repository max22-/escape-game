#include "DSP.h"

class Sensor : public DSP {
public:
  Sensor(uint8_t number);
  static void begin();
  float output() override;

private:
  const uint8_t number;
};

