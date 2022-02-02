#include "config.h"
#include <sstream>

ConfigClass::ConfigClass() {
  parameters = {
      {"seuil", &_threshold},
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
#ifdef SALLE_HIVER
      {"crepuscule1", &_twilight1},
      {"crepuscule2", &_twilight2},
#endif
#ifdef SALLE_PRINTEMPS
      {"tempo3", &_delay3},
#endif

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
#ifdef SALLE_HIVER
float ConfigClass::twilight1() { return _twilight1; }
float ConfigClass::twilight2() { return _twilight2; }
#endif
uint16_t ConfigClass::threshold() { return _threshold; }
float ConfigClass::delay1() { return _delay1; }
float ConfigClass::delay2() { return _delay2; }
#ifdef SALLE_PRINTEMPS
float ConfigClass::delay3() { return _delay3; }
#endif

ConfigClass Config;