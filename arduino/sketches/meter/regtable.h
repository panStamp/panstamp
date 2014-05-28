/**
 * regtable.h
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
 * Creation date: 01/26/2012
 */

#ifndef _REGTABLE_H
#define _REGTABLE_H

#include "Arduino.h"
#include "register.h"
#include "commonregs.h"

/**
 * Register indexes
 */
DEFINE_REGINDEX_START()
  REGI_CHANNEL_ENERGY_0,  // Index to register containing channel 0 energy data
  REGI_CHANNEL_ENERGY_1,  // Index to register containing channel 1 energy data
  REGI_CHANNEL_ENERGY_2,  // Index to register containing channel 2 energy data
  REGI_CHANNEL_ENERGY_3,  // Index to register containing channel 3 energy data
  REGI_CHANNEL_ENERGY_4,  // Index to register containing channel 4 energy data
  REGI_CHANNEL_ENERGY_5,  // Index to register containing channel 5 energy data
  REGI_CHANNEL_ENERGY_6,  // Index to register containing channel 6 energy data
  REGI_CHANNEL_CONFIG_0,  // Index to register containing channel 0 energy data
  REGI_CHANNEL_CONFIG_1,  // Index to register containing channel 1 energy data
  REGI_CHANNEL_CONFIG_2,  // Index to register containing channel 2 energy data
  REGI_CHANNEL_CONFIG_3,  // Index to register containing channel 3 energy data
  REGI_CHANNEL_CONFIG_4,  // Index to register containing channel 4 energy data
  REGI_CHANNEL_CONFIG_5,  // Index to register containing channel 5 energy data
  REGI_CHANNEL_CONFIG_6,  // Index to register containing channel 6 energy data
  REGI_PULSE_COUNT_0,     // Index to register containing pulse counter 0 data
  REGI_PULSE_COUNT_1,     // Index to register containing pulse counter 1 data
  REGI_PULSE_COUNT_2,     // Index to register containing pulse counter 2 data
  REGI_PULSE_CONFIG_0,    // Index to register containing config settings for pulse input 0
  REGI_PULSE_CONFIG_1,    // Index to register containing config settings for pulse input 0
  REGI_PULSE_CONFIG_2     // Index to register containing config settings for pulse input 0
DEFINE_REGINDEX_END()

#endif

