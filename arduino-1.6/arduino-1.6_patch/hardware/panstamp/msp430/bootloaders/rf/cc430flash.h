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
 * Creation date: 09/19/2014
 */
 
 /**
  * Based on Rick Kimnball's fabooh code
  * https://github.com/RickKimball/msp430_code/tree/master/fabooh
  */

#ifndef _CC430FLASH_H_
#define _CC430FLASH_H_

#include <cc430f5137.h>
#include "datatypes.h"

class CC430FLASH
{
  private:
  
  /**
   * waitReady
   * 
   * Wait until the flash memory is ready for the next operation
   */
  ALWAYS_INLINE
	void waitReady()
  {
  	while(FCTL3 & BUSY);
	}

  public:
  
  /**
   * eraseSegment
   * 
   * Erase flash segment
   * 
   * @param memAddress starting address of the flash segment to be erased
   */
  NEVER_INLINE
  void eraseSegment(uint8_t *memAddress)
  {
	  waitReady();
    FCTL3 = FWKEY;              // Clear LOCK
    FCTL1 = FWKEY | ERASE;      // Enable segment erase
    *memAddress = 0;            // Dummy write, erase Segment
  	waitReady();
    FCTL3 = FWKEY | LOCK;       // Done, set LOCK
  }

  /**
   * read
   * 
   * Red contents from flash memory
   *
   * @param memAddress starting address in flash memory
   * @param buffer array to copy the contents into
   * @param length Length in bytes to be read
   *
   * @return amount of bytes read
   */
  NEVER_INLINE
  uint8_t read(uint8_t *memAddress, uint8_t *buffer, uint8_t length)
  {
    uint8_t i;
    uint8_t * flashPtr = memAddress;
                                          
    for (i = 0; i < length; i++)
      buffer[i] = *flashPtr++;           // Read byte from flash

    return length;
  }

  /**
   * write
   * 
   * Write buffer in info memory
   *
   * @param memAddress destination address in flash memory
   * @param buffer array to be written. This buffer must contain the whole contents of the
                   flash section to be written
   * @param length Length to be written
   *
   * @return amount of bytes copied
   */
  NEVER_INLINE
  uint8_t write(uint8_t *memAddress, uint8_t *buffer, uint8_t length)
  {
    uint16_t i = 0;
                                          
    waitReady();
    FCTL3 = FWKEY;                         // Clear Lock bit
    FCTL1 = FWKEY+WRT;                     // Set WRT bit for byte write operation

    while (i < length)
    {
      *memAddress = buffer[i];             // Write byte in flash
      
      waitReady();                         // Wait for write to complete
      
      if (*memAddress == buffer[i])        // Check flash contents before skipping to the next position
      {
        *memAddress++;
        i++;
      }
    }

    waitReady();
    FCTL1 = FWKEY;                         // Clear WRT bit
    FCTL3 = FWKEY+LOCK;                    // Set LOCK bit

    return length;
  }
};

#endif
