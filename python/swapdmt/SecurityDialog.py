#########################################################################
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
__date__  = "$June 9, 2012$"
#########################################################################

from swap.xmltools.XmlSettings import XmlSettings
from swap.xmltools.XmlNetwork import XmlNetwork

from ConfigDialog import ConfigDialog
from validators import TextValidator, BoolValidator

import wx

class SecurityDialog(ConfigDialog):
    """
    Network configuration dialog
    """
    def _createControls(self):
        """
        Create GUI controls
        """
        # Add controls to the layout       
        self.addToLayout(wx.CheckBox(self, -1, "Anti-playback", validator=BoolValidator(self, "playbk")))
        self.addToLayout(wx.CheckBox(self, -1, "Smart encryption", validator=BoolValidator(self, "smartencrypt")))

        self.addToLayout(wx.TextCtrl(self, validator=TextValidator(self, "password", regex="^[A-Fa-f0-9-]{24}$"), size=(150, 26)), "Password (12 bytes, hex)")
        self.addOkCancelButtons()
        

    def __init__(self, parent=None, secu=0, password=0):
        """
        Class constructor

        @param parent    Parent object
        """
        ConfigDialog.__init__(self, parent, title="Security settings")
        ## Configuration settings
        self.config = XmlNetwork(XmlSettings.network_file)
        ## Anti-playback
        self.playbk = secu & 0x01 == 0x01
        ## Smart encryption
        self.smartencrypt = secu & 0x02 == 0x02
        ## Encryption password
        self.password = password
        # Create widgets
        self._createControls()
        # Layout widgets
        self.doLayout()
        # Fit dialog size to its contents
        self.Fit()
