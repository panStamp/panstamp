#########################################################################
#
# Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
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
# along with panLoader; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Mar 12, 2012$"
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
    ## Database (0: no db, 1: sqlitedb)
    database=0
    ## Latitude
    latitude = 0
    ## Longitude
    longitude = 0
    ## Name/path of the error log file
    error_file = "error.log"

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
        # Get database flag
        elem = root.find("database")
        if elem is not None:
            XmlSettings.database = elem.text.lower() in ["1", "true", "enable"]
        else:
            XmlSettings.database = False
        # Get location section
        location = root.find("location")
        if location is not None:
            latitude = location.find("latitude")
            if latitude is not None:
                try:
                    XmlSettings.latitude = float(latitude.text)
                except ValueError:
                    XmlSettings.latitude = int(latitude.text)
            longitude = location.find("longitude")
            if longitude is not None:
                try:
                    XmlSettings.longitude = float(longitude.text)
                except ValueError:
                    XmlSettings.longitude = int(longitude.text)
        # Get path name of the error log file
        elem = root.find("errlog")
        if elem is not None:
            XmlSettings.error_file = elem.text


    def save(self):
        """
        Save serial port settings in disk
        """
        f = open(XmlSettings.file_name, 'w')
        f.write("<?xml version=\"1.0\"?>\n")
        f.write("<settings>\n")
        f.write("\t<debug>" + str(XmlSettings.debug) + "</debug>\n")
        f.write("\t<database>" + str(XmlSettings.database) + "</database>\n")
        f.write("\t<location>\n")
        f.write("\t\t<latitude>" + str(XmlSettings.latitude) + "</latitude>\n")
        f.write("\t\t<longitude>" + str(XmlSettings.longitude) + "</longitude>\n")
        f.write("\t</location>\n")
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
        XmlSettings.error_file = os.path.join(direc, XmlSettings.error_file)
