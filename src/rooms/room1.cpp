#if defined(ROOM1)

#include "../devices/relay.h"
#include "../devices/restart.h"

void init_room()
{
    new Relay(0, 19);
    new Restart(1);
}

#endif