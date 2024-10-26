#include <ModbusRTUSlave.h>
#include <SoftwareSerial.h>
#include<avr/wdt.h>
#include <EEPROM.h>


#define FLOAT_SWITCH_1 54
#define FLOAT_SWITCH_2 56
#define FLOAT_SWITCH_3 58

#define OVERLOAD_START 60 
#define OVERLOAD_END 68

#define OVERLOAD_START_1 32
#define OVERLOAD_END_1 40

//#define FEEDING_PUMP_1 16
#define FEEDING_PUMP_2 17
#define FEEDING_PUMP_1 19
#define AERATION_PUMP_START 18
#define AERATION_PUMP_END 24
#define DISCHARGE_PUMP_START 26
#define DISCHARGE_PUMP_END 27

#define MODE_A_ON_DURATION 3600000 // 1 hour in seconds
#define MODE_A_OFF_DURATION 7200000 // 2 hours in seconds
#define MODE_B_ON_DURATION 7200000 // 2 hours in seconds
#define MODE_B_OFF_DURATION 3600000 // 1 hour in seconds*/

//#define MODE_A_ON_DURATION 5000 // 1 hour in seconds
//#define MODE_B_ON_DURATION 10000 // 2 hours in seconds*/


int mode = 0;
long runtimeA,runtimeB;
int angageA=0;
int angageB=0;

uint16_t holding[20]={0};
uint16_t input[20]={0};
bool dis[13]={0};
bool outcoil[60]={0};



bool isFloatSwitchActivated(int switchNumber) {
    // Implementation of reading float switch input
    if(digitalRead(switchNumber)==LOW){
      return true;
    }else{
       return false;
    }
    // replace with actual implementation
}

bool isOverloadActivated(int switchNumber) {
    // Implementation of reading overload trip input
       if(digitalRead(switchNumber)==LOW){
      return true;
    }else{
       return false;
    } // replace with actual implementation
}

void powerOutput(int outputNumber, bool state){
  digitalWrite(outputNumber, state);
    //Implementation of controlling output
}

void handlePumpFailure(int pumpNumber) {

    // Handle pump failure
    // Implement based on your requirements
}

void poweronAerationPumps(){
  /*int e = AERATION_PUMP_START;
  for(int r=0; r<=AERATION_PUMP_END-AERATION_PUMP_START; r++){
    digitalWrite(e,LOW);
    e=e+1;
  }//*/
    digitalWrite(18,LOW);
    digitalWrite(20,LOW);
    digitalWrite(21,LOW);
    digitalWrite(22,LOW);
    digitalWrite(23,LOW);
    digitalWrite(24,LOW);

  outcoil[12]=1;
  outcoil[13]=1;
  outcoil[14]=1;
  outcoil[15]=1;
  outcoil[2]=1;
  outcoil[3]=1;
}

void poweroffAerationPumps(){
  /*int f = AERATION_PUMP_START;
  for(int r=0; r<=AERATION_PUMP_END-AERATION_PUMP_START; r++){
    digitalWrite(f,HIGH);
    f=f+1;
  }//*/

    digitalWrite(18,HIGH);
    digitalWrite(20,HIGH);
    digitalWrite(21,HIGH);
    digitalWrite(22,HIGH);
    digitalWrite(23,HIGH);
    digitalWrite(24,HIGH);

  outcoil[12]=0;
  outcoil[13]=0;
  outcoil[14]=0;
  outcoil[15]=0;
  outcoil[2]=0;
  outcoil[3]=0;
}

SoftwareSerial mySerial(10, 11);// RX, TX
ModbusRTUSlave slave(mySerial);
const uint8_t ID =1;
int address;
long pstate,pstate2,intial;

void setup() {
 /*address=0;
 EEPROM.get(address, intial);
 if(intial!=2){
  pstate=0;
  pstate2=0;
  intial=2;
  runtimeA=0;
  runtimeB=0;

   address=5;
 EEPROM.put(address, pstate);
  address=15;
 EEPROM.put(address, pstate2);
 address=0;
 EEPROM.put(address, intial);
 address=25;
 EEPROM.put(address,runtimeA);
 address=35;
 EEPROM.put(address,runtimeB);
 }//*/
runtimeB=millis() + MODE_B_ON_DURATION;
mode=2;
  ioconf();
  slave.configureHoldingRegisters(holding, 20);
  slave.configureInputRegisters(input, 20);
  slave.configureDiscreteInputs(dis, 13);
  slave.configureCoils(outcoil, 60);
  slave.begin(ID,38400,SERIAL_8N1);

  wdt_disable();
  wdt_enable(WDTO_4S);
}

void loop() {
  wdt_reset();
  if(outcoil[4]==1){
pro();
  }else{
int y=16;
for(int u=0; u<=9; u++){
digitalWrite(y,HIGH);
y=y+1;
}
  }
  slave.poll();
}

