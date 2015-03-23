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
 * Creation date: 04/01/2014
 */

#include "rfloader.h"

// Responses from server have to be received before 10000 ms after sending
// the query
#define RESPONSE_TIMEOUT  10000
// Maximum number of queries sent to the server for a given line of firmware
#define MAX_REPEAT_QUERY  10

// User code address
uint16_t userCodeAddr;
// Global packet
CCPACKET packet;
// New firmware line received
volatile bool lineReceived = false;
// First hex line flag
bool firstLine = true;
// Length of last line received
uint8_t lineLength = 0;
// Pointer to line buffer
uint8_t *ptrLine;
// ISR vector table
uint8_t isrTable[8][16];


/**
 * readHexLine
 *
 * Read wireless packet and extract HEX line
 * 
 * @param lineNb Line number being queried
 * 
 * @return Correct line received (true) or not (false)
 */
bool readHexLine(uint16_t lineNumber)
{
  // Any packet waiting to be read?
  if (swap.radio.receiveData(&packet) > 0)
  {
    // Is CRC OK?
    if (packet.crc_ok)
    {
      #ifdef SWAP_EXTENDED_ADDRESS
      if ((packet.SWAP_ADDRTYPE) == SWAPADDR_EXTENDED)
      #else
      if ((packet.SWAP_ADDRTYPE) == SWAPADDR_SIMPLE)
      #endif
      {
        // Function
        if ((packet.SWAP_FUNCTION) == SWAPFUNCT_STA)
        {                
          // Data payload
          ptrLine = packet.data + SWAP_DATA_HEAD_LEN + 1;
          lineLength = packet.length - SWAP_DATA_HEAD_LEN - 1;

          // // Firmware page received?
          if(packet.SWAP_REGID == REGI_FIRMWARE)
          {
            // Correct data length?
            if (lineLength <= BYTES_PER_LINE)
            {
              // Correct line number?
              if (getLineNumber(ptrLine) == lineNumber)
              {
                ptrLine += 2;
                lineLength -= 2;
                lineReceived = true;
                return true;
              }                       
            }
          }
        }
      }
    }
  }
  return false;
}

/**
 * main
 *
 * Main routine
 */
int main(void)
{
  uint8_t state, status, bytes, i, count=0;
  // Current firmware line being queried from hex file
  uint16_t lineNumber = 0;

  CONFIG_LED();

  // This flag will tell us whether wireless bootloading needs to start or not
  bool * ptr1;
  ptr1 = (bool*)RAM_END_ADDRESS;  // Memory address at the end of the stack
  bool runUserCode = *ptr1;       // Read value

  // Read user code starting address
  uint16_t * ptr2;
  ptr2 = (uint16_t*)USER_RESET_VECTOR;
  userCodeAddr = *ptr2;
 
  // Disable interrupts
  __disable_interrupt();
  
  // Init core
  initCore();  

  // Valid starting address of user code?
  if (userCodeAddr != 0xFFFF)
  { 
    // Jump to user code if the wireless bootloader was not called from there
    if (runUserCode)
      jumpToUserCode();
  }

  CC430FLASH flash;
  TIMER1A0 timer;
  
  // Init SWAP comms
  swap.init();

  // Transmit default product code only if no user application is still flashed
  if (userCodeAddr == 0xFFFF)
    TRANSMIT_SWAP_STATUS_PCODE();

  // Enter upgrade mode
  state = (uint8_t)SYSTATE_UPGRADE;
  TRANSMIT_SWAP_STATUS_STATE(state);
 
  // Pointer at the begining of user flash
  uint16_t address = USER_ROMADDR;

  while(1)
  { 
    // Wait for new line from server
    while(!lineReceived)
    {
      // Query firmware line
      TRANSMIT_SWAP_QUERY_LINE(lineNumber);
      
      // Repeat query a limited amount of times if necessary
      if (count++ == MAX_REPEAT_QUERY)
      {
        // If the flash was not erased then start user code
        if (firstLine)
          jumpToUserCode();
        // Otherwise continue forever asking
      }
      
      // Start timer
      timer.start(RESPONSE_TIMEOUT);
           
      while (!timer.timeout())
      {
        status = ReadSingleReg(PKTSTATUS);
        bytes = ReadSingleReg(RXBYTES);
        
        // Poll PKSTATUS and number of bytes in the Rx FIFO
        if ((status & 0x01) && bytes)
        {
          while (ReadSingleReg(PKTSTATUS) & 0x01);
          LED_ON();
          // Packet received. Read packet and extract HEX line
          if (readHexLine(lineNumber))
          {
            count = 0;  // Reset counter
            break;
          }
          LED_OFF();
        }
      }
    }

    lineReceived = false;

    // Is the line received OK?
    if (checkCRC(ptrLine, lineLength))
    {  
      if (TYPE_OF_RECORD(ptrLine) == RECTYPE_DATA)
      {
        // Get target address
        uint16_t addrFromHexFile = getTargetAddress(ptrLine);

        // Only for the first line received
        if (firstLine)
        {
          firstLine = false;
          
          // Is the starting address from the hex file different than our user flash address?
          if (addrFromHexFile != address)
          {            
            // Jump to user code
            jumpToUserCode();
          }
          else  // Starting address is OK
          {
            LED_ON();
            // Erase user flash
            do
            {              
              flash.eraseSegment((uint8_t *) address);
              address += 512;
            } while (address < USER_END_ROMADDR);
                                    
            LED_OFF();
          }
        }        
        
        // Save vector table in buffer
        if (addrFromHexFile >= VECTOR_TABLE_ADDR)
        {
          uint8_t row = (addrFromHexFile - VECTOR_TABLE_ADDR);
          row /= 0x10;
          
          for(i=0 ; i<16 ; i++)
          {
            if (i < lineLength-3)
              isrTable[row][i] = ptrLine[i+3];
            else
              isrTable[row][i] = 0xFF;
          }
        }
        else
        {
          // Write line in flash
          flash.write((uint8_t *)addrFromHexFile, ptrLine+3, lineLength-4);         
        }
        
        lineNumber++;
      }
      else  // Probably end of file
      {
        // Erase the vector table segment
        flash.eraseSegment((uint8_t *) VECTOR_TABLE_SEGMENT);

        // Replace their reset vector with our bootloader address
        // this allows the user to provide their own interrupt vectors
        // however, the gdb boot code still runs first
        #ifdef GDB_SERIAL_BOOT
        isrTable[7][0x0E] = 0x00;   // Serial bootloader address = 0x1000
        isrTable[7][0x0F] = 0x10;

        isrTable[3][0x0E] = 0x00;   // Wireless bootloader address = 0x8000
        isrTable[3][0x0F] = 0x80;
        #endif
        
        isrTable[3][0x0C] = 0x00;   // User code address = 0x9000
        isrTable[3][0x0D] = 0x90;

        // Write ISR table        
        for(i=0 ; i<8 ; i++)
          flash.write((uint8_t *)(VECTOR_TABLE_ADDR + i*0x10), isrTable[i], sizeof(isrTable[i]));

        // Jump to user code
        jumpToUserCode();
      }      
    }
  }

  // Enter restart mode
  state = (uint8_t)SYSTATE_RESTART;
  TRANSMIT_SWAP_STATUS_STATE(state);

  return 0;
}

