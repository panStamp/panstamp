/**
 * regtable
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
static byte dtVoltSupply[2];
REGISTER regVoltSupply(dtVoltSupply, sizeof(dtVoltSupply), &updtVoltSupply, NULL);
// Co2 Sensor value 
static byte dtCo2Sensor[2];
REGISTER regCo2Sensor(dtCo2Sensor, sizeof(dtCo2Sensor), &updtCo2Sensor, NULL);
// Htu Sensor value 
static byte dtHtuSensor[4];
REGISTER regHtuSensor(dtHtuSensor, sizeof(dtHtuSensor), &updtHtuSensor, NULL);



/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  &regVoltSupply,
  &regCo2Sensor,
  &regHtuSensor
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
  unsigned long result = panstamp.getVcc();
  
  // Update register value
  regTable[rId]->value[0] = (result >> 8) & 0xFF;
  regTable[rId]->value[1] = result & 0xFF;
}

/**
 * updtCo2Sensor
 *
 * Update CO2 register
 *
 * 'rId'  Register ID
 */
const void updtCo2Sensor(byte rId)
{      
  // Update register value
  dtCo2Sensor[0] = (co2Level >> 8) & 0xFF;
  dtCo2Sensor[1] = co2Level & 0xFF;
}

/**
 * updtHtuSensor
 *
 * Measure T+H sensor data and update register
 *
 * 'rId'  Register ID
 */
const void updtHtuSensor(byte rId)
{
  // Read humidity
  float h = htu.readHumidity();
  // Read temperature
  float t = htu.readTemperature();

  uint16_t humidity = h * 10;
  uint16_t temperature = (t + 50) * 10;

  dtHtuSensor[0] = (temperature >> 8) & 0xFF;
  dtHtuSensor[1] = temperature & 0xFF;
  dtHtuSensor[2] = (humidity >> 8) & 0xFF;
  dtHtuSensor[3] = humidity & 0xFF;
}

