/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 07/03/2011
 */

#ifndef _NVOLAT_H
#define _NVOLAT_H

//#include "Arduino.h"
#include <EEPROM.h>
#include "config.h"

/**
 * EEPROM addresses
 */
#define EEPROM_FREQ_CHANNEL       0x00   // 1-byte register
#define EEPROM_NOT_USED           0x01   // 1-byte register
#define EEPROM_SYNC_WORD          0x02   // 2-byte register
#define EEPROM_DEVICE_ADDR        0x04   // 1 or 2 byte register
#define EEPROM_TX_INTERVAL        0x06   // 2-byte register
#define EEPROM_FIRST_CUSTOM       0x20

#ifdef SWAP_EXTENDED_ADDRESS
#define EEPROM_DEVICE_ADDRESS     EEPROM_DEVICE_ADDR
#else
#define EEPROM_DEVICE_ADDRESS     EEPROM_DEVICE_ADDR + 1
#endif
#endif