void pro(){
         /* address=5;
          EEPROM.get(address,pstate);
          address=15;
          EEPROM.get(address,pstate2);
          address=25;
          EEPROM.get(address,runtimeA);
          address=35;
          EEPROM.get(address,runtimeB);//*/

       if (isFloatSwitchActivated(FLOAT_SWITCH_1) && angageA==0 && angageB==0) {
            mode=1;
            angageA=1;
            /*pstate=1;
            address=5;
            EEPROM.put(address,pstate);//*/
            powerOutput(FEEDING_PUMP_1, LOW);
            outcoil[10] = 1;
            // Activate mode A cycle
            runtimeA=millis() + MODE_A_ON_DURATION;
           // address=25;
           // EEPROM.put(address,runtimeA);
            poweronAerationPumps();
            
        } else {
          if(mode==1 && millis()> runtimeA){
            powerOutput(FEEDING_PUMP_1, HIGH);
            outcoil[10]=0;
            holding[19]=0;
            holding[17]=0;
    
            poweroffAerationPumps();
            angageA=0;
           /* pstate=0;
            address=5;
            EEPROM.put(address,pstate);//*/
            }
            //powerOutput(AERATION_PUMP_START, HIGH);
        }
        
        
        if (isFloatSwitchActivated(FLOAT_SWITCH_2) && angageB==0 && angageA==0) {
          mode=2;
          angageB=1;
           /* pstate2=1;
            address=15;
            EEPROM.put(address,pstate2);//*/
            powerOutput(FEEDING_PUMP_2, LOW);
            powerOutput(FEEDING_PUMP_1, LOW);
            outcoil[10] = 1;
            outcoil[11] = 1;
            // Activate mode B
            runtimeB=millis() + MODE_B_ON_DURATION;
            //address=35;
            //EEPROM.put(address,runtimeB);
            poweronAerationPumps();
            //powerOutput(AERATION_PUMP_START, LOW);
            // Start timer for mode B on outputs 3-8
        } else {
          if(mode==2 && millis()> runtimeB){
            powerOutput(FEEDING_PUMP_2, HIGH);
            powerOutput(FEEDING_PUMP_1, HIGH);
            outcoil[10] = 0;
            outcoil[11] = 0;
            holding[19]=0;
            holding[18]=0;
            holding[17]=0;
            // Stop timer for mode B on outputs 3-8
            poweroffAerationPumps();
            angageB=0;
           /* pstate2=0;
            address=15;
            EEPROM.put(address,pstate2);//*/
          }
            //powerOutput(AERATION_PUMP_START, HIGH);
          }
        

        if (isFloatSwitchActivated(FLOAT_SWITCH_3)) {
            powerOutput(DISCHARGE_PUMP_START, LOW);
            outcoil[0] = 1;
            powerOutput(DISCHARGE_PUMP_END, LOW);
            outcoil[1] = 1;
            holding[16]=10;
            // Activate discharge pump 1 and 2
        } else {
            powerOutput(DISCHARGE_PUMP_START, HIGH);
            outcoil[0] = 0;
            powerOutput(DISCHARGE_PUMP_END, HIGH);
            outcoil[1] = 0;
            holding[16]=0;
        }

        // Check overload trips and handle accordingly
        int i = OVERLOAD_START;
        while ( i <= OVERLOAD_END){
            if (isOverloadActivated(i)) {
                handlePumpFailure(i); // Map overload trip to pump number
            }
            i=i+2;
        }

          int o = OVERLOAD_START_1;
        while ( o <= OVERLOAD_END_1) {
            if (isOverloadActivated(o)) {
                handlePumpFailure(o);// Map overload trip to pump number
            }
            o=o+2;
        }

          int p = FLOAT_SWITCH_1;
        while ( p <= FLOAT_SWITCH_3) {
            if (isOverloadActivated(p)) {
                handlePumpFailure(p); // Map overload trip to pump number
            }
            p=p+2;
        }

        // Sleep for a while before checking again
       //delay(1000); // Sleep for 1 second

hmi();

}

void ioconf()
{
pinMode(16, OUTPUT);
pinMode(17, OUTPUT);
pinMode(18, OUTPUT);
pinMode(19, OUTPUT);
pinMode(20, OUTPUT);
pinMode(21, OUTPUT);
pinMode(22, OUTPUT);
pinMode(23, OUTPUT);
pinMode(24, OUTPUT);
//pinMode(25, OUTPUT);
pinMode(26, OUTPUT);
pinMode(27, OUTPUT);
/*pinMode(28, OUTPUT);
pinMode(29, OUTPUT);
pinMode(30, OUTPUT);
pinMode(31, OUTPUT);//*/

int y=16;
for(int u=0; u<=9; u++){
digitalWrite(y,HIGH);
y=y+1;
}
digitalWrite(26,HIGH);
digitalWrite(27,HIGH);

pinMode(A0,INPUT_PULLUP);
pinMode(A2,INPUT_PULLUP);
pinMode(A4,INPUT_PULLUP);
pinMode(A6,INPUT_PULLUP);
pinMode(A8,INPUT_PULLUP);
pinMode(A10,INPUT_PULLUP);
pinMode(A12,INPUT_PULLUP);
pinMode(A14,INPUT_PULLUP);
pinMode(32,INPUT_PULLUP);
pinMode(34,INPUT_PULLUP);
pinMode(36,INPUT_PULLUP);
pinMode(38,INPUT_PULLUP);
pinMode(40,INPUT_PULLUP);
}

void hmi(){
/*int y=16;
for(int x=0; x<=9; x++){
if(outcoil[x] == 1){
  digitalWrite(y,LOW);
}else{
digitalWrite(y,HIGH);
} 
y=y+1;
} //*/
if(digitalRead(54)==LOW){
  holding[0]=500;
  holding[1]=1;
  holding[19]=10;
}

if(digitalRead(56)==LOW){
  holding[0]=1000;
  holding[18]=10;
  holding[19]=10;
}

if(digitalRead(56)==LOW || digitalRead(54)==LOW){
  holding[17]=10;
}

/*int w=54;
for(int q=10; q<=17; q++){
  if(digitalRead(w)==LOW){
    outcoil[q]=1;
  }else{outcoil[q]=0;}
  w=w+2;
}
int r=32;
for(int e=0; e<=4; e++){
   if(digitalRead(r)==LOW){
    dis[e]=1;
  }else{dis[e]=0;}
r=r+2;
}//*/
}
