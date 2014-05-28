/**
 * rgbled.h
 *
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 *
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 06/22/2011
 */

#ifndef _RGBLED_H
#define _RGBLED_H

#include "Arduino.h"

/**
 * Macros
 */
#define turnOff()   setColor(0, 0, 0)

class RGBLED
{
  private:
    /**
     * Arduino pin connected to the red LED
     */
    int pRed;

    /**
     * Arduino pin connected to the green LED
     */
    int pGreen;

    /**
     * Arduino pin connected to the blue LED
     */
    int pBlue;

  public:
    /**
     * Class constructor
     *
     * 'redPin'     Arduino pin connected to the red LED
     * 'greenPin'   Arduino pin connected to the green LED
     * 'bluePin'    Arduino pin connected to the blue LED
    */
    RGBLED(int redPin, int greenPin, int bluePin);

    /**
     * setColor
     *
     * Set color levels on the RGB LED
     *
     * 'red'      Bright level of the red LED
     * 'green'    Bright level of the green LED
     * 'blue'     Bright level of the blue LED
     */
    void setColor(byte red, byte green, byte blue);
};

#endif
