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
 * Creation date: 06/25/2014
 */

#include "cc430f5137.h"
#include "wiring.h"


uint16_t analogPeriod = SYSTEM_CLK_FREQ/490;
uint16_t analogRes = 255;
uint16_t analogRef = ADCREF_VCC;

//Arduino specifies ~490 Hz for analog out PWM so we follow suit.
#define PWM_PERIOD analogPeriod // SYSTEM_CLK_FREQ/490
#define PWM_DUTY(x) ( (unsigned long)x*PWM_PERIOD / (unsigned long)analogRes )

// Calibration data
#define CALIB_ADC_GAIN      *((uint16_t *)0x1A16)
#define CALIB_ADC_OFFSET    *((uint16_t *)0x1A18)
#define CALIB_1V5REF_GAIN   *((uint16_t *)0x1A28)
#define CALIB_2V0REF_GAIN   *((uint16_t *)0x1A2A)
#define CALIB_2V5REF_GAIN   *((uint16_t *)0x1A2C)

/**
 * analogRead
 *
 * read ADC value
 *
 * @param pin pn number
 * 
 * @return ADC value
 */
uint16_t analogRead(uint8_t pin)
{
  uint8_t port, bit=0;
  volatile uint8_t *dir=NULL;
  
  uint16_t refGain = 0;
   
  uint8_t channel;
  
  // Disable ADC
  ADC12CTL0 &= ~ADC12ENC;
   
  // Special analog channel?
  if (pin >= 128)
  {
    channel = pin - 128;
    
    if (pin == A10)
      REFCTL0 &= ~REFTCOFF; // Temp sensor enabled
  }

  // Check if pin is an analog input
	else if ((channel = digitalPinToADCIn(pin)) == NOT_ON_ADC)
		return 0;
  else
  {
    bit = digitalPinToBitMask(pin);
    port = digitalPinToPort(pin);

    // Map pin as analog input
    volatile uint8_t *map = digitalPinToPortMap(pin);
    PMAPPWD = 0x02D52;
    *map = PM_ANALOG;
    PMAPPWD = 0;
    
    // Select alternate function
    volatile uint8_t *sel = portSelRegister(port);
    *sel |= bit;
    
    dir = portDirRegister(port);
    *dir &= ~bit; // Configure pin as input
  }

  // Set ADC reference  
  if (analogRef == ADCREF_VCC)
      ADC12MCTL0 = ADC12SREF_0;  // Vr+=Vcc and Vr-=AVss
  else
  {
    // Enable shared reference
    REFCTL0 |= REFMSTR + analogRef + REFON;   
    ADC12MCTL0 = ADC12SREF_1;    // Vr+=Vref+ and Vr-=AVss
    
    // Select REF calibration gain
    switch(analogRef)
    {
      case ADCREF_1V5:
        refGain = CALIB_1V5REF_GAIN;
        break;
      case ADCREF_2V0:
        refGain = CALIB_2V0REF_GAIN;
        break;
      case ADCREF_2V5:
        refGain = CALIB_2V5REF_GAIN;
        break;
     }
  }

  ADC12IFG = 0;                                   // Clear flags
  ADC12CTL0 = ADC12SHT02 + ADC12ON;               // Sampling time=64 cycles, ADC12 on
  ADC12CTL1 = ADC12SHP;                           // Use sampling timer
  ADC12MCTL0 |= channel;                          // Select channel
  ADC12IE = 0;                                    // Disable interrupt
  
  __delay_cycles(250);

  ADC12CTL0 |= ADC12ENC | ADC12SC;                // Enable ADC and start conversion

  while (!(ADC12IFG & BIT0));                     // Wait until ADC is completed

  ADC12IFG = 0;

  // POWER: Turn ADC and reference voltage off to conserve power
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL0 &= ~ADC12ON;
  ADC12CTL0 &= ~ADC12REFON;
  REFCTL0 &= ~REFON;
  REFCTL0 |= REFTCOFF;  // Temp sensor disabled

  // Config pin as output to save current
  *dir |= bit;

  uint64_t result = ADC12MEM0;

  if (refGain)
    result *= refGain;

  result *= CALIB_ADC_GAIN;
  result /= 0x8000;
  
  if (refGain)
    result /= 0x8000;
    
  result += CALIB_ADC_OFFSET;

  return (uint16_t) result;
}

