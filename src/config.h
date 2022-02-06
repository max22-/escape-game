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

  uint16_t threshold();
  float night();
  float day();
  float delay1();
  float delay2();
#ifdef SALLE_AUTOMNE
  float chest_delay_1();
  float chest_delay_2();
#endif
#ifdef SALLE_PRINTEMPS
  float delay3();
#endif

  std::string dump();

private:
  ConfigClass(const ConfigClass &) = delete;
  ConfigClass &operator=(const ConfigClass &) = delete;
  Preferences preferences;
  std::unordered_map<std::string, float *> parameters;

  float _threshold, _night, _day, _delay1, _delay2;
#ifdef SALLE_AUTOMNE
  float _chest_delay_1;
  float _chest_delay_2;
#endif
#ifdef SALLE_PRINTEMPS
  float _delay3;
#endif
};

extern ConfigClass Config;

#define USE_OTA
#define USE_FE_REPL

#define IP_1 192
#define IP_2 168
#define IP_3 0

#if defined(SALLE_ETE)
#define IP_4 210
#define SOFT_AP_SSID "SSID-0"
#elif defined(SALLE_AUTOMNE)
#define IP_4 220
#define SOFT_AP_SSID "SSID-1"
#elif defined(SALLE_HIVER)
#define IP_4 230
#define SOFT_AP_SSID "SSID-2"
#elif defined(SALLE_PRINTEMPS)
#define IP_4 240
#define SOFT_AP_SSID "SSID-3"
#endif

#define SOFT_AP_PASSWORD "eogheog6f6z"

#define WIFI_SSID "ARMORSTUDIO"
#define WIFI_PASSWORD "1234554321"

#endif