/**
 * Copyright (c) 2013 CITECAM
 * 
 * This file is part of the wasteless project.
 * 
 * Author: panStamp
 * Creation date: 07/03/2013
 */

#ifndef _SRAM_H
#define _SRAM_H

#include "SPI.h"

/**
 * SRAM opcodes
 */
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

/**
 * SRAM Hold line override
 */
#define HOLD 1

/**
 * SRAM modes
 */
#define BYTE_MODE (0x00 | HOLD)
#define PAGE_MODE (0x80 | HOLD)
#define STREAM_MODE (0x40 | HOLD)

/**
 * Clock speeds
 */
#define RAMCLK4M   0
#define RAMCLK1M   1
#define RAMCLK250K 2
#define RAMCLK125K 3

/**
 * ADDRESSES
 */
#define START_ADDR_DM_INFO      0x0000      // Starting address for any DM entry

/**
 * LENGTHS
 */
#define DATA_LENGTH_DM          10

/**
 * Maximum amount of queued data
 */
#define DATA_MAX_QUEUE          16000
/**
 * MACROS
 */
#define getLength()             dmCount
#define removeLastDm()          dmCount--


class SRAM
{
  private:
    /**
     * Chip select pin
     */
    uint8_t csPin;

    /**
     * SPI interface
     */
    SPIClass spi;

    /**
     * Operation mode
     */
    unsigned char operationMode;

    /**
     * chipSelect
     * 
     * Enable chip via CS
     * 
     * @param state True to select chip. False to unselect
     */
     void inline chipSelect(bool state)
     {
       digitalWrite(csPin, !state);
     }

    /**
     * setMode
     *
     * Set operation mode
     *
     * @param mode Operation mode
     */
    void setMode(char mode);
    
  public:   
    /**
     * Class constructor
     * 
     * Arduino digital pin used as chip select
     */
    inline SRAM(uint8_t pin)
    {
      csPin = pin;
      operationMode = 0xFF;
    }

    /**
     * begin
     * 
     * Initialize SPI comms
     */
    void inline begin(void)
    {
      pinMode(csPin, OUTPUT);
      chipSelect(false);
      spi.begin();
    }
    
    /**
     * readByte
     *
     * Read byte from SRAM
     *
     * @param address Memory address
     *
     * @return Byte read
     */
    unsigned char readByte(unsigned int address);

    /**
     * writeByte
     *
     * Write byte to SRAM
     *
     * @param address Memory address
     * @param data Data byte to be written
     */
    void writeByte(unsigned int address, char data);

    /**
     * readDmData
     *
     * Read DM information from memory in a FIFO way
     *
     * @param dm Pointer to dm object receiving the information
     *
     * @return False if FIFO is empty
     */
    //bool readDmData(DM *dm);

    /**
     * saveDmData
     *
     * Write DM information
     *
     * @param dm Pointer to DM object
     *
     * @return true if the infromation was saved successfully. Return false otherwise
     */
    //bool saveDmData(DM *dm);

    /**
     * removeFirstItem
     *
     * Remove first item from FIFO and arrange the entire queue
     */
    //void removeFirstItem(void);
};

#endif
