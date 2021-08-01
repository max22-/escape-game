#ifndef RESTART_H
#define RESTART_H

#include "../dual_modbus.h"

class Restart : public Coil {
public:
    Restart(uint16_t offset);
    void on_set(bool val) override;
};

#endif