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

void DualModbusClass::addDiscreteInput(DiscreteInput *di)
{
    modbusRTU.addIsts(di->get_offset());
    registers.insert(di);
}

DualModbusClass Modbus;

Register::Register(uint16_t offset) : offset(offset) {}
uint16_t Register::get_offset() const
{
    return offset;
}

Coil::Coil(uint16_t offset) : Register(offset) { Modbus.addCoil(this); }
void Coil::task() {}

InputRegister::InputRegister(uint16_t offset) : Register(offset) 
{
    Modbus.addInputRegister(this);
}

void InputRegister::setValue(uint16_t val)
{
    Modbus.modbusRTU.Ireg(get_offset(), val);
}

DiscreteInput::DiscreteInput(uint16_t offset) : Register(offset)
{
    Modbus.addDiscreteInput(this);
}

void DiscreteInput::setValue(bool val)
{
    Modbus.modbusRTU.Ists(get_offset(), val);
}