/*
 * Copyright (c) 2014 Daniel Berenguer <contact@panstamp.com>
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
 * Author: Daniel Berenguer
 * Creation date: 08/08/2014
 *
 * Device:
 * Dual Temperature + Humidity sensor
 *
 * Description:
 * This sketch generates a SWAP temperature+humidity sensor device
 * relying on HTU21D sensor and Sparkfun's library
 * https://github.com/sparkfun/HTU21D_Breakout
 *
 * This library and the current sketch is also compatible with Silabs'
 * SI7021 temperature+humidity sensor. SI7021 is the sensor that can be
 * hosted by panStamp NRG on the bottom layer.
 *
 * These devices are low-power enabled so they will enter low-power mode
 * just after reading the sensor values and transmitting them over the
 * SWAP network.
 *
 * Associated Device Definition Files, defining registers, endpoints and
 * configuration parameters:
 * temphum.xml (Dual Humidity + Temperature sensor)
 */

#include "Wire.h"
#include "regtable.h"
#include "swap.h"
#include "HTU21D.h"


//Create an instance of the sensor object
HTU21D htu;

void setup()
{
  int i;

  // Init SWAP stack
  swap.init();
  
  // Initialize LED pins
  pinMode(LED, OUTPUT);

  // Initialize sensor
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

void loop()
{
//  digitalWrite(LED, HIGH);
  // Transmit sensor data
  swap.getRegister(REGI_SENSOR)->getData();
  // Transmit power voltage
  swap.getRegister(REGI_VOLTSUPPLY)->getData();
//  digitalWrite(LED, LOW);

  // Sleep
  swap.goToSleep();
}

