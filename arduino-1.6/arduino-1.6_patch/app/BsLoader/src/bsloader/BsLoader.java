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
   * @param verif Verify image
   * @param verbose Print serial traffic if true
   */
  public BsLoader(String buildPath, String strPort, boolean verif, boolean verbose)
  {
    hexFilePath = buildPath;
    int tries = 3;
    boolean tryAgain = true;
    boolean success = false;
    
    while ((tries > 0) && tryAgain)
    {
      tries--;
      tryAgain = false;
      
      try
      {
        // Connect
        System.out.println("Connecting to target board");
        bsl = new BslConnection(buildPath, strPort, verbose);
           
        // Mass erase (main + info flash)
        System.out.println("Erasing main flash...");
        if (bsl.massErase())
        {
          System.out.println("OK");
       
          // Unlock flash
          System.out.println("Unlocking flash...");
          if (bsl.unlock())
          {
            System.out.println("OK");                       
            
            System.out.println("Switching to 38400 bps...");
            bsl.enterHighSpeed();
                       
            // Write firmware image
            System.out.println("Writing data into main flash...");
            if (writeImageFast())
            {
              System.out.println("OK");
              
              if (verif)
              {
                // Verify firmware image from flash
                System.out.println("Verifying data from main flash...");
                if (verifImage())
                {
                  System.out.println("OK");
                  success = true;
                }
                else
                  System.out.println("ERROR");
              }
              else
                success = true;
            }
            else
              System.out.println("ERROR");
          }
          else
            System.out.println("ERROR");
        }
        else
          System.out.println("ERROR");

      }
      catch (BslException ex)
      {
        System.out.println(ex);
        
        if (ex.toString().contains("CRC mismatch"))
        {
          System.out.println("Trying again...");
          tryAgain = true;
        }
        if (ex.toString().contains("No response"))
        {
          System.out.println("Trying again...");
          tryAgain = true;
        }
      }
      finally
      {
        try
        {
          // Close connection
          System.out.println("Closing connection with BSL...");
          bsl.close();
          
          if (success)
            System.out.println("\nNew image uploaded successfully");
        }
        catch (BslException ex)
        {
          System.out.println(ex);
        }
      }
    }
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
   * Load firmware image (.hex) into target board running BSL
   * 
   * @return true if image is successfully loaded
   */
  public boolean writeImageFast() throws BslException
  {
    int length = 0;
    int address = -1;
    byte[] data = new byte[248];
    int nbOfLines = 0;
    
    HexFile hexFile = new HexFile(hexFilePath);
    
    int i;
    for (HexLine line : hexFile.hexLines)
    {          
      if (line.type == 0)
      {
        if (address == -1)
          address = line.address;
      
        nbOfLines++;
        for (byte  b: line.data)
          data[length++] = b;           
        
        // Send up to 15 lines from the HEX file per packet
        if (nbOfLines == 15)
        {
          nbOfLines = 0;
          if (!bsl.writeFlash(address, data, length))
            return false;

          length = 0;
          address = -1;              
        }
      }
    }
    if (nbOfLines > 0)
    {
      if (!bsl.writeFlash(address, data, length))
        return false;           
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
    boolean verbose = false;
    boolean verif = false;
    
    if (args[2].equals("--verif-on"))
      verif = true;
    
    if (args[3].equals("--verbose-on"))
      verbose = true;

    BsLoader uploader = new BsLoader(args[0], args[1], verif, verbose);
  }
}
