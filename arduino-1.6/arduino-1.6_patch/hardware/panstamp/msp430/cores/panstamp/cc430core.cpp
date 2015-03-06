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

  // Keep current port selections
  portSelection[0] = P1SEL;
  portSelection[1] = P2SEL;
  portSelection[2] = P3SEL;

  // Configure ports as binary I/O's
  P1SEL = 0;
  //P2SEL = 0;
  
  #ifdef __NRG_VERSION_1_0__
  // Current hardware version (1.0) uses P3.6 to detect interrupts from
  // the on-board accelerometer. For this application, P3.6 is configured
  // as a timer capture input to simulate pin interrupts.
  // (P3 does not natively support pin interrupts)
  // In future hardware revisions, a pin supporting true pin interrupts
  // should be used instead.
  P3SEL &= BIT6;
  P3DIR |= portSelection[2] & ~BIT6;
  #else
  P3DIR |= portSelection[2];
  #endif
    
  // I2C lines remain high to not to sink current through
  // I2C pull-up resistors
  #ifdef __NRG_VERSION_1_1__
  P1OUT |= 0x90;
  #elif __NRG_VERSION_1_0__
  P1OUT |= 0x30;
  #endif
 
  // Configure ports working as alternative functions as outputs
  P1DIR |= portSelection[0];
  //P2DIR |= portSelection[1];

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
	// Configure PMM and SCLK for RF operation
  _SET_VCORE_12MHZ(2);
  
  // Enable WDT again
  enableWatchDog();

  // Recover old port selections
  P1SEL = portSelection[0];
  P2SEL = portSelection[1];
  P3SEL = portSelection[2];
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
  
  // POWER: Turn ADC and reference voltage off to conserve power
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL0 &= ~ADC12ON;
  ADC12CTL0 &= ~ADC12REFON;
  REFCTL0 &= ~REFON;
  REFCTL0 |= REFTCOFF;  // Temp sensor disabled
  
  // Config pins as outputs by default
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P3DIR = 0xFF;
  PJDIR = 0xFF;
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
