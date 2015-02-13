#include "ws2811_hs.h"
#include "ws2811.h"

/* This function is defined in the associated .S assembly source file */
void write_ws2811_hs(uint8_t *data, uint16_t length, uint8_t pinmask);

#define LENGTH (uint16_t)16

uint8_t leds[LENGTH][COLOURS_TOTAL] = {{0}};

void RgbLedDriver_Create(void)
{
	int i, j;
	for (i = 0; i < LENGTH; i++)
		for (j = 0; j < COLOURS_TOTAL; j++)
			leds[i][j] = 0;
	write_ws2811_hs((uint8_t *)leds, LENGTH * COLOURS_TOTAL, 1);
}

void RgbLedDriver_TurnOn(int led, uint8_t r, uint8_t g, uint8_t b)
{
	leds[led - 1][RED] = r;
	leds[led - 1][GREEN] = g;
	leds[led - 1][BLUE] = b;

	write_ws2811_hs((uint8_t *)leds, LENGTH * COLOURS_TOTAL, 1);
}

uint8_t RgbLedDriver_GetLedColour(int led, int colour)
{
	return leds[led - 1][colour];
}

void write_ws2811_hs(uint8_t *data, uint16_t length, uint8_t pinmask)
{
  asm volatile(
        .section .text.write_ws2811_hs
        .p2align 1,0                    ; align on a word boundary
        .type write_ws2811_hs,@function
write_ws2811_hs:
        push    R11                     ; Save R11
byte_loop_hs:                           ;
        mov     #7, R11                 ; Do 7 bits in a loop
        mov.b   @R15+, R12              ; Get next byte from buffer
bit_loop_hs:                            ; - Bit loop - 20 cycles per bit
        rla.b   R12                     ; Get next bit
        jc      one_hs                  ; Jump if one...
        bis.b   R13, &WS2811_PORTOUT    ; Output high
        bic.b   R13, &WS2811_PORTOUT    ; Output low - 4 cycles elapsed
        bic.b   R13, &WS2811_PORTOUT    ; 4 cycle nop
        jmp     next_bit_hs             ; Next bit...
one_hs:                                 ;
        bis.b   R13, &WS2811_PORTOUT    ; Output high
        bis.b   R13, &WS2811_PORTOUT    ; 4 cycle nop
        jmp     $ + 2                   ; 2 cycle nop
        bic.b   R13, &WS2811_PORTOUT    ; Output low - 10 cycles elapsed
next_bit_hs:                            ;
        dec     R11                     ; Decrement bit count
        jne     bit_loop_hs             ; Do next bit of not zero...
                                        ;
        rla.b   R12                     ; Get final bit of byte
        jc      last_one_hs             ; Jump if one...
        bis.b   R13, &WS2811_PORTOUT    ; Output high
        bic.b   R13, &WS2811_PORTOUT    ; Output low - 4 cycles elapsed
        jmp     $ + 2                   ; 2 cycle nop
        dec     R14                     ; Decrement byte count
        jne     byte_loop_hs            ; Next byte if count not zero...
        jmp     reset_hs                ; All bytes done, reset...
last_one_hs:                            ;
        bis.b   R13, &WS2811_PORTOUT    ; Output high
        jmp     $ + 2                   ; 2 cycle nop
        mov     #7, R11                 ; Reset bit counter
        mov.b   @R15+, R12              ; Get next byte from buffer
        bic.b   R13, &WS2811_PORTOUT    ; Output low - 10 cycles elapsed
        dec     R14                     ; Decrement byte count
        jne     bit_loop_hs             ; Do next byte if count is not zero...
                                        ;
reset_hs:                               ;
        mov     #800 / 3, R12           ; 800 cycle delay for reset
        dec     R12                     ;
        jne     $ - 2                   ;
        pop     R11                     ; Restore R11
        ret                             ; Return
  );
}
