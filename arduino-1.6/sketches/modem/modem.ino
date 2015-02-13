/*
 * modem.pde
 *
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
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

#include "modem.h"
#ifdef PANSTAMP_NRG
#include "timer1a0.h"
#define TIMER         timer1a0
#define RESET_TIMER()
#define INIT_TIMER()  TIMER.attachInterrupt(isrT1event)
#define START_TIMER() TIMER.start(1000)
#define STOP_TIMER()  TIMER.stop()
#elif PANSTAMP_AVR
#include <avr/wdt.h>
#include "TimerOne.h"
byte t1Ticks = 0;     // Timer 1 ticks
#define TIMER         Timer1
#define RESET_TIMER() t1Ticks = 0
#define INIT_TIMER()  TIMER.initialize(TIMER1_TICK_PERIOD_US); TIMER.attachInterrupt(isrT1event)
#define START_TIMER() RESET_TIMER(); TIMER.attachInterrupt(isrT1event)
#define STOP_TIMER()  TIMER.detachInterrupt()
#endif

/**
 * LED pin
 */
#define LEDPIN  4

byte charToHex(byte ch);

/**
 * This function is called whenever a wireless packet is received
 */
void rfPacketReceived(CCPACKET *packet)
{
  if (packet->length > 5)
  {
    rxPacket = packet;
    packetAvailable = true;
  }
}

/**
 * isrT1event
 *
 * Timer1 interrupt routine
 */
void isrT1event(void)
{
  #ifdef PANSTAMP_AVR
  if (t1Ticks == MAX_SERIAL_SILENCE_TK)
  {
  #endif
    // Detach Timer1 interrupt
    STOP_TIMER();
    RESET_TIMER();
    // Pending "+++" command?
    if (!strcmp(strSerial, AT_GOTO_CMDMODE))
    {
      panstamp.rxOff();  // Disable wireless reception
      Serial.println("OK-Command mode");
      serMode = SERMODE_COMMAND;
    }
    memset(strSerial, 0, sizeof(strSerial));
    len = 0;
  #ifdef PANSTAMP_AVR
  }
  else
    t1Ticks++;
  #endif
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
      panstamp.radio.sendData(packet);
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
        panstamp.reset();
      }
      // Go to serial data mode
      else if (!strcmp(strSerial, AT_GOTO_DATAMODE))
      {
        serMode = SERMODE_DATA;
        Serial.println("OK-Data mode");
        panstamp.rxOn();  // Enable wireless reception
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
          panstamp.radio.setChannel(i);
          Serial.println("OK");
        }
        else
          Serial.println(panstamp.radio.channel, HEX);
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
            panstamp.radio.setSyncWord(arrV);
            Serial.println("OK");
          }
          else
            Serial.println("ERROR");
        }
        else
        {
          Serial.println((unsigned int)panstamp.radio.syncWord[0] << 8 | panstamp.radio.syncWord[1], HEX);
        }
      }
      // Device address
      else if (!strncmp(strSerial, AT_DEVADDRESS, 4))
      {
        if (atQuery == ATQUERY_COMMAND)
        {
          panstamp.radio.setDevAddress(i);
          Serial.println("OK");
        }
        else
          Serial.println(panstamp.radio.devAddress, HEX);
      }
      // Address check
      else if (!strncmp(strSerial, AT_ADDRCHECK, 4))
      {
        if (atQuery == ATQUERY_COMMAND)
        {
          if (i == 0)
          {
            panstamp.radio.disableAddressCheck();
            Serial.println("OK");
          }
          else if (i == 1)
          {
            panstamp.radio.enableAddressCheck();
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
  
  // Reset serial buffer
  memset(strSerial, 0, sizeof(strSerial));

  Serial.begin(38400);
  //Serial.begin(57600);
  Serial.flush();
  Serial.println("");
  
  // Default mode is COMMAND 
  Serial.println("Modem ready!");

  // Disable address check from the RF IC
  panstamp.radio.disableAddressCheck();

  // Declare RF callback function
  panstamp.attachInterrupt(rfPacketReceived);
  
  // Initialize Timer object
  INIT_TIMER();

  digitalWrite(LEDPIN, LOW);
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
    digitalWrite(LEDPIN, HIGH);
    // Disable wireless reception
    panstamp.rxOff();

    byte i; 
    packetAvailable = false;

    if (serMode == SERMODE_DATA)
    {
      Serial.print("(");
      if (rxPacket->rssi < 0x10)
        Serial.print("0");
      Serial.print(rxPacket->rssi, HEX);
      if (rxPacket->lqi < 0x10)
        Serial.print("0");
      Serial.print(rxPacket->lqi, HEX);
      Serial.print(")");
      for(i=0 ; i<rxPacket->length ; i++)
      {
        if (rxPacket->data[i] < 0x10)
          Serial.print(0, HEX);    // Leading zero
        Serial.print(rxPacket->data[i], HEX);
      }
      Serial.println("");
    }

    // Enable wireless reception
    panstamp.rxOn();
    digitalWrite(LEDPIN, LOW);
  }

  // Read serial command
  if (Serial.available() > 0)
  {
    // Disable wireless reception
    panstamp.rxOff();

    ch = Serial.read();

    if (len >= SERIAL_BUF_LEN-1)
    {
      memset(strSerial, 0, sizeof(strSerial));
      len = 0;
    }
    else if (ch == 0x0D) 
    {
      STOP_TIMER();
      strSerial[len] = 0;
      handleSerialCmd(strSerial);
      memset(strSerial, 0, sizeof(strSerial));
      len = 0;
    }
    else
    {
      strSerial[len] = ch; 
      len++;
      START_TIMER();
    }

    // Enable wireless reception
    panstamp.rxOn();
  }
}

/**
 * charToHex
 *
 * 'ch' Character to be converted to hexadecimal
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

