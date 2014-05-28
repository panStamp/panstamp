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
 * Creation date: 03/03/2011
 */

#ifndef _CCPACKET_H
#define _CCPACKET_H

#include "Arduino.h"

/**
 * Buffer and data lengths
 */
#define CC1101_BUFFER_LEN        64
#define CC1101_DATA_LEN          CC1101_BUFFER_LEN - 3

/**
 * Class: CCPACKET
 * 
 * Description:
 * CC1101 data packet class
 */
class CCPACKET
{
  public:
    /**
     * Data length
     */
    byte length;

    /**
     * Data buffer
     */
    byte data[CC1101_DATA_LEN];

    /**
     * CRC OK flag
     */
    boolean crc_ok;

    /**
     * Received Strength Signal Indication
     */
    byte rssi;

    /**
     * Link Quality Index
     */
    byte lqi;
};

#endif

