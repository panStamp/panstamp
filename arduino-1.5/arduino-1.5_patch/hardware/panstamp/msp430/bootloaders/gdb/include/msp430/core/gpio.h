/*
 * gpio.h - C++ template based GPIO port and pin methods
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-27-2013
 * Version: 1.0.5
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

#ifndef GPIO_H_
#define GPIO_H_

#include <logic.h>

#define GPIO_VERSION 0x0100 /* 1.0 */
#define GPIO_BUILD   0x0005 /* build #*/

typedef volatile uint8_t & u8_SFR;        /* 8 bit unsigned Special Function Register reference */
typedef const volatile uint8_t & u8_CSFR; /* 8 bit unsigned Constant Special Function Register reference */

/*
 * pin_mode - settings for port/pin direction
 * Note: values used coincide with the optimized constant
 *       generator values that result in faster msp430 instructions
 */
enum pin_mode {
  INPUT=0x00
  ,OUTPUT=0x01
  ,INPUT_PULLUP=0x02
  ,INPUT_PULLDOWN=0x04
};

enum pin_value {
  LOW=0x00
  ,HIGH=0x01
};

/*
 * GPIO_PORT<> - port template for basic ports
 *
 */
template <
  const int portno
  ,u8_CSFR pin
  ,u8_SFR pout
  ,u8_SFR pdir
  ,u8_SFR psel
  ,u8_SFR pren
>
struct GPIO_PORT {
  static u8_CSFR PIN()  { return pin;  }
  static u8_SFR POUT()  { return pout; }
  static u8_SFR PDIR()  { return pdir; }
  static u8_SFR PSEL()  { return psel; }
  static u8_SFR PREN()  { return pren; }
  static yesno_e hasPSEL2() { return NO; }
  static yesno_e hasInterrupt() { return NO; }
  static const int _portno = portno;

  /**
   * pin direction configuration methods
   */
  ALWAYS_INLINE static void set_mode(const uint8_t mask, pin_mode mode) {
    if ( 0 ) {
    }
    else if (mode == OUTPUT ) {
      setmode_output(mask);
    }
    else if ( mode == INPUT ) {
      setmode_input(mask);
    }
    else if (mode == INPUT_PULLUP) {
        setmode_inputpullup(mask);
    }
    else if (mode == INPUT_PULLDOWN) {
        setmode_inputpulldown(mask);
    }
  }

  ALWAYS_INLINE static void setmode_input(const uint8_t mask) {
    pdir &= ~mask;
  }

  ALWAYS_INLINE static void setmode_inputpullup(const uint8_t mask) {
    pdir &= ~mask; set_pins(mask); pren |= mask;
  }

  ALWAYS_INLINE static void setmode_inputpulldown(const uint8_t mask) {
    pdir &= ~mask;  clear_pins(mask); pren |= mask;
  }

  ALWAYS_INLINE static void setmode_output(const uint8_t mask ) {
    pdir |= mask;
  }

  /*
   * port wide (8bits) get/set methods
   */
  ALWAYS_INLINE static void set_value(const uint8_t value) {
    pout = value;
  }

  ALWAYS_INLINE static uint8_t get_value() {
    return pin;
  }

  ALWAYS_INLINE static void set_pins(const uint8_t pin_mask) {
    pout |= pin_mask;
  }

  ALWAYS_INLINE static void clear_pins(const uint8_t pin_mask) {
    pout &= ~pin_mask;
  }

  ALWAYS_INLINE static void toggle_pins(const uint8_t pin_mask) {
    pout ^= pin_mask;
  }

  /* TODO: devise generic scheme for setting alternate pin functions */
};

/*
* GPIO_PORT_IC_SEL<> - port template for interrupt capable ports w/single SEL
*
*/

template <
  const int portno
  ,u8_CSFR pin
  ,u8_SFR pout
  ,u8_SFR pdir
  ,u8_SFR pifg
  ,u8_SFR pies
  ,u8_SFR pie
  ,u8_SFR psel
  ,u8_SFR pren
