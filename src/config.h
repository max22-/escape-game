#ifndef CONFIG_H
#define CONFIG_H

#define USE_OTA

#if defined(SALLE_ETE)
  #define WIFI_SSID "Salle-Ete"
#elif defined(SALLE_AUTOMNE)
  #define WIFI_SSID "Salle-Automne"
#elif defined(SALLE_HIVER)
  #define WIFI_SSID "Salle-Hiver"
#elif defined(SALLE_PRINTEMPS)
  #define WIFI_SSID "Salle-Printemps"
#endif

#define WIFI_PASSWORD "chasseurs"

#define NIGHT (0.2)
#define DAY (1.0)

#endif