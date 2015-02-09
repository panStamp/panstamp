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
 * Creation date: 12/02/2014
 */

#ifndef _CC430SPI_H
#define _CC430SPI_H

#include "wiring.h"


class CC430SPI
{  
  public: 
    /**
     * begin
     * 
     * Initialize SPI port
     */
    void begin(void);

    /**
     * transfer
     * 
     * Send single byte to SPI slave and read response
     *
     * @param data Byte to be sent
     * 
     * @return byte returned by slave
     */
    inline uint8_t transfer(uint8_t data) 
    {
      while (!(UCB0IFG & UCTXIFG));               // USCI_B0 TX buffer ready?
      UCB0TXBUF = data;
        
      while (!(UCB0IFG & UCRXIFG));               // Data received?
      return UCB0RXBUF;
    }
    
    /**
     * transfer
     * 
     * Send data buffer to SPI slave
     *
     * @param data Buffer to be sent
     * @param len Amount of bytes to be transferred
     *
     * @return Amount of bytes transmitted
     */
    uint16_t transfer(uint8_t *data, uint16_t len);
};

#endif

