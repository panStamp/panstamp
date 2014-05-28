/*
 * meter
 *
 * Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
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
 * Creation date: 09/02/2012
 *
 * Device:
 * Binary input + counter module
 *
 * Description:
 * Energy meter providing the following inputs:
 *
 * 1 x Input for voltage transformer: pin 12 (A7)
 * 7 x Inputs for current transformers: pins 4 (A0), 5 (A1), 6 (A2), 8 (A3),
 *   9 (A4), 10 (A5) and 11 (A6)
 * 3 x Inputs for pulse counters: pins 20 (D5), 21 (D6) and 22 (D7)
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * meter.xml (Energy meter)
 */

#include "TimerOne.h"
#include "meter.h"

/**
 * Auxiliary variables
 */
byte channelNb;

/**
 * If true, send power data wirelessly
 */
volatile boolean transmit = false;

/**
 * Pin Change Interrupt flag
 */
volatile boolean pcIRQ = false;

/**
 * Counters
 */
const uint8_t counterPin[] = {5, 6, 7};        // Counter pins (Atmega port bits) on Port D
static unsigned long counters[] = {0, 0, 0};   // Initial counter values
static int lastStateCount[] = {-1, -1, -1};    // Initial pin states

/**
 * Timer 1 ticks
 */
unsigned int t1Ticks = 0;

/**
 * Wireless transmission interval (seconds)
 */
unsigned int txInterval;

/**
 * Vcc in mV
 */
unsigned int voltageSupply = 3260;


SIGNAL(PCINT2_vect)
{
  pcIRQ = true;
}


/**
 * updateCounters
 *
 * Update counters
 *
 * Return:
 * bit 0 set -> Counter 0 changed
 * bit 1 set -> Counter 1 changed
 * bit 2 set -> Counter 2 changed
 */
byte updateCounters(void)
{
  byte i, res = 0;
  byte state;

  for(i=0 ; i<sizeof(counterPin) ; i++)
  {
    state = bitRead(PIND, counterPin[i]);
    
    if (lastStateCount[i] != state)
    {      
      lastStateCount[i] = state;
    
      if (state == HIGH)
      {
        counters[i]++;
        res |= (1 << i);
      }
    }
  }

  return res;
}

/**
 * readVoltageSupply
 *
 * Read voltage supply (Vcc)
 *
 * Return voltage in mV
 */
