/**
 * regtable.ino
 *
 * List of registers. Definition and handlers.
 *
 * Product name: sfpddm (SFP DDM monitoring system)
 * Author: Luka Mustafa - Musti
 */
#include <EEPROM.h>
#include "product.h"
#include "panstamp.h"
#include "regtable.h"

/**
 * Declaration of common callback functions
 */
DECLARE_COMMON_CALLBACKS()

/**
 * Definition of common registers
 */
DEFINE_COMMON_REGISTERS()

/*
 * Definition of custom registers
 */
static byte dtinfo[1];
REGISTER reginfo(dtinfo, sizeof(dtinfo), &updtinfo, NULL);
static byte dtcontrol[2];
REGISTER regcontrol(dtcontrol, sizeof(dtcontrol), &updtcontrol, &setcontrol);
static byte dtmeasurement[10];
REGISTER regmeasurement(dtmeasurement, sizeof(dtmeasurement), &updtmeasurement, NULL);
static byte dtalarm[2];
REGISTER regalarm(dtalarm, sizeof(dtalarm), &updtalarm, NULL);
static byte dtwarning[2];
REGISTER regwarning(dtwarning, sizeof(dtwarning), &updtwarning, NULL);


/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  // Pointers to the custom registers
  &reginfo,
  &regcontrol,
  &regmeasurement,
  &regalarm,
  &regwarning
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */

 
/**
 * updtinfo
 *
 * Update info
 *
 * rId: register ID
 */
const void updtinfo(byte rId)
{
  // Update register from:
  // systemerror (Position = byte 0 bit 0 - Size = 1 bits)
  
  // Check the error code and signal error if code is not 0
  if (sfp.getStatus()){
    dtinfo[0]|=0x80;
  }
  else dtinfo[0]=0x00;
  // ddm (Position = byte 0 bit 1 - Size = 1 bits)
  // internalcalib (Position = byte 0 bit 2 - Size = 1 bits)
  // externalcalib (Position = byte 0 bit 3 - Size = 1 bits)
  // averagepower (Position = byte 0 bit 4 - Size = 1 bits)
  // get the remaining info
  byte infodata = sfp.getSupported();
  dtinfo[0]|=(infodata&0x78);  
}


/**
 * setcontrol
 *
 * Set control outputs
 *
 * 'rId'     Register ID
 * 'states'  New output levels
 */
const void setcontrol(byte rId, byte *control)
{
  byte i;
  
  // Update register
  memcpy(dtcontrol, control, sizeof(dtcontrol));
  
  // txdisablestate (Position = byte 0 bit 0 - Size = 1 bits)
  // txdisable (Position = byte 0 bit 1 - Size = 1 bits)
  // rateselectstate (Position = byte 0 bit 2 - Size = 2 bits)
  // rateselect (Position = byte 0 bit 4 - Size = 1 bits)
  // txfault (Position = byte 0 bit 5 - Size = 1 bits)
  // rxlos (Position = byte 0 bit 6 - Size = 1 bits)
  // dataready (Position = byte 0 bit 7 - Size = 1 bits)
  sfp.setControl(dtcontrol[0]&0x48);
  
  // led (Position = byte 1 bit 0 - Size = 1 bits)
  // gpio (Position = byte 1 bit 1 - Size = 1 bits)
  digitalWrite(LEDCOM , ((dtcontrol[1]>>7) & 0x01));
}

/**
 * updtcontrol
 *
 * Update control
 *
 * rId: register ID
 */
const void updtcontrol(byte rId)
{
  // Update register from:
  // txdisablestate (Position = byte 0 bit 0 - Size = 1 bits)
  // txdisable (Position = byte 0 bit 1 - Size = 1 bits)
  // rateselectstate (Position = byte 0 bit 2 - Size = 2 bits)
  // rateselect (Position = byte 0 bit 4 - Size = 1 bits)
  // txfault (Position = byte 0 bit 5 - Size = 1 bits)
  // rxlos (Position = byte 0 bit 6 - Size = 1 bits)
  // dataready (Position = byte 0 bit 7 - Size = 1 bits)
  dtcontrol[0]=sfp.getControl();
  
  // led (Position = byte 1 bit 0 - Size = 1 bits)
  // gpio (Position = byte 1 bit 1 - Size = 1 bits)
}

/**
 * updtmeasurement
 *
 * Update measurement
 *
 * rId: register ID
 */
const void updtmeasurement(byte rId)
{
  int i;
  // Update register from:
  // temp (Position = byte 0 bit 0 - Size = 16 bits)
  // voltage (Position = byte 2 bit 0 - Size = 16 bits)
  // txcurrent (Position = byte 4 bit 0 - Size = 16 bits)
  // txpower (Position = byte 6 bit 0 - Size = 16 bits)
  // rxpower (Position = byte 8 bit 0 - Size = 16 bits)
  // create variable
  
  // temperature is signed and SWAP supportes unsigned only
  // decreasing by 128 at the other end
  uint16_t temperature = sfp.getTemperature() + 0x8000 ;
  
  dtmeasurement[0]=(uint8_t)(temperature>>8);
  dtmeasurement[1]=(uint8_t)temperature;
  
  dtmeasurement[2]=(uint8_t)(sfp.getVoltage()>>8);
  dtmeasurement[3]=(uint8_t)sfp.getVoltage();
  
  dtmeasurement[4]=(uint8_t)(sfp.getTXcurrent()>>8);
  dtmeasurement[5]=(uint8_t)sfp.getTXcurrent();
  
  dtmeasurement[6]=(uint8_t)(sfp.getTXpower()>>8);
  dtmeasurement[7]=(uint8_t)sfp.getTXpower();
  
  dtmeasurement[8]=(uint8_t)(sfp.getRXpower()>>8);
  dtmeasurement[9]=(uint8_t)sfp.getRXpower();
}

/**
 * updtalarm
 *
 * Update alarm
 *
 * rId: register ID
 */
const void updtalarm(byte rId)
{
  // Update register from:
  // temp (Position = byte 0 bit 0 - Size = 2 bits)
  // vcc (Position = byte 0 bit 2 - Size = 2 bits)
  // txcurrent (Position = byte 0 bit 4 - Size = 2 bits)
  // txpower (Position = byte 0 bit 6 - Size = 2 bits)
  // rxpower (Position = byte 1 bit 0 - Size = 2 bits)
  
  dtalarm[0]=(uint8_t)(sfp.getAlarms()>>8);
  dtalarm[1]=(uint8_t)sfp.getAlarms();
}

/**
 * updtwarning
 *
 * Update warning
 *
 * rId: register ID
 */
const void updtwarning(byte rId)
{
  // Update register from:
  // temp (Position = byte 0 bit 0 - Size = 2 bits)
  // vcc (Position = byte 0 bit 2 - Size = 2 bits)
  // txcurrent (Position = byte 0 bit 4 - Size = 2 bits)
  // txpower (Position = byte 0 bit 6 - Size = 2 bits)
  // rxpower (Position = byte 1 bit 0 - Size = 2 bits)
  dtwarning[0]=(uint8_t)(sfp.getWarnings()>>8);
  dtwarning[1]=(uint8_t)sfp.getWarnings();
}



