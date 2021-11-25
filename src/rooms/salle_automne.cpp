#if defined(SALLE_AUTOMNE)

#include "rooms.h"
#include "sensor.h"
#include "pin_config.h"
#include "config.h"
#include "light.h"

const uint8_t button = 23;

static void light_task_1(void* params)
{
    Light.set_level(DAY);
    Light.set_level(NIGHT, 8.0);
    while(true)
        delay(1000);
}

static void light_task_2(void* params)
{
    Light.set_level(NIGHT);
    Light.set_level(DAY, 6.0);
    while(true)
        delay(1000);
}

void room_init()
{
    pinMode(button, INPUT_PULLUP);
    pinMode(DOOR_RELAY, OUTPUT);
    pinMode(ACTUATOR, OUTPUT);
    Sensors.begin();
    Light.begin();
    Light.set_level(DAY);
    Profilab.rx(7, [](bool val) {
        digitalWrite(DOOR_RELAY, val ? HIGH : LOW);
    });
    Profilab.rx(9, [](bool val) {
        if(val)
            Light.run_task(light_task_1);
    });
    Profilab.rx(10, [](bool val) {
        if(val)
            Light.run_task(light_task_2);
    });
    Profilab.rx(11, [](bool val) {
        digitalWrite(ACTUATOR, val ? HIGH : LOW);
    });
}

void room_handle()
{
    Profilab.tx(0, Sensors.read(0) > SENSORS_THRESHOLD);
    Profilab.tx(10, digitalRead(button) == LOW);
}

#endif