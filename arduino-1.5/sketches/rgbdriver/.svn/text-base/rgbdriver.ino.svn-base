/**
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
 * Creation date: 06/22/2011
 *
 * Device:
 * RGB LED driver
 *
 * Description:
 * This device controls a RGB LED or a RGB LED strip using three PWM
 * outputs. RGB levels can be controlled via the RGBlevel register
 * (see Device Definition File for details). At the same time, this device
 * listens for the first Chronos watch appearing in the network and
 * synchronizes the RGB levels with the Chronos XYZ acceleration samples.
 *
 * Associated Device Definition File: rgbdriver.xml
 */

#include "panstamp.h"
#include "regtable.h"
#include "rgbled.h"

#define CHRONOS_PRODUCT_CODE  {0,0,0,1,0,0,0,2}
#define CHRONOS_REGID_ACC     14

// Global RGBLED object. Define your own PWM outputs here:
RGBLED rgbLed = RGBLED(9, 6, 5);

// Address of the associated Chronos watch
byte chronosDevAddr = 0;

/**
 * swapStatusReceived
 *
 * Function automatically called by the panStamp API whenever a SWAP status
 * packet is received
 *
 * 'status'    SWAP status packet received
 */
 void swapStatusReceived(SWPACKET *status)
 {
   byte chronos_pCode[] = CHRONOS_PRODUCT_CODE;
   
   // Still without an associated Chronos watch?
   if (chronosDevAddr == 0)
   {
     // Product code received?
     if (status->regId == REGI_PRODUCTCODE)
     {
       // Is this a Chronos watch?
       if (!memcmp(status->value.data, chronos_pCode, sizeof(chronos_pCode)))
         chronosDevAddr = status->srcAddr;    // Save its device address
     }
   }
   // We already have an associated Chronos watch
   else
   {
     // Acceleration data received?
     if (status->regId == CHRONOS_REGID_ACC)
     {
       // Update RGB levels
       getRegister(REGI_RGBLEVELS)->setData(status->value.data);
     }
   }
 }

/**
 * setup
 *
 * One-time setup function
 */
void setup() 	 
{
  // Init panStamp
  panstamp.init();

  // Declare callback function for dispatching the incoming SWAP status packets
  panstamp.setSwapStatusCallBack(swapStatusReceived);
  
  // Broadcast product status
  getRegister(REGI_PRODUCTCODE)->getData();
}

/**
 * loop
 *
 * Main loop
 */
void loop()
{
  delay(100);
}
