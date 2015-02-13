/**
 * regtable.pde
 *
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
 * Creation date: 03/31/2011
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
DEFINE_COMMON_REGISTERS();

/*
 * Definition of custom registers
 */
// RGB levels
static byte dtRGBlevel[3];
REGISTER regRGBlevels(dtRGBlevel, sizeof(dtRGBlevel), NULL, &setRGBlevel);

/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
        &regRGBlevels
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */

/**
 * setRGBlevel
 *
 * Set RGB color levels
 *
 * 'rId'     Register ID
 * 'levels'  New RGB levels
 */
const void setRGBlevel(byte rId, byte *levels)
{
  // Update register
  memcpy(dtRGBlevel, levels, sizeof(dtRGBlevel));

  // Control RGB LED
  rgbLed.setColor(levels[0], levels[1], levels[2]);
}

