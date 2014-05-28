/**
 * sfpddm.ino
 *
 * Instructions
 * SFP DDM monitoring system base on PanStamp project and using SFPDDM library available at https://github.com/SloMusti/sfpddm
 * Product name: sfpddm (SFP DDM monitoring system)
 * Author: Luka Mustafa - Musti
 * Revision history
 * 21.1.2013 - version 1.0 released
 */
#include "regtable.h"
#include "panstamp.h"
#include "Arduino.h"
#include <SFPddm.h>
#include "I2C.h"

// Creating an object of SFP DDM library
SFPddm sfp;

// Defining variables
#define SERIALOUT

// Defining pins
//led indicating system status on=OK
#define LEDSTATUS            3
//led indicating data TX
#define LEDCOM               4

// Byte containing the SFP supported modes
byte supp;

// counter
int count;

void setupSFPddm(){
  count=0;
  //initialize SFPddm
  byte err = sfp.begin();
  //get supported operations
  supp = sfp.getSupported();
  //prinout
  #ifdef SERIALOUT
 
  if(err){
    Serial.println("Error! Check if SFP is present.");
  }
  else{
    Serial.println("Initialized SFOddm");
    Serial.print("Supported 0x");
    Serial.println(supp,HEX);
  }
  #endif
}

// Setup
void setup()
{
  // Setup variables
  int i;
  // Setup serial
  Serial.begin(9600);
  // Set pin directions and default values
  pinMode(LEDSTATUS, OUTPUT);
  pinMode(LEDCOM, OUTPUT);
  digitalWrite(LEDSTATUS, LOW);
  digitalWrite(LEDCOM, LOW);
  
  // PANSTAMP setup
  // Init panStamp
  panstamp.setHighTxPower();
  panstamp.init();
  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();
  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_SYNC);
  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LEDCOM, HIGH);
    delay(100);
    digitalWrite(LEDCOM, LOW);
    delay(400);
  }
  // Transmit periodic Tx interval
  getRegister(REGI_TXINTERVAL)->getData();
  // Switch to Rx OFF state
  panstamp.enterSystemState(SYSTATE_RXOFF);
  
  //Initialize SFPddm
  setupSFPddm();
  
  #ifdef SERIALOUT
    Serial.print("Initialized.");
  #endif
}

// Main operation loop
void loop()
{
  if(supp&0x40){
    //readMeasurements from the SFP and check for errors
    if(!sfp.readMeasurements()){
      
      digitalWrite(LEDCOM, HIGH);
      digitalWrite(LEDSTATUS, HIGH);
      getRegister(REGI_INFO)->getData();
      getRegister(REGI_CONTROL)->getData();
      getRegister(REGI_MEASUREMENT)->getData();
      getRegister(REGI_ALARM)->getData();
      getRegister(REGI_WARNING)->getData();
      
      #ifdef SERIALOUT
      Serial.println("");
      Serial.print("SFPddm monitoring, reading:");
      Serial.println(count++);
      
      Serial.print("Temperature: ");
      Serial.print(sfp.getTemperature()/256,DEC);
      Serial.println(" C");
      
      Serial.print("Voltage: ");
      Serial.print(sfp.getVoltage(),DEC);
      Serial.println(" uV");
      
      Serial.print("TX current: ");
      Serial.print(sfp.getTXcurrent(),DEC);
      Serial.println(" uA");
      
      Serial.print("TX power: ");
      Serial.print(sfp.getTXpower(),DEC);
      Serial.println(" uW");
      
      Serial.print("RX power: ");
      Serial.print(sfp.getRXpower(),DEC);
      Serial.println(" uW");
      
      Serial.print("Warnings: 0x");
      Serial.println(sfp.getWarnings(),HEX);
      
      Serial.print("Alarms: 0x");
      Serial.println(sfp.getAlarms(),HEX);
      
      //get control register
      byte control = sfp.getControl();
      Serial.print("Control: 0x");
      Serial.println(control,HEX);
      #endif
      
      digitalWrite(LEDCOM, LOW);
      
      
    }
    else{
      getRegister(REGI_INFO)->getData();
      #ifdef SERIALOUT
      Serial.println("Error! Check if SFP is present.");
      delay(100);
      digitalWrite(LEDSTATUS, LOW);
      //restart SFPddm
      setupSFPddm();
      #endif  
    }
  }
  delay(1000);

  // Sleep
  // panstamp.goToSleep();
}

