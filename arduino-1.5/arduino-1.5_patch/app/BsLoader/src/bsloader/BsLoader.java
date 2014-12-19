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

/**
 *
 * @author daniel
 */
public class BsLoader
{
  /**
   * BSL connection
   */
  BslConnection bsl;
  
  /**
   * Path to the HEX file
   */
  String hexFilePath;
  
  /**
   * Class constructor
   * 
   * @param buildPath Path to the firmware image (hex file)
   * @param strPort Name or path of the serial port
   */
  public BsLoader(String buildPath, String strPort)
  {
    hexFilePath = buildPath;
    
    try
    {
      // Connect
      System.out.println("Connecting to target board");
      bsl = new BslConnection(buildPath, strPort);
      
      // Mass erase (main + info flash)
      System.out.println("Erasing main flash");
      if (bsl.massErase())
        System.out.println("OK");
      else
        System.out.println("ERROR");
      // Unlock flash
      System.out.println("Unlocking flash...");
      if (bsl.unlock())
        System.out.println("OK");
      else
        System.out.println("ERROR");
      
      // Write firmware image
      System.out.println("Writing data into main flash...");
      if (writeImage())
        System.out.println("OK");
      else
        System.out.println("ERROR");
      
      // Verify firmware image from flash
      System.out.println("Verifying data from main flash...");
      if (verifImage())
        System.out.println("OK");
      else
        System.out.println("ERROR");
      
      // Close connection
      System.out.println("Closing connection with BSL...");
      bsl.close();
    }
    catch (BslException ex)
    {
      System.out.println(ex);
    }
  }

  /**
   * Handle verbose flag
   * 
   * @param val Set to true to enable verbose. Set to false to disable
   */
  public void setVerbose(boolean val)
  {
    bsl.setVerbose(val);
  }
  
  /**
   * Load firmware image (.hex) into target board running BSL
   * 
   * @return true if image is successfully loaded
   */
  public boolean writeImage() throws BslException
  {   
    HexFile hexFile = new HexFile(hexFilePath);
    
    int i;
    for (HexLine line : hexFile.hexLines)
    {
      if (line.type == 0)
      {
        if (!bsl.writeFlash(line.address, line.data, line.length))
          return false;
      }
    }
    
    return true;
  }

  /**
   * Verify loaded image against HEX file
   * 
   * @return true if image is successfully verified
   */
  public boolean verifImage() throws BslException
  {   
    HexFile hexFile = new HexFile(hexFilePath);
    
    int i;
    
    for (HexLine line : hexFile.hexLines)
    {
      if (line.type == 0)
      {
        byte[] data = bsl.readFlash(line.address, line.length);
        
        for(i=0 ; i<line.length ; i++)
        {
          if (line.data[i] != data[i])
            return false;
        }       
      }
    }
    
    return true;
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
   * Main method
   */
  public static void main(String [ ] args)
  {
    String args0 = "/home/daniel/Documents/firmware/simpletest.hex";
    String args1 = "/dev/ttyUSB0";
    String args2 = "--verbose-off";

    //BsLoader uploader = new BsLoader(args[0], args[1]);
    BsLoader uploader = new BsLoader(args0, args1);
    
    if (args2.equals("--verbose-on"))
      uploader.setVerbose(true);
  }
}
