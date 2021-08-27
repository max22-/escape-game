#ifndef CONFIG_H
#define CONFIG_H

// Wifi
#if defined(ROOM1)
    #define WIFI_SSID "Salle 1"
#elif defined(ROOM2)
    #define WIFI_SSID "Salle 2"
#elif defined(ROOM3)
    #define WIFI_SSID "Salle 3"
#elif defined(ROOM4)
    #define WIFI_SSID "Salle 4"
#elif defined(ROOM5)
    #define WIFI_SSID "Salle 5"
#endif
#define WIFI_PASSWORD "chasseurs"


// Pins
#define SENSORS_TRIGGER 2
#define SENSORS_DATA 36

#define S0 32
#define S1 33
#define S2 27

#define IO0 19
#define IO1 18
#define IO2 5
#define IO3 17
#define IO4 16
#define IO5 4

#define LIGHT 25
#define PWM_FREQ 1000

// Optical sensors
#define SENSORS_DELAY 20   // microseconds
#define SENSORS_THRESHOLD 2000



#endif