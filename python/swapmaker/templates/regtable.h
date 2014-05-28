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
${REGTABLE_INDEX}
DEFINE_REGINDEX_END()

#endif

