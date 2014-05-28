#########################################################################
#
# XmlSerial
#
# Copyright (c) 2011 panStamp <contact@panstamp.com>
# 
# This file is part of the panStamp project.
# 
# panStamp  is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# any later version.
# 
# panStamp is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with panStamp; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__ ="$Aug 20, 2011 10:36:00 AM$"
#########################################################################

from swap.SwapException import SwapException

import xml.etree.ElementTree as xml

class XmlSerial(object):
    """
    Serial configuration settings
    """
    def read(self):
        """ 
        Read configuration file
        """
        # Parse XML file
        tree = xml.parse(self.file_name)
        if tree is None:
            return
        # Get the root node
        root = tree.getroot()
        # Get serial port
        elem = root.find("port")
        if elem is not None:
            self.port = elem.text
        # Get serial speed
        elem = root.find("speed")
        if elem is not None:
            self.speed = int(elem.text)
    
    def save(self):
        """
        Save serial port settings in disk
        """
        try:
            f = open(self.file_name, 'w')
            f.write("<?xml version=\"1.0\"?>\n")
            f.write("<serial>\n")
            f.write("\t<port>" + self.port + "</port>\n")
            f.write("\t<speed>" + str(self.speed) + "</speed>\n")
            f.write("</serial>\n")
            f.close()
        except:
            raise SwapException("Unable to save " + self.file_name)
    
    def __init__(self, file_name = "serial.xml"):
        """
        Class constructor
        
        @param filename: Path to the serial configuration file
        """
        ## Name/path of the current configuration file
        self.file_name = file_name
        ## Name/path of the serial port
        self.port = "/dev/ttyUSB0"
        ## Speed of the serial port in bps
        self.speed = 9600
        # Read XML file
        self.read()

  
