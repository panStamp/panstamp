#########################################################################
#
# ConfigDialog
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
__date__  ="$Aug 29, 2011 11:57:47 AM$"
#########################################################################

import wx

class ConfigDialog(wx.Dialog):
    """
    Main configuration dialog class
    """

    def addToLayout(self, control, label=None, size=None):
        """
        Add control and label to the current layout
        
        'control'   Widget to be added to the layout
        'label'     Label string on top of the control
        "size"      Size of the control
        """
        # Horizontal sizer
        hSizer =  wx.BoxSizer(wx.HORIZONTAL)
        # Add left spacer
        hSizer.Add((10,-1), proportion=1)
        # Vertical sizer
        vSizer = wx.BoxSizer(wx.VERTICAL)
        # Add top spacer
        vSizer.Add((-1,10), proportion=1)
        # Create label
        if label is not None:
            lab = wx.StaticText(self, label=label)
            vSizer.Add(lab, 0, wx.ALIGN_LEFT)
        # Add control
        if control is not None:
            vSizer.Add(control, 0, wx.ALIGN_LEFT)
        # Add vertical sizer into the horizontal one
        hSizer.Add(vSizer, 0, wx.ALIGN_LEFT)
        # Add right spacer
        hSizer.Add((10,-1), proportion=1)
        # Add sizer to the list
        self.lstSizers.append(hSizer)


    def doLayout(self):
        """
        Layout widgets by means of sizers
        """
        # Add sizers to the main grid sizer
        for sizer in self.lstSizers:
            self.gridSizer.Add(sizer, 0, wx.ALIGN_LEFT)

        # Add button sizer
        self.gridSizer.Add(self.buttonSizer, 0, wx.ALIGN_RIGHT)

        self.SetSizer(self.gridSizer)


    def addOkCancelButtons(self):
        """
        Add OK and CANCEL buttons to the dialog
        """
        # Top spacer
        self.addToLayout(None)
        # Create OK button
        okButton = wx.Button(self, wx.ID_OK, "OK")
        okButton.SetDefault()
        # Create CANCEL button
        cancelButton = wx.Button(self, wx.ID_CANCEL, "CANCEL")
        cancelButton.SetDefault()

        # Add left spacer
        self.buttonSizer.Add((10,-1), proportion=1)
        # Horizontal sizer
        hSizer = wx.BoxSizer(wx.HORIZONTAL)
        # Add buttons
        hSizer.Add(okButton, 0, wx.ALIGN_LEFT)
        hSizer.Add((5,-1), proportion=1)
        hSizer.Add(cancelButton, 0, wx.ALIGN_LEFT)
        # Add horizontal sizer into buttonSizer
        self.buttonSizer.Add(hSizer, 0, wx.ALIGN_LEFT)
        # Add right spacer
        self.buttonSizer.Add((10,-1), proportion=1)
        
        
    def addCancelButton(self):
        """
        Add OK and CANCEL buttons to the dialog
        """
        # Top spacer
        self.addToLayout(None)
        # Create CANCEL button
        cancelButton = wx.Button(self, wx.ID_CANCEL, "CANCEL")
        cancelButton.SetDefault()

        # Add left spacer
        self.buttonSizer.Add((10,-1), proportion=1)
        # Horizontal sizer
        hSizer = wx.BoxSizer(wx.HORIZONTAL)
        # Add buttons
        hSizer.Add(cancelButton, 0, wx.ALIGN_LEFT)
        # Add horizontal sizer into buttonSizer
        self.buttonSizer.Add(hSizer, 0, wx.ALIGN_LEFT)
        # Add right spacer
        self.buttonSizer.Add((10,-1), proportion=1)        


    def __init__(self, parent, title):
        """
        Class constructor

        'parent'    Parent object
        'title'     Title of the dialog
        """
        wx.Dialog.__init__(self, parent, title=title)
        # List of sizers
        self.lstSizers = []
        # Main sizer
        #self.gridSizer = wx.FlexGridSizer(rows=3, cols=1, vgap=3, hgap=3)
        self.gridSizer = wx.FlexGridSizer(cols=1, vgap=3, hgap=3)
        # Bottom button sizer
        self.buttonSizer = wx.BoxSizer(wx.HORIZONTAL)
