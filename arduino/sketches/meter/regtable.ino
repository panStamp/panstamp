/**
 * regtable
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
 * Creation date: 01/26/2012
 */

#include <EEPROM.h>
#include "product.h"
#include "panstamp.h"
#include "regtable.h"
#include "config.h"

/**
 * Declaration of common callback functions
 */
DECLARE_COMMON_CALLBACKS()

/**
 * Definition of common registers
 */
DEFINE_COMMON_REGISTERS()

/*
 * Definition of custom registers
 */

// Data containers

// AC frequency, RMS voltage, RMS current, apparent power, active power, power factor
// and Energy in KWh
static byte dtChannelsEnergy[NB_OF_CHANNELS][14];       
// Voltage transformer scaling factor [0-655.35], current transformer scaling
// factor [0-655.35], power factor offset [0.0-0.99] and channel enable [0, 1]
static byte dtChannelsConfig[NB_OF_CHANNELS][CONFIG_CHANNEL_SIZE];
// Pulse counts
static byte dtPulseCount[NB_OF_COUNTERS][4];
// Configuration for the pulse inputs: initial energy count, amount of pulses per unit step
static byte dtPulseConfig[NB_OF_COUNTERS][CONFIG_PULSEINPL_SIZE];

// Energy values
//-----------------------
// Channel 0 energy
REGISTER regChannelEnergy0(dtChannelsEnergy[0], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);
// Channel 1 energy
REGISTER regChannelEnergy1(dtChannelsEnergy[1], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);
// Channel 2 energy
REGISTER regChannelEnergy2(dtChannelsEnergy[2], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);
// Channel 3 energy
REGISTER regChannelEnergy3(dtChannelsEnergy[3], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);
// Channel 4 energy
REGISTER regChannelEnergy4(dtChannelsEnergy[4], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);
// Channel 5 energy
REGISTER regChannelEnergy5(dtChannelsEnergy[5], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);
// Channel 6 energy
REGISTER regChannelEnergy6(dtChannelsEnergy[6], sizeof(dtChannelsEnergy[0]), &updtChannelEnergy, NULL);

// Configuration registers for energy channels
//----------------------------------------------
// Channel 0 config
REGISTER regChannelConfig0(dtChannelsConfig[0], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);
// Channel 1 config
REGISTER regChannelConfig1(dtChannelsConfig[1], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);
// Channel 2 config
REGISTER regChannelConfig2(dtChannelsConfig[2], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);
// Channel 3 config
REGISTER regChannelConfig3(dtChannelsConfig[3], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);
// Channel 4 config
REGISTER regChannelConfig4(dtChannelsConfig[4], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);
// Channel 5 config
REGISTER regChannelConfig5(dtChannelsConfig[5], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);
// Channel 6 config
REGISTER regChannelConfig6(dtChannelsConfig[6], sizeof(dtChannelsConfig[0]), NULL, &setChannelConfig);

// Pulse counters
//---------------------
// Pulse input 0
REGISTER regPulseCount0(dtPulseCount[0], sizeof(dtPulseCount[0]), &updtPulseCount, NULL);
// Pulse input 1
REGISTER regPulseCount1(dtPulseCount[1], sizeof(dtPulseCount[0]), &updtPulseCount, NULL);
// Pulse input 2
REGISTER regPulseCount2(dtPulseCount[2], sizeof(dtPulseCount[0]), &updtPulseCount, NULL);

// Configuration registers for pulse inputs
//----------------------------------------------
// Channel 0 config
REGISTER regPulseConfig0(dtPulseConfig[0], sizeof(dtPulseConfig[0]), NULL, &setPulseConfig);
// Channel 1 config
REGISTER regPulseConfig1(dtPulseConfig[1], sizeof(dtPulseConfig[1]), NULL, &setPulseConfig);
// Channel 2 config
REGISTER regPulseConfig2(dtPulseConfig[2], sizeof(dtPulseConfig[2]), NULL, &setPulseConfig);

/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  &regChannelEnergy0,
  &regChannelEnergy1,
  &regChannelEnergy2,
  &regChannelEnergy3,
  &regChannelEnergy4,
  &regChannelEnergy5,
  &regChannelEnergy6,
  &regChannelConfig0,
  &regChannelConfig1,
  &regChannelConfig2,
  &regChannelConfig3,
  &regChannelConfig4,
  &regChannelConfig5,
  &regChannelConfig6,
  &regPulseCount0,
  &regPulseCount1,
  &regPulseCount2,
  &regPulseConfig0,
  &regPulseConfig1,
  &regPulseConfig2
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * updtChannelEnergy
 *
 * Update channel register with energy consumed
 *
 * 'rId'  Register ID
 */
