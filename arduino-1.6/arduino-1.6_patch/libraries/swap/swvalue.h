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

#ifndef _SWVALUE_H
#define _SWVALUE_H

#include "datatypes.h"

/**
 * Class: SWAPVALUE
 * 
 * Description:
 * SWAP value class
 */
template< typename T > class SWVALUE
{
  private:
    /**
     * Byte position in register
     */
    uint8_t position;

    /**
     * Number of bytes
     */
    uint8_t length;

    /**
     * value in byte-array format
     */
    uint8_t *data;

  public:
    /**
     * Class constructor
     *
     * @param pos byte-position in register
     * @param size byte-length of the value
     */
    SWVALUE(uint8_t pos, uint8_t size)
    {
      position = pos;
      length = size;
      data = new uint8_t[size];
    }

    /**
     * get
     *
     * @param val Value in unsigned integer format
     */
    void get(T val)
    {
      int i;

      for(i=length-1 ; i>=0 ; i--)
      {
        data[i] = val & 0xFF;
        val >>= 8;
      }
    }

// constructor with user pre-defined size
	array (int s) {
	size = s;
	myarray = new T [size];
	}
	
    template<class T> void setRegValue(T val, uint8_t size=0 , uint8_t offset=0)
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

#endif

