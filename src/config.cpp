#include "config.h"
#include <sstream>

ConfigClass::ConfigClass() {
  parameters = {
      {"seuil0", &_thresholds[0]},
      {"seuil1", &_thresholds[1]},
      {"seuil2", &_thresholds[2]},
      {"seuil3", &_thresholds[3]},
      {"seuil4", &_thresholds[4]},
      {"seuil5", &_thresholds[5]},
      {"seuil6", &_thresholds[6]},
#ifdef SALLE_AUTOMNE
      {"tempo_coffre_1", &_chest_delay_1},
      {"tempo_coffre_2", &_chest_delay_2},
#endif
#ifndef SALLE_HIVER
      {"nuit", &_night},
      {"jour", &_day},
#endif
      {"tempo1", &_delay1},
      {"tempo2", &_delay2},

  };
}

bool ConfigClass::begin() {
  preferences.begin("config", false);
  for (const auto p : parameters) {
    if (preferences.isKey(p.first.c_str()))
      *p.second = preferences.getFloat(p.first.c_str());
    else
      return false;
  }
  return true;
}

bool ConfigClass::define(char *name, float value) {
  if (parameters.count(name) == 0)
    return false;
  preferences.putFloat(name, value);
  *parameters[name] = value;
  return true;
}

std::string ConfigClass::dump() {
  std::stringstream result;
  for (const auto p : parameters) {
    result << p.first << ":\t";
    if (preferences.isKey(p.first.c_str()))
      result << preferences.getFloat(p.first.c_str());
    else
      result << "?";
    result << std::endl;
  }
  return result.str();
}

#ifdef SALLE_AUTOMNE
float ConfigClass::chest_delay_1() { return _chest_delay_1; }
float ConfigClass::chest_delay_2() { return _chest_delay_2; }
#endif

#ifndef SALLE_HIVER
float ConfigClass::night() { return _night; }
float ConfigClass::day() { return _day; }
#endif
uint16_t ConfigClass::thresholds(uint8_t i) { return _thresholds[i]; }
float ConfigClass::delay1() { return _delay1; }
float ConfigClass::delay2() { return _delay2; }

ConfigClass Config;