const void updtChannelEnergy(byte rId)
{
  int channel = rId - REGI_CHANNEL_ENERGY_0;
  unsigned long tmpValue;

  // AC frequency (in Hz)
  dtChannelsEnergy[channel][0] = channels[channel]->frequency;
  
  // RMS voltage (in volts)
  tmpValue = channels[channel]->rmsVoltage;
  dtChannelsEnergy[channel][1] = (tmpValue >> 8) & 0xFF;
  dtChannelsEnergy[channel][2] = tmpValue & 0xFF;
  
  // RMS current (in 1/100 amps)
  tmpValue = channels[channel]->rmsCurrent * 100;
  dtChannelsEnergy[channel][3] = (tmpValue >> 8) & 0xFF;
  dtChannelsEnergy[channel][4] = tmpValue & 0xFF;
  
  // Apparent power (in VA)
  tmpValue = channels[channel]->appPower;
  dtChannelsEnergy[channel][5] = (tmpValue >> 8) & 0xFF;
  dtChannelsEnergy[channel][6] = tmpValue & 0xFF;
  
  // Active power (in W)
  tmpValue = channels[channel]->actPower;
  dtChannelsEnergy[channel][7] = (tmpValue >> 8) & 0xFF;
  dtChannelsEnergy[channel][8] = tmpValue & 0xFF;
  
  // Power factor (1/100)
  tmpValue = channels[channel]->powerFactor * 100;
  dtChannelsEnergy[channel][9] = tmpValue & 0xFF;

  // KWh
  tmpValue = (channels[channel]->initialKwh + channels[channel]->kwh) * 100;
  dtChannelsEnergy[channel][10] = (tmpValue >> 24) & 0xFF;
  dtChannelsEnergy[channel][11] = (tmpValue >> 16) & 0xFF;
  dtChannelsEnergy[channel][12] = (tmpValue >> 8) & 0xFF;
  dtChannelsEnergy[channel][13] = tmpValue & 0xFF;
}

/**
 * setChannelConfig
 *
 * Set channel configuration
 *
 * 'rId'     Register ID
 * 'config'  New configuration parameters
 */
const void setChannelConfig(byte rId, byte *config)
{
  int channel = rId - REGI_CHANNEL_CONFIG_0;
  unsigned int tmpValue;
  byte i;
  
  // Update register
  memcpy(dtChannelsConfig[channel], config, sizeof(dtChannelsConfig[channel]));

  // Voltage scale
  tmpValue = config[0];
  tmpValue = (tmpValue << 8) | config[1];
  channels[channel]->voltageScale = tmpValue / 100.0;
  
  // Current scale
  tmpValue = config[2];
  tmpValue = (tmpValue << 8) | config[3];
  channels[channel]->currentScale = tmpValue / 100.0;
  
  // Power factor offset
  channels[channel]->pfOffset = config[4] / 100.0;

  // Enable
  channels[channel]->enable = config[5] & 0x01;
  
  // Save config settings in EEPROM
  Serial.println(sizeof(dtChannelsConfig[channel]), HEX);
  for(i=0 ; i<sizeof(dtChannelsConfig[channel]) ; i++)
  {
    EEPROM.write(EEPROM_CONFIG_CHANNEL0 + CONFIG_CHANNEL_SIZE * channel + i, dtChannelsConfig[channel][i]);
  }
}

/**
 * updtPulseCount
 *
 * Update counting register
 *
 * 'rId'  Register ID
 */
const void updtPulseCount(byte rId)
{
  int input = rId - REGI_PULSE_COUNT_0;
  unsigned long initial, tmpValue;
  float scale;

  // Calculate initial value
  tmpValue = dtPulseConfig[input][0];
  initial = tmpValue << 24;
  tmpValue = dtPulseConfig[input][1];
  initial |= tmpValue << 16;
  tmpValue = dtPulseConfig[input][2];
  initial |= tmpValue << 8;
  initial |= dtPulseConfig[input][3];

  // Calculate scaling factor
  tmpValue = dtPulseConfig[input][4];
  tmpValue = (tmpValue << 8) | dtPulseConfig[input][5];
  scale = 100 / tmpValue;
  
  // Calculate final reading
  tmpValue = initial + counters[input] * scale;

  // Set register
  dtPulseCount[input][0] = (tmpValue >> 24) & 0xFF;
  dtPulseCount[input][1] = (tmpValue >> 16) & 0xFF;
  dtPulseCount[input][2] = (tmpValue >> 8) & 0xFF;
  dtPulseCount[input][3] = tmpValue & 0xFF;
}


/**
 * setPulseConfig
 *
 * Set configuration for pulse input
 *
 * 'rId'     Register ID
 * 'config'  New configuration parameters
 */
const void setPulseConfig(byte rId, byte *config)
{
  byte i;
  int input = rId - REGI_PULSE_CONFIG_0;
  
  // Update register
  memcpy(dtPulseConfig[input], config, sizeof(dtPulseConfig[input]));
  
  // Save config settings in EEPROM
  for(i=0 ; i<sizeof(dtPulseConfig[input]) ; i++)
    EEPROM.write(EEPROM_CONFIG_PULSE0 + CONFIG_PULSEINPL_SIZE * input + i, dtPulseConfig[input][i]);
}

