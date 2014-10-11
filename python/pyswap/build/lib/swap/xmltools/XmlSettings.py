#########################################################################
#
# XmlSettings
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
import os


class XmlSettings(object):
    """
    Main configuration settings: config files and directories
    """
    ## Name/path of the current configuration file
    file_name = "settings.xml"
    ## Debug level (0: no debug, 1: print SWAP packets, 2: print SWAP packets and network events)
    debug = 0
    ## Name/path of the serial configuration file
    serial_file = "serial.xml"
    ## Name/path of the wireless network configuration file
    network_file = "network.xml"
    ## Name/path of the SWAP net status/config file
    swap_file = "swapnet.json"
    ## Directory where all device config files are stored
    device_localdir = None
    ## Remote Devide Definition folder for updates
    device_remote = "http://panstamp.googlecode.com/files/devices.tar"
    ## Automatic udate of local Device Definition folder from internet server
    ## on start-up
    updatedef = False
    ## Name/path of the error log file
    error_file = "swap.err"

    def read(self):
        """
        Read configuration file file
        """
        # Parse XML file
        tree = xml.parse(XmlSettings.file_name)
        if tree is None:
            return
        # Get the root node
        root = tree.getroot()
        # Debug flag
        elem = root.find("debug")
        if elem is not None:
            XmlSettings.debug = int(elem.text)
        # Get "devices" folder
        devices = root.find("devices")
        if devices is not None:
            local = devices.find("local")
            if local is not None:
                XmlSettings.device_localdir = local.text
            remote = devices.find("remote")
            if remote is not None:
                XmlSettings.device_remote = remote.text
                update = devices.find("update")
                if update is not None:
                    XmlSettings.updatedef = update.text.lower() in ["true", "enabled", "yes"]
        # Get serial config file
        elem = root.find("serial")
        if elem is not None:
            XmlSettings.serial_file = elem.text
        # Get network config file
        elem = root.find("network")
        if elem is not None:
            XmlSettings.network_file = elem.text
        # Get SWAP network file
        elem = root.find("swapnet")
        if elem is not None:
            XmlSettings.swap_file = elem.text
        # Get path name of the error log file
        elem = root.find("errlog")
        if elem is not None:
            XmlSettings.error_file = elem.text


    def save(self):
        """
        Save serial port settings in disk
        """
        f = open(self.file_name, 'w')
        f.write("<?xml version=\"1.0\"?>\n")
        f.write("<settings>\n")
        f.write("\t<debug>" + str(self.debug) + "</debug>\n")
        f.write("\t<devices>\n")
        f.write("\t\t<local>" + self.device_localdir + "</local>\n")
        f.write("\t\t<remote>" + self.device_remote + "</remote>\n")
        f.write("\t\t<update>" + str(self.updatedef) + "</update>\n")
        f.write("\t</devices>\n")
        f.write("\t<serial>" + self.serial_file + "</serial>\n")
        f.write("\t<network>" + self.network_file + "</network>\n")
        f.write("\t<swapnet>" + self.swap_file + "</swapnet>\n")
        f.write("</settings>\n")
        f.close()


    def __init__(self, file_name=None):
        """
        Class constructor
        
        @param filename: Path to the configuration file
        """
        # Name/path of the current configuration file
        if file_name is None:
            file_name = "settings.xml"
        XmlSettings.file_name = file_name
        # Read XML file
        self.read()
        
        direc = os.path.dirname(XmlSettings.file_name)
        
        # Convert to absolute paths
        if XmlSettings.device_localdir is None:
            XmlSettings.device_localdir = direc
        else:
            XmlSettings.device_localdir = os.path.join(direc, self.device_localdir)

        XmlSettings.serial_file = os.path.join(direc, self.serial_file)
            
        XmlSettings.network_file = os.path.join(direc, self.network_file)

        XmlSettings.swap_file = os.path.join(direc, self.swap_file)
