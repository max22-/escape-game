#if defined(SALLE_ETE)

#include "rooms.h"
#include "sensor.h"
#include "pin_config.h"
#include "config.h"
#include "light.h"

const uint8_t buttons[] = {23, 35, 34, 39};

static void light_task_1(void* params)
{
    Light.set_level(NIGHT);
    Light.set_level(DAY, 8.0);
    while(true)
        delay(1000);
}

static void light_task_2(void* params)
{
    Light.set_level(DAY);
    Light.set_level(NIGHT, 6.0);
    while(true)
        delay(1000);
}

void room_init()
{
    for(int i = 0; i < sizeof(buttons); i++)
        pinMode(buttons[i], INPUT_PULLUP);
    pinMode(BLACK_LIGHT, OUTPUT);
    pinMode(DOOR_RELAY, OUTPUT);
    Sensors.begin();
    Light.begin();
    Light.set_level(NIGHT);
    Profilab.rx(7, [](bool val) {
        digitalWrite(DOOR_RELAY, val ? HIGH : LOW);
    });
    Profilab.rx(8, [](bool val) {
        digitalWrite(BLACK_LIGHT, val ? HIGH : LOW);
    });
    Profilab.rx(9, [](bool val) {
        if(val)
            Light.run_task(light_task_1);
    });
    Profilab.rx(10, [](bool val) {
        if(val)
            Light.run_task(light_task_2);
    });
}

void room_handle()
{
    for(int i = 0; i < 7; i++)
        Profilab.tx(i, Sensors.read(i) > SENSORS_THRESHOLD);
    
    for(int i = 0; i < sizeof(buttons); i++)
        Profilab.tx(10+i, digitalRead(buttons[i]) == LOW);
}

#endif