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
 
#ifndef MMA8652_h
#define MMA8652_h

#include "Arduino.h"
#include "mma8652_regs.h"

/**
 * MACROS
 */
// Enable double-pulse detection on each axis
#define enableDoubleTapInt(sens)  enableTapInt(sens, true)
// True if Portrait/Landscape orientation interrupt is enabled
#define _IS_PL_INT_ENABLED()  (ctrlReg4 & SRC_LNDPRT_MASK)
// Read interrupt source register (see MMA8652FC datasheet, p. 28)
#define readIntSource()  read(MMA8652_INT_SOURCE)
// Read portrait/landscape status register (see MMA8652FC datasheet, p. 32)
#define readPlStatus()  read(MMA8652_PL_STATUS)
// Read pulse source register (see MMA8652FC datasheet, p. 45)
#define readPulseSource()  read(MMA8652_PULSE_SRC)
// Read free-fall source register (see MMA8652FC datasheet, p. 36)
#define readFreeFallSource()  read(MMA8652_FF_MT_SRC)
// Read "who am I" register. It should return 0x4A
#define whoAmI()  read(MMA8652_WHO_AM_I)


struct AXIS
{
  int16_t x;
  int16_t y;
  int16_t z;
};

class MMA8652
{
  private:
    /**
     * MCU's digital pin connected to sensor's INT2 pin
     */
    uint8_t intPin;
    
    /**
     * Current MMA8652_CTRL_REG4 value
     */
    uint8_t ctrlReg4;
      
    /**
     * read
     * 
     * Read multiple bytes from register
     * 
     * @param address  Register address
     * @param buffer output buffer
     * @param length amount of bytes to be read
     * 
     * @return amount of bytes read
     */
    uint8_t read(uint8_t address, uint8_t *buffer, uint8_t length);
   
    /**
     * enableInt
     * 
     * Enable interrupt and attach interrupt to output pin
     * 
     * @param source Source of interrupt - mask (SRC_ASLP_MASK, SRC_FIFO_MASK, SRC_TRANS_MASK
     *   SRC_LNDPRT_MASK, SRC_PULSE_MASK, SRC_FF_MT_MASK, SRC_DRDY_MASK)
     */
    void enableInt(uint8_t source);
    
    /**
     * disableInt
     * 
     * Disable interrupt
     * 
      * @param source Source of interrupt - mask (SRC_ASLP_MASK, SRC_FIFO_MASK, SRC_TRANS_MASK
     *   SRC_LNDPRT_MASK, SRC_PULSE_MASK, SRC_FF_MT_MASK, SRC_DRDY_MASK)
     */
    void disableInt(uint8_t source);
    
  public:

    /**
     * Class constructor
     */
    MMA8652(const uint8_t pin): intPin(pin) {};
    
    /**
     * Axis information
     */
    AXIS axis;
       
    /**
     * init
     * 
     * Initialize MMA8652 sensor
     */
    void init(void);

    /**
     * attachInterrupt
     * 
     * Attach custom ISR to MMA8652 event
     * 
     * @param funct custom callback function
     */
    inline void attachInterrupt(void (*funct)(void))
    {
      pinMode(intPin, INPUT);
      ::attachInterrupt(intPin, funct, FALLING);
    }

    /**
     * read
     * 
     * Read single byte from register
     * 
     * @param address  Register address
     */
    uint8_t read(uint8_t address);

    /**
     * write
     * 
     * Write single byte in register
     * 
     * @param address  Register address
     * @param data
     */
    void write(uint8_t address, uint8_t data);
    
    /**
     * reset
     * 
     * Reset device
     */
    void reset(void);

    /**
     * readXYZ
     * 
     * Read XYZ axis. Place result in global variable axis
     * Each count equals to 1/1024 gr
     */
    void readXYZ(void);
       
    /**
     * enableTapInt
     * 
     * Enable tap interrupt and attach interrupt to output pin
     * 
     * @param sensibility 0 is the highest sensibility, 255 is the lowest
     */
    void enableTapInt(uint8_t sensibility, bool doubleTap=false);

    /**
     * disableTapInt
     * 
     * Disable tap interrupt
     */
    void disableTapInt(void);

    /**
     * enableFreeFallInt
     * 
     * Enable free-fall interrupt and attach interrupt to output pin
     * 
     * @param sensibility 0 is the highest sensibility, 255 is the lowest
     */
    void enableFreeFallInt(uint8_t sensibility);

    /**
     * disableFreeFallInt
     * 
     * Disable free-fall interrupt
     */
    void disableFreeFallInt(void);

    /**
     * enablePlInt
     * 
     * Enable portrait-landscape orientation interrupt and attach interrupt
     * to output pin
     */
    void enablePlInt(void);
    
    /**
     * disablePlInt
     * 
     * Disable portrait-landscape orientation interrupt
     */
    void disablePlInt(void);
    
    /**
     * sleep
     * 
     * Enter sleep mode, ready to be brought back to the active mode by an
     * interrupt
     */
    void sleep(void);
    
    /**
     * eventAvailable
     * 
     * Return true if the MMA8652 sensor has generated a new event window
     * 
     * @return true if our application needs to read a new event from the sensor
     */
    inline bool eventAvailable(void)
    {
      return !(::digitalRead(intPin));
    }
    
    /**
     * active
     * 
     * Enter active mode
     */
    inline void active(void)
    {
      // Active=1 to take the part out of standby and enable sampling
      // Data rate = 400 Hz, sleep rate = 12.5 Hz
      write(MMA8652_CTRL_REG1, 0x49);
    }
    
    /**
     * standBy
     * 
     * Enter standby mode.
     */
    inline void standBy(void)
    {
      write(MMA8652_CTRL_REG1, 0);
    }    
};

#endif
