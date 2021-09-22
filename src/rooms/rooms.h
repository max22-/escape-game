#ifndef ROOMS_H
#define ROOMS_H

#include <Arduino.h>
#include "profilab_tcp.h"

void room_init(ProfilabTCP&);
void room_handle(ProfilabTCP&);


#endif