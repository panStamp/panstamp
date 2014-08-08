#include "swap.h"

/**
 * Hardware version
 */
#define HARDWARE_VERSION        0x00000100L

/**
 * Firmware version
 */
#define FIRMWARE_VERSION        0x00000100L

/**
 * Manufacturer SWAP ID
 */
#define SWAP_MANUFACT_ID        0x00000001L

/**
 * Product SWAP ID
 */
#define SWAP_PRODUCT_ID         0x0000000EL

DECLARE_COMMON_CALLBACKS()
DEFINE_COMMON_REGISTERS()
DECLARE_REGISTERS_START()
DECLARE_REGISTERS_END()
DEFINE_COMMON_CALLBACKS()

void setup()
{
}

void loop() {
  // put your main code here, to run repeatedly:

}
