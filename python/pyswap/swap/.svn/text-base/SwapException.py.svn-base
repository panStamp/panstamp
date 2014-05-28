#########################################################################
#
# SwapException
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

from xmltools.XmlSettings import XmlSettings

import time, datetime, os

class SwapException(Exception):
    """
    Main exception class for pySwap
    """
    def display(self):
        """
        Print exception description
        """
        print datetime.datetime.fromtimestamp(self.timestamp).strftime("%d-%m-%Y %H:%M:%S"), self.description


    def log(self):
        """
        Write exception in log file
        """
        f = open(XmlSettings.error_file, 'a')
        f.write(datetime.datetime.fromtimestamp(self.timestamp).strftime("%d-%m-%Y %H:%M:%S") + ": " + self.description + "\n")
        f.close()
        

    @staticmethod
    def clear():
        """
        Clear error file
        """
        # Remove existing error file
        if os.path.exists(XmlSettings.error_file):
            os.remove(XmlSettings.error_file)
            
                  
    def __str__(self):
        """
        String representation of the exception
        """
        return repr(self.description)


    def __init__(self, value):
        """
        Class constructor
        
        @param value: Description about the error
        """
        self.timestamp = time.time()
        # Exception description
        self.description = "SwapException occurred: " + value
  
