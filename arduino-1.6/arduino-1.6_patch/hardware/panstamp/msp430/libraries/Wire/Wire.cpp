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
 * Creation date: 07/01/2014
 */

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "Wire.h"

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

/**
 * begin
 * 
 * Initialize I2C master port
 */
void TwoWire::begin(void)
{
  i2cPort.begin();
}

/**
 * begin
 * 
 * Initialize I2C master port
 * 
 * @param address Slave address to request data from
 */
void TwoWire::begin(uint8_t address)
{
  i2cPort.begin();
  i2cPort.slaveAddress = address;
}

void TwoWire::begin(int address)
{
  begin((uint8_t)address);
}

/**
 * requestFrom
 * 
 * Used by the master to request bytes from a slave device.
 * The bytes may then be retrieved with the available() and read() functions.
 * 
 * @param address Slave address to request data from
 * @param quantity number of bytes to request
 * @param sendStop true will send a stop message after the request, releasing the bus.
 *                  false will continually send a restart after the request, keeping
 *                  the connection active.
 * 
 * @return number of bytes returned from the slave device
 */
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
  // clamp to buffer length
  if(quantity > BUFFER_LENGTH)
    quantity = BUFFER_LENGTH;

  i2cPort.slaveAddress = address;
  // perform blocking read into buffer
  uint8_t res = (uint8_t) i2cPort.read(rxBuffer, (uint16_t) quantity, (bool) sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = res;

  return res;
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

/**
 * beginTransmission
 * 
 * Start I2C transaction
 *
 * @param slaAddr I2C slave address
 */
void TwoWire::beginTransmission(uint8_t address)
{
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  
  // reset rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = 0;
  
  i2cPort.beginTransmission((uint16_t)address);
}

void TwoWire::beginTransmission(int address)
{
  beginTransmission((uint8_t)address);
}

/**
 * endTransmission
 * 
 * Ends a transmission to a slave device that was begun by beginTransmission() and transmits
 * the bytes that were queued by write()
 * 
 * @param sendStop true will send a stop message after the request, releasing the bus.
 *                  false will continually send a restart after the request, keeping
 *                  the connection active.
 * 
 * @return number of bytes transmitted to the slave device
 */
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
  // transmit buffer (blocking)
  int8_t ret = (uint8_t) i2cPort.write(txBuffer, txBufferLength, (bool)sendStop);

  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
 
  return ret;
}

/**
 * write
 * 
 * Send single byte to I2C slave
 *
 * @param data byte to be transmitted
 *
 * @return Amount of bytes transmitted
 */
size_t TwoWire::write(uint8_t data)
{
  txBuffer[txBufferLength++] = data;
 
  return 1;
}

/**
 * write
 * 
 * Send data to I2C slave
 *
 * @param data Buffer to be sent
 * @param quantity Amount of bytes to be transferred
 *
 * @return Amount of bytes transmitted
 */
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  uint8_t i;
  
  for(i=0 ; i<quantity ; i++)
    txBuffer[txBufferLength++] = data[i];

  return i;
}

/**
 * available
 * 
 * Number of bytes available in the Rx buffer
 * 
 * @return number of bytes
 */
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

/**
 * read
 * 
 * Read 1 byte from Rx buffer
 *
 * @return Amount of bytes received
 */
int TwoWire::read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength)
  {
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

/**
 * peek
 * 
 * Return byte from Rx buffer at the current position
 * 
 * @return byte
 */
int TwoWire::peek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

/**
 * flush
 * 
 * Empty Rx buffer
 */
void TwoWire::flush(void)
{
  rxBufferLength = 0;
  rxBufferIndex = 0;
}

// Pre-instantiate object
TwoWire Wire;
