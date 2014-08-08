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
#include "calibration.h"
#include "Arduino.h"
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define enableIRQ_GDO0()          ::attachInterrupt(0, radioISR, FALLING);
#define disableIRQ_GDO0()         ::detachInterrupt(0);

/**
 * radioISR
 *
 * Radio interrupt routine
 */
void radioISR(void)
{
  // Disable interrupt
  disableIRQ_GDO0();

  if (panstamp.radio.rfState == RFSTATE_RX)
  {
    static CCPACKET ccPacket;
    bool eval = true;

    // Any packet waiting to be read?
    if (panstamp.radio.receiveData(&ccPacket) > 0)
    {
      // Is CRC OK?
      if (ccPacket.crc_ok)
      {
        if (panstamp.ccPacketReceived != NULL)
          panstamp.ccPacketReceived(&ccPacket);
      }
    }
  }
  
  // Enable interrupt
  enableIRQ_GDO0();
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
 * rxOn
 *
 * Enable RF reception
 */
void PANSTAMP::rxOn(void)
{
  ::attachInterrupt(0, radioISR, FALLING);
}

/**
 * rxOff
 *
 * Disable RF reception
 */
void PANSTAMP::rxOff(void)
{
  detachInterrupt(0);
}

/**
 * init
 * 
 * Initialize panStamp board
 * 
 * @param freq Carrier frequency
 */
void PANSTAMP::init(uint8_t freq) 
{
  // Calibrate internal RC oscillator
  rcOscCalibrate();

  // Setup CC1101
  radio.init(freq);

  delayMicroseconds(50);  

  // Enter RX state
  radio.setRxState();

  // Attach callback function for GDO0 (INT0)
  enableIRQ_GDO0();

  // Default values
  state = RXON;
}

/**
 * reset
 * 
 * Reset panStamp
 */
void PANSTAMP::reset() 
{
  // Reset module
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {}
}

/**
 * wakeUp
 *
 * Wake from sleep mode
 */
void PANSTAMP::wakeUp(void)
{
  rtc.wakeUp();
  state = RXON;
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
   
  // Power-down panStamp
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  delayMicroseconds(10);
  // Disable ADC
  ADCSRA &= ~(1 << ADEN);
  // Unpower functions
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
  PRR0 = 0xFF;
#else
  PRR = 0xFF;
#endif
  // Enter sleep mode
  sleep_mode();

  // ZZZZZZZZ...

  // Wake-up!!
  wakeUp();
}

/**
 * sleepSec
 *
 * put the MCU in sleep mode
 *
 * @param time Sleep time in seconds
 * @param source Source of interruption (RTCSRC_WDT or RTCSRC_XT2)
 */
void PANSTAMP::sleepSec(uint16_t time, RTCSRC source)
{
  int i, loops;
  byte minTime;
  
  // No interval? Then return
  if (time == 0)
    return;

  // Search the maximum sleep time passed as argument to sleepWd that best
  // suits our desired interval
  if (time % 8 == 0)
  {
    loops = time / 8;
    
    if (source == RTCSRC_XT2)
      minTime = RTC_8S;
    else
      minTime = WDTO_8S;
  }
  else if (time % 4 == 0)
  {
    if (source == RTCSRC_XT2)
    {
      loops = time / 2;
      minTime = RTC_2S;
    }
    else
    {
      loops = time / 4;
      minTime = WDTO_4S;
    }
  }
  else if (time % 2 == 0)
  {
    loops = time / 2;
    if (source == RTCSRC_XT2)    
      minTime = RTC_2S;
    else
      minTime = WDTO_2S;
  }
  else
  {
    loops = time;
    if (source == RTCSRC_XT2)
      minTime = RTC_1S;
    else
      minTime = WDTO_1S;
  }

  state = RXOFF;

  // Power-down CC1101
  radio.setPowerDownState();

  // Sleep
  for (i=0 ; i<loops ; i++)
  {
    // Exit sleeping loop?
    if (state == RXON)
      break;

    if (source == RTCSRC_XT2)
      rtc.sleepRtc(minTime);
    else
      rtc.sleepWd(minTime);
  }

  // Reset CC1101 IC
  radio.wakeUp();

  // set system state to RF Rx ON
  state = RXON; 
}

/**
 * getVcc
 *
 * Read voltage supply
 *
 * @return voltage in mV
 */
uint16_t PANSTAMP::getVcc(void)
{
  uint16_t result;
  
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  
  return result;
}

/**
 * Pre-instantiate PANSTAMP object
 */
PANSTAMP panstamp;
