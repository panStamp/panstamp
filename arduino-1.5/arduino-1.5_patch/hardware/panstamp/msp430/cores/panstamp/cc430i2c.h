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
 * Creation date: 08/05/2013
 */

#ifndef _CC430I2C_H
#define _CC430I2C_H

#include "wiring.h"

#define I2C_TIMEOUT     0xFFF

class CC430I2C
{
  public:
    /**
     * Slave address
     */
    uint16_t slaveAddress;
  
    /**
     * begin
     * 
     * Initialize I2C port
     */
    void begin(void);

    /**
     * beginTransmission
     * 
     * Start I2C transaction
     *
     * @param slaAddr I2C slave address
     */
    void beginTransmission(uint16_t slaAddr);

    /**
     * write
     * 
     * Send data to I2C slave
     *
     * @param buf Buffer to be sent
     * @param len Amount of bytes to be transferred
     * @param stop If true, transmit stop condition
     *
     * @return Amount of bytes transmitted
     */
    uint16_t write(const uint8_t *buf, uint16_t len, bool stop=true);

    /**
     * read
     * 
     * Receive data from I2C slave
     *
     * @param buf Buffer received
     * @param len Amount of bytes to be read
     * @param stop If true, transmit stop condition
     *
     * @return Amount of bytes received
     */
    uint16_t read(uint8_t *buf, uint16_t len, bool stop=true);
};

#endif

