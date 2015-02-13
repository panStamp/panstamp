#ifndef RGBLEDDRIVER_H_PCE9AR5O
#define RGBLEDDRIVER_H_PCE9AR5O

#include <stdint.h>

enum {
	RED = 1,
	GREEN = 0,
	BLUE = 2,

	COLOURS_TOTAL
};

void RgbLedDriver_Create(void);
void RgbLedDriver_TurnOn(int led, uint8_t r, uint8_t g, uint8_t b);
uint8_t RgbLedDriver_GetLedColour(int led, int colour);

#endif /* end of include guard: RGBLEDDRIVER_H_PCE9AR5O */