unsigned int readVoltSupply(void)
{
  unsigned int result;
   
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
 * isrT1event
 *
 * Timer1 interrupt routine
 */
void isrT1event(void)
{
  t1Ticks++;

  if (t1Ticks == txInterval)
  {
    t1Ticks = 0;
    transmit = true;
  }
}

/**
 * readInitValues
 *
 * Read initial values from EEPROM
 */
void readInitValues(void)
{
  byte val, i, j;
  byte channelConfig[CONFIG_CHANNEL_SIZE];
  byte pulseConfig[CONFIG_PULSEINPL_SIZE];
  unsigned long tmpValue;
  boolean setToZero;
   
  // Read configuration for the energy channels
  for(i=0 ; i < NB_OF_CHANNELS ; i++)
  {
    setToZero = true;
    for(j=0 ; j<CONFIG_CHANNEL_SIZE ; j++)
    {
      channelConfig[j] = EEPROM.read(EEPROM_CONFIG_CHANNEL0 + CONFIG_CHANNEL_SIZE * i + j);
      if (channelConfig[j] != 0xFF)
        setToZero = false;
    }
    if (!setToZero)
      getRegister(REGI_CHANNEL_CONFIG_0 + i)->setData(channelConfig);
  }
  
  // Read initial KWh values for each channel
  for(i=0 ; i < NB_OF_CHANNELS ; i++)
  {
    tmpValue = 0;
    setToZero = true;
    for(j=0 ; j<CONFIG_INITKWH_SIZE ; j++)
    {
      val = EEPROM.read(EEPROM_INITIAL_KWH0 + CONFIG_INITKWH_SIZE * i + j);
      if (val != 0xFF)
        setToZero = false;
      tmpValue = tmpValue << 8;
      tmpValue |= val;
    }

    if (setToZero)
      channels[i]->initialKwh = 0;
    else
      channels[i]->initialKwh = tmpValue / 100.0;
  }

  // Read configuration for the pulse inputs
  for(i=0 ; i < NB_OF_COUNTERS ; i++)
  {
    setToZero = true;
    for(j=0 ; j<CONFIG_PULSEINPL_SIZE ; j++)
    {
      pulseConfig[j] = EEPROM.read(EEPROM_CONFIG_PULSE0 + CONFIG_PULSEINPL_SIZE * i + j);
      if (pulseConfig[j] != 0xFF)
        setToZero = false;
    }
    if (!setToZero)
      getRegister(REGI_PULSE_CONFIG_0 + i)->setData(pulseConfig);
  }
}

/**
 * saveValues
 * 
 * Save values in EEPROM
 */
void saveValues(void) 
{
  byte i, j, val;
  unsigned long tmpValue;

  // Save current KWh readings from channels
  for(i=0 ; i < NB_OF_CHANNELS ; i++)
  {
    tmpValue = channels[i]->kwh * 100;
    for(j=0 ; j<sizeof(tmpValue) ; j++)
    {
      val = (tmpValue >> (8 * (3-j))) & 0xFF;
      EEPROM.write(EEPROM_INITIAL_KWH0 + CONFIG_INITKWH_SIZE * i + j, val);
    }
  }

  // Save current readings from pulse inputs
  for(i=0 ; i < NB_OF_COUNTERS ; i++)
  {
    for(j=0 ; j<sizeof(counters[0]) ; j++)
    {
      val = (counters[i] >> (8 * (3-j))) & 0xFF;
      EEPROM.write(EEPROM_CONFIG_PULSE0 + CONFIG_PULSEINPL_SIZE * i + j, val);
    }
  }
}

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
   
  // Read Vcc
  voltageSupply = readVoltSupply();
 
  // Create energy channel objects
  static CHANNEL channel0(voltageSupply, PIN_ACVOLTAGE, 0, 17.75, 5);
  channels[0] = &channel0;
  static CHANNEL channel1(voltageSupply, PIN_ACVOLTAGE, 1, 17.75, 5);
  channels[1] = &channel1;
  static CHANNEL channel2(voltageSupply, PIN_ACVOLTAGE, 2, 17.75, 5);
  channels[2] = &channel2;
  static CHANNEL channel3(voltageSupply, PIN_ACVOLTAGE, 3, 17.75, 5);
  channels[3] = &channel3;
  static CHANNEL channel4(voltageSupply, PIN_ACVOLTAGE, 4, 17.75, 5);
  channels[4] = &channel4;
  static CHANNEL channel5(voltageSupply, PIN_ACVOLTAGE, 5, 17.75, 5);
  channels[5] = &channel5;
  static CHANNEL channel6(voltageSupply, PIN_ACVOLTAGE, 6, 17.75, 5);
  channels[6] = &channel6;

  // Pulse inputs
  // Set pins as inputs
  DDRD &= ~PCINTMASK;

  // Set PC interrupt mask
  PCMSK0 = 0;
  PCMSK1 = 0;
  PCMSK2 = PCINTMASK;

  // Init panStamp
  panstamp.init();

  Serial.begin(38400);
  Serial.flush();
  Serial.println("Power meter ready!");

  // Wireless transmission interval
  txInterval = panstamp.txInterval[0];
  txInterval = txInterval << 8 | panstamp.txInterval[1];

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_RXON);

  // Read initial configuration settings from EEPROM
  readInitValues();

  // Initialize Timer1
  Timer1.initialize(TIMER1_TICK_PERIOD_US);
  Timer1.attachInterrupt(isrT1event);

  // Enable PCINT interrupt on counter pins
  pcEnableInterrupt();
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{ 
  // Measure energy data
  for(channelNb=0 ; channelNb < NB_OF_CHANNELS ; channelNb++)
  {
    if (channels[channelNb]->enable)
    {
      // Read power data
      if (channels[channelNb]->run() == CHANNEL_NO_VAC_SIGNAL)
        saveValues();  // NO VAC signal detected. Save data in EEPROM
    }
  }

  if (transmit)
  {
    transmit = false;
    
    for(channelNb=0 ; channelNb < NB_OF_CHANNELS ; channelNb++)
    {
      if (channels[channelNb]->enable)
      {
        // Transmit channel data?
        getRegister(REGI_CHANNEL_ENERGY_0 + channelNb)->getData();
      }
    }
  }

  // Read pulses
  if (pcIRQ)
  {
    byte res = updateCounters();
    byte mask;
    if (res)
    {
      for(channelNb=0 ; channelNb<sizeof(counterPin) ; channelNb++)
      {
        mask = 1 << channelNb;
        // Input changed?
        if (res & mask)
        {
          // Transmit counter value
          getRegister(REGI_PULSE_COUNT_0 + channelNb)->getData();
        }
      }
    }
    //Ready to receive new PC interrupts
    pcIRQ = false;
  }

  delay(100);
}
