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
 * Main class
 */
public class HexLine
{
  /**
   * Data length
   */
  int length;
  
  /**
   * Memory address
   */
  int address;
  
  /**
   * Type of line
   */
  int type;
  
  /**
   * Data from HEX line
   */
  byte[] data;
  
  /**
   * CRC field
   */
  int crc;
  
  /**
   * class constructor
   * 
   * @param line String containing a line from an HEX file
   */
  public HexLine(String line)
  {
    int i, j=0;
    
    length = Integer.parseInt(line.substring(1, 3), 16);
    address = Integer.parseInt(line.substring(3, 7), 16);
    type = Integer.parseInt(line.substring(7, 9), 16);
    data = new byte[length];
    
    for(i=9 ; i<(9 + length * 2); i+=2)
    {
      int b = Integer.parseInt(line.substring(i, i+2), 16);
      data[j++] = (byte)b;
    }
    
    crc = Integer.parseInt(line.substring(9+length*2, 11+length*2), 16);
  }
}