>
struct GPIO_PORT_IC_SEL :
GPIO_PORT<portno, pin, pout, pdir, psel, pren>
{
  static u8_SFR PIFG()  { return pifg; }
  static u8_SFR PIES()  { return pies; }
  static u8_SFR PIE()   { return pie; }
  static yesno_e hasInterrupt() { return YES; }
};

/*
 * gpio_pincaps_t - gpio_pin capabilities
 *
 * @desc: provide boolean tests that describe the capabilities
 *       of a GPIO_PIN
 */
template<
  bool active_low = false
  ,bool uart_tx_pin = false
  ,bool uart_rx_pin = false
  ,bool spi_sclk_pin = false
  ,bool spi_mosi_pin = false
  ,bool spi_mis0_pin = false
>
struct gpio_pincaps_t {
  static const bool is_active_low = active_low;
  static const bool canbe_uart_tx = uart_tx_pin;
  static const bool canbe_uart_rx = uart_rx_pin;
};

typedef gpio_pincaps_t<true> gpio_pin_button;
typedef gpio_pincaps_t<true,true> gpio_pin_uart_tx;
typedef gpio_pincaps_t<true,false,true> gpio_pin_uart_rx;
typedef gpio_pincaps_t<false,false,false,true> gpio_pin_spi_sclk;

/*
 * GPIO_PIN<> - pin template
 */

template <const uint8_t MASK, typename PORT, typename CAPS_T = gpio_pincaps_t<> >
struct GPIO_PIN {
  typedef GPIO_PIN<MASK,PORT> T;
  static const uint8_t pin_mask=MASK;
  static const int _portno = PORT::_portno;
  typedef CAPS_T pin_cap;

  static u8_CSFR PIN()  { return PORT::PIN();  }
  static u8_SFR POUT()  { return PORT::POUT(); }
  static u8_SFR PDIR()  { return PORT::PDIR(); }
  static u8_SFR PSEL()  { return PORT::PSEL(); }
  static u8_SFR PSEL2() { return PORT::PSEL2();}
  static u8_SFR PREN()  { return PORT::PREN(); }

  static u8_SFR PIFG()  { return PORT::PIFG(); }
  static u8_SFR PIES()  { return PORT::PIES(); }
  static u8_SFR PIE()   { return PORT::PIE(); }

