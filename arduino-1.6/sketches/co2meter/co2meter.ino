/*
 * co2meter
 *
 * Copyright (c) 2015 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 * 
 * Author: Francisco Barrena
 * Creation date: 04/10/2015
 *
 * Device:
 * CO2 + Temperature + Humidity sensor
 *
 * Description:
 * This application measures:
 *  - Ambient CO2 level from a 0-2000 ppm COZIR sensor
 *    (http://www.co2meter.com/products/cozir-0-2-co2-sensor)
 *  - Temperature and humidity from SI7021 sensor
 *
 * These devices are low-power enabled so they will enter low-power mode
 * just after reading the sensor values and transmitting them over the
 * SWAP network.
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * co2meter.xml
 */
 
#include "regtable.h"
#include "swap.h"
#include "HTU21D.h"
#include "Wire.h"
#include <HardwareSerial.h>

#define CO2_ENABLE_PIN   15

uint16_t co2Level = 0; // Holds the actual CO2 value

// Humidity and temperature sensor
HTU21D htu;

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  Serial.begin(9600);
  int i;
  
  pinMode(CO2_ENABLE_PIN, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  // Init SWAP stack
  swap.init();
  
  // Initialize Hhumidity+temperature sensor
  htu.begin();
  
  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  swap.enterSystemState(SYSTATE_SYNC);

  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(400);
  }

  // Transmit periodic Tx interval
  swap.getRegister(REGI_TXINTERVAL)->getData();
  // Transmit power voltage
  swap.getRegister(REGI_VOLTSUPPLY)->getData();
   // Switch to Rx OFF state
  swap.enterSystemState(SYSTATE_RXOFF);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  // Enable CO2 sensor
  digitalWrite(CO2_ENABLE_PIN, HIGH);
  
  // Transmit power voltage
  swap.getRegister(REGI_VOLTSUPPLY)->getData();

    // Transmit Htu sensor data
  swap.getRegister(REGI_HTUSENSOR)->getData();

  co2Level = readCO2();  
  // Transmit CO2 sensor data
  swap.getRegister(REGI_CO2SENSOR)->getData();

  
  // disable CO2 sensor
  digitalWrite(CO2_ENABLE_PIN, LOW);
     
  // Sleep
  swap.goToSleep();
 }

/**
 * readCO2
 *
 * Read CO2 level
 *
 * @return Current CO2 level
 */
uint16_t readCO2(void) 
{
 uint8_t co2Buffer[25];
 uint8_t co2Index = 0;
 uint8_t  ch;
 bool start = false, stop = false;
 
 while(!stop)
  {
    if(Serial.available())
    { 
      ch = Serial.read();
      switch (ch)
      {
        case 'Z':
          start = true;
          break;
        case 'z':      
          stop = true;
          break;
        default:
          if (start)
          {
            if(ch != 0x20)
              co2Buffer[co2Index++] = ch;
          }    
          break;          
      }
    }
  }

  return (uint16_t) atoi((char*)co2Buffer);
}