/**
 * analogReference
 *
 * set ADC reference
 *
 * @param ref ADC reference: DEFAULT (AVcc), INTERNAL1V5, INTERNAL2V0 or INTERNAL2V5
 */
void analogReference(uint16_t ref)
{
  analogRef = ref;
}

/**
 * analogWrite
 *
 * set PWM output level
 *
 * @param pin pin number
 * @param val duty cycle
 */
void analogWrite(uint8_t pin, uint16_t val)
{     
  if (val == 0)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  else
  {
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);

    volatile uint8_t *dir = portDirRegister(port);
    volatile uint8_t *sel = portSelRegister(port);
    volatile uint8_t *map = digitalPinToPortMap(pin);
     
    *dir |= bit;                               // Config pin as an output
    *sel |= bit;                               // Select alternate function
    
    // Set PWM period
    TA0CCR0 = analogPeriod;
    TA1CCR0 = analogPeriod;
    
    uint8_t timer = digitalPinToTimer(pin);

    PMAPPWD = 0x02D52;                         // Get write-access to port mapping regs 
    PMAPCTL |= PMAPRECFG;                      // Leave Pin mapping open
        
    switch(timer)
    {
      case T0A1:
        *map = PM_TA0CCR1A;
        TA0CCTL1 = OUTMOD_7;                    // CCR1 reset/set
        TA0CCR1 = PWM_DUTY(val);                // CCR1 PWM duty cycle
        break;
      case T0A2:
        *map = PM_TA0CCR2A;
        TA0CCTL2 = OUTMOD_7;                    // CCR1 reset/set
        TA0CCR2 = PWM_DUTY(val);                // CCR1 PWM duty cycle
        break;
      case T0A3:
        *map = PM_TA0CCR3A;
        TA0CCTL3 = OUTMOD_7;                    // CCR1 reset/set
        TA0CCR3 = PWM_DUTY(val);                // CCR1 PWM duty cycle
        break;
      case T0A4:
        *map = PM_TA0CCR4A;
        TA0CCTL4 = OUTMOD_7;                    // CCR1 reset/set
        TA0CCR4 = PWM_DUTY(val);                // CCR1 PWM duty cycle
        break;
      case T1A1:
        *map = PM_TA1CCR1A;
        TA1CCTL1 = OUTMOD_7;                    // CCR1 reset/set
        TA1CCR1 = PWM_DUTY(val);                // CCR1 PWM duty cycle
        break;
      case T1A2:
        *map = PM_TA1CCR2A;
        TA1CCTL2 = OUTMOD_7;                    // CCR1 reset/set
        TA1CCR2 = PWM_DUTY(val);                // CCR1 PWM duty cycle
        break;
      default:
        break;
    }

    PMAPPWD = 0;                               // Lock port mapping registers 
  
    if (timer < T1A1)
    {
      if (val == TA0CCR0)                     // duty cycle = period?
      {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
      }
      else
        TA0CTL = TASSEL_2 + MC_1 + TACLR;     // SMCLK, up mode, clear TAR
    }
    else
    {
      if (val == TA1CCR0)                     // duty cycle = period?
      {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
      }
      else
        TA1CTL = TASSEL_2 + MC_1 + TACLR;     // SMCLK, up mode, clear TAR
    }
  }
}

/**
 * analogFrequency
 *
 * set PWM period
 *
 * @param freq PWM frequency
 */
void analogFrequency(uint16_t freq)
{
  analogPeriod = SYSTEM_CLK_FREQ/freq;
}
