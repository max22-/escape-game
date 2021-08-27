#ifndef ROOMS_H
#define ROOMS_H

#include <Arduino.h>
#include "profilab.h"

void room_init();
void room_receive(profilab_data_t);
profilab_data_t room_send();


#endif