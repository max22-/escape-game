#if defined(ROOM1)

#include "../devices/devices.h"

void light_profile(Light *light)
{
    light->set_level(0.1);
    delay(7000);
    light->ramp(1, 1000);
}

void init_room()
{
    new Relay(0, 19);
    new Restart(1);
    new Light(2, 25, 0, light_profile);

    new Sensor(0, 0);
    new Sensor(1, 1);

    new Button(0, 0);
}

#endif