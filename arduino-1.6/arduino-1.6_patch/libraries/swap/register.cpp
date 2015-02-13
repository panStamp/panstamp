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
 * Creation date: 06/03/2013
 */

#include "register.h"
#include "swstatus.h"

unsigned char regIndex = 0;


/**
 * init
 *
 * Initialize register
 */
void REGISTER::init(void)
{
  // Does the value need to be read from EEPROM?
  if (eepromAddress >= 0)
  {
    STORAGE nvMem;
    
    // Read from info memory
    nvMem.read(value, eepromBank, eepromAddress, length);
  }
}

/**
 * getData
 * 
 * Update and get register value
 */
void REGISTER::getData(void) 
{
  // Update register value
  if (updateValue != NULL)
    updateValue(id);

  // Send SWAP status message about the new value
  sendSwapStatus();
}

/**
 * setData
 * 
 * Set register value
 * 
 * @param data New register value
 */
void REGISTER::setData(unsigned char *data) 
{
  // Update register value
  if (setValue != NULL)
    setValue(id, data);

  // Send SWAP status message
  sendSwapStatus();

  // Does the value need to be saved in info memory (flash)?
  if (eepromAddress >= 0)
  {
    STORAGE nvMem;   
    // Write info memory
    nvMem.write(value, eepromBank, eepromAddress, length);
  }
}

/**
 * sendSwapStatus
 * 
 * Send SWAP status message
 */
void REGISTER::sendSwapStatus(void) 
{
  SWSTATUS packet = SWSTATUS(id, value, length, type);
  packet.send();
}

/**
 * setValueFromBeBuffer
 *
 * Set curent value from a Big Endian buffer passed as argument
 *
 * @param beBuffer Big Endian buffer
 */
void REGISTER::setValueFromBeBuffer(unsigned char* beBuffer)
{
  unsigned char i;

  for(i=0 ; i<length ; i++)
    value[i] = beBuffer[length-1-i];
}

