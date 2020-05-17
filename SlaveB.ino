#include <ModbusRtu.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);
 
#define slaveID 11
#define trigPin 9
#define echoPin 10
#define pwmLED  11

int distCM =0;
int pwmValue=0;
unsigned long lastPrint=0;

// data array for modbus network sharing
uint16_t au16data[4] = {
  slaveID, 225, 8888, 9999};

Modbus slave(slaveID,mySerial,12); // this is slave @1 and RS-232 or USB-FTDI

void setup() {
  Serial.begin(9600);
  mySerial.begin( 19200 ); // baud-rate at 19200
  slave.start();
  delay(10);
}

void loop() {
  slave.poll( au16data, 4 );
  if (millis() - lastPrint>200){
    Serial.print(au16data[0]); Serial.print(":");
    Serial.print(au16data[1]); Serial.print(":");
    Serial.print(au16data[2]); Serial.println();
    lastPrint = millis();
  }
  au16data[1]=analogRead(A2); //for heartbeat sensor
  actuator();
}

void actuator(){
 pwmValue = au16data[2]*0.25;
 analogWrite(pwmLED,pwmValue);
}
