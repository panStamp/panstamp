/**
 * regtable
 *
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
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
 * Creation date: 01/26/2012
 */

#include "product.h"
#include "regtable.h"

/**
 * Definition of common registers
 */
DEFINE_COMMON_REGISTERS()

/*
 * Definition of custom registers
 */
// Repater config. Not being used by the application. Only kept for backward compatibility
byte dtRepeaterCfg[1];       // Repeater config
REGISTER regRepeaterCfg(dtRepeaterCfg, sizeof(dtRepeaterCfg), NULL, NULL);
// Binary output registers
byte dtBinOutput0[1];       // Binary output state 0
REGISTER regBinOutput0(dtBinOutput0, sizeof(dtBinOutput0), NULL, &setBinOutput);
byte dtBinOutput1[1];       // Binary output state 1
REGISTER regBinOutput1(dtBinOutput1, sizeof(dtBinOutput1), NULL, &setBinOutput);
byte dtBinOutput2[1];       // Binary output state 2
REGISTER regBinOutput2(dtBinOutput2, sizeof(dtBinOutput2), NULL, &setBinOutput);
byte dtBinOutput3[1];       // Binary output state 3
REGISTER regBinOutput3(dtBinOutput3, sizeof(dtBinOutput3), NULL, &setBinOutput);
byte dtBinOutput4[1];       // Binary output state 4
REGISTER regBinOutput4(dtBinOutput4, sizeof(dtBinOutput4), NULL, &setBinOutput);
byte dtBinOutput5[1];       // Binary output state 5
REGISTER regBinOutput5(dtBinOutput5, sizeof(dtBinOutput5), NULL, &setBinOutput);
byte dtBinOutput6[1];       // Binary output state 6
REGISTER regBinOutput6(dtBinOutput6, sizeof(dtBinOutput6), NULL, &setBinOutput);
byte dtBinOutput7[1];       // Binary output state 7
REGISTER regBinOutput7(dtBinOutput7, sizeof(dtBinOutput7), NULL, &setBinOutput);
// PWM output registers
byte dtPwmOutput0[1];       // PWM output 0
REGISTER regPwmOutput0(dtPwmOutput0, sizeof(dtPwmOutput0), NULL, &setPwmOutput);
byte dtPwmOutput1[1];       // PWM output 1
REGISTER regPwmOutput1(dtPwmOutput1, sizeof(dtPwmOutput1), NULL, &setPwmOutput);
byte dtPwmOutput2[1];       // PWM output 2
REGISTER regPwmOutput2(dtPwmOutput2, sizeof(dtPwmOutput2), NULL, &setPwmOutput);
byte dtPwmOutput3[1];       // PWM output 3
REGISTER regPwmOutput3(dtPwmOutput3, sizeof(dtPwmOutput3), NULL, &setPwmOutput);

/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  &regRepeaterCfg, // Not used
  &regBinOutput0,
  &regBinOutput1,
  &regBinOutput2,
  &regBinOutput3,
  &regBinOutput4,
  &regBinOutput5,
  &regBinOutput6,
  &regBinOutput7,
  &regPwmOutput0,
  &regPwmOutput1,
  &regPwmOutput2,
  &regPwmOutput3
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */

/**
 * setBinOutput
 *
 * Set binary output
 *
 * 'rId'     Register ID
 * 'state'   New output level
 */
const void setBinOutput(byte rId, byte *state)
{
  byte output = rId - REGI_BINOUTPUT0;

  // Update register
  regTable[rId]->value[0] = state[0];

  // Control pin
  digitalWrite(binaryPin[output], state[0]);
}

/**
 * setPwmOutput
 *
 * Set PWM level
 *
 * 'rId'     Register ID
 * 'level'   New PWM level
 */
const void setPwmOutput(byte rId, byte *level)
{
  byte output = rId - REGI_PWMOUTPUT0;
  
  // Update register
  regTable[rId]->value[0] = level[0];

  // Control PWM output
  analogWrite(pwmPin[output], level[0]);
}

