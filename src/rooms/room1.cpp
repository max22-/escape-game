#if defined(ROOM1)

#include "../devices/devices.h"

void light_profile(Light *light)
{
    light->set_level(0);
    light->ramp(1, 5000);
}

void init_room()
{
    new Relay(0, 19);
    new Light(1, 25, 0, light_profile);
    //new Restart(3);

    new Sensor(0, 0);
    new Sensor(1, 1);
    new Sensor(2, 2);
    new Sensor(3, 3);
    new Sensor(4, 4);
    new Sensor(5, 5);
    new Sensor(6, 6);

    new Button(0, 23);
    new Button(1, 35);
    new Button(2, 34);
    new Button(3, 39);

    

}

#endif