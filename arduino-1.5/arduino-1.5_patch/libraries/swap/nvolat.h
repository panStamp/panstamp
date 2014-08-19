/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
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
 * Creation date: 07/04/2014
 */

#ifndef _NVOLAT_H
#define _NVOLAT_H

#include "config.h"

/**
 * EEPROM/Infomem addresses
 */
#define NVOLAT_FREQ_CHANNEL       0x00   // 1-byte register
#define NVOLAT_NOT_USED           0x01   // 1-byte register
#define NVOLAT_SYNC_WORD          0x02   // 2-byte register
#define NVOLAT_DEVICE_ADDR        0x04   // 1 or 2 byte register
#define NVOLAT_TX_INTERVAL        0x06   // 2-byte register
#define NVOLAT_SIGNATURE          0x08   // 2-byte register
#define NVOLAT_CARRIER_FREQ       0x0A   // 3-byte register
#define NVOLAT_FIRST_CUSTOM       0x20

// Signature
#define NVOLAT_SIGNATURE_HIGH     0xAB
#define NVOLAT_SIGNATURE_LOW      0xCD

// Extended address
#ifdef SWAP_EXTENDED_ADDRESS
#define NVOLAT_DEVICE_ADDRESS     NVOLAT_DEVICE_ADDR
#else
#define NVOLAT_DEVICE_ADDRESS     NVOLAT_DEVICE_ADDR + 1
#endif

#endif
