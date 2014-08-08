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
 * Port state structure
 */
struct PORTCFG
{
    // Function selection
    uint8_t selection;

    // Port mapping
    uint16_t mapping[8];

    // Port direction
    uint8_t direction;
};

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
    PORTCFG portConfig[3];

    /**
     * getUID
     * 
     * Read Die Record from Device Descriptor memory and build UID
     */
    void getUID(void);

  public:
    /**
     * Unique ID based on the Die Record
     */
    uint8_t uid[8];

    /**
     * init
     * 
     * Initializa MCU core
     */
    void init(void);

    /**
     * setLowPowerMode
     *
     * Enter low power mode
     *
     * @param lpm4 true if LPM4 has to be entered. Otherwise the MCU will
     * enter LPM3
     */
    void setLowPowerMode(bool lpm4 = false);

    /*
     * setNormalMode
     *
     * Revert from low power mode
     */
    void setNormalMode(void);

    /**
     * getVcc
     *
     * Read voltage supply
     *
     * @return voltage in mV
     */
    uint16_t getVcc(void);

    /**
     * getTemp
     *
     * Read internal temperature from CC430 MCU
     *
     * @return voltage in 0.1 ºC
     */
    int getTemp(void);

    /**
     * delayClockCycles
     *
     * Clock cycle delay
     *
     * @param n clock cycles to wait
     */
    void __inline__ delayClockCycles(register unsigned int n);
};

#endif

