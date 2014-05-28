#########################################################################
#
# WaitDialog
#
# Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
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
__author__= "Daniel Berenguer"
__date__  ="$Sep 21, 2011 08:58:05 AM$"
#########################################################################

from ConfigDialog import ConfigDialog

import wx
import wxversion

if wxversion.checkInstalled("2.8"):
    wx_version = "2.8"
    from wx.lib.pubsub import Publisher
    pub = Publisher()
elif wxversion.checkInstalled("2.9"):
    wx_version = "2.9"
    from wx.lib.pubsub import pub
else:
    print "version of wxpython not supported"


class WaitDialog(ConfigDialog):
    """
    Waiting dialog class
    """
    def _createControls(self):
        """
        Create GUI controls
        """
        # Add control for every parameter contained in the register
        self.addToLayout(None, self.message)
        
        if self.wait_time is None:
            # Add cancel button
            self.addCancelButton()


    def close(self):
        """
        Close dialog
        """
        self.EndModal(wx.ID_OK)
        #self.Destroy()
        
    
    def show(self):
        """
        Show dialog during wait_time seconds only
                                
        @return False in case of timeout. Return True otherwise
        """
        if self.wait_time is not None:
            self.timer.Start(self.wait_time * 1000, True)
        res = self.ShowModal()
           
        return res == wx.ID_OK
        
    
    def _cb_timedout(self, event):
        """
        Callback function called when timer expires
        """
        self.EndModal(wx.ID_CANCEL)
        #self.Destroy()
        

    def cb_close(self, msg):
        """
        Request from main frame to close the current waiting dialog
        """
        self.close()


    def __init__(self, parent=None, message=None, wait_time=None):
        """
        Class constructor

        @param parent    Parent object
        @param message   Message to be displayed
        @param wait_time Max time to wait for a response (in seconds)
        """
        ConfigDialog.__init__(self, parent, title="Waiting...")
        
        pub.subscribe(self.cb_close, "close_wait")
        
        ## Message to be displayed
        self.message = message
        ## Max time to wait for a response (in seconds)
        self.wait_time = wait_time
        ## Timer
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self._cb_timedout, self.timer)
        ## Create widgets
        self._createControls()
        ## Layout widgets
        self.doLayout()
        ## Fit dialog size to its contents
        self.Fit()
