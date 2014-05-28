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
 * Creation date: 04/24/2011
 */

#ifndef _REGISTER_H
#define _REGISTER_H

#include "Arduino.h"
#include "datatypes.h"

extern byte regIndex;

/**
 * Class: REGISTER
 * 
 * Description:
 * Register class
 */
class REGISTER
{
  private:
    /**
     * Pointer to the register "updater" function
     *
     *  'rId'  Register ID     
     */
    const void (*updateValue)(byte rId);

    /**
     * Pointer to the register "setter" function
     *
     *  'rId'  Register ID     
     *  'v'    New register value
     */
    const void (*setValue)(byte rId, byte *v);

  public:
    /**
     * Register id
     */
    const byte id;
    
    /**
     * Register value
     */
    byte *value;
    
    /**
     * Data length
     */
    const byte length;

    /**
     * Data type
     */
    const SWDTYPE type;

    /**
     * Adddress in EEPROM. Set to -1 if no storage in EEPROM has to be done
     */
    const int eepromAddress;

    /**
     * REGISTER
     * 
     * Constructor
     * 
     * 'val'	      Pointer to the register value
     * 'len'	      Length of the register value
     * 'getValH'    Pointer to the getter function
     * 'setValH'    Pointer to the setter function
     * 'typ'        Type of SWAP data (SWDTYPE)
     * 'eepromAddr' address in EEPROM. Set to -1 if the register value has not to
     * be saved in EEPROM
     */
    REGISTER(byte *val, const byte len, const void (*updateValH)(byte rId), const void (*setValH)(byte rId, byte *v), const SWDTYPE typ=SWDTYPE_OTHER, const int eepromAddr=-1):id(regIndex++), value(val), length(len), updateValue(updateValH), setValue(setValH), type(typ), eepromAddress(eepromAddr) {};

    /**
     * init
     *
     * Initialize register
     */
    void init(void);

    /**
     * getData
     * 
     * Update and get register value
     * 
     */
    void getData();

    /**
     * setData
     * 
     * Set register value
     * 
     * 'data'	New register value
     */
    void setData(byte *data);

    /**
     * sendSwapStatus
     * 
     * Send SWAP status message
     */
    void sendSwapStatus(void);

    /**
     * setValueFromBeBuffer
     *
     * Set curent value from a Big Endian buffer passed as argument
     *
     * @param beBuffer Big Endian buffer
     */
    void setValueFromBeBuffer(unsigned char* beBuffer);

    /**
     * setRegValue
     *
     * Set register value from different data formats
     * Use this method to simplify LE to BE conversion
     *
     * @param val New register value
     * @param size length of the value
     * @param offset starting point for the new partial value
     */
    template<class T> void setRegValue(T val, unsigned char size=0 , unsigned char offset=0)
    {
      int i, len;

      size > 0 ? len = size : len = length;

      for(i=len+offset-1 ; i<=offset ; i--)
      {
        value[i] = val & 0xFF;
        val >>= 8;
      }
    }
};

/**
 * Array of registers
 */
extern REGISTER* regTable[];

/**
 * Extern global functions
 */
extern void setupRegisters(void);

#endif

