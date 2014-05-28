/**
 * regtable.h
 *
 * List of registers. Header file.
 *
 * Product name: sfpddm (SFP DDM monitoring system)
 * Author: Luka Mustafa - Musti
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
  REGI_INFO,
  REGI_CONTROL,
  REGI_MEASUREMENT,
  REGI_ALARM,
  REGI_WARNING
DEFINE_REGINDEX_END()

#endif

