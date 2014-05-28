/*
 * modem.pde
 *
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
 * Creation date: 15/02/2011
 *
 * Device:
 * Serial gateway or modem
 *
 * Description:
 * This is not a proper SWAP gateway but a simple transparent UART-RF
 * interface. This device can be used from a serial console as hyperterminal
 * or minicom. Wireless packets are passed to/from the host computer in ASCII
 * format whilst configuration is done via AT commands.
 *
 * Visit our wiki for details about the protocol in case you want to develop
 * your own PC library for this device.
 */

#include "cc1101.h"
#include "TimerOne.h"
#include "EEPROM.h"
#include "modem.h"
#include "calibration.h"

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * isrINT0event
 *
 * Event on GDO0 pin (INT0)
 */
void isrINT0event(void)
{
  packetAvailable = true;
}

/**
 * isrT1event
 *
 * Timer1 interrupt routine
 */
void isrT1event(void)
{
  if (t1Ticks == MAX_SERIAL_SILENCE_TK)
  {
    // Detach Timer1 interrupt
    Timer1.detachInterrupt();
    Timer1.stop();
    resetTimer();
    // Pending "+++" command?
    if (!strcmp(strSerial, AT_GOTO_CMDMODE))
    {
      disableINT0irq();  // Disable wireless reception interrupt
      Serial.println("OK-Command mode");
      serMode = SERMODE_COMMAND;
    }
    memset(strSerial, 0, sizeof(strSerial));
    len = 0;
    Timer1.resume();
  }
  else
    t1Ticks++;
}

/**
 * handleSerialCmd
 *
 * Handle serial command received
 *
 * 'command'  AT command received
 */
