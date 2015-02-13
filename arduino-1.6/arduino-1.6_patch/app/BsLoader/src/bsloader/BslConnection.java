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
 * Creation date: 12/04/2014
 */
package bsloader;

import java.util.Arrays;

/**
 * Main class
 */
public class BslConnection
{
  /**
   * BSL core commands
   */
  private static final int CMD_RX_DATA_BLOCK = 0x10;
  private static final int CMD_RX_DATA_BLOCK_FAST = 0x1B;
  private static final int CMD_RX_PASSWORD = 0x11;
  private static final int CMD_ERASE_SEGMENT = 0x12;
  private static final int CMD_LOCK_UNLOCK_INFO = 0x13;
  private static final int CMD_MASS_ERASE = 0x15;
  private static final int CMD_CRC_CHECK = 0x16;
  private static final int CMD_LOAD_PC = 0x17;
  private static final int CMD_TX_DATA_BLOCK = 0x18;
  private static final int CMD_TX_BSL_VERSION = 0x19;
  private static final int CMD_TX_BUFFER_SIZE = 0x1A;

  /**
   * BSL core responses
   */
  private static final int BSL_OK = 0x00;
  private static final int BSL_FLASH_WRITE_NOK = 0x01;
  private static final int BSL_FLASH_FAIL_BIT = 0x02;
  private static final int BSL_VOLTAGE_CHANGED = 0x03;
  private static final int BSL_LOCKED = 0x04;
  private static final int BSL_PASSWORD_ERROR = 0x05;
  private static final int BSL_BYTE_WRITE_FORBIDDEN = 0x06;
  private static final int BSL_UNKNOWN_COMMAND = 0x07;
  private static final int BSL_PACKET_TOO_LARGE = 0x08;
  private static final int BSL_DATA_BLOCK = 0x3A;
  private static final int BSL_MESSAGE = 0x3B;  
  
  /**
   * UART error messages
   */
  private static final int UART_ACK = 0x00;
  private static final int UART_HEADER_NOK = 0x51;
  private static final int UART_CRC_NOK = 0x52;
  private static final int UART_PACKET_SIZE_ZERO = 0x53;
  private static final int UART_PACKET_SIZE_EXCEEDS = 0x54;
  private static final int UART_UNKNOWN_ERROR = 0x55;
  private static final int UART_UNKNOWN_BAUDRATE = 0x56;
  
  /**
   * Start of main flash for CC430F5137
   */
  private static final int MEM_START_MAIN_FLASH = 0x8000;
   
  /**
   * BSL UART object
   */
  BslUart uart;

  /**
   * Build path or location of the firmware image
   */
  private String buildPath;
  
  /**
   * Class constructor
   * 
   * @param buildPath Path to the firmware image (hex file)
   * @param strPort Name or path of the serial port
   * @param verbose Print serial traffic if true
   */
  public BslConnection(String buildPath, String strPort, boolean verbose) throws BslException
  {
    this.buildPath = buildPath;
    uart = new BslUart(strPort, verbose);
  }
 
  /**
   * Close connection
   */
  public void close() throws BslException
  {
    uart.close();
    uart.open();
    uart.close();
  }
 
  /**
   * Enter password to unlock the flash
   * 
   * @throws bsloader.BslException
   * 
   * @return True if command successes
   */
  public boolean unlock() throws BslException
  {
    byte[] command = new byte[33];
    Arrays.fill( command, (byte) 0xFF );
    command[0] = CMD_RX_PASSWORD;
    
    // Transmit command to BSL
    sendCommand(command);
    
    // check reply from BSL
    return checkReply();
  }
  
  public boolean startApp() throws BslException
  {
    byte[] command = {(byte)CMD_LOAD_PC, (byte)0x00, (byte)0x80, (byte)0x00};
    
    // Transmit command to BSL
    sendCommand(command);
    
    // check reply from BSL
    //return checkReply();
    return true;
  }

  /**
   * Configure serial speed
   * 
   * @param speed New serial speed in bps
   * 
   * @throws bsloader.BslException
   */
  private void setSpeed(int speed) throws BslException
  {
    byte config;
    
    switch(speed)
    {
      case 19200:
        config = 0x03;
        break;
      case 38400:
        config = 0x04;
        break;
      case 57600:
        config = 0x05;
        break;
      case 115200:
        config = 0x06;
        break;
      default: // 9600
        config = 0x01;
        break;
    }
    
    byte[] command = {0x52, config};
    
    // Transmit command to BSL
    sendCommand(command);      
    
    checkReply();
  }
  
  /**
   * Enter high serial speed mode
   * @throws bsloader.BslException
   */
  public void enterHighSpeed() throws BslException
  {
    setSpeed(38400);
    
    uart.setHighSpeed();
  }
  
  /**
   * Apply mass erase by sending an incorrect password to BSL
   * 
   * @throws bsloader.BslException
   * 
   * @return True if command successes
   */
  public boolean massErase() throws BslException
  {
    byte[] command = {CMD_MASS_ERASE};
    
    // Transmit command to BSL
    sendCommand(command);
    
    // check reply from BSL
    return checkReply();
  }

  /**
   * Erase main flash
   * 
   * @throws bsloader.BslException
   * 
   * @return True if command successes
   */
  public boolean mainFlashErase() throws BslException
  {   
    byte[] command = new byte[33];
    // Incorrect password to force mass erase
    Arrays.fill( command, (byte) 0xEE );
    command[0] = CMD_RX_PASSWORD;
    
    // Transmit command to BSL
    sendCommand(command);
    
    // check reply from BSL
    return !checkReply();
  }

