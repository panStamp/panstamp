/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 10/27/2013
 */

#include "cc430f5137.h"
#include "cc430uart.h"


uint8_t CC430UART::rxBuffer[SERIAL_BUFFER_SIZE];
uint8_t CC430UART::rxLength = 0;
uint8_t CC430UART::rxIndex = 0;

uint8_t CC430UART::txBuffer[SERIAL_BUFFER_SIZE];
volatile uint8_t CC430UART::tx_buffer_tail = 0;
uint8_t CC430UART::tx_buffer_head = 0;

/**
 * begin
 * 
 * Initialize UART port
 *
 * @param baud Serial baudrate
 */
void CC430UART::begin(uint32_t baud)
{
	uint16_t ucbrf, divider;
  uint32_t clkfreq = SYSTEM_CLK_FREQ;
  
  /**
   * Pin mapping
   */
	PMAPPWD = 0x02D52;	  // Get write-access to port mapping regs
  PMAPCTL |= PMAPRECFG; // Leave Pin mapping open
  pinUARTmap();         // Map UART pins
	PMAPPWD = 0;		      // Lock port mapping registers

  pinUARTconfig();

	UCA0CTL1 = UCSWRST;
	UCA0CTL1 |= UCSSEL_2;  // SMCLK
	UCA0CTL0 = 0;
	UCA0ABCTL = 0;

  divider = (clkfreq >> 4)/baud;
  ucbrf = (clkfreq % (baud << 4)) / baud;

  UCA0BR0 = divider;                   // Integer part of UART frequency scaler (low byte)
  UCA0BR1 = divider>>8;                // Integer part of UART frequency scaler (high byte)
  UCA0MCTL = ucbrf + UCBRS_0 + UCOS16; // This turns on oversampling and sets the decimal part of the scaler

	UCA0CTL1 &= ~UCSWRST;
	UCA0IE |= UCRXIE;                    // Enable Rx interrupt
}


/**
 * write
 * 
 * Send single byte
 *
 * @param c Character to be transmitted
 *
 * @return Amount of bytes transmitted
 */
size_t CC430UART::write(uint8_t c) 
{ 
  // If the buffer and the data register are empty, just write the byte
  // to the data register and be done.
  if ((CC430UART::tx_buffer_head == CC430UART::tx_buffer_tail) && (UCA0IFG & UCTXIFG)) {
    UCA0TXBUF = c;
    return 1;
  }

  uint8_t i = (CC430UART::tx_buffer_head + 1) % SERIAL_BUFFER_SIZE;

  // If the output buffer is full, then wait for the interrupt
  // handler to drain it of a character.
  while (i == CC430UART::tx_buffer_tail)
    ;

  CC430UART::txBuffer[CC430UART::tx_buffer_head] = c;
  CC430UART::tx_buffer_head = i;

  // Turn on interrupts, since there's a character in the buffer
  UCA0IE |= UCTXIE;
  
  return 1;
}

/**
 * write
 * 
 * Send data buffer
 *
 * @param buf Buffer to be sent
 * @param len Amount of bytes to be transferred
 *
 * @return Amount of bytes transmitted
 */
size_t CC430UART::write(uint8_t *buf, uint16_t len) 
{ 
  uint16_t i;

  // Send bytes
  for(i=0 ; i<len ; i++)
    write(buf[i]);

  return len;
}

/**
 * read
 * 
 * Read single byte from Rx FIFO
 *
 * @return byte read
 */
int CC430UART::read(void)
{
  if (CC430UART::rxLength == 0)
    return -1;
  
  if (CC430UART::rxIndex >= CC430UART::rxLength)
    return -1;
  
  uint8_t b = CC430UART::rxBuffer[CC430UART::rxIndex++];

  if (CC430UART::rxIndex == CC430UART::rxLength)
  {
    CC430UART::rxLength = 0;
    CC430UART::rxIndex = 0;
  }
  
  return (int)b;
}

/**
 * read
 * 
 * Read string from the Rx FIFO
 *
 * @param buf Buffer to be sent
 *
 * @return amount of bytes read or 0 if the Rx FIFO is empty
 */
int CC430UART::read(uint8_t *buf)
{
  uint8_t i;
  
  for(i=0 ; i<CC430UART::rxLength ; i++)
    buf[i] = CC430UART::rxBuffer[i];
  
  CC430UART::rxLength = 0;
  CC430UART::rxIndex = 0;
  
  return i;
}

/**
 * available
 * 
 * Get he amount of bytes waiting to be read from the
 * UART Rx FIFO
 *
 * @return true if there is at least one byte in the Rx FIFO
 */
bool CC430UART::available(void)
{
  return (CC430UART::rxLength > 0);
}

/**
 * uartISR
 *
 * UART interrupt routine
 */
__attribute__((interrupt(USCI_A0_VECTOR)))
void uartISR(void)
{
  switch (UCA0IV) 
  { 
    case USCI_UCRXIFG:
      CC430UART::rxBuffer[CC430UART::rxLength++] = UCA0RXBUF;
      break;

    case USCI_UCTXIFG:
      UCA0TXBUF = CC430UART::txBuffer[CC430UART::tx_buffer_tail];

      CC430UART::tx_buffer_tail = (CC430UART::tx_buffer_tail + 1) % SERIAL_BUFFER_SIZE;
      if (CC430UART::tx_buffer_head == CC430UART::tx_buffer_tail) {
        // Buffer empty, so disable interrupts
	UCA0IE &= ~UCTXIE;
      }
      break;

    default:
      break;
  }
}
