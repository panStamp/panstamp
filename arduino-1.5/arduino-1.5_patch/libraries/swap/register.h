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

#ifndef _REGISTER_H
#define _REGISTER_H

#include "storage.h"
#include "datatypes.h"

extern uint8_t regIndex;

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
     *  @param rId  Register ID     
     */
    const void (*updateValue)(uint8_t rId);

    /**
     * Pointer to the register "setter" function
     *
     *  @param rId  Register ID     
     *  @param v    New register value
     */
    const void (*setValue)(uint8_t rId, uint8_t *v);

  public:
    /**
     * Register id
     */
    const uint8_t id;
    
    /**
     * Register value
     */
    uint8_t *value;
    
    /**
     * Data length
     */
    const uint8_t length;

    /**
     * Data type
     */
    const SWDTYPE type;

    /**
     * Adddress in EEPROM. Set to -1 if no storage in EEPROM has to be done
     */
    const int eepromAddress;

    /**
     * Bank address in EEPROM or flash
     */
    const uint16_t eepromBank;

    /**
     * REGISTER
     * 
     * Constructor
     * 
     * @param val   Pointer to the register value
     * @param len   Length of the register value
     * @param getValH  Pointer to the getter function
     * @param setValH  Pointer to the setter function
     * @param typ      Type of SWAP data (SWDTYPE)
     * @param eepromAddr address in EEPROM. Set to -1 if the register value has not to
     * be saved in EEPROM
     * @param bank sector in eeprom or flash
     */
    REGISTER(uint8_t *val, const uint8_t len, const void (*updateValH)(uint8_t rId), const void (*setValH)(uint8_t rId, uint8_t *v), const SWDTYPE typ=SWDTYPE_OTHER, const int eepromAddr=-1, const uint16_t bank=DEFAULT_NVOLAT_SECTION): id(regIndex++), value(val), length(len), updateValue(updateValH), setValue(setValH), type(typ), eepromAddress(eepromAddr), eepromBank(bank) {};

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
     * @param data	New register value
     */
    void setData(uint8_t *data);

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
    void setValueFromBeBuffer(uint8_t* beBuffer);

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
    /*
    template<class T> void setRegValue(T val, uint8_t size=0 , uint8_t offset=0)
    {
      int i, len;

      size > 0 ? len = size : len = length;

      for(i=len+offset-1 ; i>=offset ; i--)
      {
        value[i] = val & 0xFF;
        val >>= 8;
      }
    }
    */
};

/**
 * Array of registers
 */
extern REGISTER* regTable[];

#endif

