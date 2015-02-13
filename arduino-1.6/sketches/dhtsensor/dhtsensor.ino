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
 * Creation date: 06/05/2014
 *
 * Device:
 * Dual Temperature + Humidity sensor
 *
 * IMPORTANT: This sketch works only for panStamp NRG. For panStamp AVR
 * use temphumpress instead.
 *
 * Description:
 * This sketch generates a SWAP temperature+humidity sensor device
 * relying on the popular DHT22 senor and Adafruit's library
 * https://github.com/adafruit/DHT-sensor-library
 *
 * These devices are low-power enabled so they will enter low-power mode
 * just after reading the sensor values and transmitting them over the
 * SWAP network.
 *
 * Associated Device Definition Files, defining registers, endpoints and
 * configuration parameters:
 * temphum.xml (Dual Humidity + Temperature sensor)
 */
 
#include "regtable.h"
#include "swap.h"
#include "DHT.h"


/**
 * Uncomment if you are reading Vcc from A0. All battery-boards do this
 */
#define VOLT_SUPPLY_A0   1

#ifdef PANSTAMP_NRG
#define DHTPIN 14
#else
#define DHTPIN 16
#endif
DHT dht(DHTPIN, DHT22);
#define PWRPIN 15


/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  int i;

  // Init SWAP stack
  swap.init();
  
  // Initialize LED pins
  pinMode(LED, OUTPUT);

  // Initialize sensor pins
  dht.begin();
  pinMode(PWRPIN, OUTPUT);

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
//  digitalWrite(LED, HIGH);
  // Transmit sensor data
  swap.getRegister(REGI_SENSOR)->getData();
  // Transmit power voltage
  swap.getRegister(REGI_VOLTSUPPLY)->getData();
//  digitalWrite(LED, LOW);

  // Sleep
  swap.goToSleep();
}

