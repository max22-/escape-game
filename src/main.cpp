#include <Arduino.h>
#include <ModbusRTU.h>

const int SENSOR_IREG = 0;
const int RELAY_OFFSET = 0;
ModbusRTU modbusRTU;
const int SLAVE_ID = 1;

#define IR_TRIGGER 13
#define IR_DATA 36
#define RELAY 23
#define LED 2

//const int pin = 5;

uint16_t cbRelay(TRegister* reg, uint16_t val) {
  digitalWrite(RELAY, COIL_BOOL(val));
  return val;
}

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  pinMode(IR_TRIGGER, OUTPUT);
  pinMode(IR_DATA, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);

  modbusRTU.begin(&Serial);
  modbusRTU.slave(SLAVE_ID);
  modbusRTU.addIreg(SENSOR_IREG);
  modbusRTU.addCoil(RELAY_OFFSET);
  modbusRTU.onSetCoil(RELAY_OFFSET, cbRelay);

}

void loop() {
  modbusRTU.task();
  
  digitalWrite(IR_TRIGGER, LOW);
  delayMicroseconds(200);
  int v1 = analogRead(IR_DATA);    // Mesure de la lumière infrarouge ambiante (capteur éteint)
  digitalWrite(IR_TRIGGER, HIGH);
  delayMicroseconds(200);
  int v2 = analogRead(IR_DATA);    // Mesure de la lumière réfléchie avec le capteur allumé
  int ir_val = abs(v2 - v1);
  modbusRTU.Ireg(SENSOR_IREG, ir_val);
  digitalWrite(IR_TRIGGER, LOW);


  delay(10);
}