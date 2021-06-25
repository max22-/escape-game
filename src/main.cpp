#include <Arduino.h>
#include <ModbusRTU.h>
#include <BluetoothSerial.h>
#include "sensors.h"

#include "config.h"

const int SENSOR_IREG = 0;
const int RELAY_OFFSET = 0;
ModbusRTU modbusRTU;
const int SLAVE_ID = 1;

Sensors sensors;

BluetoothSerial btSerial;


uint16_t cbRelay(TRegister* reg, uint16_t val) {
  digitalWrite(RELAY, COIL_BOOL(val));
  return val;
}

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  btSerial.begin("Escape-game");
  sensors.begin();
  pinMode(RELAY, OUTPUT);

  modbusRTU.begin(&Serial);
  modbusRTU.slave(SLAVE_ID);
  modbusRTU.addIreg(SENSOR_IREG);
  modbusRTU.addCoil(RELAY_OFFSET);
  modbusRTU.onSetCoil(RELAY_OFFSET, cbRelay);

}

void loop() {
  modbusRTU.task();
  uint16_t sensorsData[8];

  for(int i = 0; i < 8; i++)
    sensorsData[i] = sensors.read(i);
    
  for(int i = 0; i < 8; i++) 
    btSerial.printf("%d ", sensorsData[i]);
  btSerial.println();

  digitalWrite(RELAY, (millis() % 10000) < 5000);

  

  //modbusRTU.Ireg(SENSOR_IREG, val);


  //delay(500);
}