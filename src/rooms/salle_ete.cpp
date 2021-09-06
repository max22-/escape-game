#if defined(SALLE_ETE)

#include "rooms.h"
#include "sensor.h"
#include "config.h"
#include "light.h"

#define BUTTON_PIN 23

static void light_task(void* params)
{
    light_set_level(0);
    light_ramp(1, 10000);
    while(true)
        delay(1000);
}

void room_init()
{
    pinMode(IO0, OUTPUT);
    digitalWrite(IO0, LOW);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    sensors_init();
    light_begin(light_task);
}

void room_receive(profilab_data_t data)
{
    printf("channel = %d, n = %lf\n", data.channel, data.n);
    digitalWrite(IO0, (((int)data.n) & 1) == 1 ? HIGH : LOW);
    if(((int)data.n) & 2) light_trigger();
}

profilab_data_t room_send()
{
    profilab_data_t data = {0, 0.1};
    for(int i = 0; i < 8; i++)
        data.n += (sensor_read(i) > SENSORS_THRESHOLD) << i;
    
    if(digitalRead(BUTTON_PIN) == HIGH)
        data.n += 1 << 8;
    return data;
}

#endif