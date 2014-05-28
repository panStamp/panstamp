/**
 * channel.h
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
 * Creation date: 08/24/2012
 */

#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "Arduino.h"

/**
 * Definition section
 */
#define NB_OF_SAMPLES       80

/**
 * Offset introduced in the AC voltage signal by the half-wave rectifier
 * diode (in mV)
 */
#define ACVOLT_OFFSET_DIODE 350

/**
 * AC voltage scale due to the voltage divider
 */
#define ACVOLT_SCALE        8.5

/**
 * Return codes
 */
#define CHANNEL_SAMPLES_NOT_COMPLETED    0
#define CHANNEL_SAMPLES_COMPLETED        1
#define CHANNEL_NO_VAC_SIGNAL            2

/**
 * Class: CHANNEL
 * 
 * Description:
 * AC channel class
 */
class CHANNEL
{
  private:   
    /**
     * Voltage supply in mV
     */
    unsigned int voltageSupply;
     
    /**
     * Arduino analog pin connected to the AC voltage signal
     */
    byte voltagePin;

    /**
     * Arduino analog pin connected to the AC current signal
     */
    byte currentPin;

    /**
     * Peak voltage in volts
     */
    float peakVoltage;

    /**
     * Peak current in mA
     */
    float peakCurrent;
    
    /**
     * Time when the last measurements were taken (ms)
     */
    unsigned long lastTime;
    
  public:  
    /**
     * Scaling factor for the AC voltage signal coming from the voltage transformer
     */
    float voltageScale;

    /**
     * Scaling factor for the AC voltage signal coming from the current transformer
     */
    float currentScale;

    /**
     * Power factor offset (obtained during calibration)
     */
    float pfOffset;
    
    /**
     * Enable/disable reading
     */
    bool enable;
    
    /**
     * AC frequency
     */
    byte frequency;
    
    /**
     * RMS voltage
     */
    float rmsVoltage;

    /**
     * RMS current
     */
    float rmsCurrent;

    /**
     * Apparent power
     */
    float appPower;

    /**
     * Active (real) power
     */
    float actPower;

    /**
     * Power factor
     */
    float powerFactor;
    
    /**
     * Initial energy consummed in KWh
     */
    double initialKwh;
    
    /**
     * Energy consumed in KWh
     */
    double kwh;

    /**
     * CHANNEL
     * 
     * Class constructor
     */
    CHANNEL(void);
    
    /**
     * CHANNEL
     * 
     * Class constructor
     *
     * 'vcc'     Voltage supply
     * 'vPin'    Arduino analog pin connected to the AC voltage signal
     * 'iPin'    Arduino analog pin connected to the AC current signal
     * 'vScale'  Scaling factor for the voltage signal
     * 'iScale'  Scaling factor for the current signal
     * 'pfShift' Power factor offset to be applied (obtained during calibration)
     */
    CHANNEL(unsigned int vcc, int vPin, int iPin, float vScale=1.0, float iScale=1.0, float pfShift=0.0);

    /**
     * update
     * 
     * Update AC channel readings
     *
     * Return:
     *    0 if the function still needs to take additional samples
     *    1 if the function read the necessary amount of samples
     *    2 if the function detected no VAC signal after reading the samples
     */
    byte update(void);
    
    /**
     * run
     * 
     * Run channel readings and calculations
     *
     * Return:
     *   Code returned by update()
     */
    byte run(void);
};

#endif

