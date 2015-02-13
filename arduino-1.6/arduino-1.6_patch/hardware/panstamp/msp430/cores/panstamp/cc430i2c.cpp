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
 * Creation date: 08/05/2013
 */

#include "cc430i2c.h"
#include "cc430f5137.h"

/**
 * begin
 * 
 * Initialize I2C port
 */
void CC430I2C::begin(void) 
{
  /**
   * Pin mapping
   */
  PMAPPWD = 0x02D52;	                    // Get write-access to port mapping regs
  PMAPCTL |= PMAPRECFG;                   // Leave Pin mapping open
  pinI2Cmap();                            // Map I2C pins
  PMAPPWD = 0;		                        // Lock port mapping registers
   
  pinI2Cconfig();                         // Configure I2C pins
}

/**
 * beginTransmission
 * 
 * Start I2C transaction
 *
 * @param slaAddr I2C slave address
 */
void CC430I2C::beginTransmission(uint16_t slaAddr) 
{
  UCB0CTL1 |= UCSWRST;                    // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;          // Use SMCLK
  UCB0BR0 = 120;                          // fSCL = SMCLK/120 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = slaAddr;                    // Slave Address
  UCB0CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation
  UCB0IFG = 0;                            // Reset interrupt flags
  
  slaveAddress = slaAddr;
}

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
uint16_t CC430I2C::write(const uint8_t *buf, uint16_t len, bool stop) 
{
  uint16_t i, res = len;
  uint32_t timeout;

  UCB0I2CSA = slaveAddress;
  UCB0IFG = 0;                         // Reset interrupt flags
  UCB0CTL1 |= UCTR + UCTXSTT;          // Send start condition and slave address
  
  while (!(UCB0IFG & UCTXIFG));       // Wait until start condition is sent

  // Send bytes
  for(i=0 ; i<len ; i++)
  {
    timeout = I2C_TIMEOUT;
    UCB0TXBUF = buf[i];
    // Wait until each data byte is sent
    while ((!(UCB0IFG & UCTXIFG)) && --timeout);
    if (!timeout)
      res = 0;
  }

  if (stop)
  {
    UCB0CTL1 |= UCTXSTP;               // I2C stop condition
    while ((UCB0CTL1 & UCTXSTP));     // Ensure stop condition got sent
  }

  return res;
}

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
uint16_t CC430I2C::read(uint8_t *buf, uint16_t len, bool stop) 
{
  uint16_t i, res = len;
  uint32_t timeout;

  buf[0] = UCB0RXBUF;                   // Clear UCRXIFG
  UCB0I2CSA = slaveAddress;
  UCB0IFG = 0;                          // Reset interrupt flags
  UCB0CTL1 &= ~UCTR;                    // Receiver mode
  UCB0CTL1 |= UCTXSTT;                  // Send start condition

  while ((UCB0IFG & UCTXSTT));         // Wait until start condition is sent

  // For every byte
  for(i=0 ; i<len ; i++)
  {
    timeout = I2C_TIMEOUT;
    // Poll for UCRXIFG event to occur    
    while ((!(UCB0IFG & UCRXIFG)) && --timeout);
    buf[i] = UCB0RXBUF;                 // Read byte
  }

  if (stop)
  {
    UCB0CTL1 |= UCTXSTP;                // I2C stop condition
    while ((UCB0CTL1 & UCTXSTP));      // Ensure stop condition got sent
  }

  return res;
}
