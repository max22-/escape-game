#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

#define USE_OTA

#define IP_1 192
#define IP_2 168
#define IP_3 0

#if defined(SALLE_ETE)
  #define IP_4 210
#elif defined(SALLE_AUTOMNE)
  #define IP_4 220
#elif defined(SALLE_HIVER)
  #define IP_4 230
#elif defined(SALLE_PRINTEMPS)
  #define IP_4 240
#endif



#define WIFI_SSID "ARMORSTUDIO"
#define WIFI_PASSWORD "1234554321"

#define NIGHT (0.2)
#define DAY (1.0)

#define SENSORS_THRESHOLD 500

#endif