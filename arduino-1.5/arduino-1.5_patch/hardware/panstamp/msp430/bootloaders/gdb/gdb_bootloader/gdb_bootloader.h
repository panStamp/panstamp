/*
 * gdb_bootloader.h - 
 *
 *  Created: Apr 11, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: XX-XX-2013
 *  Version: 1.0.0
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

#ifndef GDB_BOOTLOADER_H_
#define GDB_BOOTLOADER_H_

/*
 * provide union access to 4 unsigned bytes,
 * that can be also be accessed as 4 char bytes
 * data[3] will always be '\0 to terminate data[0..1]
 */
union hex_work_t {
  struct {
    uint8_t c_str[4];
  };
  // -- or as 4 8 bit usigned values --
  struct {
    uint8_t data[4]; // [0] low byte
                     // [1] high byte lil-endian order
                     // [2] always 0
                     // [3] checksum converted to uint8_t
                     //     so 'F','A' becomes a uint8_t value of 0xfa
  };
};

void gdb_bootloader();
unsigned hexstr2u16(uint8_t *s);
char u16tohexc(const unsigned n);
ALWAYS_INLINE uint16_t compute_checksum(uint8_t * data, unsigned count);

#endif /* GDB_BOOTLOADER_H_ */
