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

#ifdef TEMPHUM
#ifdef DHT11
#include "dht11.h"
#endif
#elif TEMPPRESS
#include "Wire.h"
#include "Adafruit_BMP085.h"
Adafruit_BMP085 bmp;
#endif

/**
 * Local functions
 */
#ifdef TEMPHUM
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
  
  #ifdef DHT11

  dht11 sensor;
  
  dhtSensorON();
  delay(1500);   
  chk = sensor.read(PIN_DHT_DATA);
  dhtSensorOFF();

  if (chk != DHTLIB_OK)
    return -1;

  temperature = sensor.temperature * 10 + 500;
  humidity = sensor.humidity * 10;
  
  #elif DHT22

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
  //setDataPin();
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

  #endif

  dtSensor[0] = (temperature >> 8) & 0xFF;
  dtSensor[1] = temperature & 0xFF;
  dtSensor[2] = (humidity >> 8) & 0xFF;
  dtSensor[3] = humidity & 0xFF;
   
  return 0;
}

#elif TEMP
/**
 * sensor_ReadTemp
 *
 * Read temperature from TMP36 sensor
 *
 * Return -1 in case of error. Return 0 otherwise
 */
int sensor_ReadTemp(void)
{ 
  // Switch on sensor
  tempSensorON();
  delay(200);
  
  // Read voltage from ADC pin
  unsigned int reading = analogRead(PIN_ADCTEMP);  

  // Switch off sensor
  tempSensorOFF();

  // Convert reading to voltage (mV)
  float fVolt = (reading * voltageSupply) / 1024.0;
  unsigned int voltage = fVolt;

  // Fill register
  dtSensor[0] = (voltage >> 8) & 0xFF;
  dtSensor[1] = voltage & 0xFF;
  
  return 0;
}

#elif TEMPPRESS
/**
 * sensor_ReadTempPress
 *
 * Read temperature and pressure from BMP085 sensor
 *
 * Return -1 in case of error. Return 0 otherwise
 */
int sensor_ReadTempPress(void)
{
  delay(400);
  unsigned int temperature = bmp.readTemperature() * 10 + 500;
  unsigned long pressure = bmp.readPressure(); // Pa

  dtSensor[0] = (temperature >> 8) & 0xFF;
  dtSensor[1] = temperature & 0xFF;
  dtSensor[2] = (pressure >> 24) & 0xFF;
  dtSensor[3] = (pressure >> 16) & 0xFF;
  dtSensor[4] = (pressure >> 8) & 0xFF;
  dtSensor[5] = pressure & 0xFF;

  return 0;
}

#endif

/**
 * initSensor
 *
 * Initialize sensor pins
 */
void initSensor(void)
{
#ifdef TEMP
  pinMode(PIN_PWRTEMP, OUTPUT);   // Configure Power pin as output
  tempSensorOFF();
#elif TEMPHUM
  pinMode(PIN_PWRDHT, OUTPUT);    // Configure Power pin as output
  dhtSensorOFF();
#elif TEMPPRESS
  pinMode(PIN_PWRPRESS, OUTPUT);  // Configure Power pin as output
  pressSensorON();
  delay(200);
  bmp.begin();
#endif
}

