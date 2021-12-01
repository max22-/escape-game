#if defined(SALLE_PRINTEMPS)

#include "rooms.h"
#include "sensor.h"
#include "pin_config.h"
#include "config.h"
#include "light.h"

const uint8_t buttons[] = {23, 35, 34, 39, IO3, IO4, IO5};

static void light_task_1(void* params)
{
    Light.set_level(Config.night());
    while(true)
        delay(1000);
}

static void light_task_2(void* params)
{
    Light.set_level(Config.night());
    Light.set_level(Config.day(), Config.delay1());
    while(true)
        delay(1000);
}

static void light_task_3(void* params)
{
    Light.set_level(Config.day());
    Light.set_level(Config.night(), Config.delay2());
    while(true)
        delay(1000);
}

static void light_task_4(void* params)
{
    Light.set_level(Config.day());
    Light.set_level(Config.night(), Config.delay3());
    while(true)
        delay(1000);
}



void room_init()
{
    for(int i = 0; i < sizeof(buttons); i++)
        pinMode(buttons[i], INPUT_PULLUP);
    pinMode(DOOR_RELAY, OUTPUT);
    pinMode(LEDS_RELAY, OUTPUT);
    pinMode(LASER, OUTPUT);
    digitalWrite(LASER, LOW);
    Light.begin();
    Light.set_level(Config.night());
    Profilab.rx(0, [](bool val) {
        if(val)
            Light.run_task(light_task_1);
    });
    Profilab.rx(9, [](bool val) {
        if(val)
            Light.run_task(light_task_2);
    });
    Profilab.rx(6, [](bool val) {
        if(val)
            Light.run_task(light_task_3);
    });
    Profilab.rx(10, [](bool val) {
        if(val)
            Light.run_task(light_task_4);
    });
    Profilab.rx(7, [](bool val) {
        digitalWrite(DOOR_RELAY, val ? HIGH : LOW);
    });
    Profilab.rx(12, [](bool val) {
        digitalWrite(LEDS_RELAY, val ? HIGH : LOW);
    });
    Profilab.rx(14, [](bool val) {
        digitalWrite(LASER, val ? HIGH : LOW);
    });   

}

void room_handle()
{
    for(int i = 0; i < sizeof(buttons); i++)
        Profilab.tx(1+i, digitalRead(buttons[i]) == LOW);
}

#endif