// arduino mega master testing code
// modbus RS485 read and write register
// V1.1 
// 1 master 2 slave, 
// 1 slave : 1 input 1 output

#include <ModbusRtu.h>
#define slaveNumber 5
#define delayCom 15
#define maxQuery 2*2//slaveNumer*2

uint8_t u8state; //!< machine state
uint8_t u8query; //!< pointer to message query

uint16_t dataBus[slaveNumber*3];
uint16_t lastPrint=100;
int slaveID[slaveNumber] = {11,12,13,14,15};
/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : Serial1 port
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus master(0,Serial1,2); // ID, seriapNumber, enablePin

/**
 * This is an structe which contains a query to an slave device
 */
modbus_t telegram[slaveNumber*2];

unsigned long u32wait;

void init_modBus(){
  int num=0;
  int addr=0;
////SLAVE 1
  // Read 1 data from Slave 11
  telegram[num].u8id = slaveID[0]; // slave address
  telegram[num].u8fct = 3; // function code (this one is registers read)
  telegram[num].u16RegAdd = 0; // start address in slave
  telegram[num].u16CoilsNo = 2; // number of elements (coils or registers) to read
  telegram[num].au16reg = dataBus; // pointer to a memory array in the Arduino
  num+=1;
  addr+=2;
  
  // Write 1 data to Slave 11
  telegram[num].u8id = slaveID[0]; // slave address
  telegram[num].u8fct = 6; // function code (this one is write a multiple register)
  telegram[num].u16RegAdd = 2; // start address in slave
  telegram[num].u16CoilsNo = 1; // number of elements (coils or registers) to write
  telegram[num].au16reg = dataBus+2; // pointer to a memory array in the Arduino
  num+=1;
  addr+=1;

//SLAVE 2
  // Read 1 data from Slave 2
  telegram[num].u8id = slaveID[1]; // slave address
  telegram[num].u8fct = 3; // function code (this one is registers read)
  telegram[num].u16RegAdd = 0; // start address in slave
  telegram[num].u16CoilsNo = 2; // number of elements (coils or registers) to read
  telegram[num].au16reg = dataBus+3; // pointer to a memory array in the Arduino
  num+=1;
  addr+=2;
  
  // Write 1 data to Slave 2
  telegram[num].u8id = slaveID[1]; // slave address
  telegram[num].u8fct = 16; // function code (this one is write a multiple register)
  telegram[num].u16RegAdd = 2; // start address in slave
  telegram[num].u16CoilsNo = 1; // number of elements (coils or registers) to write
  telegram[num].au16reg = dataBus+5; // pointer to a memory array in the Arduino
  num+=1;
  addr+=1;

  master.start();
  master.setTimeOut( 100 ); // if there is no answer in 100 ms, roll over
  u32wait = millis() + 40;
  u8state = u8query = 0; 
  
}

void rtuState(){
  switch( u8state ) {
  case 0: 
    if (millis() >= u32wait) u8state++; // wait state
    break;
  case 1: 
    master.query( telegram[u8query] ); // send query (only once)
    u8state++;
    u8query++;
    if (u8query >= maxQuery) 
      u8query = 0;
    break;
  case 2:
    master.poll(); // check incoming messages if communication in idle state
    if (master.getState() == COM_IDLE) {
      u8state = 0;
      u32wait = millis() + delayCom;  //delay for next state
    }
    break;
  }
}
void printData(){
  if (millis() - lastPrint>200){
    //print data to validate
    Serial.print(dataBus[0]); Serial.print(":");
    Serial.print(dataBus[1]); Serial.print(":");
    Serial.print(dataBus[2]); Serial.print("\t:\t");
    Serial.print(dataBus[3]); Serial.print(":");
    Serial.print(dataBus[4]); Serial.print(":");
    Serial.print(dataBus[5]); 
    Serial.println();
  }
}

void processData(){
  //data to be sent so slave 1 based on slave 2 data
  dataBus[2] = dataBus[4]*0.25; //convert 1023 to 255
  //data to be sent so slave 2 based on slave 1 data
  dataBus[5] = dataBus[1]*0.25; 
}

void setup() {
  Serial.begin (9600); //baud rate of Serial PC
  Serial1.begin( 19200 ); // baud-rate of RS485
  init_modBus();
}

void loop() {
  rtuState();
  printData();
  processData();
}
