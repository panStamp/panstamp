#include "regtable.h"
#include "panstamp.h"

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  byte regIndex;

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  // Init panStamp
  panstamp.init();

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Transmit I/O data
  REGISTER *reg;
  int index = 11;
  while((reg = getRegister(index)) != NULL)
  {
    reg->getData();
    index++;
  }
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
}

