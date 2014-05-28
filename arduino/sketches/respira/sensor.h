/**
 * sensor.h
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
 * Creation date: 09/01/2012
 */

#ifndef _SENSOR_H
#define _SENSOR_H

/**
 * Sensor values
 */
byte dtHtSensor[4];	  // Humidity + temperature sensor
byte dtAirSensor[4];	// Air quality (CO + NO2) sensor

/**
 * Pin definitions
 */
// Temperature + Humidity (DHT22 sensor)
#define PORTW_DHT_DATA    PORTD
#define PORTR_DHT_DATA    PIND
#define PORTD_DHT_DATA    DDRD
#define BIT_DHT_DATA      6
#define PIN_DHT_DATA      6

#define setDataPin()      bitSet(PORTW_DHT_DATA, BIT_DHT_DATA)
#define clearDataPin()    bitClear(PORTW_DHT_DATA, BIT_DHT_DATA)
#define getDataPin()      bitRead(PORTR_DHT_DATA, BIT_DHT_DATA)
#define setDataInput()    bitClear(PORTD_DHT_DATA, BIT_DHT_DATA)
#define setDataOutput()   bitSet(PORTD_DHT_DATA, BIT_DHT_DATA)

#define PIN_PWRDHT        5
#define dhtSensorON()     digitalWrite(PIN_PWRDHT, HIGH);
#define dhtSensorOFF()    digitalWrite(PIN_PWRDHT, LOW);

// CO + NO2 (MiCS-4514 sensor)
#define ANALOG_NO2        3   // Arduino analog pin
#define ANALOG_CO         4   // Arduino analog pin
#define PIN_HEATING_NO2   19  // Arduino digital pin

#define preHeaterON()     digitalWrite(PIN_HEATING_NO2, HIGH);
#define preHeaterOFF()    digitalWrite(PIN_HEATING_NO2, LOW);

#define CALIB_R0_NO2      2200      // R0 calibration value for the NO2 sensor
#define CALIB_R0_CO       750000    // R0 calibration value for the CO sensor

#endif

