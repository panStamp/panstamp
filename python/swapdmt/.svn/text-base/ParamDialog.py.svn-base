#########################################################################
#
# ParamDialog
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
__date__  = "$Sep 1, 2011 10:39:23 AM$"
#########################################################################

from ConfigDialog import ConfigDialog
from validators import TextValidator, ListValidator, BoolValidator

from swap.protocol.SwapDefs import SwapType

import wx

class ParamDialog(ConfigDialog):
    """
    General parameter configuration dialog
    """
    def _createControls(self):
        """
        Create GUI controls
        """
        # Add control for every parameter contained in the register
        for ind in range(len(self.parameters)):
            self._addWidget(ind)

        self.addOkCancelButtons()


    def _addWidget(self, index):
        """
        Add widget to the layout
        
        'index'    Index of hte parameter within self.parameters
        """
        param = self.parameters[index]
        if param.type == SwapType.BINARY:
            self.addToLayout(wx.CheckBox(self, -1, param.name, validator=BoolValidator(self, "parameters", index)))
        elif param.type in [SwapType.NUMBER, SwapType.STRING]:
            addTextCtrl = False
            choices = []
            if param.verif is not None:
                if param.verif == "MONTH":
                    choices = ["January", "February", "March", "April", "June", "May", "July", "August", "September", "October", "November", "December"]
                elif param.verif == "YEAR":
                    for i in range(2011, 2060):
                        choices.append(str(i))
                elif param.verif == "MDAY":
                    for i in range(1, 32):
                        choices.append(str(i))
                elif param.verif == "WDAY":
                    choices = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
                elif param.verif in ["MINUTE", "SECOND"]:
                    for i in range(0, 10):
                        choices.append("0" + str(i))
                    for i in range(10, 60):
                        choices.append(str(i))
                elif param.verif == "HOUR":
                    for i in range(0, 10):
                        choices.append("0" + str(i))
                    for i in range(10, 24):
                        choices.append(str(i))
                elif len(param.verif) > 6:
                    if param.verif[:6] == "RANGE(":
                        comma = param.verif.find(",")
                        if comma > -1:
                            startRange = int(param.verif[6:comma])
                            endRange = int(param.verif[comma+1:-1])
                        else:
                            startRange = 0
                            endRange = int(param.verif[6:-1]) + 1
                        for i in range(startRange, endRange):
                            choices.append(str(i))
                    else:
                        addTextCtrl = True
                else:
                    addTextCtrl = True

            if addTextCtrl:
                self.addToLayout(wx.TextCtrl(self, -1, validator=TextValidator(self, "parameters", index, param.verif)), param.name)
            else:
                self.addToLayout(wx.ComboBox(self, validator=ListValidator(self, "parameters", index), choices=choices, style=wx.CB_READONLY), param.name)


    def __init__(self, parent=None, register=None):
        """
        Class constructor

        'parent'     Parent object
        'register'   SWAP Register to be configured by this dialog
        """
        ConfigDialog.__init__(self, parent, title=register.name)

        # Register to be configured
        # FIXME: parameters need to be cloned in order to not to alter the original ones
        self.parameters = register.parameters[:]
                      
        # Create widgets
        self._createControls()
        # Layout widgets
        self.doLayout()
        # Fit dialog size to its contents
        self.Fit()
