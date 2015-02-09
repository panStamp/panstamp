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

#include "cc430spi.h"
#include "cc430f5137.h"

/**
 * begin
 * 
 * Initialize SPI port
 */
void CC430SPI::begin(void) 
{
  /**
   * Pin mapping
   */
  PMAPPWD = 0x02D52;	                    // Get write-access to port mapping regs
  PMAPCTL |= PMAPRECFG;                   // Leave Pin mapping open
  pinSPImap();                            // Map SPI pins
  PMAPPWD = 0;		                        // Lock port mapping registers
  
  pinSPIconfig();                         // Configure SPI pins

  UCB0CTL1 |= UCSWRST;                    // **Put state machine in reset**
  UCB0CTL0 = UCMST+UCSYNC+UCMSB+UCMODE_0; // 3-pin, 8-bit SPI master
                                          // Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2;                   // SMCLK
   
  UCB0BR0 = 0x02;                         // SMCLK/2
  UCB0BR1 = 0;
  UCB0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
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
uint16_t CC430SPI::transfer(uint8_t *data, uint16_t len)
{
  uint16_t i;
  
  for(i=0 ; i<len ; i++)
    transfer(data[i]);
    
  return i;
}