/**
 * initCore
 * 
 * Initialize CC430 core
 */
ALWAYS_INLINE
void initCore(void) 
{
	// Configure PMM
	SetVCore(2);

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

  /**
   * Configure CPU clock for 12MHz
   */
  _BIS_SR(SCG0);              // Disable the FLL control loop
  UCSCTL0 = 0x0000;           // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;        // Select suitable range
  UCSCTL2 = FLLD_1 + 0x16E;   // Set DCO Multiplier
  _BIC_SR(SCG0);              // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
  delayClockCycles(250000L);

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
 * getLineNumber
 *
 * Get number of line from payload
 *
 * @param data payload from packet received
 *
 * @return line number
 */
ALWAYS_INLINE
uint16_t getLineNumber(uint8_t *data)
{
  uint16_t lineNb = data[0];
  lineNb <<= 8;
  lineNb |= data[1];

  return lineNb;
}

/**
 * getTargetAddress
 *
 * Get address from HEX line where the write has to be done
 *
 * @param line line from HEX file
 *
 * @return target address
 */
ALWAYS_INLINE
uint16_t getTargetAddress(uint8_t *line)
{
  uint16_t address = line[0];
  address <<= 8;
  address |= line[1];

  return address;
}

/**
 * checkCRC
 *
 * Check CRC from hex line. The CRC byte (last byte) is a 2's complement
 * of the whole data buffer
 *
 * @param data payload from packet received
 * @param len packet length
 *
 * @return true if CRC is correct
 */
ALWAYS_INLINE
bool checkCRC(uint8_t *data, uint8_t len)
{
  uint8_t crc = len - 4;
  uint8_t i, dataLen = len - 1;
  
  for(i=0 ; i<dataLen ; i++)
    crc += data[i];  

  crc = ~crc + 1;

  if (crc == data[dataLen])
    return true;
  
  return false;
}

/**
 * sleep
 *
 * Enter low-power mode
 */
ALWAYS_INLINE
void sleep(void)
{
  // Power down radio
  swap.radio.setPowerDownState();
  
  // Stop WDT
  WDTCTL = WDTPW | WDTHOLD;

  // Turn off SVSH, SVSM
  PMMCTL0_H = 0xA5;
  SVSMHCTL = 0;
  SVSMLCTL = 0;
  PMMCTL0_H = 0x00;

  // Enter LPM4 with interrupts
  __bis_SR_register(LPM4_bits + GIE);
}

/**
 * jumpToUserCode
 *
 * Jump to user code after exiting SWAP upgrade mode
 */
ALWAYS_INLINE
void jumpToUserCode(void)
{
  // Exit upgrade mode
  uint8_t state = (uint8_t)SYSTATE_RESTART;
  TRANSMIT_SWAP_STATUS_STATE(state);
  
  void (*p)(void);                     // Declare a local function pointer
  p = (void (*)(void))USER_ROMADDR;    // Assign the pointer address
  (*p)();                               // Call the function
}

/**
 * delayClockCycles
 *
 * Clock cycle delay
 *
 * @param n clock cycles to wait
 */
ALWAYS_INLINE
void delayClockCycles(register uint32_t n)
{
    __asm__ __volatile__ (
                "1: \n"
                " dec        %[n] \n"
                " jne        1b \n"
        : [n] "+r"(n));
}
