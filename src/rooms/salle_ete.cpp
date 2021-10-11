#if defined(SALLE_ETE)

#include "rooms.h"
#include "sensor.h"
#include "config.h"
#include "light.h"

const uint8_t buttons[] = {23, 35, 34, 39};
/* LIGHT = 25 (in config.h) */
const uint8_t black_light = 26;
const uint8_t relay = IO0;

static void light_task(void* params)
{
    light_set_level(0);
    light_ramp(1, 10000);
    while(true)
        delay(1000);
}

void room_init(ProfilabTCP& profilabTCP)
{
    for(int i = 0; i < sizeof(buttons); i++)
        pinMode(buttons[i], INPUT_PULLUP);
    pinMode(black_light, OUTPUT);
    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);
    sensors_init();
    light_begin(light_task);
    profilabTCP.rx(7, [](bool val) {
        digitalWrite(relay, val ? HIGH : LOW);
    });
    profilabTCP.rx(8, [](bool val) {
        digitalWrite(black_light, val ? HIGH : LOW);
    });
    profilabTCP.rx(9, [](bool val) {
        light_trigger();
    });
}

void room_handle(ProfilabTCP& profilabTCP)
{
    for(int i = 0; i < 7; i++)
        profilabTCP.tx(i, sensor_read(i) > SENSORS_THRESHOLD);
    
    for(int i = 0; i < sizeof(buttons); i++)
        profilabTCP.tx(10+i, digitalRead(buttons[i]) == LOW);
}

#endif