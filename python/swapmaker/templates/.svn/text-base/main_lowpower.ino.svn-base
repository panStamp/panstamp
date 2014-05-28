#include "regtable.h"
#include "panstamp.h"

/**
 * LED pin
 */
#define LEDPIN               4

/**
 * Global variables
 */
int regIndex = 11;
REGISTER *reg;

/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  byte i;

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  // Init panStamp
  panstamp.init();

  // Transmit product code
  getRegister(REGI_PRODUCTCODE)->getData();

  // Enter SYNC state
  panstamp.enterSystemState(SYSTATE_SYNC);

  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
    delay(400);
  }
  // Transmit periodic Tx interval
  getRegister(REGI_TXINTERVAL)->getData();
  // Transmit power voltage
  getRegister(REGI_VOLTSUPPLY)->getData();
  // Switch to Rx OFF state
  panstamp.enterSystemState(SYSTATE_RXOFF);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  // Transmit sensor data
  while((reg = getRegister(regIndex)) != NULL)
  {
    reg->getData();
    regIndex++;
  }

  // Sleep
  panstamp.goToSleep();
}

