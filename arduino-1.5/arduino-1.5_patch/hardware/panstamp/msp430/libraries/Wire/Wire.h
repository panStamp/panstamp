/*
  ************************************************************************
  *	Wire.h
  *
  *	Arduino core files for MSP430
  *		Copyright (c) 2012 Robert Wessels. All right reserved.
  *
  *
  ***********************************************************************
  Derived from:
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

#ifndef TwoWire_h
#define TwoWire_h

#include "Stream.h"
#include "cc430i2c.h"

#define BUFFER_LENGTH 32

class TwoWire : public Stream
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);
    
    CC430I2C i2cPort;
    
  public:
    /**
     * begin
     * 
     * Initialize I2C master port
     */
    void begin(void);

    /**
     * begin
     * 
     * Initialize I2C master port
     * 
     * @param address Slave address to request data from
     */    
    void begin(uint8_t);
    void begin(int);
    
    /**
     * beginTransmission
     * 
     * Start I2C transaction
     *
     * @param slaAddr I2C slave address
     */   
    void beginTransmission(uint8_t);
    void beginTransmission(int);

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
    uint8_t endTransmission(uint8_t sendStop=true);
   
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
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop=true);    
    uint8_t requestFrom(int, int, int);
    
    /**
     * write
     * 
     * Send single byte to I2C slave
     *
     * @param data byte to be transmitted
     *
     * @return Amount of bytes transmitted
     */
    virtual size_t write(uint8_t);
    
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
    virtual size_t write(const uint8_t *, size_t);
    
    /**
     * available
     * 
     * Number of bytes available in the Rx buffer
     * 
     * @return number of bytes
     */
    virtual int available(void);
    
    /**
     * read
     * 
     * Read 1 byte from Rx buffer
     *
     * @return Amount of bytes received
     */
    virtual int read(void);

    /**
     * peek
     * 
     * Return byte from Rx buffer at the current position
     * 
     * @return byte
     */
    virtual int peek(void);
    
    /**
     * flush
     * 
     * Empty Rx buffer
     */
    virtual void flush(void);
    
  
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

extern TwoWire Wire;

#endif

