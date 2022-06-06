#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include <Preferences.h>
#include <unordered_map>

class ConfigClass {
public:
  ConfigClass();
  bool begin();

  bool define(char *name, float value);

  uint16_t thresholds(uint8_t i);
  float night();
  float day();
  float delay1();
  float delay2();
  float door_delay();
#ifdef SALLE_AUTOMNE
  float chest_delay_1();
  float chest_delay_2();
#endif

  std::string dump();

private:
  ConfigClass(const ConfigClass &) = delete;
  ConfigClass &operator=(const ConfigClass &) = delete;
  Preferences preferences;
  std::unordered_map<std::string, float *> parameters;

  float _thresholds[7], _night, _day, _delay1, _delay2, _door_delay;
#ifdef SALLE_AUTOMNE
  float _chest_delay_1;
  float _chest_delay_2;
#endif
};

extern ConfigClass Config;

#define BUTTON_FILTER_DELAY 50000 /* us */
#define SENSOR_FILTER_DELAY 50000 /* us */
#define FILTER_COEFF 0.5

#endif