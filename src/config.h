#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include <Preferences.h>

extern Preferences preferences;

#define USE_OTA
#define USE_FE_REPL

#define IP_1 192
#define IP_2 168
#define IP_3 0

#if defined(SALLE_ETE)
  #define IP_4 210
  #define DEVICE_NAME "Salle Eté"
#elif defined(SALLE_AUTOMNE)
  #define IP_4 220
  #define DEVICE_NAME "Salle Automne"
#elif defined(SALLE_HIVER)
  #define IP_4 230
  #define DEVICE_NAME "Salle Hiver"
#elif defined(SALLE_PRINTEMPS)
  #define IP_4 240
  #define DEVICE_NAME "Salle Printemps"
#endif



#define WIFI_SSID "ARMORSTUDIO"
#define WIFI_PASSWORD "1234554321"

#define NIGHT (0.2)
#define DAY (1.0)

#define SENSORS_THRESHOLD 500

#endif