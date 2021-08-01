#include "restart.h"
#include "../debug.h"
Restart::Restart(uint16_t offset) : Coil(offset) {}

void Restart::on_set(bool val)
{
    debug.printf("Restart::on_set(%s)\n", val ? "true" : "false");
    if(val)
        ESP.restart();
}