  /**
   * Write data block into main flash at a given address
   * 
   * @param address Starting address for the data write
   * @param data data buffer to be written. Maximum length = 250 bytes
   * @param length Amount of bytes to be written
   * 
   * @return True if command successes
   * 
   * @throws bsloader.BslException
   */
  public boolean writeFlash(long address, byte[] data, int length) throws BslException
  {
    byte highAddr = (byte)(address >> 16);
    byte midAddr = (byte)(address >> 8);
    byte lowAddr = (byte)(address & 0xFF);
       
    byte[] command = new byte[length + 4];

    command[0] = CMD_RX_DATA_BLOCK;
    command[1] = lowAddr;
    command[2] = midAddr;
    command[3] = highAddr;
    
    int i;
    for(i=0 ; i<data.length ; i++)
      command[i+4] = data[i];
    
    // Transmit command to BSL
    sendCommand(command);
    
    // check reply from BSL
    return checkReply();
  }

  /**
   * Read data block from main flash
   * 
   * @param address Address to start reading
   * @param length Amount of bytes to be read
   * 
   * @return data block received
   * 
   * @throws bsloader.BslException
   */
  public byte[] readFlash(long address, int length) throws BslException
  {
    byte highAddr = (byte)(address >> 16);
    byte midAddr = (byte)(address >> 8);
    byte lowAddr = (byte)(address & 0xFF);
       
    byte[] command = new byte[6];

    command[0] = CMD_TX_DATA_BLOCK;
    command[1] = lowAddr;
    command[2] = midAddr;
    command[3] = highAddr;
    command[4] = (byte)length;
    command[5] = 0;
    
    sendCommand(command);
    
    // Read reply from BSL
    byte[] reply = readReply();

    if (reply[0] == BSL_DATA_BLOCK)
    {
      if (reply.length == length+1)
        return Arrays.copyOfRange(reply, 1, length+1);
    }
    
    byte[] empty = {0};
    return empty;
  }
 
  /**
   * Read reply from BSL
   * 
   * @return BSL core message received
   * @throws BslException 
   */
  private byte[] readReply() throws BslException
  {
    byte[] reply = uart.read();
    
    // Convert to array of integers
    int[] intReply = new int[reply.length];
    int i;
    
    for(i=0 ; i<reply.length ; i++)
    {
      if (reply[i] < 0)
        intReply[i] = 0x100 + reply[i];
      else
        intReply[i] = reply[i];
    }
      
    // Check first byte received from BSL
    if (intReply[0] != UART_ACK)
    {
      String reason;
      
      switch(intReply[0])
      {
        case UART_HEADER_NOK:
          reason = "Incorrect command header";
          break;
        case UART_CRC_NOK:
          reason = "Incorrect command CRC field";
          break;
        case UART_PACKET_SIZE_ZERO:
          reason = "Command size is zero";
          break;
        case UART_PACKET_SIZE_EXCEEDS:
          reason = "Command size is too large";
          break;
        case UART_UNKNOWN_ERROR:
          reason = "Unknown error";
          break;
        case UART_UNKNOWN_BAUDRATE:
          reason = "Unknown baudrate";
          break;
        default:
          reason = "Command error";
          break;
      }
      
      throw new BslException("Command not acepted by BSL: " + reason);
    }
    
    //if (intReply.length < 4)
    //  throw new BslException("Reply received from BSL is too short");
    
    if (intReply.length >= 4)
    {
      // Get length of core message
      int coreLen = intReply[3] << 8 | intReply[2];

      // Get core message
      byte[] coreMsg = Arrays.copyOfRange(reply, 4, 4 + coreLen);

      // Check CRC
      int crc_ok = crc16(coreMsg);        

      // Get CRC field
      int crc = (int)intReply[intReply.length-1] << 8 | (int)intReply[intReply.length-2];

      if (crc != crc_ok)
        throw new BslException("CRC mismatch in BSL response");

      return coreMsg;
    }
    
    byte nothing[] = {0};
    return nothing;
  }
  
  /**
   * send command to BSL
   * 
   * @param command Array of bytes forming the command
   */
  private void sendCommand(byte[] command) throws BslException
  {
    int i;
    byte[] buffer = new byte[command.length + 5];   
    int length = command.length;
      
    // Message header
    buffer[0] = (byte) 0x80;
       
    // Command length
    buffer[1] = (byte)(length & 0xFF);       // Low byte
    buffer[2] = (byte)((length >> 8) & 0xFF); // High byte
    
    // BSL core command
    for(i=0 ; i<command.length ; i++)
      buffer[i+3] = (byte)command[i];

    // CRC
    int crc = crc16(command); 
    
    buffer[i+3] = (byte)(crc & 0xFF);
    buffer[i+4] = (byte)((crc >> 8) & 0xFF);

    uart.write(buffer);
  }
  
  /**
   * Check reply received from BSL
   * 
   * @return True in case of positive reply
   */
  private boolean checkReply() throws BslException
  {
    // Read reply from BSL
    byte[] reply = readReply();
    
    if (reply.length > 1)
    {
      if (reply[0] == BSL_MESSAGE)
      {
        if (reply[1] == BSL_OK)
          return true;
      }
    }
    
    return false;
  }
  
  /**
   * CRC16 calculation
   * 
   * @param buffer array of bytes to get the CRC from
   * 
   * @return CRC16 value
   */
  static int crc16(byte[] buffer)
  {
    int crc = 0xFFFF;
    int x;
    
    for (byte b : buffer)
    {
      x = ((crc>>8) ^ b) & 0xff;
      x ^= x>>4;
      crc = (crc << 8) ^ (x << 12) ^ (x <<5) ^ x;
    }
    crc &= 0xffff;
    
    return crc;
  }
}
