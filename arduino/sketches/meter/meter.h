/**
 * meter.h
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
 * Creation date: 09/08/2012
 */
 
#include "channel.h"
#include "regtable.h"
#include "panstamp.h"

#ifndef _METER_H
#define _METER_H

/**
 * LED pin
 */
#define LEDPIN                   4

/**
 * global definitions
 */
#define NB_OF_CHANNELS           7
#define NB_OF_COUNTERS           3
#define CONFIG_CHANNEL_SIZE      6
#define CONFIG_INITKWH_SIZE      4
#define CONFIG_PULSEINPL_SIZE    6

/**
 * EEPROM addresses
 */
#define EEPROM_CONFIG_CHANNEL0   EEPROM_FIRST_CUSTOM
#define EEPROM_INITIAL_KWH0      EEPROM_CONFIG_CHANNEL0 + CONFIG_CHANNEL_SIZE * NB_OF_CHANNELS
#define EEPROM_CONFIG_PULSE0     EEPROM_INITIAL_KWH0 + CONFIG_INITKWH_SIZE * NB_OF_CHANNELS

/**
 * Pin definitions
 */
#define PIN_ACVOLTAGE    7    // Arduino analog pin 7

/**
 * Time 1 interval
 */
#define TIMER1_TICK_PERIOD_US    1000000  // Timer1 tick = 1 sec

/**
 * AC channels
 */
CHANNEL *channels[NB_OF_CHANNELS];

/**
 * Interrupt masks
 */
#define PCINTMASK    0xE0  // PD[5:7]

/**
 * PCINT macros
 */
#define pcEnableInterrupt()     PCICR = 0x04    // Enable Pin Change interrupt on Port D
#define pcDisableInterrupt()    PCICR = 0x00    // Disable Pin Change interrupts

#endif
