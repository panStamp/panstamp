#########################################################################
#
# Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
# 
# This file is part of the panStamp project.
# 
# panStamp  is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# panStamp is distributed in the hope that it will be useful,
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
__date__ ="$Aug 20, 2011 10:36:00 AM$"
#########################################################################

import time

import wx


class SwapMakerException(Exception):
    """
    Main exception class for pySwap
    """
    def display(self):
        """
        Print exception description
        """       
        msg = wx.MessageDialog(None,
                               self.description,
                               "Warning",
                               wx.OK | wx.ICON_WARNING)
        msg.ShowModal()
        msg.Destroy()
               
                  
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
        self.description = value
  
