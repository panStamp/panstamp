#!/usr/bin/env python
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
__date__ ="$Apr 23, 2012"
#########################################################################

from wizard import SwapWizard
from errors import SwapMakerException

import os
import wx


class SwapMakerSplash(wx.SplashScreen):
    """
    Custom splash screen class
    """
    def __init__(self, parent=None):
        """
        Class constructor
        
        @param parent: parent object
        """
        logo_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "images", "swapmaker.png")
        bmp = wx.Image(name = logo_path).ConvertToBitmap()
        splash_style = wx.SPLASH_CENTRE_ON_SCREEN | wx.SPLASH_TIMEOUT
        splash_duration = 2000 # milliseconds
        # Call the constructor with the above arguments in exactly the
        # following order.
        wx.SplashScreen.__init__(self, bmp, splash_style, splash_duration, parent)
        self.Bind(wx.EVT_CLOSE, self.on_exit)

        wx.Yield()


    def on_exit(self, evt):
        """
        Exit callback
        """
        self.Hide()

        try:
            # Start wizard
            wizard = SwapWizard()
    
            app.SetTopWindow(wizard)
            wizard.CenterOnScreen()
            wizard.Show(True)
        except SwapMakerException as ex:
            ex.display()
            
        # The program will freeze without this line.
        evt.Skip()  # Make sure the default handler runs too...


if __name__ == "__main__":
    # wxPython app
    app = wx.PySimpleApp(0)
    wx.InitAllImageHandlers()
        
    # Run splashScreen    
    SwapMakerSplash()
    
    app.MainLoop()
