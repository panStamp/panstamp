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
 * Creation date: 06/27/2013
 */

#ifndef _CC430CORE_H
#define _CC430CORE_H

#include "wiring.h"

/**
 * VCORE + MCLK config macros
 */
#define _SET_VCORE_1MHZ(vCore)  setVcoreMCLK(vCore, DCORSEL_0, 0x01F)
#define _SET_VCORE_8MHZ(vCore)  setVcoreMCLK(vCore, DCORSEL_3, 0x0F4)
#define _SET_VCORE_12MHZ(vCore) setVcoreMCLK(vCore, DCORSEL_5, 0x16E)

/**
 * MCLK config macros with fixed VCORE
 */
#define _SET_MCLK_12MHZ()   _SET_VCORE_12MHZ(2)
#define _SET_MCLK_20MHZ()   setVcoreMCLK(3, DCORSEL_4, 0x262)

/**
 * Class: CC430CORE
 * 
 * Description:
 * CC430 Core class
 */
class CC430CORE
{
  private:
    /**
     * Configuration of ports
     */
    uint8_t portSelection[3];

    /**
     * setVcoreMCLK
     *
     * Config VCORE and MCLK registers
     *
     * @param vCore VCORE level
     * @param dcorsel CPU DCORSEL value
     * @param flln MCLK multiplier bits
     */
    void inline setVcoreMCLK(uint8_t vCore, uint16_t dcorsel, uint16_t flln);

    /**
     * setMCLK
     *
     * Config MCLK registers
     *
     * @param dcorsel CPU DCORSEL value
     * @param flln MCLK multiplier bits
     */
    void inline setMCLK(uint16_t dcorsel, uint16_t flln);

  public:
    /**
     * init
     * 
     * Initialize CC430 core
     * VCORE = 2 and SCLK = 12 MHz when no argument is passed
     * 
     * @param vCore VCORE level
     * @param dcorsel CPU DCORSEL value
     * @param flln MCLK multiplier bits
     */
    void init(uint8_t vCore=2, uint16_t dcorsel=DCORSEL_5, uint16_t flln=0x16E);

    /**
     * setLowPowerMode
     *
     * Enter low power mode
     *
     * @param lpm4 true if LPM4 has to be entered. Otherwise the MCU will
     * enter LPM3
     */
    void setLowPowerMode(bool lpm4 = false);

    /**
     * setNormalMode
     *
     * Revert from low power mode
     */
    void setNormalMode(void);

    /**
     * getUID
     * 
     * Read Die Record from Device Descriptor memory and build UID
     * 
     * @param buffer Pointer to the buffer that will receive the result
     */
    inline void getUID(uint8_t *buffer)
    {
      uint8_t *flashPtr = (uint8_t *) 0x1A0A;
      buffer[0] = flashPtr[3]; // Wafer ID
      buffer[1] = flashPtr[2];
      buffer[2] = flashPtr[1];
      buffer[3] = flashPtr[0];
      buffer[6] = flashPtr[5]; // Die X position
      buffer[7] = flashPtr[4];
      buffer[4] = flashPtr[7]; // Die Y position
      buffer[5] = flashPtr[6];
    }

    /**
     * getShortUID
     * 
     * Read Die Record from Device Descriptor memory and build a short 2-byte id
     * 
     * @return unsigned integer containing a 2-byte uid
     */
    inline uint16_t getShortUID(void)
    {
      uint16_t result;
      
      uint8_t *flashPtr = (uint8_t *) 0x1A0A;     
      result = flashPtr[5];    // Die X position
      result = (result << 8) | flashPtr[4];
      
      return result;
    }

    /**
     * getVcc
     *
     * Read voltage supply
     *
     * @return voltage in mV
     */
    inline uint16_t getVcc(void)
    {
      analogReference(INTERNAL2V0);
      
      uint32_t data = analogRead(A11);
      data *= 4000;
      data /= 4095;

      return (uint16_t) data;
    }

    /**
     * getTemp
     *
     * Read internal temperature from CC430 MCU
     *
     * @return voltage in 0.1 ºC
     */
    inline int getTemp(void)
    {
      analogReference(INTERNAL1V5);
      long data = analogRead(A10);
      data = data * 1.45 - 6.68;

      return (int) data;
    }

    /**
     * delayClockCycles
     *
     * Clock cycle delay
     *
     * @param n clock cycles to wait
     */
    inline void delayClockCycles(register uint16_t n)
    {
        __asm__ __volatile__ (
                    "1: \n"
                    " dec        %[n] \n"
                    " jne        1b \n"
            : [n] "+r"(n));
    }
};

#endif

