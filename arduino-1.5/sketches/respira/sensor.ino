/**
 * sensor
 *
 * Copyright (c) 2014 Daniel Berenguer <contact@panstamp.com>
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
 * Creation date: 03/31/2011
 */

#include "Arduino.h"
#include "sensor.h"

/**
 * sensor_ReadTempHum
 *
 * Read temperature and humidity values from DHT11 sensor
 *
 * Return -1 in case of error. Return 0 otherwise
 */
int sensor_ReadTempHum(void)
{
  // Read humidity
  float h = dht.readHumidity();
  // Read temperature
  float t = dht.readTemperature();
  // Unpower sensor
  digitalWrite(PWRPIN, LOW);

  uint16_t humidity = h * 10;
  uint16_t temperature = (t + 50) * 10;

  dtHtSensor[0] = (temperature >> 8) & 0xFF;
  dtHtSensor[1] = temperature & 0xFF;
  dtHtSensor[2] = (humidity >> 8) & 0xFF;
  dtHtSensor[3] = humidity & 0xFF;
   
  return 0;
}

/**
 * sensor_ReadMics4514
 *
 * Read CO and NO2 levels from MiCS-4514 sensor
 *
 * Return -1 in case of error. Return 0 otherwise
 */
int sensor_ReadMics4514(void)
{
  unsigned int reading;
  unsigned long calib;
  float fVolt, fRes, fConc;

  /**  
   * Read CO sensor
   */
  reading = analogRead(ANALOG_CO);
  // Convert reading to voltage (Volts)
  fVolt = (reading * 3.3) / 1024.0;
  // Get Rs/R0 value
  calib = CALIB_R0_CO;
  fRes = (5000.0/fVolt - 1000) / calib;

  // Convert to ppm
  if (fRes > 0.7)
    fRes = 0.7;
  if (fRes > 0.6)
    fConc = (0.711 - fRes) / 0.011;
  else if (fRes > 0.3)
    fConc = (0.7 - fRes) / 0.01;
  else
    fConc = (0.3233 - fRes) / 0.00058;
    
  reading = fConc;
  dtAirSensor[0] = (reading >> 8) & 0xFF;
  dtAirSensor[1] = reading & 0xFF;

  /**  
   * Read NO2 sensor
   */

  reading = analogRead(ANALOG_NO2);
  // Convert reading to voltage (Volts)
  fVolt = 3.3;
  fVolt *= reading;
  fVolt /= 1024.0;

  // Get Rs/R0 value
  calib = CALIB_R0_NO2; 
  
  fRes = (5000.0/fVolt - 1000) / calib;

  // Convert to ppm
  if (fRes < 3.0)
    fRes = 3.0;
  if (fRes >= 3.0 && fRes < 8.0)
    fConc = (fRes - 0.5) / 0.25;
  else
    fConc = (fRes + 129.655) / 4.589;
    
  reading = fConc;
  dtAirSensor[2] = (reading >> 8) & 0xFF;
  dtAirSensor[3] = reading & 0xFF;
}

/**
 * initSensor
 *
 * Initialize sensor pins
 */
void initSensor(void)
{
  // MiCS-4514 sensor
  pinMode(PIN_HEATING_NO2, OUTPUT);   // Configure pre-heating pin as output
  preHeaterON();
  delay(30000);
  preHeaterOFF();
}

