/*
 * usciserial430.h - USCI hardware Serial UART template for msp430
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-08-2013
 * Version: 1.0.3
 *
 * 11-12-2012: 0.0.001 rick@kimballsoftware.com - initial version
 * 02-05-2013: 0.0.002 rick@kimballsoftware.com - tweaks to remove TX RX instances
 *
 * Acknowledgments:
 *  Serial API inspired by Arduino.
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

#ifndef USCISERIAL_430_H_
#define USCISERIAL_430_H_

#if 1 /* make eclipse happy so i can get code completion */
    #include <cc430f5137.h>
    #include <fabooh.h>
    #include <gpio.h>
    #include <pins.h>
#endif

#if  defined(__MSP430_HAS_USCI__) || defined(__MSP430_HAS_USCI_A0__)

template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct serial_base_usci_t {

  /**
   * begin( baudrate ) - initialize TX/RX pins
   */
  void begin(const uint32_t baud_ignored = BAUD /* not used */) {

    PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
    PMAPCTL |= PMAPRECFG;// Leave Pin mapping open
    P1MAP6 = PM_UCA0TXD;
    P1MAP7 = PM_UCA0RXD;
    PMAPPWD = 0;                              // Lock port mapping registers

#if 1
    const unsigned long baud_rate_20_bits = (MCLK_HZ + (BAUD >> 1)) / BAUD;  // Bit rate divisor
    //const unsigned long baud_rate_16_bits = MCCLK_HZ / BAUD;
    // Configure TXPIN and RXPIN as UART controlled pins
    if ( TXPIN::_portno != NO_PIN::_portno ) {
      TXPIN::setmode_output();
      TXPIN::PSEL() |= (TXPIN::pin_mask | RXPIN::pin_mask);
    }

    UCA0CTL1 = UCSWRST;                             // Hold USCI in reset to allow configuration
    UCA0CTL0 = 0;// No parity, LSB first, 8 bits, one stop bit, UART (async)
    UCA0ABCTL = 0;
    UCA0BR1 = (baud_rate_20_bits >> 12) & 0xFF;// High byte of whole divisor
    UCA0BR0 = (baud_rate_20_bits >> 4) & 0xFF;// Low byte of whole divisor
    UCA0MCTL = ((baud_rate_20_bits << 4) & 0xF0) | UCOS16;// Fractional divisor, over sampling mode
    UCA0CTL1 = UCSSEL_2;// Use SMCLK for bit rate generator, then release reset

#else
    // port mapping
    P1DIR |= BIT6;
    P1SEL |= BIT6 | BIT7;

    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 6;                              // 9600 @ 1048576Hz (see User's Guide)
    UCA0BR1 = 0;                              // 9600 @ 1048576Hz
    UCA0MCTL = UCBRS_0 | UCBRF_13 | UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
                                              // over sampling
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
#endif
    (void) baud_ignored;
  }

  /*
   * flush() - wait for all bits to be sent/recvd
   */
  void flush() {
    while (UCA0STAT & UCBUSY) ;
  }

  /*
   * end() - set TX/RX pin back to default
   */
  void end() {
    // wait for TX/RX to finish
    flush();

    // put USCI in reset
    UCA0CTL1 = UCSWRST;

    // Revert TX/RX pins back to their defaults as input pins
    if ( TXPIN::pin_mask && RXPIN::pin_mask ) {
      TXPIN::PDIR()  &= ~(TXPIN::pin_mask);
      TXPIN::PSEL()  &= ~(TXPIN::pin_mask | RXPIN::pin_mask);
    }
    else if ( TXPIN::pin_mask ) {
      TXPIN::PDIR()  &= ~(TXPIN::pin_mask);
      TXPIN::PSEL()  &= ~(TXPIN::pin_mask);
    }
    else if ( RXPIN::pin_mask ) {
      RXPIN::PSEL()  &= ~(RXPIN::pin_mask);
    }
  }

  /*
   * read() - blocking read
   */
  int read(void) {
    while(!(UCA0IFG & UCRXIFG));                  // wait for something in the RX buffer
    return UCA0RXBUF;
  }

  /*
   * write_impl() - blocking write CRTP function used by print<> template
   *
   * this template implements enough code to be considered a Writer pattern
   * for use with the print_t<Writer>
   *
   */
  int write_impl(uint8_t c) {
    while(!(UCA0IFG & UCTXIFG));                  // wait for TX buffer to be empty
    UCA0TXBUF = c;
    return 1;
  }

};

// typical usage
template <uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct usci_serial_t:
  serial_base_usci_t<BAUD, MCLK_HZ, TXPIN, RXPIN>,
  print_t<usci_serial_t<BAUD, MCLK_HZ, TXPIN, RXPIN> >
{
    // make it easier to instance in user code
};

#endif /* __MSP430_HAS_USCI__ */

#endif /* USCISERIAL_430_H_ */
