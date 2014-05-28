/**
 * regtable.h
 *
 * List of registers. Header file.
 *
 * Product name: pwrmeter (Energy monitor)
 * Author: panStamp
 * Creation date: 25 Nov 2012
 */
#ifndef _REGTABLE_H
#define _REGTABLE_H

#include "Arduino.h"
#include "register.h"
#include "commonregs.h"

/**
 * Register indexes
 */
DEFINE_REGINDEX_START()
  // First index here = 11
  REGI_MYREG
DEFINE_REGINDEX_END()

#endif

