/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
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
 * Creation date: 03/03/2011
 */

#include "panstamp.h"
#include "commonregs.h"
#include "calibration.h"
#include <avr/wdt.h>

#define enableIRQ_GDO0()          attachInterrupt(0, isrGDO0event, FALLING);
#define disableIRQ_GDO0()         detachInterrupt(0);

DEFINE_COMMON_REGINDEX_START()
DEFINE_COMMON_REGINDEX_END()

/**
 * Array of registers
 */
extern REGISTER* regTable[];
extern byte regTableSize;

/**
 * PANSTAMP
 *
 * Class constructor
 */
PANSTAMP::PANSTAMP(void)
{
  statusReceived = NULL;
  repeater = NULL;  
}

/**
 * enableRepeater
 *
 * Enable repeater mode
 *
 * 'maxHop'  MAximum repeater count. Zero if omitted
 */
void PANSTAMP::enableRepeater(byte maxHop)
{
  if (repeater == NULL)
  {
    static REPEATER repe;
    repeater = &repe;
    repeater->init(maxHop);
  }

  if (maxHop == 0)
    repeater->stop();
}

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * 'regId'  Register ID
 */
REGISTER * getRegister(byte regId)
{
  if (regId >= regTableSize)
    return NULL;

  return regTable[regId]; 
}

/**
 * isrGDO0event
 *
 * Event on GDO0 pin (INT0)
 */
void isrGDO0event(void)
{
  // Disable interrupt
  disableIRQ_GDO0();

  if (panstamp.cc1101.rfState == RFSTATE_RX)
  {
    static CCPACKET ccPacket;
    static SWPACKET swPacket;
    REGISTER *reg;
    bool eval = true;

    if (panstamp.cc1101.receiveData(&ccPacket) > 0)
    {
      if (ccPacket.crc_ok)
      {
        swPacket = SWPACKET(ccPacket);

        #ifdef SWAP_EXTENDED_ADDRESS
        if (swPacket.addrType == SWAPADDR_EXTENDED)
        #else
        if (swPacket.addrType == SWAPADDR_SIMPLE)
        #endif
        {
          // Repeater enabled?
          if (panstamp.repeater != NULL)
            panstamp.repeater->packetHandler(&swPacket);

          // Smart encryption locally enabled?
          if (panstamp.security & 0x02)
          {
            // OK, then incoming packets must be encrypted too
            if (!(swPacket.security & 0x02))
              eval = false;
          }
        }
        else
          eval = false;

        if (eval)
        {
          // Function
          switch(swPacket.function)
          {
            case SWAPFUNCT_CMD:
              // Command not addressed to us?
              if (swPacket.destAddr != panstamp.swapAddress)
                break;
              // Current version does not support data recording mode
              // so destination address and register address must be the same
              if (swPacket.destAddr != swPacket.regAddr)
                break;
              // Valid register?
              if ((reg = getRegister(swPacket.regId)) == NULL)
                break;

              // Anti-playback security enabled?
              if (panstamp.security & 0x01)
              {
                // Check received nonce
                if (panstamp.nonce != swPacket.nonce)
                {
                  // Nonce missmatch. Transmit correct nonce.
                  reg = getRegister(REGI_SECUNONCE);
                  reg->sendSwapStatus();
                  break;
                }
              }
              // Filter incorrect data lengths
              if (swPacket.value.length == reg->length)
                reg->setData(swPacket.value.data);
              else
                reg->sendSwapStatus();
              break;
            case SWAPFUNCT_QRY:
              // Only Product Code can be broadcasted
              if (swPacket.destAddr == SWAP_BCAST_ADDR)
              {
                if (swPacket.regId != REGI_PRODUCTCODE)
                  break;
              }
              // Query not addressed to us?
              else if (swPacket.destAddr != panstamp.swapAddress)
                break;
              // Current version does not support data recording mode
              // so destination address and register address must be the same
              if (swPacket.destAddr != swPacket.regAddr)
                break;
              // Valid register?
              if ((reg = getRegister(swPacket.regId)) == NULL)
                break;
              reg->getData();
              break;
            case SWAPFUNCT_STA:
              // User callback function declared?
              if (panstamp.statusReceived != NULL)
                panstamp.statusReceived(&swPacket);
              break;
            default:
              break;
          }
        }
      }
    }
  }
  // Enable interrupt
  enableIRQ_GDO0();
}

/**
 * init
 * 
 * Initialize panStamp board
 * 
 * 'freq'	New carrier frequency
 */
