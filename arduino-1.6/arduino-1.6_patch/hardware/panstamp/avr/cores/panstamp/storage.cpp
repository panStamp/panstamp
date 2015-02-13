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
 * Creation date: 07/23/2014
 */

#include <avr/eeprom.h>
#include "Arduino.h"
#include "storage.h"

/**
 * read
 * 
 * Read buffer from info memory
 *
 * @param buffer pointer to the buffer where to write the result
 * @param section storage media or bank (0 for internal EEPROM)
 * @param position position in section
 * @pararm length Length to be read
 *
 * @return amount of bytes read
 */
uint8_t STORAGE::read(uint8_t *buffer, uint8_t section, uint8_t position, uint8_t length)
{
  uint16_t i = 0;
  
  if (section == 0)
  {
    cli();
    for (i=0 ; i<length ; i++)
      buffer[i] = eeprom_read_byte((uint8_t *) position+i);
    sei();
  }
  
  return i;
}

/**
 * write
 * 
 * Write buffer in info memory
 *
 * @param buffer array to be written
 * @param section storage media or bank (0 for internal EEPROM)
 * @param position position in section
 * @pararm length Length to be written
 *
 * @return amount of bytes copied
 */
uint8_t STORAGE::write(uint8_t *buffer, uint8_t section, uint8_t position, uint8_t length)
{
  uint16_t i = 0;
  
  if (section == 0)
  {
    cli();
    for (i=0 ; i<length ; i++)
      eeprom_write_byte((uint8_t *) position+i, buffer[i]);
    sei();
  }
  
  return i;
}
