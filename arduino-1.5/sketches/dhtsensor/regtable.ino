/**
 * regtable
 *
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
 * Creation date: 03/31/2011
 */

#include "product.h"
#include "regtable.h"

/**
 * Definition of common registers
 */
DEFINE_COMMON_REGISTERS()

/*
 * Definition of custom registers
 */
// Voltage supply
static unsigned long voltageSupply = 3300;
static byte dtVoltSupply[2];
REGISTER regVoltSupply(dtVoltSupply, sizeof(dtVoltSupply), &updtVoltSupply, NULL);
// Sensor value register
static byte dtSensor[4];
REGISTER regSensor(dtSensor, sizeof(dtSensor), &updtSensor, NULL);

/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  &regVoltSupply,
  &regSensor
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */
 
/**
 * updtVoltSupply
 *
 * Measure voltage supply and update register
 *
 * 'rId'  Register ID
 */
const void updtVoltSupply(byte rId)
{  
  unsigned long result;

  #ifdef VOLT_SUPPLY_A0 
  // Read voltage supply from A0
  unsigned short ref = voltageSupply;
  result = analogRead(A0);
  result *= ref;
  result /= 4095;
  #else
  result = panstamp.getVcc();
  #endif

  /**
   * register[eId]->member can be replaced by regVoltSupply in this case since
   * no other register is going to use "updtVoltSupply" as "updater" function
   */

  // Update register value
  regTable[rId]->value[0] = (result >> 8) & 0xFF;
  regTable[rId]->value[1] = result & 0xFF;
}

/**
 * updtSensor
 *
 * Measure sensor data and update register
 *
 * 'rId'  Register ID
 */
const void updtSensor(byte rId)
{
  // Power sensor
  digitalWrite(PWRPIN, HIGH);
  delay(1500);
  // Read humidity
  float h = dht.readHumidity();
  // Read temperature
  float t = dht.readTemperature();
  // Unpower sensor
  digitalWrite(PWRPIN, LOW);

  uint16_t humidity = h * 10;
  uint16_t temperature = (t + 50) * 10;

  dtSensor[0] = (temperature >> 8) & 0xFF;
  dtSensor[1] = temperature & 0xFF;
  dtSensor[2] = (humidity >> 8) & 0xFF;
  dtSensor[3] = humidity & 0xFF;
}
