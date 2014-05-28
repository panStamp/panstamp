/**
 * sensor
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
 * Creation date: 03/31/2011
 */

#include "Arduino.h"
#include "sensor.h"

/**
 * Local functions
 */
int sensor_ReadByte(void);

/**
 * sensor_ReadByte
 *
 * Read data byte from DHT11 sensor
 */
int sensor_ReadByte(void)
{
  byte i, result = 0;
  unsigned int count = 20000;

  for(i=0; i< 8; i++)
  {
    while(!getDataPin())
    {
      if (--count == 0)
        return -1;
    }
    delayMicroseconds(30);
		
    if (getDataPin())
      result |=(1<<(7-i));

    count = 20000;

    while(getDataPin())
    {
      if (--count == 0)
        return -1;
    }
  }
  return result;
}

/**
 * sensor_ReadTempHum
 *
 * Read temperature and humidity values from DHT11 sensor
 *
 * Return -1 in case of error. Return 0 otherwise
 */
int sensor_ReadTempHum(void)
{
  int temperature, humidity, chk;
  
  byte dhtData[5];
  byte i, dhtCrc;
  boolean success = false;

  // Power ON sensor
  dhtSensorON();
  delay(1500);
  
  setDataOutput();
  setDataPin();
   
  // Start condition
  clearDataPin();
  delay(18);
  setDataPin();
  delayMicroseconds(40);	
  setDataInput();
  delayMicroseconds(40);

  if (!getDataPin())
  {
    // Start condition met
    delayMicroseconds(80);	
    if (getDataPin())
    {
      // Start condition met
      delayMicroseconds(80);

      // Now ready for data reception 
      for (i=0; i<5; i++)
      {
        if ((chk = sensor_ReadByte()) < 0)
          return -1;
          
        dhtData[i] = (byte)chk;
      }
      success = true;
    }
  }

  setDataOutput();
  clearDataPin();

  // Power OFF sensor
  dhtSensorOFF();
  
  if (!success)
    return -1;

  dhtCrc = dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3];

  // check check_sum
  if(dhtData[4]!= dhtCrc)
    return -1;  // CRC error

  // Prepare values for 2-decimal format:
  int sign = 1;
  if (dhtData[2] & 0x80)
  {
    sign = -1;
    dhtData[2] &= 0x7F; 
  }
  temperature = sign * word(dhtData[2], dhtData[3]) + 500;  // 50.0 ºC offset in order to accept negative temperatures
  humidity = word(dhtData[0], dhtData[1]);

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
  // DHT22 sensor
  pinMode(PIN_PWRDHT, OUTPUT);        // Configure Power pin as output
  dhtSensorOFF();

  // MiCS-4514 sensor
  pinMode(PIN_HEATING_NO2, OUTPUT);   // Configure pre-heating pin as output
  preHeaterON();
  delay(30000);
  preHeaterOFF();
}

