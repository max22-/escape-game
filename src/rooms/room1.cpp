#if defined(ROOM1)

#include "../devices/relay.h"

void init_room()
{
    new Relay(0, 19);
}

#endif