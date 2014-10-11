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
 * Creation date: 11/05/2014
 */

#ifndef _THERMISTOR_H
#define _THERMISTOR_H

#include "Arduino.h"

#define DEFAULT_NUM_SAMPLES  10
#define DEFAULT_BCOEF        3950
#define DEFAULT_NOMINAL_RES 10000
#define DEFAULT_NOMINAL_RES 10000

class THERMISTOR
{
  public:
    /**
     * Nominal resistance
     */
    uint16_t nominalResistance;

    /**
     * Serial resistance
     */
    uint16_t serialResistance;

    /**
     * Analog pin
     */
    uint16_t analogPin;

    /**
     * Beta coefficient of the thermistor
     */
    uint16_t bCoefficient;

    /**
     * THERMISTOR
     * 
     * Class constructor
     *
     * @param adcPin Analog pin where the thermistor is connected
     * @param nomRes Nominal resistance at 25 degrees Celsius
     * @param bCoef beta coefficient of the thermistor
     * @param serialRes Value of the serial resistor
     */
    THERMISTOR(uint8_t adcPin, uint16_t nomRes, uint16_t bCoef, uint16_t serialRes);

    /**
     * read
     *
     * Read temperature from thermistor
     *
     * @return temperature in 0.01 ºC
     */
    int read(void);
};

#endif