  /*
   * pin direction functions
   */
  ALWAYS_INLINE static void setmode_input() {
    PORT::PDIR() &= ~MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void setmode_inputpullup() {
    PORT::PDIR() &= ~MASK;
    high();
    PORT::PREN() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void setmode_inputpulldown() {
    PORT::PDIR() &= ~MASK;
    low();
    PORT::PREN() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void setmode_output() {
    PORT::PDIR() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void set_mode(const pin_mode mode) {
    if (0) {
    }
    else if (mode == OUTPUT) {
      setmode_output();
    }
    else if (mode == INPUT) {
      setmode_input();
    }
    else if (mode == INPUT_PULLUP) {
      setmode_inputpullup();
    }
    else if (mode == INPUT_PULLDOWN) {
      setmode_inputpulldown();
    }
  }

  /*
   * pin query functions
   */
  ALWAYS_INLINE static unsigned read() {
    return (PORT::PIN() & MASK ) != 0;
  }

  /*
   *
   */
  ALWAYS_INLINE static unsigned is_low() {
    return (PORT::PIN() & MASK ) == 0;
  }

  /*
   *
   */
  ALWAYS_INLINE static unsigned is_high() {
    return (PORT::PIN() & MASK ) != 0;
  }

  /*
   *
   */
  ALWAYS_INLINE static unsigned is_pushed() {
    if ( pin_cap::is_active_low ) {
      return is_low();
    }
    else {
      return is_high();
    }
  }

  /*
   * pin modification functions
   */
  ALWAYS_INLINE static void write(pin_value value) {
    if (value == HIGH) {
      high();
    }
    else if (value == LOW) {
      low();
    }
  }

  /*
   *
   */
  ALWAYS_INLINE static void high() {
    //__asm__ volatile("bis.b %1, %0": "+m" (PORT::POUT()) : "r" (MASK));
    PORT::POUT() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void low() {
    if ( MASK ) {
//      __asm__ volatile("bic.b %1, %0": "+m" (PORT::POUT()) : "r" (MASK));
      PORT::POUT() &= ~MASK;
    }
  }

  /*
   *
   */
  ALWAYS_INLINE static void toggle() {
    if ( MASK )
      PORT::POUT() ^= MASK;
  }

  /*
   * port wide (8bits) set direction using pin as port
   */
  ALWAYS_INLINE static void set_modes(const uint8_t pins_mask, pin_mode mode) {
    PORT::set_mode(pins_mask, mode);
  }

  /*
   * port wide (8bits) modifications using pin as port
   */
  ALWAYS_INLINE static void set_pins(const uint8_t pins_mask) {
    PORT::POUT() |= pins_mask;
  }

  /*
   *
   */
  ALWAYS_INLINE static void clear_pins(const uint8_t pins_mask) {
    PORT::POUT() &= ~pins_mask;
  }

  /*
   *
   */
  ALWAYS_INLINE static void toggle_pins(const uint8_t pins_mask) {
    PORT::POUT() ^= pins_mask;
  }

};

/*
 * DummyGPIO - minimal template th  int read(void) {
    while(!(UCRXIFG & UCA0IFG))
      ;                  // wait for something in the RX buffer
    return UCA0RXBUF;
  }
 * at looks like a GPIO but does nothing
 *
 * Useful to provide NOP pin as in the case of an output only serial port.
 * This will satisfy the compiler but not add to the code size.
 *
 */
template<uint8_t PORTPIN>
struct DummyGPIO {
  struct {
    static uint16_t PIN()    { return 0; }
    static uint16_t POUT()   { return 0; }
    static uint16_t PSEL()   { return 0; }
    static uint16_t PSEL2()  { return 0; }
    static uint16_t PREN()   { return 0; }
    static uint8_t PINMASK() { return 0; }

    static void set_mode(const uint8_t, pin_mode) {}
    static const int _portno = 0;
  } port;
  static volatile uint8_t _psel;
  static volatile uint8_t _fake_pin;

  static u8_SFR PSEL() { return (u8_SFR)_psel; }
  static u8_SFR PSEL2() { return (u8_SFR)_psel; }
  static u8_SFR PIN() { return (u8_SFR)_fake_pin; }

  static const uint8_t pin_mask=0;
  static const int _portno = 0;

  static void setmode_input() {}
  static void setmode_inputpullup() {}
  static void setmode_inputpulldown() {}
  static void setmode_output() {}
  static void set_mode(pin_mode) {}

  static void high() { }
  static void low() { }
  static void toggle() { }

  static int read() { return 0; }
  static void write(pin_value) {}
};

typedef DummyGPIO<0> NO_PIN;

template<typename P1, typename P2>
struct is_same_port {
  enum  { yes_no = (P1::_portno == P2::_portno) ? 1 : 0 };
};

/*
 * port helper macros
 * assumes all pins are from the same port, no check performed , user tasked with being smart
 */
#define port_mode(B0,B1, _mode) B0::set_modes((const uint8_t)(B0::pin_mask|B1::pin_mask),_mode);
#define port_toggle(B0,B1) B0::toggle_pins(B0::pin_mask|B1::pin_mask);

/*
 * nod to Arduino API
 */
#define digitalRead(PIN_T) PIN_T::read()
#define digitalWrite(PIN_T,_value) PIN_T::write(_value)
#define pinMode(PIN_T,_mode) PIN_T::set_mode(_mode)

#endif /* GPIO_H_ */
