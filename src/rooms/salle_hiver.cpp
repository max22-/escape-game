#if defined(SALLE_HIVER)

#include "rooms.h"
#include "sensor.h"
#include "pin_config.h"
#include "config.h"
#include "light.h"


static void light_task(void* params)
{
    Light.set_level(0.4);
    for(int i = 0; i < 5; i++) {
        Light.set_level(0.6, 4);
        Light.set_level(0.4, 4);
    }
    while(true)
        delay(1000);
}

void room_init()
{
    pinMode(DOOR_RELAY, OUTPUT);
    pinMode(HOT_PLATE, OUTPUT);
    pinMode(SPOT, OUTPUT);
    Sensors.begin();
    Light.begin();
    Light.set_level(0.4);
    Profilab.rx(0, [](bool val) {
        if(val)
            Light.run_task(light_task);
    });
    Profilab.rx(6, [](bool val) {
        digitalWrite(SPOT, val ? HIGH : LOW);
    });
    Profilab.rx(7, [](bool val) {
        digitalWrite(DOOR_RELAY, val ? HIGH : LOW);
    });
    Profilab.rx(8, [](bool val) {
        digitalWrite(HOT_PLATE, val ? HIGH : LOW);
    });

}

void room_handle()
{
    for(int i = 0; i < 7; i++)
        Profilab.tx(i, Sensors.read(i) > SENSORS_THRESHOLD);
}

#endif