void PANSTAMP::init(byte freq) 
{
  int i;

  // Calibrate internal RC oscillator
  rtcCrystal = rcOscCalibrate();

  // Intialize registers
  for(i=0 ; i<regTableSize ; i++)
    regTable[i]->init();

  // Setup CC1101
  cc1101.init(freq);

  // Security disabled by default
  security = 0;

  delayMicroseconds(50);

  // Enter RX state
  cc1101.setRxState();

  // Attach callback function for GDO0 (INT0)
  enableIRQ_GDO0();

  // Default values
  nonce = 0;
  systemState = SYSTATE_RXON;
}

/**
 * reset
 * 
 * Reset panStamp
 */
void PANSTAMP::reset() 
{
  // Tell the network that our panStamp is restarting
  systemState = SYSTATE_RESTART;
  getRegister(REGI_SYSSTATE)->sendSwapStatus();

  // Reset panStamp
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
  systemState = SYSTATE_RXON;
}

/**
 * goToSleep
 *
 * Sleep whilst in power-down mode. This function currently uses sleepWd or
 * sleepRtc in a loop
 */
void PANSTAMP::goToSleep(void)
{
  // Get the amount of seconds to sleep from the internal register
  int intInterval = txInterval;
  int i, loops;
  byte minTime;
  
  // No interval? Then return
  if (intInterval == 0)
    return;

  // Search the maximum sleep time passed as argument to sleepWd that best
  // suits our desired interval
  if (intInterval % 8 == 0)
  {
    loops = intInterval / 8;
    
    if (rtcCrystal)
      minTime = RTC_8S;
    else
      minTime = WDTO_8S;
  }
  else if (intInterval % 4 == 0)
  {
    if (rtcCrystal)
    {
      loops = intInterval / 2;
      minTime = RTC_2S;
    }
    else
    {
      loops = intInterval / 4;
      minTime = WDTO_4S;
    }
  }
  else if (intInterval % 2 == 0)
  {
    loops = intInterval / 2;
    if (rtcCrystal)    
      minTime = RTC_2S;
    else
      minTime = WDTO_2S;
  }
  else
  {
    loops = intInterval;
    if (rtcCrystal)
      minTime = RTC_1S;
    else
      minTime = WDTO_1S;
  }

  systemState = SYSTATE_RXOFF;

  // Power-down CC1101
  cc1101.setPowerDownState();

  // Sleep
  for (i=0 ; i<loops ; i++)
  {
    // Exit sleeping loop?
    if (systemState == SYSTATE_RXON)
      break;

    if (rtcCrystal)
      rtc.sleepRtc(minTime);
    else
      rtc.sleepWd(minTime);
  }

  // Reset CC1101 IC
  cc1101.wakeUp();

  // set system state to RF Rx ON
  systemState = SYSTATE_RXON;
}

/**
 * enterSystemState
 *
 * Enter system state
 *
 * 'state'  New system state
 */
void PANSTAMP::enterSystemState(SYSTATE state)
{
  // Enter SYNC mode (full Rx mode)
  byte newState[] = {state};
  regTable[REGI_SYSSTATE]->setData(newState);
}

/**
 * getInternalTemp
 * 
 * Read internal (ATMEGA328 only) temperature sensor
 * Reference: http://playground.arduino.cc/Main/InternalTemperatureSensor
 * 
 * Return:
 * 	Temperature in degrees Celsius
 */
long PANSTAMP::getInternalTemp(void) 
{
  unsigned int wADC;
  long t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celcius.
  return (t);
}

/**
 * setSmartPassword
 * 
 * Set Smart Encryption password
 * 
 * 'password'	Encryption password
 */
void PANSTAMP::setSmartPassword(byte* password)
{
  // Save password
  memcpy(encryptPwd, password, sizeof(encryptPwd));
  // Enable Smart Encryption
  security |= 0x02;
}

/**
 * enableRfRx
 * 
 * Enable or disable RF reception
 *
 * @param ena Enable if true. Disable otherwise
 */
void PANSTAMP::enableRfRx(bool ena)
{
  if (ena)
    enableIRQ_GDO0()
  else
    disableIRQ_GDO0();
}

/**
 * updateEeprom
 *
 * Prepare EEPROM for extended addresses in case it is not yet
 */
/*
void PANSTAMP::updateEeprom(void)
{
  unsigned int addr;
  unsigned char b;


  b = EEPROM.read(EEPROM_TX_INTERVAL);
  if ((b != 0x00) && (b != 0xFF))
  EEPROM.write(addr+1, b);
}
*/
/**
 * Pre-instantiate PANSTAMP object
 */
PANSTAMP panstamp;

