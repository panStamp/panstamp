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
 * Creation date: 08/06/2014
 */
 
/**
 * Basic sketch usinglow-power modes and interrupts witha MMA8652FC 3-Axis
 * accelerometer.
 */
 
#include "HardwareSerial.h"
#include "Wire.h"
#include "mma8652.h"

// Accelerometer object. Interruptable pin = internal ACC_INT pin
MMA8652 accel = MMA8652(ACC_INT);

// Used to read statuses and source registers
uint8_t status, intSource;

/**
 * accEvent
 *
 * Custom ISR. Will be called whenever the accelerometer generates an interupt
 */
void accEvent(void)
{
  panstamp.wakeUp();
}

/**
 * setup
 */
void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");

  pinMode(LED, OUTPUT);

  // Turn ON accelerometer
  digitalWrite(ACC_POWER_PIN, HIGH);
  delay(100);
  
  // Init accelerometer
  accel.init();
  delay(1);
  
  // Enable single-tap detection interruption
  accel.enableTapInt(0x10);
  delay(1);
  // Enable Portrait/Landscape orientation interrupt
  //accel.enablePlInt();
  delay(1);
  // Enable Free-Fall detection interrupt
  accel.enableFreeFallInt(0x30);
  delay(1);

  // Declare custom ISR
  accel.attachInterrupt(accEvent);
}

/**
 * loop
 */
void loop()
{ 
  // Go to sleep
  digitalWrite(LED, LOW);
  accel.sleep();    // Accelerometer in sleep mode
  panstamp.sleep(); // panStamp in sleep mode
  digitalWrite(LED, HIGH);
    
  /**
   * We could be polling for an ACC event but we prefer to be interrupted instead
   * If you want to display all the above Serial.prints correctly then comment
   * panstamp.sleep() out and uncomment the following line
   */
   //if (accel.eventAvailable()) {}
  
  // Read source of interrupt
  intSource = accel.readIntSource();
  
  // Read XYZ data. Available from accel.axis
  accel.readXYZ();

  /*
  Serial.print("X axis : ");
  Serial.println(accel.axis.x);
  Serial.print("Y axis : ");
  Serial.println(accel.axis.y);
  Serial.print("Z axis : ");
  Serial.println(accel.axis.z);
  */
  
  // Portrait/Landscape orientation interrupt?
  if(intSource & SRC_LNDPRT_MASK)
  {
    status = accel.readPlStatus();
    Serial.print("PL status : ");
    Serial.println(status, HEX);
  }
  // Pulse detection interrupt?
  if(intSource & SRC_PULSE_MASK)
  {
    status = accel.readPulseSource();
    Serial.print("TAP status : ");
    Serial.println(status, HEX);
  }
  //
  if(intSource & SRC_FF_MT_MASK)
  {
    status = accel.readFreeFallSource();
    Serial.print("FFMT status : ");
    Serial.println(status, HEX);
  }
  if(intSource & SRC_ASLP_MASK)
  {
    Serial.println("Auto-WAKE event");
  }
  else
  {
    // Still doing something until the interrupt pin goes high again
  }
}

