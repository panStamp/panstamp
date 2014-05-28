/*
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
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
 * Creation date: 02/08/2013
 *
 * Device:
 * Respira sensor for CO and NO2
 *
 * Description:
 * This sketch can runs on panStamp's respira sensor. This sensor measures
 * humidity, temperature, CO and NO2 levels.
 *
 * Associated Device Definition Files, defining registers, endpoints and
 * configuration parameters:
 * respira.xml
 */
 
#include "regtable.h"
#include "panstamp.h"

/**
 * Uncomment if you are reading Vcc from A7. All battery-boards do this
 */
//#define VOLT_SUPPLY_A7

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * Wireless transmission interval (seconds)
 */
unsigned int txInterval;

unsigned int loopCnt;

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  int i;

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  // Init panStamp
  panstamp.init();

  /*
  Serial.begin(38400);
  Serial.println("Respira sensor ready!");
  */
  
  // Wireless transmission interval
  txInterval = panstamp.txInterval[0];
  txInterval = txInterval << 8 | panstamp.txInterval[1];
  //txInterval = 10;

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter Rx ON state
  panstamp.enterSystemState(SYSTATE_RXON);

  // Transmit periodic Tx interval
  getRegister(REGI_TXINTERVAL)->getData();
  // Transmit power voltage
  getRegister(REGI_VOLTSUPPLY)->getData();
  
  // Initialize sensors
  initSensor();

}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  digitalWrite(LEDPIN, HIGH);

  // Transmit Humidity + temperaturesensor data
  getRegister(REGI_HTSENSOR)->getData();

  delay(100);

  // Transmit CO + NO2 sensor level
  getRegister(REGI_AIRSENSOR)->getData();

  digitalWrite(LEDPIN, LOW);
  
  for (loopCnt=0 ; loopCnt < txInterval ; loopCnt++)
    delay(1000);
}