void handleSerialCmd(char* command)
{
  byte i, len;
  byte arrV[2];
  CCPACKET packet;
  ATQUERY atQuery = ATQUERY_REQUEST;  
 
  // Data mode?
  if (serMode == SERMODE_DATA)
  {
    packet.length = strlen(command)/2;
    
    if (packet.length > 0)
    {
      // Convert ASCII string into array of bytes
      for(i=0 ; i<packet.length ; i++)
      {     
        packet.data[i] = charToHex(command[i*2]) << 4;
        packet.data[i] |= charToHex(command[i*2 + 1]);
      }
      // Send packet via RF
      cc1101.sendData(packet);
    }
  }
  // Command mode?
  else  // serMode = SERMODE_COMMAND
  {
    len = strlen(command);
    
    if (len < 5)
    {
      // Basic attention command
      if (!strcmp(strSerial, AT_ATTENTION))
        Serial.println("OK");
      // Reset modem
      else if (!strcmp(strSerial, AT_RESET))
      {
        Serial.println("OK");
        swReset();
      }
      // Go to serial data mode
      else if (!strcmp(strSerial, AT_GOTO_DATAMODE))
      {
        serMode = SERMODE_DATA;
        Serial.println("OK-Data mode");
        enableINT0irq();  // Enable wireless reception interrupt
      }
    }
    // Set new value
    else 
    {
      if ((strSerial[4] == '=') && (len >= 6))
      {
        // Get new value
        i = (charToHex(strSerial[5]) << 4) & 0xF0;
        i |= charToHex(strSerial[6]) & 0x0F;
        atQuery = ATQUERY_COMMAND;
      }
      // Hardware version
      if (!strncmp(strSerial, AT_HVERSION, 4))
      {
        if (atQuery == ATQUERY_REQUEST)
          Serial.println(HARDWARE_VERSION, HEX);
      }
      // Firmware version
      else if (!strncmp(strSerial, AT_FVERSION, 4))
      {
        if (atQuery == ATQUERY_REQUEST)
          Serial.println(FIRMWARE_VERSION, HEX);
      }
      // Frequency channel
      else if (!strncmp(strSerial, AT_FREQCHANNEL, 4))
      {
        if (atQuery == ATQUERY_COMMAND)
        {
          cc1101.setChannel(i, true);
          Serial.println("OK");
        }
        else
          Serial.println(cc1101.channel, HEX);
      }
      // Synchronization word
      else if (!strncmp(strSerial, AT_SYNCWORD, 4))
      {
        if (atQuery == ATQUERY_COMMAND)
        {
          if ((len-5) == 4)
          {
            arrV[0] = charToHex(strSerial[5]) << 4;
            arrV[0] |= charToHex(strSerial[6]);
            arrV[1] = charToHex(strSerial[7]) << 4;
            arrV[1] |= charToHex(strSerial[8]);
            cc1101.setSyncWord(arrV, true);
            Serial.println("OK");
          }
          else
            Serial.println("ERROR");
        }
        else
        {
          Serial.println((unsigned int)cc1101.syncWord[0] << 8 | cc1101.syncWord[1], HEX);
        }
      }
      // Device address
      else if (!strncmp(strSerial, AT_DEVADDRESS, 4))
      {
        if (atQuery == ATQUERY_COMMAND)
        {
          cc1101.setDevAddress(i, true);
          Serial.println("OK");
        }
        else
          Serial.println(cc1101.devAddress, HEX);
      }
      // Address check
      else if (!strncmp(strSerial, AT_ADDRCHECK, 4))
      {
        if (atQuery == ATQUERY_COMMAND)
        {
          if (i == 0)
          {
            cc1101.disableAddressCheck();
            Serial.println("OK");
          }
          else if (i == 1)
          {
            cc1101.enableAddressCheck();
            Serial.println("OK");
          }
          else
            Serial.println("ERROR");
        }
        else
          Serial.println("ERROR");
      }
      else
        Serial.println("ERROR");
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
  digitalWrite(LEDPIN, HIGH);
 
  // Calibrate internal RC oscillator
  rcOscCalibrate();
  
  // Reset serial buffer
  memset(strSerial, 0, sizeof(strSerial));

  Serial.begin(38400);
  Serial.flush();
  Serial.println("");
  
  // Default mode is COMMAND 
  Serial.println("Modem ready!");
  // Setup CC1101
  cc1101.init();

  // Disable address check from the CC1101 IC
  cc1101.disableAddressCheck();

  delay(100);  

  // Enter RX state
  cc1101.setRxState();

  // Attach callback function for GDO0 (INT0)
  enableINT0irq();
  
  // Init Timer1
  Timer1.initialize(TIMER1_TICK_PERIOD_US);
  // Attach interrupt function to Timer1
  //Timer1.attachInterrupt(isrT1event);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  // Read wireless packet?
  if (packetAvailable)
  {
    // Disable wireless reception interrupt
    disableINT0irq();

    byte i;
    CCPACKET packet;
   
    packetAvailable = false;

    if (cc1101.receiveData(&packet) > 0)
    {
      if (packet.crc_ok && packet.length > 5 && serMode == SERMODE_DATA)
      {
        Serial.print("(");
        if (packet.rssi < 0x10)
          Serial.print("0");
        Serial.print(packet.rssi, HEX);
        if (packet.lqi < 0x10)
          Serial.print("0");
        Serial.print(packet.lqi, HEX);
        Serial.print(")");
        for(i=0 ; i<packet.length ; i++)
        {
          if (packet.data[i] < 0x10)
            Serial.print(0, HEX);    // Leading zero
          Serial.print(packet.data[i], HEX);
        }
        Serial.println("");
      }
    }

    // Enable wireless reception interrupt
    enableINT0irq(); 
  }

  // Read serial command
  if (Serial.available() > 0)
  {
    // Disable wireless reception interrupt
    disableINT0irq();

    ch = Serial.read();

    if (len >= SERIAL_BUF_LEN-1)
    {
      memset(strSerial, 0, sizeof(strSerial));
      len = 0;
    }
    else if (ch == 0x0D) 
    {
      Timer1.detachInterrupt();
      strSerial[len] = 0;
      handleSerialCmd(strSerial);
      memset(strSerial, 0, sizeof(strSerial));
      len = 0;
    }
    else
    {
      strSerial[len] = ch; 
      len++;
      resetTimer();
      // Attach interrupt function to Timer1
      Timer1.attachInterrupt(isrT1event);
    }

    // Enable wireless reception interrupt
    enableINT0irq();
  }
}

/**
 * charToHex
 *
 * 'ch'    Character to be converted to hexadecimal
 *
 * Returns:
 *  Hex value
 */
byte charToHex(byte ch)
{
  byte val;
  
  if (ch >= 'A' && ch <= 'F')
    val = ch - 55;
  else if (ch >= 'a' && ch <= 'f')
    val = ch - 87;
  else if (ch >= '0' && ch <= '9')
    val = ch - 48;
  else
    val = 0x00;

  return val;  
}

/**
 * swReset
 * 
 * Software reset
 */
void swReset(void) 
{
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {}
}

