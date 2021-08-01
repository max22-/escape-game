#include "restart.h"

Restart::Restart(uint16_t offset) : Coil(offset) {}

void Restart::on_set(bool val)
{
    if(val)
        ESP.restart();
}