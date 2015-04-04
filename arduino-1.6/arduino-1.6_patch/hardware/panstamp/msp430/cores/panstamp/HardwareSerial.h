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
 * Creation date: 07/02/2014
 */

#ifndef _HARDWARESERIAL_H
#define _HARDWARESERIAL_H

#include <Stream.h>
#include "cc430uart.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class HardwareSerial : public Stream
{ 
  private:
    /**
     * UART object
     */
     CC430UART uart;
     
  public:  
    /**
     * begin
     * 
     * Initialize UART port
     *
     * @param baud Serial baudrate
     */
    void begin(uint32_t baud);

    /**
     * write
     * 
     * Send single byte
     *
     * @param c byte to be sent
     */
    size_t write(uint8_t c);


    /**
     * write
     * 
     * Send data buffer
     *
     * @param buf Buffer to be sent
     * @param len Amount of bytes to be transferred
     *
     * @return Amount of bytes transmitted
     */
    size_t write(uint8_t *buf, unsigned int len);

    /**
     * read
     * 
     * Read string from the Rx FIFO
     *
     * @param buf Buffer to be sent
     *
     * @return amount of bytes read or -1 if the Rx FIFO is empty
     */
    int read(uint8_t *buf);

    /**
     * read
     * 
     * Read single byte from Rx FIFO
     *
     * @return byte read
     */
    int read(void);
    
    /**
     * available
     * 
     * Get he amount of bytes waiting to be read from the
     * UART Rx FIFO
     *
     * @return amount of bytes in the FIFO
     */
    int available(void);

    /**
     * end
     * 
     * Close serial port
     */
    inline void end(void)
    {
      uart.end();  
    }
    
    // Not implemented methods
    int peek(void);
    void flush(void);
};

extern HardwareSerial Serial;

#endif

