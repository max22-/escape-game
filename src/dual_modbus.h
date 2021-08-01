#ifndef MODBUS_H
#define MODBUS_H

#include <ModbusRTU.h>
#include <unordered_set>
#include "config.h"
#if defined(USE_WIFI)
#include <ModbusIP_ESP8266.h>
#endif

class Register {
public:
    Register(unsigned int offset);
    unsigned int get_offset() const;
    virtual void task() = 0;

private:
    const unsigned int offset;
};

class Coil : public Register {
public:
    Coil(unsigned int offset);
    virtual void on_set(bool val) = 0;
    void task() override;
};

class InputRegister : public Register {
public:
    InputRegister(unsigned int offset);
    virtual void task() = 0;
protected:
    void setValue(uint16_t val);
};

class DualModbusClass {
public:
    void begin();
    void task();

private:
    ModbusRTU modbusRTU;
    const int SLAVE_ID = 1;

    #if defined(USE_WIFI)
    ModbusIP modbusIP;
    #endif

    std::unordered_set<Register*> registers;

    void addCoil(Coil*);
    void addInputRegister(InputRegister*);
    friend class Register;
    friend class Coil;
    friend class InputRegister;
};

extern DualModbusClass Modbus;

#endif