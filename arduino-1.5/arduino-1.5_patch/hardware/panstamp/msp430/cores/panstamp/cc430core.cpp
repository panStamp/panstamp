/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
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
 * Creation date: 06/27/2013
 */

#include "cc430core.h"
#include "cc430x513x.h"


#ifdef __cplusplus
 extern "C" {
#endif
#include "pmm.h"
#ifdef __cplusplus
}
#endif

/**
 * setLowPowerMode
 *
 * Enter low power mode
 *
 * @param lpm4 true if LPM4 has to be entered. Otherwise the MCU will
 * enter LPM3
 */
void CC430CORE::setLowPowerMode(bool lpm4)
{
  // Stop WDT
  disableWatchDog();
  
  /*
  // Disable ADC
  ADC12CTL0 &= ~ADC12ON;
  ADC12CTL0 &= ~ADC12REFON;
  ADC12CTL0 &= ~ADC12ENC;
  */

  // Keep current port mapping
  portConfig[0].mapping[0] = P1MAP0;
  portConfig[0].mapping[1] = P1MAP1;
  portConfig[0].mapping[2] = P1MAP2;
  portConfig[0].mapping[3] = P1MAP3;
  portConfig[0].mapping[4] = P1MAP4;
  portConfig[0].mapping[5] = P1MAP5;
  portConfig[0].mapping[6] = P1MAP6;
  portConfig[0].mapping[7] = P1MAP7;

  portConfig[1].mapping[0] = P2MAP0;
  portConfig[1].mapping[1] = P2MAP1;
  portConfig[1].mapping[2] = P2MAP2;
  portConfig[1].mapping[3] = P2MAP3;
  portConfig[1].mapping[4] = P2MAP4;
  portConfig[1].mapping[5] = P2MAP5;
  portConfig[1].mapping[6] = P2MAP6;
  portConfig[1].mapping[7] = P2MAP7;

  portConfig[2].mapping[0] = P3MAP0;
  portConfig[2].mapping[1] = P3MAP1;
  portConfig[2].mapping[2] = P3MAP2;
  portConfig[2].mapping[3] = P3MAP3;
  portConfig[2].mapping[4] = P3MAP4;
  portConfig[2].mapping[5] = P3MAP5;
  portConfig[2].mapping[6] = P3MAP6;
  portConfig[2].mapping[7] = P3MAP7;

  // Keep current port selections
  portConfig[0].selection = P1SEL;
  portConfig[1].selection = P2SEL;
  portConfig[2].selection = P3SEL;

  // Keep current port direction
  portConfig[0].direction = P1DIR;
  portConfig[1].direction = P2DIR;
  portConfig[2].direction = P3DIR;

  // Configure ports as binary I/O's
  P1SEL = 0;
  P2SEL = 0;
  P3SEL = 0;

/*
  // Set port levels
  PJOUT = 0;
  P1OUT = 0;//0x30; // I2C lines remain high to not to sink current from
                // I2C pull-up resistors (when present)                            
  P2OUT = 0;
  P3OUT = 0;

  // Configure ports as outputs
  PJDIR = 0xFF;
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P3DIR = 0xFF;
*/

  // Enter lowest power VCore level and MCLK = 1 MHz
  _SET_VCORE_1MHZ(0);

  // Turn off SVSH, SVSM
  PMMCTL0_H = 0xA5;
  SVSMHCTL = 0;
  SVSMLCTL = 0;
  PMMCTL0_H = 0x00;

  if (lpm4)
  {
    // Enter LPM4 with interrupts
    __bis_SR_register(LPM4_bits + GIE);
  }
  else
  {
    // Enter LPM3 with interrupts
    __bis_SR_register(LPM3_bits + GIE);
  }
}

/*
 * setNormalMode
 *
 * Revert from low power mode
 */
