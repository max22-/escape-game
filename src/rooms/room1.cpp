#if defined(ROOM1)

#include "../devices/relay.h"
#include "../devices/restart.h"
#include "../devices/sensor.h"

void init_room()
{
    new Relay(0, 19);
    new Restart(1);
    new Sensor(0, 0);
    new Sensor(1, 1);
}

#endif