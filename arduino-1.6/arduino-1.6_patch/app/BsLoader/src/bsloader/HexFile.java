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
 * Creation date: 12/16/2014
 */
package bsloader;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Main class
 */
public class HexFile
{ 
  /**
   * Array of HEX lines
   */
  ArrayList<HexLine> hexLines = new ArrayList<HexLine>();
  
  /**
   * Class constructor
   * 
   * @param path Path to the HEX file
   */
  public HexFile(String path) throws BslException
  {
    File hexFile =  new File(path);
    
    try
    {
      BufferedReader br = new BufferedReader(new FileReader(hexFile));
 
      String line;
      while ((line = br.readLine()) != null)
      {
        HexLine hexLine = new HexLine(line);        
        hexLines.add(hexLine);
      }
 
      br.close();
    }
    catch (FileNotFoundException ex)
    {
      throw new BslException(hexFile.getPath() + " not found");
    }
    catch (IOException ex)
    {
      throw new BslException("Unable to close HEX file " + hexFile.getPath());
    }
  }
}
