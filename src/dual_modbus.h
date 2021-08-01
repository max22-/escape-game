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
    Register(uint16_t offset);
    uint16_t get_offset() const;
    virtual void task() = 0;

private:
    const uint16_t offset;
};

class Coil : public Register {
public:
    Coil(uint16_t offset);
    virtual void on_set(bool val) = 0;
    void task() override;
};

class InputRegister : public Register {
public:
    InputRegister(uint16_t offset);
protected:
    void setValue(uint16_t val);
};

class DiscreteInput : public Register {
public:
    DiscreteInput(uint16_t offset);
protected:
    void setValue(bool val);
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
    void addDiscreteInput(DiscreteInput *);
    friend class Register;
    friend class Coil;
    friend class InputRegister;
    friend class DiscreteInput;
};

extern DualModbusClass Modbus;

#endif