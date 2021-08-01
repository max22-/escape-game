#include "dual_modbus.h"

void DualModbusClass::begin()
{
    modbusRTU.begin(&Serial);
    modbusRTU.slave(SLAVE_ID);
    #if defined(USE_WIFI)
    modbusIP.server();
    #endif
}

void DualModbusClass::task()
{
    modbusRTU.task();
    #if defined(USE_WIFI)
    modbusIP.task();
    #endif
}

void DualModbusClass::addCoil(Coil* coil)
{
    const uint16_t offset = coil->get_offset();
    cbModbus callback = [coil](TRegister* reg, uint16_t val) -> uint16_t { coil->on_set(COIL_BOOL(val)); return val;};

    modbusRTU.addCoil(offset);
    modbusRTU.onSetCoil(offset, callback);
    #if defined(USE_WIFI)
    modbusIP.addCoil(coil->get_offset());
    modbusIP.onSetCoil(offset, callback);
    #endif
}

DualModbusClass Modbus;

Register::Register(unsigned int offset) : offset(offset) {}
unsigned int Register::get_offset() const
{
    return offset;
}

Coil::Coil(unsigned int offset) : Register(offset) { Modbus.addCoil(this); }

InputRegister::InputRegister(unsigned int offset) : Register(offset) {}