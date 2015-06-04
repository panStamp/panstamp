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
 * Creation date: 06/03/2013
 */

#include "panstamp.h"

/**
 * radioISR
 *
 * Radio interrupt routine
 */
__attribute__((interrupt(CC1101_VECTOR)))
void radioISR(void)
{
  unsigned int coreIntSource = RF1AIV;            // Radio Core      interrupt register

  // Radio Core interrupt
  if(coreIntSource)
  {
    // Check for SYNC interrupt
    if(coreIntSource == RF1AIV_RFIFG9)
    {
      if(MRFI_SYNC_PIN_INT_IS_ENABLED())
      {
        static CCPACKET ccPacket;

        /*  clear the sync pin interrupt, run sync pin ISR */
        /*
         *  NOTE!  The following macro clears the interrupt flag but it also *must*
         *  reset the interrupt capture.  In other words, if a second interrupt
         *  occurs after the flag is cleared it must be processed, i.e. this interrupt
         *  exits then immediately starts again.  Most microcontrollers handle this
         *  naturally but it must be verified for every target.
         */

        MRFI_CLEAR_SYNC_PIN_INT_FLAG();
        MRFI_DISABLE_SYNC_PIN_INT();

        // Any packet waiting to be read?
        if (panstamp.radio.receiveData(&ccPacket) > 0)
        {
          // Is CRC OK?
          if (ccPacket.crc_ok)
          {            
            if (panstamp.ccPacketReceived != NULL)
              panstamp.ccPacketReceived(&ccPacket);
          }
          MRFI_ENABLE_SYNC_PIN_INT();
        }
      }
    }
    // Check for RF_RDY (Event1 WOR) interrupt
    else if(coreIntSource == RF1AIV_RFIFG14)
    {
      RF1AIE |= BIT9 + BIT1;
      RF1AIFG &= ~(BIT9 + BIT1);
      RF1AIES |= BIT9; // Falling edge of RFIFG9
      panstamp.radio.setRxState();
      __bic_SR_register_on_exit(LPM3_bits);
    }
  }
}

/**
 * PANSTAMP
 *
 * Class constructor
 */
PANSTAMP::PANSTAMP(void)
{
  ccPacketReceived = NULL;
}

/**
 * init
 * 
 * Initialize panStamp board
 * 
 * @param freq Carrier frequency
 * @param mode Working mode (speed, ...)
 */
void PANSTAMP::init(uint8_t freq, uint8_t mode) 
{
  // Disable wireless bootloader
  enableWirelessBoot(false);
  
  // Initialize MCU core
  core.init();

  // Initialize onboard LED pin
  INIT_ONBOARD_LED();

  // Initialize internal ACC power pin
  INIT_ACC_POWER();

  // Setup radio interface
  radio.init(freq, mode);

  delayMicroseconds(50);
}

/**
 * rxOn
 *
 * Enable RF reception
 */
void PANSTAMP::rxOn(void)
{
  MRFI_ENABLE_SYNC_PIN_INT();
}

/**
 * rxOff
 *
 * Disable RF reception
 */
void PANSTAMP::rxOff(void)
{
  MRFI_DISABLE_SYNC_PIN_INT();
}

/**
 * sleep
 *
 * Enter LPM4
 */
void PANSTAMP::sleep(void)
{
  // Power down radio
  radio.setPowerDownState();
  
  // If RTC calendar is running
  if (rtc.calendarIsRunning)
    core.setLowPowerMode();        // Enter LPM3
  else
    core.setLowPowerMode(true);    // Enter LPM4
  
  // Wake-up
  wakeUp();
}

/**
 * sleepSec
 *
 * put the MCU in sleep mode
 *
 * @param time Sleep time in seconds
 * @param source Source of interruption (RTCSRC_VLO or RTCSRC_XT1)
 */
void PANSTAMP::sleepSec(uint16_t time, RTCSRC source)
{ 
  if (time == 0)
    return;
   
  // Power down radio
  radio.setPowerDownState();

  core.delayClockCycles(0xFFFF);

  // Sleep
  rtc.sleep(time, source);
   
  // Wake-up radio
  radio.setRxState();
}

/**
 * reset
 * 
 * Reset panStamp
 */
void PANSTAMP::reset(void)
{
  WDTCTL = 0;
  while (1) {}
}
    
/**
 * Pre-instantiate PANSTAMP object
 */
PANSTAMP panstamp;

