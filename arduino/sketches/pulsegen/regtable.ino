/**
 * regtable
 *
 * Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
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
 * Creation date: 07/23/2012
 */

#include <EEPROM.h>
#include "product.h"
#include "panstamp.h"
#include "regtable.h"

/**
 * Declaration of common callback functions
 */
DECLARE_COMMON_CALLBACKS()

/**
 * Definition of common registers
 */
DEFINE_COMMON_REGISTERS()

/*
 * Definition of custom registers
 */
// Frequency 0 register
REGISTER regFrequency0(&outFrequency[0], sizeof(outFrequency[0]), NULL, &setFrequency);
// Frequency 1 register
REGISTER regFrequency1(&outFrequency[1], sizeof(outFrequency[1]), NULL, &setFrequency);
// Frequency 0 register
REGISTER regFrequency2(&outFrequency[2], sizeof(outFrequency[2]), NULL, &setFrequency);
// Frequency 0 register
REGISTER regFrequency3(&outFrequency[3], sizeof(outFrequency[3]), NULL, &setFrequency);

/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  &regFrequency0,
  &regFrequency1,
  &regFrequency2,
  &regFrequency3
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */
 
/**
 * setFrequency
 *
 * Set output frequency
 *
 * 'rId'     Register ID
 * 'freq'    New frequency
 */
const void setFrequency(byte rId, byte *freq)
{
  byte index = rId - 11;
  float msec = (float)(TIMER1_TICK_PERIOD_US) / 1000;
  float tmp;
  
  // Update register
  memcpy(regTable[rId]->value, freq, regTable[rId]->length);
  
  // Calculate amount of ticks
  tmp = 1000.0 / (freq[0] * msec);
  maxTicks[index] = tmp;
  highTicks[index] = maxTicks[index] / 2;
}

