#########################################################################
#
# hexfile
#
# Copyright (c) 2014 Daniel Berenguer <dberenguer@usapiens.com>
#
# This file is part of the lagarto project.
#
# lagarto  is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# lagarto is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with panStamp and swapdmt; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Jun 24, 2014$"
#########################################################################

import os
import sys

working_dir = os.path.dirname(__file__)
firmware_dir = os.path.join(working_dir, "firmware")
sys.path.append(firmware_dir) 


class HexLine():
    """
    HEX line management class
    """

    def __init__(self, line):
        """
        Class constructor

        @param line: Line extracted from HEX file   
        """ 
        self.length = int(line[1:3], 16)
        self.address = int(line[3:7], 16)
        self.type = int(line[7:9], 16)
        self.data = []
        
        # Capture data line, including hex address, type and CRC
        for i in range(3, 3 + ((self.length+4) * 2), 2):
            byte = int(line[i:i + 2], 16)
            self.data.append(byte)
        
        self.crc = int(line[9 + self.length : 11 + self.length], 16)
        
           
class HexFile():
    """
    HEX file management class
    """  
    
    
    def get_first_line(self):
        """
        Get first line from list and reset navigation index
        
        @return first line
        """
        self.index = 0
        return self.get_line()        
    
    
    def get_next_line(self):
        """
        Get next line from list
        
        @return next line
        """
        if len(self.lines) > self.index:
            line = self.lines[self.index]
            self.index += 1
            return line
        else:
            return None 

    
    def get_line(self, nb):
        """
        Get a given line from list
        
        @param nb number of line requested
        @return line requested
        """
        if len(self.lines) > nb:
            line = self.lines[nb]
            self.index = nb
            return line
        else:
            return None 


    def __init__(self, file_name):
        """
        Class constructor

        @param file_name: name of the HEX file        
        """             
        self.index = 0
        self.lines = []
        self.nbof_data_lines = 0
        
        try:
            hexfile = open(file_name, "r")       
            for line in hexfile.readlines():
                hexline = HexLine(line)
                self.lines.append(hexline)
                # If this is a data line
                if hexline.type == 0:
                    self.nbof_data_lines += 1

            hexfile.close()
        except IOError:
            print "Can not find " + file_name
        except:
            print "Incorrect file format"
