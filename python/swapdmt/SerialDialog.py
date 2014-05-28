#########################################################################
#
# SerialDialog
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
__date__  = "$Aug 27, 2011 9:35:21 PM$"
#########################################################################

from swap.xmltools.XmlSettings import XmlSettings
from swap.xmltools.XmlSerial import XmlSerial

from ConfigDialog import ConfigDialog
from validators import TextValidator

import wx

class SerialDialog(ConfigDialog):
    """
    Serial port configuration dialog
    """
    def _createControls(self):
        """
        Create GUI controls
        """
        # Add controls to the layout
        self.addToLayout(wx.TextCtrl(self, validator=TextValidator(self, "portname"), size=(200, 26)), "Serial port")
        self.addToLayout(wx.TextCtrl(self, validator=TextValidator(self, "portspeed")), "Serial baud rate (bps)")
        self.addOkCancelButtons()
        

    def _save(self):
        """
        Save Serial settings into config file
        """
        # Update serial port config
        self.config.port = self.portname
        self.config.speed = self.portspeed
        # Save data in config file
        self.config.save()


    def __init__(self, parent=None):
        """
        Class constructor

        'parent'    Parent object
        """
        ConfigDialog.__init__(self, parent, title="Serial port")
        # Configuration settings
        self.config = XmlSerial(XmlSettings.serial_file)
        # Name/path of the serial port
        self.portname = self.config.port
        # Serial port baud rate (in bps)
        self.portspeed = self.config.speed
        # Create widgets
        self._createControls()
        # Layout widgets
        self.doLayout()
        # Fit dialog size to its contents
        self.Fit()
        # Display dialog
        self.ShowModal()
        # Save data
        self._save()
