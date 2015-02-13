/*
 * flashmgr.h - simple class to write to flash memory
 *
 *  Created: Apr 7, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: 04-07-2013
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

#ifndef FLASHMGR_H_
#define FLASHMGR_H_

#include <fabooh.h>
#include <cc430f5137.h>

// make sure flash clock is within parameters
#define FLASHCLOCK (FSSEL1 | ((F_CPU/400000L) & 0x3f))

struct FlashMgr {

	static void wait_ready() {
  	while(FCTL3 & BUSY) { ;	}
	}

  static void enable_write(void) {
  	wait_ready();
    FCTL3 = FWKEY;              // Clear LOCK
    FCTL1 = FWKEY | WRT;        // Enable write
  }

  static void disable_write() {
  	wait_ready();
    FCTL1 = FWKEY;              // Done. Clear WRT
    FCTL3 = FWKEY | LOCK;       // Set LOCK
  }

 NEVER_INLINE
  static void erase_segment(unsigned char *flash) {
	 wait_ready();
    FCTL3 = FWKEY;              // Clear LOCK
    FCTL1 = FWKEY | ERASE;      // Enable segment erase
    *flash = 0;                 // Dummy write, erase Segment
  	wait_ready();
    FCTL3 = FWKEY | LOCK;       // Done, set LOCK
  }

  //NEVER_INLINE
  static void write_u16(unsigned * flash_dest, const unsigned value) {
  	wait_ready();
    FCTL3 = FWKEY;              // Clear LOCK
    FCTL1 = FWKEY | WRT;        // Enable write
    *flash_dest = value;
  	wait_ready();
    FCTL1 = FWKEY;              // Done. Clear WRT
    FCTL3 = FWKEY | LOCK;       // Set LOCK
  }

};

#endif /* FLASHMGR_H_ */
