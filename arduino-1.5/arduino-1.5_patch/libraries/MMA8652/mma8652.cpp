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

#include "Wire.h"
#include "mma8652.h"

/**
 * init
 * 
 * Initialize MMA8652 sensor
 */
void MMA8652::init(void)
{
  Wire.begin();
  ctrlReg4 = 0;
  
  // Place MMA8652 in standby mode
  standBy();
  delay(1);
   
  // Accelerometer range of +/-2g range with 0.244mg/LSB
  write(MMA8652_XYZ_DATA_CFG, 0);
  delay(1);
  // Enable interrupts - Open-drain output
  // Pulse function interrupt can wake-up system
  // Orientation interrupt can wake-up system
  // Free-fall/motion function interrupt can wake-up system
  write(MMA8652_CTRL_REG3, 0x39);
  delay(1);
  // Enable Pulse detection Interrupt
  // Enable Orientation Interrupt
  // Enable Free-fall/motion Interrupt
  write(MMA8652_CTRL_REG4, ctrlReg4);
  delay(1);
  // All interrupt sources hooked to INT2 pin
  write(MMA8652_CTRL_REG5, 0);
  delay(1);

  // Enter active mode
  active();
}

/**
 * read
 * 
 * Read single byte from register
 * 
 * @param address  Register address
 */
uint8_t MMA8652::read(uint8_t address)
{
  Wire.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  Wire.write(address);
  Wire.endTransmission(false);
  
  Wire.requestFrom((int)MMA8652_CTRL_ID_DEFAULT, (int)1);
  while(!Wire.available());
  
  return Wire.read();
}

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
uint8_t MMA8652::read(uint8_t address, uint8_t *buffer, uint8_t length)
{
  uint8_t i;
  
  Wire.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  Wire.write(address);
  Wire.endTransmission(false);
  
  Wire.requestFrom((int)MMA8652_CTRL_ID_DEFAULT, (int)length);

  for(i=0 ; i<length ; i++)
  {
    while(!Wire.available());
    buffer[i] = Wire.read();
  }
  
  return i;
}

/**
 * write
 * 
 * Write single byte in register
 * 
 * @param address  Register address
 * @param data
 */
