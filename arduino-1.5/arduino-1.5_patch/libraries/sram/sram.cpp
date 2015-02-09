/*
 * binouts
 *
 * Copyright (c) 2015 panStamp <contact@panstamp.com>
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
 * Creation date: 01/30/2015
 */

#include "sram.h"

/**
 * setMode
 *
 * Set operation mode
 *
 * @param mode Operation mode
 */
void SRAM::setMode(char mode)
{
  if (mode != operationMode)
  {
    chipSelect(true);
    spi.transfer(WRSR);
    spi.transfer(mode);
    chipSelect(false);
    operationMode = mode;
  }
}

/**
 * readByte
 *
 * Read byte from SRAM
 *
 * @param address Memory address
 *
 * @return Byte read
 */
unsigned char SRAM::readByte(unsigned int address)
{
  unsigned char res;

  // Set byte mode
  setMode(BYTE_MODE);
  
  // Write address, read data
  chipSelect(true);
  spi.transfer(READ);
  spi.transfer((unsigned char)(address >> 8));
  spi.transfer((unsigned char)address);
  res = spi.transfer(0xFF);
  chipSelect(false);
  
  return res;
}

/**
 * writeByte
 *
 * Write byte to SRAM
 *
 * @param address Memory address
 * @param data Data byte to be written
 */
void SRAM::writeByte(unsigned int address, char data)
{
  // Set byte mode
  setMode(BYTE_MODE);
  
  // Write address, read data
  chipSelect(true);
  spi.transfer(WRITE);
  spi.transfer((unsigned char)(address >> 8));
  spi.transfer((unsigned char)address);
  spi.transfer(data);
  chipSelect(false);
}

/**
 * readDmData
 *
 * Read DM information from memory in a FIFO way
 *
 * @param dm Pointer to dm object receiving the information
 *
 * @return False if FIFO is empty
 */
/*
bool SRAM::readDmData(DM *dm)
{
  unsigned char i;
  unsigned int address = START_ADDR_DM_INFO;

  // Set byte mode
  setMode(STREAM_MODE);
  
  // Write address, read data
  chipSelect(true);
  spi.transfer(READ);
  spi.transfer((unsigned char)(address >> 8));
  spi.transfer((unsigned char)address);

  dm->address = spi.transfer(0xFF);
  dm->address <<= 8;
  dm->address |= spi.transfer(0xFF);  
  dm->fillLevel = spi.transfer(0xFF);
  dm->fillLevel <<= 8;
  dm->fillLevel |= spi.transfer(0xFF);
  dm->temperature = spi.transfer(0xFF);
  dm->temperature <<= 8;
  dm->temperature |= spi.transfer(0xFF);
  dm->battVoltage = spi.transfer(0xFF);
  dm->battVoltage <<= 8;
  dm->battVoltage |= spi.transfer(0xFF);
  dm->sunExposition = spi.transfer(0xFF);
  dm->sunExposition <<= 8;
  dm->sunExposition |= spi.transfer(0xFF);

  chipSelect(false);

  return true;
}
*/

/**
 * saveDmData
 *
 * Write DM information
 *
 * @param dm Pointer to DM object
 *
 * @return true if the infromation was saved successfully. Return false otherwise
 */
/*
bool SRAM::saveDmData(DM *dm)
{
  unsigned char ucByte, i;
  unsigned int address = START_ADDR_DM_INFO + dmCount * DATA_LENGTH_DM;

  // Don't exceed the capacity of the external SRAM
  if (dmCount > DATA_MAX_QUEUE)
    return false;

  // Set byte mode
  setMode(STREAM_MODE);
  
  chipSelect(true);

  spi.transfer(WRITE);
  spi.transfer((unsigned char)(address >> 8));
  spi.transfer((unsigned char)address);
  
  // Write DM data
  ucByte = (dm->address >> 8) & 0xFF;
  spi.transfer(ucByte);
  ucByte = dm->address & 0xFF;
  spi.transfer(ucByte);
  ucByte = (dm->fillLevel >> 8) & 0xFF;
  spi.transfer(ucByte);
  ucByte = dm->fillLevel & 0xFF;
  spi.transfer(ucByte);
  ucByte = (dm->temperature >> 8) & 0xFF;
  spi.transfer(ucByte);
  ucByte = dm->temperature & 0xFF;
  spi.transfer(ucByte);
  ucByte = (dm->battVoltage >> 8) & 0xFF;
  spi.transfer(ucByte);
  ucByte = dm->battVoltage & 0xFF;
  spi.transfer(ucByte);
  ucByte = (dm->sunExposition >> 8) & 0xFF;
  spi.transfer(ucByte);
  ucByte = dm->sunExposition & 0xFF;
  spi.transfer(ucByte);

  chipSelect(false);

  dmCount++;

  return true;
}
*/
/**
 * removeFirstItem
 *
 * Remove first item from FIFO and arrange the entire queue
 */
/*
void SRAM::removeFirstItem(void)
{
  unsigned int i, j;
  unsigned char buffer[DATA_LENGTH_DM];

  if (dmCount == 0)
    return;

  unsigned int address;

  // Set byte mode
  setMode(STREAM_MODE);
  
  for(i=0 ; i<dmCount ; i++)
  {
    address = START_ADDR_DM_INFO + (i+1) * DATA_LENGTH_DM;
    chipSelect(true);
    // Write address, read data
    spi.transfer(READ);
    spi.transfer((unsigned char)(address >> 8));
    spi.transfer((unsigned char)address);
  
    for(j=0 ; j<DATA_LENGTH_DM ; j++)
      buffer[j] = spi.transfer(0xFF);
      
    chipSelect(false);
    delay(1);
    address = START_ADDR_DM_INFO + i * DATA_LENGTH_DM;
    chipSelect(true);    
      // Write address, write data  
    spi.transfer(WRITE);
    spi.transfer((unsigned char)(address >> 8));
    spi.transfer((unsigned char)address);
    
    for(j=0 ; j<DATA_LENGTH_DM ; j++)
       spi.transfer(buffer[j]);
      
    chipSelect(true);
    delay(1);
  }
  
  dmCount--;
}
*/
