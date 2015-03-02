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

import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;

/**
 * Main class
 */
public class BslUart
{
  /**
   * Byte to char conversion
   */
  final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();
  
  /**
   * Verbose flag
   */
  private boolean verbose = false;
  
  /**
   * Path to the serial port
   */
  private String serialPath;
  
  /**
   * Serial port
   */
  private SerialPort serialPort;

  /**
   * Rx buffer
   */
  private byte[] rxBuffer = new byte[1132];
  
  /**
   * Amount of bytes waiting to be read from serial port
   */
  private int rxLength = 0;
    
  /**
   * Class constructor
   * 
   * @param path Path to the serial port
   * @param verbose Print serial traffic if true
   */
  public BslUart(String path, boolean verbose) throws BslException
  {
    serialPath = path;
    this.verbose = verbose;
    open();    
    invokeBsl();
  }

  /**
   * Open serial comms
   */
  public void open() throws BslException
  {
    try
    {
      serialPort = new SerialPort(serialPath);
      serialPort.openPort();
      serialPort.setParams(SerialPort.BAUDRATE_9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_EVEN);

      int mask = SerialPort.MASK_RXCHAR + SerialPort.MASK_CTS + SerialPort.MASK_DSR;//Prepare mask
      serialPort.setEventsMask(mask);
      serialPort.addEventListener(new SerialPortReader());
    }
    catch (SerialPortException ex)
    {
      throw new BslException("Unable to start serial comms with BSL. Reason:" + ex.getMessage());
    }
  }
  
  /**
   * Set high serial speed
   * 
   * @throws bsloader.BslException
   */
  public void setHighSpeed() throws BslException
  {
    try
    {
      serialPort.setParams(SerialPort.BAUDRATE_38400, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_EVEN, true, false);
    }
    catch (SerialPortException ex)
    {
      throw new BslException("Unable to set serial speed. Reason:" + ex.getMessage());
    }
  }
  
  /**
   * Close serial comms
   */
  public void close() throws BslException
  {
    try
    {
      serialPort.closePort();      
    }
    catch (SerialPortException ex)
    {
      throw new BslException("Unable to close serial comms with BSL. Reason:" + ex.getMessage());
    }
  }

  /**
   * Invoke BSL by driving the correct RST+TEST sequence
   * 
   * @return True if BSL is successfully invoked
   */
  private boolean invokeBsl() throws BslException
  {
    try
    {
      // RESET + TEST sequence
      setRESET(true);
      setTEST(true);
      sleep(250);
      setRESET(false);
      sleep(10);
      setTEST(true);
      sleep(10);
      setTEST(false);
      sleep(10);
      setTEST(true);
      sleep(10);
      setTEST(false);
      sleep(10);
      setTEST(true);
      sleep(10);
      setTEST(false);
      sleep(10);
      setRESET(true);
      sleep(10);
      setTEST(true);
      sleep(250);
    }
    catch (SerialPortException ex)
    {
      throw new BslException("Unable to invoke BSL. Reason: " + ex.getMessage());
    }

    return true;
  }
  
  /**
   * Exit from BSL by driving the correct RST+TEST sequence
   * 
   * @return True if BSL is successfully quitted
   */
  private boolean quitBsl() throws BslException
  {
    try
    {
      // RESET + TEST sequence
      //setRESET(true);
      //setTEST(true);
      //sleep(250);
      setRESET(false);
      sleep(100);
      setTEST(false);
      sleep(50);
      setTEST(true);
      sleep(50);
      setTEST(false);
      sleep(50);
      setTEST(true);
      sleep(50);
      setTEST(false);
      sleep(50);
      setTEST(true);
      sleep(50);
      setRESET(true);
      sleep(250);
    }
    catch (SerialPortException ex)
    {
      throw new BslException("Unable to exit from BSL. Reason: " + ex.getMessage());
    }

    return true;
  }

  /**
   * Drive RESET line from MSP430 MCU
   * 
   * @param state True for HIGH state. False for LOW state
   */
  private void setRESET(boolean state) throws SerialPortException
  {
    serialPort.setDTR(!state);
  }
  
  /**
   * Drive TEST line from MSP430 MCU
   * 
   * @param state True for HIGH state. False for LOW state
   */
  private void setTEST(boolean state) throws SerialPortException
  {
    serialPort.setRTS(!state);
  }
  
  /**
   * Read serial packet received
   * 
   * @return array of bytes received
   * @throws bsloader.BslException
   */
  public byte[] read() throws BslException
  {
    long startTime = System.currentTimeMillis();
    long elapsedTime;
    
    while (rxLength == 0)
    {
      sleep(1);
      elapsedTime = System.currentTimeMillis() - startTime;
      
      if (elapsedTime == 100)
        throw new BslException("No response received from BSL");
    }

    // Wait some time for the rest of the response from BSL
    sleep(50);
   
    byte[] buffer = Arrays.copyOfRange(rxBuffer, 0, rxLength);

    if (verbose)
    {
      String reply = bytesToHex(buffer, buffer.length);      
      System.out.println("IN <--- (" + buffer.length + " bytes) " + reply);
    }
    
    // Empty global Rx buffer
    rxLength = 0;
            
    return buffer;
  }

  /**
   * Write bytes through the serial port
   * 
   * @param buffer to be sent
   * @throws bsloader.BslException
   */
  public void write(byte[] buffer) throws BslException
  {
    try
    {
      if (verbose)
      {
        String reply = bytesToHex(buffer, buffer.length);      
        System.out.println("OUT --> (" + buffer.length + " bytes) " + reply);
      }
    
      // Send command
      serialPort.writeBytes(buffer);
    }
    catch (SerialPortException ex)
    {
      throw new BslException("Unable to send command to BSL. Reason: " + ex.getMessage());
    }
  }
   
  /**
   * Convert array of bytes into a printable hex string
   * 
   * @param bytes array of bytes
   * @param len amount of bytes to be converted
   * 
   * @return printable hex string
   */
  public static String bytesToHex(byte[] bytes, int len) {
      char[] hexChars = new char[bytes.length * 2];
      for ( int j = 0; j < len; j++ ) {
          int v = bytes[j] & 0xFF;
          hexChars[j * 2] = hexArray[v >>> 4];
          hexChars[j * 2 + 1] = hexArray[v & 0x0F];
      }
      return new String(hexChars);
  }
  
  /**
   * Simple sleeping function
   * 
   * @param millis Milliseconds to sleep
   */
  private void sleep(int millis)
  {
    try {
      Thread.sleep(millis);
    } catch(InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
  }
          
  /**
   * Serial reader class
   */
  class SerialPortReader implements SerialPortEventListener
  {
    @Override
    public void serialEvent(SerialPortEvent event)
    {
      // Serial data received?
      if(event.isRXCHAR())
      {
        int len = event.getEventValue();
               
        try
        {
          byte[] buffer = serialPort.readBytes(len);
          
          int i;
          for (i=0 ; i<len ; i++)
            rxBuffer[rxLength + i] = buffer[i];
          rxLength += len;
          
          //String reply = bytesToHex(rxBuffer, rxLength);      
          //System.out.println(reply);
        }
        catch (SerialPortException ex)
        {
          System.out.println(ex);
        }
      }
    }
  }
}
