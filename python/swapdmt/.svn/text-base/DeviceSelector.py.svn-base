#########################################################################
#
# DeviceSelector
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
__author__  ="Daniel Berenguer"
__date__    ="$Sep 20, 2011 4:38:12 PM$"
#########################################################################

from ConfigDialog import ConfigDialog

from swap.xmltools.XmlDevice import XmlDeviceDir 

import wx

class DeviceSelector(ConfigDialog):
    """
    SWAP device selsctor
    """
    def _createControls(self):
        """
        Create GUI controls
        """
        # Create list box
        self.listDev = wx.ListCtrl(self, -1, style=wx.LC_REPORT, size=wx.Size(500,400))
        self.listDev.InsertColumn(0, 'Option')
        self.listDev.InsertColumn(1, 'Device')
        self.listDev.InsertColumn(2, 'Manufacturer')
        self.listDev.SetColumnWidth(0, 75)
        self.listDev.SetColumnWidth(1, 325)
        self.listDev.SetColumnWidth(2, 100)
        # Add controls to the layout
        self.addToLayout(self.listDev, "Select device from list:")
        self.addToLayout(None)
        self.addOkCancelButtons()
        

    def _fillListWithDevices(self):
        """
        Fill listDev with devices
        """
        # Read device directory
        devDir = XmlDeviceDir()
        # Fill list widget
        for developer in devDir.developers:
            for i, device in enumerate(developer.devices):
                self.listDev.InsertStringItem(i, device.option)
                self.listDev.SetStringItem(i, 1, device.label)
                self.listDev.SetStringItem(i, 2, developer.name)

       
    def getSelection(self):
        """
        Return item selected from the list
        """
        itemIndex = self.listDev.GetFirstSelected()
        return self.listDev.GetItemText(itemIndex)
        
    def __init__(self, parent=None):
        """
        Class constructor

        'parent'    Parent object
        """
        ConfigDialog.__init__(self, parent, title="Device selector")
        # Create widgets
        self._createControls()
        # Fill list widget
        self._fillListWithDevices()
        # Layout widgets
        self.doLayout()        
        # Fit dialog size to its contents
        self.Fit()
