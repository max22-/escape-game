#include <Arduino.h>
#include <ModbusIP_ESP8266.h>
#include <ModbusRTU.h>
#include <WiFi.h>

const int SENSOR_IREG = 0;
const int SWITCH_ISTS = 0;
const int RELAY_OFFSET = 0;
ModbusIP modbusIP;
ModbusRTU modbusRTU;
const int SLAVE_ID = 1;

#define IR_TRIGGER 13
#define IR_DATA 36
#define RELAY 23
#define POT 39
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

  WiFi.softAP("Escape game");

  modbusIP.server();		//Start Modbus IP
  // Add SENSOR_IREG register - Use addIreg() for analog Inputs
  modbusIP.addIreg(SENSOR_IREG);
  modbusIP.addIsts(SWITCH_ISTS);
  modbusIP.addCoil(RELAY_OFFSET);
  modbusIP.onSetCoil(RELAY_OFFSET, cbRelay);

  modbusRTU.begin(&Serial);
  modbusRTU.slave(SLAVE_ID);
  modbusRTU.addIreg(SENSOR_IREG);
  modbusRTU.addIsts(SWITCH_ISTS);
  modbusRTU.addCoil(RELAY_OFFSET);
  modbusRTU.onSetCoil(RELAY_OFFSET, cbRelay);

}

void loop() {
  modbusIP.task();
  modbusRTU.task();
  
/*
  while(true) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(pin, LOW);
    delayMicroseconds(500);
  }
*/
  digitalWrite(IR_TRIGGER, LOW);
  delayMicroseconds(200);
  int v1 = analogRead(IR_DATA);    // Mesure de la lumière infrarouge ambiante (capteur éteint)
  digitalWrite(IR_TRIGGER, HIGH);
  delayMicroseconds(200);
  int v2 = analogRead(IR_DATA);    // Mesure de la lumière réfléchie avec le capteur allumé
  int ir_val = abs(v2 - v1);
  modbusIP.Ireg(SENSOR_IREG, ir_val);
  modbusRTU.Ireg(SENSOR_IREG, ir_val);
  digitalWrite(IR_TRIGGER, LOW);

  int threshold = analogRead(POT);

  if(ir_val > threshold) {
    digitalWrite(LED, HIGH);
    modbusIP.Ists(SWITCH_ISTS, true);
    modbusRTU.Ists(SWITCH_ISTS, true);
  }
  else {
    digitalWrite(LED, LOW);
    modbusIP.Ists(SWITCH_ISTS, false);
    modbusRTU.Ists(SWITCH_ISTS, false);
  }

  delay(10);
}