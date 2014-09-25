/*
 * memconfig.h - 
 *
 *  Created: Apr 11, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: 04-11-2013
 *  Version: 1.0.1
 *
 *  09-18-2013 rkimball - added configuration for cc4305137
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MEMCONFIG_H_
#define MEMCONFIG_H_

#include "datatypes.h"

/*
 * These are chip specific values that indicate the start
 * of user flash and the start of our gdb_bootloader code
 *
 * Look at the linker map, you will have to adjust based on the
 * size of this code. These values are for a cc430f5137
 *
 * Make sure the values here match the ones in gdb_memory.x linker file
 *
 */
 
/**
 * RAM memory
 */
const uint16_t RAM_END_ADDRESS         = 0x2BFF;
/**
 * INFO MEMORY
 */
const uint16_t INFOMEM_SECTIO_A        = 0x1980;
const uint16_t INFOMEM_SECTIO_B        = 0x1900;
const uint16_t INFOMEM_SECTIO_C        = 0x1880;
const uint16_t INFOMEM_SECTIO_D        = 0x1800;

#define INFOMEM_CONFIG  INFOMEM_SECTIO_D

/**
 * Stored config
 */
const uint16_t INFOMEM_FREQ_CHANNEL    = INFOMEM_CONFIG + 0x00;   // 1-byte register
const uint16_t INFOMEM_NOT_USED        = INFOMEM_CONFIG + 0x01;   // 1-byte register
const uint16_t INFOMEM_SYNC_WORD       = INFOMEM_CONFIG + 0x02;   // 2-byte register
const uint16_t INFOMEM_DEVICE_ADDR     = INFOMEM_CONFIG + 0x04;   // 1 or 2 byte register
const uint16_t INFOMEM_TX_INTERVAL     = INFOMEM_CONFIG + 0x06;   // 2-byte register
const uint16_t INFOMEM_FIRST_CUSTOM    = INFOMEM_CONFIG + 0x20;

#ifdef SWAP_EXTENDED_ADDRESS
const uint16_t INFOMEM_DEVICE_ADDRESS  = INFOMEM_DEVICE_ADDR;
#else
const uint16_t INFOMEM_DEVICE_ADDRESS  = INFOMEM_DEVICE_ADDR + 1;
#endif

/**
 * Serial bootloader in BSL flash
 */
const uint16_t GDB_LOADER_ADDR = 0x1000;          // flash start address for the start of boot loader
const uint16_t GDB_LOADER_LEN  = 0x600;
const uint16_t GDB_BOOT_END    = GDB_LOADER_ADDR + GDB_LOADER_LEN - 1;

/**
 * Standard flash
 */
const uint16_t USER_ROMADDR     = 0x9000;          // flash start address for user code
const uint16_t USER_ROM_LEN     = 0x6800;
const uint16_t USER_END_ROMADDR = USER_ROMADDR + USER_ROM_LEN - 1;

const uint16_t VECTOR_TABLE_SEGMENT = 0xFE00;     // flash segment address containing isr vectors
const uint16_t VECTOR_TABLE_ADDR = 0xFF80;        // Starting address containing isr vectors
const uint16_t USER_RESET_VECTOR = 0xFFBC;        // flash location that stores user's reset vector
const uint16_t GDB_BOOT_RESET_VECTOR = 0xFFFE;    // msp430's reset vector

#endif /* MEMCONFIG_H_ */
