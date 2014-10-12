#########################################################################
#
# wxcomms
#
# Copyright (c) 2014 Daniel Berenguer <dberenguer@usapiens.com>
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
__date__  ="$Oct 12, 2014 07:10:00 PM$"
#########################################################################

import wx
import wxversion

if wxversion.checkInstalled("2.8"):
    wx_version = "2.8"
    from wx.lib.pubsub import Publisher
    pub = Publisher()
elif wxversion.checkInstalled("2.9"):
    wx_version = "2.9"
    from wx.lib.pubsub import pub
elif wxversion.checkInstalled("3.0"):
    wx_version = "3.0"
    from wx.lib.pubsub import pub
else:
    print "version of wxpython not supported"


class WxComms():
    """
    Wx message handling class
    """
    @staticmethod
    def send_message(event, message=None):
        """
        Send message to main fraim
        Handle wx.CallAfter properly according to the version of wxPython
        
        @param event Event name
        @param message Message text
        """
        if wx_version == "3.0":
            wx.CallAfter(pub.sendMessage, event, msg=message)
        else:
            wx.CallAfter(pub.sendMessage, event, message)
