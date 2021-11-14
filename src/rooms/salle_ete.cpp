#if defined(SALLE_ETE)

#include "rooms.h"
#include "sensor.h"
#include "pin_config.h"
#include "light.h"

const uint8_t buttons[] = {23, 35, 34, 39};

static void light_task(void* params)
{
    light_set_level(0);
    light_ramp(1, 10.0);
    while(true)
        delay(1000);
}

void room_init()
{
    for(int i = 0; i < sizeof(buttons); i++)
        pinMode(buttons[i], INPUT_PULLUP);
    pinMode(BLACK_LIGHT, OUTPUT);
    pinMode(DOOR_RELAY, OUTPUT);
    digitalWrite(DOOR_RELAY, LOW);
    sensors_init();
    light_begin(light_task);
    Profilab.rx(7, [](bool val) {
        digitalWrite(DOOR_RELAY, val ? HIGH : LOW);
    });
    Profilab.rx(8, [](bool val) {
        digitalWrite(BLACK_LIGHT, val ? HIGH : LOW);
    });
    Profilab.rx(9, [](bool val) {
        if(val)
            light_trigger();
    });
}

void room_handle()
{
    for(int i = 0; i < 7; i++)
        Profilab.tx(i, sensor_read(i) > SENSORS_THRESHOLD);
    
    for(int i = 0; i < sizeof(buttons); i++)
        Profilab.tx(10+i, digitalRead(buttons[i]) == LOW);
}

#endif