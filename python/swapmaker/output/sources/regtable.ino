/**
 * regtable.ino
 *
 * List of registers. Definition and handlers.
 *
 * Product name: pwrmeter (Energy monitor)
 * Author: panStamp
 * Creation date: 25 Nov 2012
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
static byte dtmyreg[3];
REGISTER regmyreg(dtmyreg, sizeof(dtmyreg), &updtmyreg, NULL);


/**
 * Initialize table of registers
 */
DECLARE_REGISTERS_START()
  // Pointers to the custom registers
  &regmyreg
DECLARE_REGISTERS_END()

/**
 * Definition of common getter/setter callback functions
 */
DEFINE_COMMON_CALLBACKS()

/**
 * Definition of custom getter/setter callback functions
 */
/**
 * updtmyreg
 *
 * Update myreg
 *
 * rId: register ID
 */
const void updtmyreg(byte rId)
{
  // Update register from:
  // param0 (Position = byte 0 bit 0 - Size = 4 bits)
  // param1 (Position = byte 0 bit 4 - Size = 16 bits)
}



