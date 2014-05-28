#########################################################################
#
# validators
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
__author__="Daniel Berenguer"
__date__  ="$Aug 29, 2011 1:40:53 PM$"
#########################################################################

import re
import wx


class CtrlValidator(wx.PyValidator):
    """
    Validator for wxPython widgets
    """

    def _getControlValue(self):
        """
        Get value from control
        """
        raise NotImplementedError, 'Subclass must implement _getControlValue'


    def _setControlValue(self, value):
        """
        Set control value
        
        'value'  New control value
        """
        raise NotImplementedError, 'Subclass must implement _setControlValue'


    def Validate(self, win):
        """
        Validation method
        """
        raise NotImplementedError, 'Subclass must implement Validate'


    def TransferToWindow(self):
        """
        Method that automatically sets the initial contents of the field
        """
        # Get value from calling opject's variable
        if type(self.attr) is list and self.index is not None:           
            if self.attr[self.index].__class__.__name__ == "SwapCfgParam":
                if self.attr[self.index].value is not None:
                    val = self.attr[self.index].getValueInAscii()
            else:
                val = self.attr[self.index]
        else:
            val = self.attr

        # Set widget value
        if val is not None:
            self._setControlValue(str(val))
        return True


    def TransferFromWindow(self):
        """
        Method that automatically gets the contents of the field and sets
        the affected attribute on obj
        """
        # Get widget value
        tmpVal = self._getControlValue()
        # Convert to native type
        if type(self.attr) is list and self.index is not None:
            if self.attr[self.index].__class__.__name__ == "SwapCfgParam":
                self.attr[self.index].setValue(tmpVal)
            else:
                self.attr[self.index] = tmpVal
        elif type(self.attr) is int:
            try:
                self.attr = int(tmpVal)
            except ValueError:
                try:
                    self.attr = int(tmpVal, 16)
                except:
                    self.attr = 0
        elif type(self.attr) is float:
            self.attr = float(tmpVal)
        else:
            self.attr = tmpVal

        # Set parent's attribute'
        setattr(self.obj, self.attrName, self.attr)
        return True


    def __init__(self, obj, attrName, index=None, regex=None):
        """
        Class constructor

        'obj'      Modifiable class instance
        'attrName' Name of the attribute to be modified by this validator
        """
        wx.PyValidator.__init__(self)
        self.obj = obj
        self.attrName = attrName
        self.index = index
        self.attr = getattr(self.obj, self.attrName)
        self.type = None
        self.regex = regex


    def Clone(self):
        """
        Return a new validator for the same field of the same object
        """
        return self.__class__(self.obj, self.attrName, self.index, self.regex)
    
    
    def _Warning(self, message, caption="Warning!"):
        """
        Display warning message
        """
        dialog = wx.MessageDialog(None, message, caption, wx.OK | wx.ICON_WARNING)
        dialog.ShowModal()
        dialog.Destroy()
    

class TextValidator(CtrlValidator):
    """
    Validator for TextCtrl widgets
    """

    def _getControlValue(self):
        """
        Get value from control
        """
        wgt = self.GetWindow()
        return wgt.GetValue()


    def _setControlValue(self, value):
        """
        Set control value
        
        'value'  New control value
        """
        wgt = self.GetWindow()
        wgt.SetValue(value)
        
        
    def Validate(self, win):
        """
        Validation method
        """
        val = self._getControlValue()
        
        if self.regex is not None:
            # Match against regex string      
            if re.match(self.regex, val) is None:
                self._Warning("Invalid value: " + val)
                return False
        
        return True


    def __init__(self, obj, attrName, index=None, regex=None):
        """
        Class constructor
        """
        CtrlValidator.__init__(self, obj, attrName, index, regex)


class ListValidator(CtrlValidator):
    """
    Validator for combo boxes and list controls
    """

    def _getControlValue(self):
        """
        Get value from control
        """
        wgt = self.GetWindow()
        val = wgt.GetValue()
        if not val.isdigit():
            val = str(wgt.GetSelection() + 1)
            self.workWithSelection = True
        return val


    def _setControlValue(self, value):
        """
        Set control value
        
        'value'  New control value
        """
        wgt = self.GetWindow()
        if self.workWithSelection:
            if value > 0:
                wgt.SetSelection(value-1)
        else:       
            wgt.SetValue(value)
        
        
    def Validate(self, win):
        """
        Validation method
        """
        val = self._getControlValue()
        
        if self.regex is not None:
            # Match against regex string      
            if re.match(self.regex, val) is None:
                self._Warning("Invalid value: " + val)
                return False
        
        return True


    def __init__(self, obj, attrName, index=None, regex=None):
        """
        Class constructor
        """        
        CtrlValidator.__init__(self, obj, attrName, index, regex)
        
        self.workWithSelection = False
        
        
class BoolValidator(CtrlValidator):
    """
    Validator for Radio buttons and Check boxes
    """

    def _getControlValue(self):
        """
        Get value from control
        """
        wgt = self.GetWindow()
        return wgt.GetValue()


    def _setControlValue(self, value):
        """
        Set control value
        
        'value'  New control value
        """
        wgt = self.GetWindow()
        if type(value) is bool:
            boolVal = value
        else:
            boolVal = value.lower() in ["on", "true", "1", 1, "yes"]
        wgt.SetValue(boolVal)
        
        
    def Validate(self, win):
        """
        Validation method
        """
        return True


    def __init__(self, obj, attrName, index=None, regex=None):
        """
        Class constructor
        """
        CtrlValidator.__init__(self, obj, attrName, index, regex)
