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
__date__  ="$Feb 26, 2012$"
#########################################################################

import time
import datetime


class MaxException(Exception):
    """
    Main exception class for the lagarto-max process
    """
    def display(self):
        """
        Print exception description
        """
        print datetime.datetime.fromtimestamp(self.timestamp).strftime("%d-%m-%Y %H:%M:%S"), self.description
              
                  
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
        self.description = "MaxException occurred: " + value
