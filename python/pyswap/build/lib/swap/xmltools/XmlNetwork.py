#########################################################################
#
# XmlNetwork
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

import xml.etree.ElementTree as xml


class XmlNetwork(object):
    """
    Wireless network configuration settings
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
        # Get frequency channel
        elem = root.find("channel")
        if elem is not None:
            self.freq_channel = int(elem.text)
        # Get Network ID
        elem = root.find("netid")
        if elem is not None:
            self.network_id = int(elem.text, 16)
        # Get device address
        elem = root.find("address")
        if elem is not None:
            self.devaddress = int(elem.text)
        # Get security option
        elem = root.find("security")
        if elem is not None:
            self.security = int(elem.text)
        # Get wncryption password
        elem = root.find("password")
        if elem is not None:
            if elem.text is not None:
                self.password = elem.text


    def save(self):
        """
        Save network settings in file
        """
        f = open(self.file_name, 'w')
        f.write("<?xml version=\"1.0\"?>\n")
        f.write("<network>\n")
        f.write("\t<channel>" + str(self.freq_channel) + "</channel>\n")
        f.write("\t<netid>" + hex(self.network_id)[2:] + "</netid>\n")
        f.write("\t<address>" + str(self.devaddress) + "</address>\n")
        f.write("\t<security>" + str(self.security) + "</security>\n")
        if self.password != "":
            f.write("\t<password>" + self.password + "</password>\n")
        f.write("</network>\n")
        f.close()

    
    def __init__(self, file_name="network.xml"):
        """
        Class constructor
        
        @param filename: Path to the network configuration file
        """
        ## Name/path of the current configuration file
        self.file_name = file_name
        ## Frequency channel
        self.freq_channel = 0
        ## Network identifier (synchronization word)
        self.network_id = 0xB547
        ## Device address
        self.devaddress = 1
        ## Security option
        self.security = 0
        ## Encryption password (12 bytes)
        self.password = ""
        # Read XML file
        self.read()
  
