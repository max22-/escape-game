#include "dual_modbus.h"
#include "debug.h"

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
    for(Register* r: registers)
        r->task();
}

void DualModbusClass::addCoil(Coil* coil)
{
    const uint16_t offset = coil->get_offset();
    cbModbus callback = [coil](TRegister* reg, uint16_t val) -> uint16_t { coil->on_set(COIL_BOOL(val)); return val;};

    modbusRTU.addCoil(offset);
    modbusRTU.onSetCoil(offset, callback);
}


void DualModbusClass::addInputRegister(InputRegister *ireg)
{
    modbusRTU.addIreg(ireg->get_offset());
    registers.insert(ireg);
}

DualModbusClass Modbus;

Register::Register(unsigned int offset) : offset(offset) {}
unsigned int Register::get_offset() const
{
    return offset;
}

Coil::Coil(unsigned int offset) : Register(offset) { Modbus.addCoil(this); }
void Coil::task() {}

InputRegister::InputRegister(unsigned int offset) : Register(offset) {
    Modbus.addInputRegister(this);
}

void InputRegister::setValue(uint16_t val)
{
    Modbus.modbusRTU.Ireg(get_offset(), val);
}