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
 * Creation date: 06/25/2014
 */

#include "cc430f5137.h"
#include "pins.h"
#include "wiring.h"


/**
 * pinMode
 * 
 * Config pin mode
 * 
 * @param pin pin number
 * @param mode pin mode
 */
void pinMode(uint8_t pin, uint8_t mode) 
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	volatile uint8_t *dir = portDirRegister(port);
	volatile uint8_t *ren = portRenRegister(port);
	volatile uint8_t *out = portOutputRegister(port);
	volatile uint8_t *sel = portSelRegister(port);

  *sel &= ~bit;     // Unselect alternate function
   
  switch(mode)
  {
    case INPUT:
      *dir &= ~bit;
      break;
    case INPUT_PULLUP:
      *dir &= ~bit;
      *out |= bit;
      *ren |= bit;
      break;
    case INPUT_PULLDOWN:
      *dir &= ~bit;
      *out &= ~bit;
      *ren |= bit;
      break;
    case OUTPUT:
      *dir |= bit;
      break;
    default:
      break;
  }
}

/**
 * digitalRead
 *
 * read binary state
 *
 * @param pin pin mumber
 * 
 * @return input state
 */
uint8_t digitalRead(uint8_t pin)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *input = portInputRegister(port);

  return (((*input) & bit) > 0);
}

/**
 * digitalWrite
 *
 * set binary state
 *
 * @param pin pin mumber
 * @param state output state
 */
void digitalWrite(uint8_t pin, uint8_t state)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out = portOutputRegister(port);
   
  if (state)
    *out |= bit;
  else
    *out &= ~bit;
}

