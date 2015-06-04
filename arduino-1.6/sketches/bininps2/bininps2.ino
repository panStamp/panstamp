/*
 * bininps2
 *
 * Copyright (c) 2015 panStamp <contact@panstamp.com>
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
 * Creation date: 04/01/2015
 *
 * Device:
 * Binary input + counter module
 *
 * Description:
 * Device that reports the binary state of 12 digital inputs, 4 of them being
 * used as pulse counters as well.
 *
 * PANSTAMP AVR
 * This sketch is not supported by panStamp AVR.
 * panStamp AVR does not support attachInterrupt on pins other than INT1 (D3).
 * RTC calendar mode is not supported by AVR modules either.
 * With panStamp AVR use the classic "bininps" sketch or "pulsecounter"
 *
 * PANSTAMP NRG
 * Binary inputs only: D0, D1, D2, D3, D4, D5, D6 and D7
 * Binary/counter inputs: D8, D9, D10 and D11
 * This sketch uses internal pull-up's. Counters increment when inputs turn down
 * (LOW state)
 *
 * This sketch can be used to detect key/switch presses, binary alarms or
 * any other binary sensor. It can also be used to count pulses from a vaste
 * variety of devices such as energy meters, water meters, gas meters, etc.
 * You may want to add some delays in updateValues in order to better debounce
 * the binary states. We suggest to add external capacitors to the inputs as well.
 * Capacitor values and delays should depend on the nature and frequence of the
 * input signals.
 *
 * This device is low-power enabled so it will enter low-power mode just
 * after reading the binary states and transmitting them over the SWAP
 * network.
 *
 * Associated Device Definition File, defining registers, endpoints and
 * configuration parameters:
 * bininps.xml (Binary/Counter input module)
 */
 
#include "swap.h"
#include "regtable.h"

/**
 * Register data containers made global
 */
byte dtBinInputs[2];    // Binary input states
byte dtCounters[16];    // Pulse counters

// RTC data
RTCDATA rtcData;

/**
 * Input Change Interrupts
 */
// D0
void d0irq(void)
{
  dtBinInputs[0] = digitalRead(0);         // Update register
}
// D1
void d1irq(void)
{
  dtBinInputs[0] = digitalRead(1) << 1;    // Update register
}
// D2
void d2irq(void)
{
  dtBinInputs[0] = digitalRead(2)  << 2;   // Update register
}
// D3
void d3irq(void)
{
  dtBinInputs[0] = digitalRead(3) << 3;    // Update register
}
// D4
void d4irq(void)
{
  dtBinInputs[0] = digitalRead(4) << 4;    // Update register
}
// D5
void d5irq(void)
{
  dtBinInputs[0] = digitalRead(5) << 5;    // Update register
}
// D6
void d6irq(void)
{
  dtBinInputs[0] = digitalRead(6) << 6;    // Update register
}
// D7
void d7irq(void)
{
  dtBinInputs[0] |= digitalRead(7) << 7;   // Update register
}
// D8
void d8irq(void)
{
  dtBinInputs[1] = digitalRead(8);         // Update register
  if (!(dtBinInputs[1] & 0x01))
    dtCounters[0]++;                       // Increment counter if pin is low
}
// D9
void d9irq(void)
{
  dtBinInputs[1] = digitalRead(9) << 1;    // Update register
  if (!(dtBinInputs[1] & 0x02))
    dtCounters[1]++;                       // Increment counter if pin is low
}
// D10
void d10irq(void)
{
  dtBinInputs[1] = digitalRead(10) << 2;   // Update register
  if (!(dtBinInputs[1] & 0x04))
    dtCounters[2]++;                       // Increment counter if pin is low
}
// D11
void d11irq(void)
{
  dtBinInputs[1] = digitalRead(11) << 3;   // Update register
  if (!(dtBinInputs[1] & 0x08))
    dtCounters[3]++;                       // Increment counter if pin is low
}

/**
 * configPorts
 *
 * Configure binary ports to generate interrupts
 */
void configPorts(void)
{
  // Configure digital pins as inputs with internal pull-up's
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  
  // Attach pins to custom IRQ's
  attachInterrupt(0, d0irq, CHANGE);
  attachInterrupt(1, d1irq, CHANGE);
  attachInterrupt(2, d2irq, CHANGE);
  attachInterrupt(3, d3irq, CHANGE);
  attachInterrupt(4, d4irq, CHANGE);
  attachInterrupt(5, d5irq, CHANGE);
  attachInterrupt(6, d6irq, CHANGE);
  attachInterrupt(7, d7irq, CHANGE);
  attachInterrupt(8, d8irq, CHANGE);
  attachInterrupt(9, d9irq, CHANGE);
  attachInterrupt(10, d10irq, CHANGE);
  attachInterrupt(11, d11irq, CHANGE);
}

/**
 * updateValues
 *
 * Update binary state registers and counters
 */
void updateValues(void)
{
  int i;
  
  for (i=0 ; i<8 ; i++)
    dtBinInputs[0] |= digitalRead(i) << i;
    
  for (i=0 ; i<4 ; i++)
    dtBinInputs[1] |= digitalRead(i+8) << i;
}

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  int i;

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Configure ports
  configPorts();

  // Init SWAP stack
  swap.init();
  
  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  swap.enterSystemState(SYSTATE_SYNC);

  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(400);
  }
  // Transmit periodic Tx interval
  swap.getRegister(REGI_TXINTERVAL)->getData();
  // Transmit power voltage
  swap.getRegister(REGI_VOLTSUPPLY)->getData();
  
  updateValues();
  // Transmit initial binary states
  swap.getRegister(REGI_BININPUTS)->getData();
  // Transmit initial counter values
  swap.getRegister(REGI_COUNTERS)->getData();
  
  // Switch to Rx OFF state
  swap.enterSystemState(SYSTATE_RXOFF);

  // Set current time : Thursday 8-Apr 2015 12:15:00 (PM)
  rtcData.year = 2015;
  rtcData.mon = 4;
  rtcData.day = 9;
  rtcData.wday = 4;
  rtcData.hour = 13;
  rtcData.min = 48;
  rtcData.sec = 0;
  
  panstamp.rtc.startCalendar(&rtcData);
  
  //panstamp.rtc.setAlarmHour(15);
  panstamp.rtc.setAlarmMinutes(50);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  // Sleep until next (RTC) calendar alarm or pin interrupt
  panstamp.sleep();

  // Transmit binary states
  swap.getRegister(REGI_BININPUTS)->getData();  
  // Transmit counter values
  swap.getRegister(REGI_COUNTERS)->getData();
}

