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
#include "sensor.h"

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
// Humidity + temperature sensor register
REGISTER regHtSensor(dtHtSensor, sizeof(dtHtSensor), &updtHtSensor, NULL);
// CO + NO2 sensor register
REGISTER regAirSensor(dtAirSensor, sizeof(dtAirSensor), &updtAirSensor, NULL);

/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  &regVoltSupply,
  &regHtSensor,
  &regAirSensor
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
 * updtHtSensor
 *
 * Read humidity + temperature sensor and update register
 *
 * 'rId'  Register ID
 */
const void updtHtSensor(byte rId)
{
  // Read temperature and humidity from sensor
  sensor_ReadTempHum();
}

/**
 * updtAirSensor
 *
 * Read Co + NO2 sensor and update register
 *
 * 'rId'  Register ID
 */
const void updtAirSensor(byte rId)
{
  // Read temperature and humidity from sensor
  sensor_ReadMics4514();
}

