/*
 * swserial430.h - cycle counting based RS232 Serial template for msp430
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-04-2013
 * Version: 1.0.4
 *
 * Acknowledgments:
 *  Inspiration for cycle counting RX/TX routines from Kevin Timmerman.
 *  see: http://forum.43oh.com/topic/1284-software-async-serial-txrx-without-timer/
 *  Serial API inspired by Arduino Serial.
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __GNUC__
#error msp430-gcc is required to compile this code as it using inline msp430 asm
#endif

#ifndef SWSERIAL_430_H_
#define SWSERIAL_430_H_

/*
 * serial_base_sw_t - blocking bit bang cycle counting UART
 *
 */
template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct serial_base_sw_t {

// private
  template <uint16_t DUR, uint16_t DUR_HALF>
  struct BITRATE {
      enum {
          dur=DUR,
          dur_half=DUR_HALF
      };
  };

  enum { LOOP1_OVERHEAD=16, LOOP2_OVERHEAD=32 }; /* see asm code */
  static const BITRATE<((MCLK_HZ/BAUD)-LOOP1_OVERHEAD) << 1,(MCLK_HZ/BAUD)-LOOP2_OVERHEAD> bit_rate;

  int _read();                    /* asm read routine */
  void _write(const unsigned c);  /* asm write routine */

// public

  /**
   * begin() - initialize TX/RX pins
   */
  void begin(const uint32_t baud=BAUD) {
      TXPIN::high();          // TX pin idles with a high value
      TXPIN::setmode_output();
      RXPIN::setmode_input();

      (void)baud; /* use template to set baud rate */
  }

  void flush() {
    __asm__ volatile("nop");
  }

  /*
   * end() - set TX pin back to default
   */
  void end() {
      // no need to flush, we do all the work
      TXPIN::low();
      TXPIN::setmode_input();
  }

  /*
   * read() - blocking read
   */
  int read(void) {
      return _read();
  }

  /*
   * write_impl() - provide our implementation for print_t<WRITER> class
   *
   * blocking cycle counting write routine
   */
  int write_impl(uint8_t c) {
      _write(c);
      return 1;
  }

};

/*
 * serial_base_sw_t<>::_write() - write one byte to the TX pin
 */
template<uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
void serial_base_sw_t<BAUD, MCLK_HZ, TXPIN, RXPIN>::_write(register unsigned c) {
    register unsigned bitrate_cycles = bit_rate.dur;
    register unsigned mask = TXPIN::pin_mask;
    register unsigned cnt = 0;

    c |= 0x300; // add stop bits

    __asm__ volatile (
            "   jmp     3f                   ; send start bit... (2)\n"
            "1:                              ;\n"
            "   mov     %[brd],%[cnt]        ; set cnt to number of cycles for bitrate (1)\n"
            "2:                              ;\n"
            "   nop                          ; 4 cycles loop (1)\n"
            "   sub     #8, %[cnt]           ; (1)\n"
            "   jc      2b                   ; (2)\n"
            "   subc    %[cnt], r0           ; 0 to 3 cycle delay (4)\n"
            "   nop                          ; 3\n"
            "   nop                          ; 2\n"
            "   nop                          ; 1\n"
            "   rra     %[c]                 ; Get bit to tx, test for zero (1)\n"
            "   jc      4f                   ; If high... (2)\n"
            "3:                              ;\n"
            "   bic.b   %[mask],%[PXOUT]     ; Send zero bit (2)\n"
            "   jmp     1b                   ; Next bit...(2)\n"
            "4:                              ;\n"
            "   bis.b   %[mask], %[PXOUT]    ; Send one bit (2)\n"
            "   jnz     1b                   ; If tx data is not zero, then there are more bits to send...(2)\n"

            : /* return value */
            [c] "+r" (c),
            [cnt] "+r" (cnt)

            : /* external variables */
            [brd] "r" (bitrate_cycles),
            [mask] "r" (mask),          /* #BIT2  */
            [PXOUT] "m" (TXPIN::POUT()) /* &P1OUT */

            : /* _write() clobbers these registers */
    );

    return;
}

/*
 * serial_base_sw_t<>::_read() - read one byte from RX pin
 */
template<uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
int serial_base_sw_t<BAUD, MCLK_HZ, TXPIN, RXPIN>::_read() {
    register unsigned result;
    register unsigned bitrate_cycles = bit_rate.dur;
    register unsigned work1 = RXPIN::pin_mask;
    register unsigned work2;

    __asm__ volatile (
            " mov     #0x01FF, %[rc]      ; 9 bits - 8 data + stop\n"
            "1:                           ; Wait for start bit\n"
            " mov.b   %[PXIN], %[w02]     ; Read serial input pin value\n"
            " and     %[w01], %[w02]      ; Mask and test bit\n"
            " jc      1b                  ; Wait for low...\n"
            " mov     %[brd_div2], %[w01] ; Wait for 1/2 bit time\n"
            "2:                           ; \n"
            " nop                         ; Bit delay\n"
            " sub     #8, %[w01]          ; 1/2 bit adjust\n"
            " jc      2b\n"
            " subc    %[w01], r0          ; 0 to 3 cycle delay\n"
            " nop                         ; 3\n"
            " nop                         ; 2\n"
            " nop                         ; 1\n"
            " mov.b   %[PXIN], %[w02]     ; Get serial input\n"
            " and     %[rx_pin], %[w02]   ;\n"
            " rrc     %[rc]               ; Shift in a bit\n"
            " mov     %[brd], %[w01]      ; Setup bit timer\n"
            " jc      2b                  ; Next bit...\n"
            "3:                           ; exit\n"
            " rla     %[rc]               ; Move stop bit to carry\n"
            " swpb    %[rc]               ; Move rx byte to lower byte, start bit in msb\n"

            : /* return value */
            [rc]  "=r" (result),
            [brd] "+r" (bitrate_cycles),
            [w01] "+r" (work1),
            [w02] "=r" (work2)
            : /* external variables */
            [brd_div2] "i" (bit_rate.dur_half),
            [rx_pin]   "i" (RXPIN::pin_mask),     /* #BIT1 */
            [PXIN]     "m" (RXPIN::PIN())         /* &P1IN */
            : /* _read() clobbers these registers */
    );

    return result;
}

// typical usage
template <const uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct sw_serial_t:
    serial_base_sw_t<BAUD,MCLK_HZ,TXPIN,RXPIN>,
    print_t<sw_serial_t<BAUD, MCLK_HZ, TXPIN, RXPIN> >
{
};

#endif /* SWSERIAL_430_H_ */
