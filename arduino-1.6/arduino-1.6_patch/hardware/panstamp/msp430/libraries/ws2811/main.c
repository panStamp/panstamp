//#include <msp430g2553.h>   /* Genericizable */
#include <msp430.h>
#include "RgbLedDriver.h"
#include "RotaryEncoder.h"

void initializeEncoderRegisters(void)
{
	_BIS_SR(OSCOFF);          // XTAL not used
	P2DIR &= ~(BIT7|BIT6);
	P2SEL &= ~(BIT7|BIT6);
	P2SEL2 &= ~(BIT7|BIT6);
	P2REN |= BIT7|BIT6;
}

void initializeLedRegisters(void)
{
	P1DIR = BIT0;
}

void initializeClock(void)
{
	if (CALBC1_16MHZ ==0xFF || CALDCO_16MHZ == 0xFF)
	{
		/* If calibration constants erased
		 * do not load, trap CPU!!
		 */
		while(1);
	}
	BCSCTL1 = CALBC1_16MHZ;  // Set DCO to 16MHz
	DCOCTL = CALDCO_16MHZ;
}

static int i = 0;

void loop(void)
{
	RgbLedDriver_TurnOn((1  - i + 16) % 16 + 1, 5, 1, 1);
	RgbLedDriver_TurnOn((2  - i + 16) % 16 + 1, 1, 5, 1);
	RgbLedDriver_TurnOn((3  - i + 16) % 16 + 1, 1, 1, 5);
	RgbLedDriver_TurnOn((4  - i + 16) % 16 + 1, 5, 5, 5);
	RgbLedDriver_TurnOn((5  - i + 16) % 16 + 1, 5, 1, 1);
	RgbLedDriver_TurnOn((6  - i + 16) % 16 + 1, 1, 5, 1);
	RgbLedDriver_TurnOn((7  - i + 16) % 16 + 1, 1, 1, 5);
	RgbLedDriver_TurnOn((8  - i + 16) % 16 + 1, 5, 5, 5);
	RgbLedDriver_TurnOn((9  - i + 16) % 16 + 1, 5, 1, 1);
	RgbLedDriver_TurnOn((10 - i + 16) % 16 + 1, 1, 5, 1);
	RgbLedDriver_TurnOn((11 - i + 16) % 16 + 1, 1, 1, 5);
	RgbLedDriver_TurnOn((12 - i + 16) % 16 + 1, 5, 5, 5);
	RgbLedDriver_TurnOn((13 - i + 16) % 16 + 1, 5, 1, 1);
	RgbLedDriver_TurnOn((14 - i + 16) % 16 + 1, 1, 5, 1);
	RgbLedDriver_TurnOn((15 - i + 16) % 16 + 1, 1, 1, 5);
	RgbLedDriver_TurnOn((16 - i + 16) % 16 + 1, 5, 5, 5);
	RotaryEncoder_Read();
	i = RotaryEncoder_GetRotation();
}

int main(void)
{
	/* Hold the watchdog timer so it doesn't reset our chip */
	WDTCTL = WDTPW + WDTHOLD;
	initializeEncoderRegisters();
	initializeLedRegisters();
	initializeClock();

	RgbLedDriver_Create();
	RotaryEncoder_Create((uint8_t *)(&P2IN), 6, 7);

	/* infinite loop */
	for( ; ; ) {
		loop();
	}
}
