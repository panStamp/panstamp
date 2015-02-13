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
const unsigned INFOMEM_ADDR    = 0x1800;          // flash start address of info mem
const unsigned INFOMEM_END     = 0x1980-1;

const unsigned GDB_ROMADDR     = 0x8000;          // flash start address for user code
const unsigned GDB_ROM_LEN     = 0x7800;
const unsigned GDB_END_ROMADDR = GDB_ROMADDR + GDB_ROM_LEN - 1;

const unsigned GDB_LOADER_ADDR = 0x1000;          // flash start address for the start of boot loader
const unsigned GDB_LOADER_LEN  = 0x600;
const unsigned GDB_BOOT_END    = GDB_LOADER_ADDR + GDB_LOADER_LEN - 1;

const unsigned VECTOR_TABLE_SEGMENT = 0xFE00;     // flash segment address containing isr vectors
const unsigned USER_RESET_VECTOR = 0xFFBE;        // flash location that stores user's reset vector
const unsigned GDB_BOOT_RESET_VECTOR = 0xFFFE;    // msp430's reset vector

const bool ERASE_INFO_MEM = false;  // enable to erase info mem
                                    // proceed with caution!! info mem contains
                                    // chip calibration data
#endif /* MEMCONFIG_H_ */