void CC430CORE::setNormalMode(void)
{
  PJOUT |= BIT1;

	// Configure PMM and SCLK for RF operation
  //_SET_VCORE_12MHZ(2);
  _SET_VCORE_8MHZ(0);
  
  // Enable WDT again
  enableWatchDog();
     
  // Recover old port mapping
  P1MAP0 = portConfig[0].mapping[0];
  P1MAP1 = portConfig[0].mapping[1];
  P1MAP2 = portConfig[0].mapping[2];
  P1MAP3 = portConfig[0].mapping[3];
  P1MAP4 = portConfig[0].mapping[4];
  P1MAP5 = portConfig[0].mapping[5];
  P1MAP6 = portConfig[0].mapping[6];
  P1MAP7 = portConfig[0].mapping[7];

  P2MAP0 = portConfig[1].mapping[0];
  P2MAP1 = portConfig[1].mapping[1];
  P2MAP2 = portConfig[1].mapping[2];
  P2MAP3 = portConfig[1].mapping[3];
  P2MAP4 = portConfig[1].mapping[4];
  P2MAP5 = portConfig[1].mapping[5];
  P2MAP6 = portConfig[1].mapping[6];
  P2MAP7 = portConfig[1].mapping[7];

  P3MAP0 = portConfig[2].mapping[0];
  P3MAP1 = portConfig[2].mapping[1];
  P3MAP2 = portConfig[2].mapping[2];
  P3MAP3 = portConfig[2].mapping[3];
  P3MAP4 = portConfig[2].mapping[4];
  P3MAP5 = portConfig[2].mapping[5];
  P3MAP6 = portConfig[2].mapping[6];
  P3MAP7 = portConfig[2].mapping[7];

  // Recover old port selections
  P1SEL = portConfig[0].selection;
  P2SEL = portConfig[1].selection;
  P3SEL = portConfig[2].selection;

  // Keep current port direction
  P1DIR = portConfig[0].direction;
  P2DIR = portConfig[1].direction;
  P3DIR = portConfig[2].direction;
 
  PJOUT &= ~BIT1;
}

/**
 * init
 * 
 * Initialize CC430 core
 * VCORE = 2 and SCLK = 12 MHz when no argument is passed
 * 
 * @param vCore VCORE level
 * @param dcorsel CPU DCORSEL value
 * @param flln MCLK multiplier bits
 */
void CC430CORE::init(uint8_t vCore, uint16_t dcorsel, uint16_t flln)
{
	// Configure PMM
	SetVCore(vCore);

  // Set the High-Power Mode Request Enable bit so LPM3 can be entered
  // with active radio enabled
  PMMCTL0_H = 0xA5;
  PMMCTL0_L |= PMMHPMRE_L; 
  PMMCTL0_H = 0x00; 

  /**  
   * Enable 32kHz ACLK	
   */
  P5SEL |= 0x03;                      // Select XIN, XOUT on P5.0 and P5.1
  UCSCTL6 &= ~XT1OFF;        				  // XT1 On, Highest drive strength
  UCSCTL6 |= XCAP_3;                  // Internal load cap

  /*
   * Select XT1 as FLL reference
   */
  UCSCTL3 = SELA__XT1CLK;
  UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV;  

  /*
   * Configure CPU clock
   */
  setMCLK(dcorsel, flln);
 
  // Loop until XT1 & DCO stabilizes, use do-while to ensure that 
  // the body is executed at least once
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  } while ((SFRIFG1 & OFIFG));	

  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
                                            // strength

  /* 
   * Select Interrupt edge for PA_PD and SYNC signal:
   * Interrupt Edge select register: 1 == Interrupt on High to Low transition.
   */
  RF1AIES = BIT0 | BIT9;
}

/**
 * setVcoreMCLK
 *
 * Config VCORE and MCLK registers
 *
 * @param vCore VCORE level
 * @param dcorsel CPU DCORSEL value
 * @param flln MCLK multiplier bits
 */
void __inline__ CC430CORE::setVcoreMCLK(uint8_t vCore, uint16_t dcorsel, uint16_t flln)
{
	// Configure PMM
	SetVCore(vCore);
  
  // Set MCLK
  setMCLK(dcorsel, flln);
}

/**
 * setMCLK
 *
 * Config MCLK registers
 *
 * @param dcorsel CPU DCORSEL value
 * @param flln MCLK multiplier bits
 */
void __inline__ CC430CORE::setMCLK(uint16_t dcorsel, uint16_t flln)
{
  /**
   * Configure CPU clock for 12MHz
   */
  _BIS_SR(SCG0);              // Disable the FLL control loop
  UCSCTL0 = 0x0000;           // Set lowest possible DCOx, MODx
  UCSCTL1 = dcorsel;          // Select suitable range
  UCSCTL2 = FLLD_1 + flln;    // Set DCO Multiplier
  _BIC_SR(SCG0);              // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
  //__delay_cycles(250000);
  delayClockCycles(0xFFFF);
}
