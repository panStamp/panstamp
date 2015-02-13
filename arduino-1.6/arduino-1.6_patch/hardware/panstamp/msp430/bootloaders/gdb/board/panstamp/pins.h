/*
  * pins.h - msp430fr5738 GPIO port and pins
 *
 * Created: Nov 20, 2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-04-2012
 * Version: 1.0.3
 *
 * Note: this file is a work in progress don't expect it to be final ...
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
 */

#if 0


//                      +----------+
//               GND   1|   ||||   |24  GND
//         (D16) P3.0  2|          |23  P1.0 (D0)
//         (D17) P3.1  3|          |22  P1.1 (D1)
//         (D18) P3.2  4|          |21  P1.2 (D2)
//         (D15) P2.7  5|          |20  P1.3 (SPI_MOSI) (D3)
//         (D14) P2.6  6|          |19  P1.4 (SPI_MISO / I2C_SDA) (D4)
//               GND   7|          |18  P1.5 (SPI_SCK / I2C_SCL) (D5)
//      (D12/A4) P2.4  8|          |17  P1.6 (UART_TX) (D6)
//      (D11/A3) P2.3  9|          |16  P1.7 (UART_RX) (D7)
//      (D10/A2) P2.2 10|          |15  GND
//       (D9/A1) P2.1 11|          |14  VCC
//       (D8/A0) P2.0 12|          |13  RST
//                      +----------+
//
//                  TEMPSENSOR  -- P2.5
//                  ONBOARD_LED -- PJ.1


Also the small socket for the ez430 programmer has the following pin diagram. From left to right:
N.C. -- VCC -- SBWTCK -- SBWTDIO -- GND -- N.C.

#endif

#ifndef CC430F5137_PANSTAMP_H_
#define CC430F5137_PANSTAMP_H_

#include <msp430.h>
#include <cc430f5137.h>

/*
 * P1 port and P1.X shorthand typedefs
 */

#ifdef __MSP430_HAS_PORT1_R__
 typedef GPIO_PORT_IC_SEL<
         1
         ,PAIN_L
         ,PAOUT_L
         ,PADIR_L
         ,PAIFG_L
         ,PAIES_L
         ,PAIE_L
         ,PASEL_L
         ,PAREN_L
         > P1;

 typedef GPIO_PIN<BIT0,P1> P1_0;
 typedef GPIO_PIN<BIT1,P1> P1_1;
 typedef GPIO_PIN<BIT2,P1> P1_2;
 typedef GPIO_PIN<BIT3,P1> P1_3;
 typedef GPIO_PIN<BIT4,P1> P1_4;
 typedef GPIO_PIN<BIT5,P1> P1_5;
 typedef GPIO_PIN<BIT6,P1> P1_6;
 typedef GPIO_PIN<BIT7,P1> P1_7;

 typedef P1_3 MOSI;
 typedef P1_4 MISO;
 typedef P1_5 SCLK;
 typedef P1_6 TX_PIN;
 typedef P1_7 RX_PIN;

#endif /* __MSP430_HAS_PORT1_R__ */

 /*
  * P2 port and P2.X shorthand typedefs
  */

#ifdef __MSP430_HAS_PORT2_R__
 typedef GPIO_PORT_IC_SEL<
         2
         ,PAIN_H
         ,PAOUT_H
         ,PADIR_H
         ,PAIFG_H
         ,PAIES_H
         ,PAIE_H
         ,PASEL_H
         ,PAREN_H
         > P2;

 typedef GPIO_PIN<BIT0,P2> P2_0;
 typedef GPIO_PIN<BIT1,P2> P2_1;
 typedef GPIO_PIN<BIT2,P2> P2_2;
 typedef GPIO_PIN<BIT3,P2> P2_3;
 typedef GPIO_PIN<BIT4,P2> P2_4;
 typedef GPIO_PIN<BIT5,P2> P2_5;
 typedef GPIO_PIN<BIT6,P2> P2_6;
 typedef GPIO_PIN<BIT7,P2> P2_7;

 typedef GPIO_PIN<BIT0,P2,gpio_pin_button> PUSH2;

 #endif /* __MSP430_HAS_PORT2_R__ */

 /*
  * P3 port and P3.X shorthand typedefs
  */

#ifdef __MSP430_HAS_PORT3_R__
 typedef GPIO_PORT<
         3
         ,PBIN_L
         ,PBOUT_L
         ,PBDIR_L
         ,PBSEL_L
         ,PBREN_L
         > P3;

 typedef GPIO_PIN<BIT0,P3> P3_0;
 typedef GPIO_PIN<BIT1,P3> P3_1;
 typedef GPIO_PIN<BIT2,P3> P3_2;
 typedef GPIO_PIN<BIT3,P3> P3_3;
 typedef GPIO_PIN<BIT4,P3> P3_4;
 typedef GPIO_PIN<BIT5,P3> P3_5;
 typedef GPIO_PIN<BIT6,P3> P3_6;
 typedef GPIO_PIN<BIT7,P3> P3_7;

 #endif /* __MSP430_HAS_PORTJ_R__ */

#ifdef __MSP430_HAS_PORTJ_R__
 typedef GPIO_PORT<
         'J'
         ,PBIN_L
         ,PBOUT_L
         ,PBDIR_L
         ,PBSEL_L
         ,PBREN_L
         > PJ;

 typedef GPIO_PIN<BIT0,PJ> PJ_0;
 typedef GPIO_PIN<BIT1,PJ> PJ_1;
 typedef GPIO_PIN<BIT2,PJ> PJ_2;

 typedef PJ_1 RED_LED;

 #endif /* __MSP430_HAS_PORTJ_R__ */

 /*
  * P5 port and P5.X shorthand typedefs
  */

#ifdef __MSP430_HAS_PORT5_R__
 typedef GPIO_PORT<
         5
         ,PCIN_L
         ,PCOUT_L
         ,PCDIR_L
         ,PCSEL_L
         ,PCREN_L
         > P5;

 typedef GPIO_PIN<BIT0,P5> P5_0;
 typedef GPIO_PIN<BIT1,P5> P5_1;
 typedef GPIO_PIN<BIT2,P5> P5_2;
 typedef GPIO_PIN<BIT3,P5> P5_3;
 typedef GPIO_PIN<BIT4,P5> P5_4;
 typedef GPIO_PIN<BIT5,P5> P5_5;
 typedef GPIO_PIN<BIT6,P5> P5_6;
 typedef GPIO_PIN<BIT7,P5> P5_7;

 #endif /* __MSP430_HAS_PORT5_R__ */

 #endif /* CC430F5137_PANSTAMP_H_ */
