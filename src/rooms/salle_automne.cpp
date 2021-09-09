#if defined(SALLE_AUTOMNE)

#include "rooms.h"
#include "sensor.h"
#include "config.h"
#include "light.h"

const uint8_t buttons[] = {23, 35, 34, 39, 21};
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

void room_init()
{
    for(int i = 0; i < sizeof(buttons); i++)
        pinMode(buttons[i], INPUT_PULLUP);
    pinMode(black_light, OUTPUT);
    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);
    sensors_init();
    light_begin(light_task);
}

void room_receive(profilab_data_t data)
{
    printf("channel = %d, n = %lf\n", data.channel, data.n);
    uint8_t channel = data.channel;
    int ni = (int)data.n;

    if(channel == 0) {
        if(ni & (1 << 7)) digitalWrite(relay, HIGH); else digitalWrite(relay, LOW);
        if(ni & (1 << 8)) digitalWrite(black_light, HIGH); else digitalWrite(black_light, LOW);
        if(ni & (1 << 9)) light_trigger();
    }
}

profilab_data_t room_send()
{
    profilab_data_t data = {0, 0.1};
    
    for(int i = 0; i < 7; i++)
        data.n += (sensor_read(i) > SENSORS_THRESHOLD) << i;
    
    for(int i = 0; i < sizeof(buttons); i++) {
        if(digitalRead(buttons[i]) == LOW)
            data.n += 1 << (10 + i);
    }

    return data;
}

#endif