void MMA8652::write(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 * reset
 * 
 * Reset device
 */
void MMA8652::reset(void)
{
  standBy();
  delay(1);
  write(MMA8652_CTRL_REG2, RST_MASK);
  delay(1);
  active();
}

/**
 * readXYZ
 * 
 * Read XYZ axis. Place result in global variable axis
 * Each count equals to 1/1024 gr
 */
void MMA8652::readXYZ(void)
{
  uint8_t buf[7];
  uint16_t twoCompl;
  
  read(MMA8652_STATUS_00, buf, 7);

  twoCompl = ((buf[1] << 8) | buf[2]) >> 4;
  if (buf[1] > 0x7F)
    axis.x = (~twoCompl + 1) * (-1);
  else
    axis.x = twoCompl;

  twoCompl = ((buf[3] << 8) | buf[4]) >> 4;
  if (buf[3] > 0x7F)
    axis.y = (~twoCompl + 1) * (-1);
  else
    axis.y = twoCompl;

  twoCompl = ((buf[5] << 8) | buf[6]) >> 4;
  if (buf[5] > 0x7F)
    axis.z = (~twoCompl + 1) * (-1);
  else
    axis.z = twoCompl;
}

/**
 * enableInt
 * 
 * Enable interrupt and attach interrupt to output pin
 * 
 * @param source Source of interrupt - mask (SRC_ASLP_MASK, SRC_FIFO_MASK, SRC_TRANS_MASK
 *   SRC_LNDPRT_MASK, SRC_PULSE_MASK, SRC_FF_MT_MASK, SRC_DRDY_MASK)
 */
void MMA8652::enableInt(uint8_t source)
{
  // Enable interrupt
  ctrlReg4 |= source;
  write(MMA8652_CTRL_REG4, ctrlReg4);
}

/**
 * disableInt
 * 
 * Disable interrupt
 * 
  * @param source Source of interrupt - mask (SRC_ASLP_MASK, SRC_FIFO_MASK, SRC_TRANS_MASK
 *   SRC_LNDPRT_MASK, SRC_PULSE_MASK, SRC_FF_MT_MASK, SRC_DRDY_MASK)
 */
void MMA8652::disableInt(uint8_t source)
{
  // Disable interrupt
  ctrlReg4 &= ~source;
  write(MMA8652_CTRL_REG4, ctrlReg4);
}

/**
 * enableTapInt
 * 
 * Enable tap interrupt and attach interrupt to output pin
 * 
 * @param sensibility 0 is the highest sensibility, 255 is the lowest
 */
void MMA8652::enableTapInt(uint8_t sensibility, bool doubleTap)
{
  uint8_t cfg;
  
  // Enter stand-by mode
  standBy();
  delay(1); 
  enableInt(SRC_PULSE_MASK);
  delay(1);
  if (doubleTap)
    cfg = 0x2A;
  else
    cfg = 0x15;
    
  // Enable single pulse detection on each axis
  write(MMA8652_PULSE_CFG, cfg);
  delay(1);
  
  write(MMA8652_PULSE_THSX, sensibility);
  delay(1);
  write(MMA8652_PULSE_THSY, sensibility);
  delay(1);
  write(MMA8652_PULSE_THSZ, sensibility);
  delay(1);  
  
  // Back to active mode
  active();
}

/**
 * disableTapInt
 * 
 * Disable tap interrupt
 */
void MMA8652::disableTapInt(void)
{
  // Enter stand-by mode
  standBy();
  delay(1); 
  disableInt(SRC_PULSE_MASK);
  delay(1);
  // Disable Portrait/Ladscape orientation detection
  write(MMA8652_PULSE_CFG, 0);
  delay(1);
  // Back to active mode
  active();
}

/**
 * enableFreeFallInt
 * 
 * Enable free-fall interrupt and attach interrupt to output pin
 * 
 * @param sensibility 0 is the highest sensibility, 255 is the lowest
 */
void MMA8652::enableFreeFallInt(uint8_t sensibility)
{
  // Enter stand-by mode
  standBy();
  delay(1); 
  enableInt(SRC_FF_MT_MASK);
  delay(1);
  // Enable single pulse detection on each axis
  write(MMA8652_FF_MT_CFG, 0xF8); // Motion detection - XYZ events enabled
  delay(1);
  write(MMA8652_FF_MT_THS, sensibility);
  delay(1);
  // Back to active mode
  active();
}

/**
 * disableFreeFallInt
 * 
 * Disable free-fall interrupt
 */
void MMA8652::disableFreeFallInt(void)
{
  // Enter stand-by mode
  standBy();
  delay(1); 
  disableInt(SRC_FF_MT_MASK);
  delay(1);
  // Disable Portrait/Ladscape orientation detection
  write(MMA8652_FF_MT_CFG, 0);
  delay(1);
  // Back to active mode
  active();
}

/**
 * enablePlInt
 * 
 * Enable portrait-landscape orientation interrupt and attach interrupt
 * to output pin
 */
void MMA8652::enablePlInt(void)
{
  // Enter stand-by mode
  standBy();
  delay(1);
  enableInt(SRC_LNDPRT_MASK);
  delay(1);
  // Enable Portrait/Ladscape orientation detection
  write(MMA8652_PL_CFG, PL_EN_MASK);
  delay(1);
  // Back to active mode
  active();
}

/**
 * disablePlInt
 * 
 * Disable portrait-landscape orientation interrupt
 */
void MMA8652::disablePlInt(void)
{
  // Enter stand-by mode
  standBy();
  delay(1); 
  disableInt(SRC_LNDPRT_MASK);
  delay(1);
  // Disable Portrait/Ladscape orientation detection
  write(MMA8652_PL_CFG, 0);
  delay(1);
  // Back to active mode
  active();
}

/**
 * sleep
 * 
 * Enter sleep mode, ready to be brought back to the active mode by an
 * interrupt
 */
void MMA8652::sleep(void)
{
  if (_IS_PL_INT_ENABLED())
  {
    // Disable Portrait/Landscape orientation interrupt, which is incompatible
    // With the current sleep mode
    disablePlInt();
    delay(1);
  }

  // Re-enter active mode first
  active();
  delay(2);
  // Enter stand-by mode
  standBy();
  delay(2);
  write(MMA8652_ASLP_COUNT, 1);
  delay(2); 
  write(MMA8652_CTRL_REG2, 0x1C);

  delay(1);
  // Back to active mode
  active();
}
