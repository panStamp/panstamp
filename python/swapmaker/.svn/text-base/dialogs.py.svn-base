#########################################################################
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
__date__ ="$Apr 28, 2012"
#########################################################################

from validators import TextValidator

import wx


class ConfigDialog(wx.Dialog):
    """
    Main configuration dialog class
    """
    def add_list_buttons(self):
        """
        Add list management buttons to the page
        """
        # Top spacer
        self.add_to_layout(None)
        # Create "Add" button
        button_add = wx.Button(self, -1, "Add")
        button_add.Bind(wx.EVT_BUTTON, self.button_add_pressed)
        button_add.SetToolTip(wx.ToolTip("Add new parameter"))
        button_add.SetDefault()
        # Create "Edit" button
        button_edit = wx.Button(self, -1, "Edit")
        button_edit.Bind(wx.EVT_BUTTON, self.button_edit_pressed)
        button_edit.SetToolTip(wx.ToolTip("Edit parameter"))
        # Create "Remove" button
        button_remove = wx.Button(self, -1, "Delete")
        button_remove.Bind(wx.EVT_BUTTON, self.button_remove_pressed)
        button_remove.SetToolTip(wx.ToolTip("Delete parameter"))

        sizer_buttons = wx.BoxSizer(wx.HORIZONTAL)
        # Horizontal sizer
        hsizer = wx.BoxSizer(wx.HORIZONTAL)

        # Add buttons
        hsizer.Add(button_add, 0, wx.ALIGN_LEFT)
        hsizer.Add((5,-1), proportion=1)
        hsizer.Add(button_edit, 0, wx.ALIGN_LEFT)
        hsizer.Add((5,-1), proportion=1)
        hsizer.Add(button_remove, 0, wx.ALIGN_LEFT)

        # Add horizontal sizer into sizer_buttons
        sizer_buttons.Add(hsizer, 0, wx.ALIGN_LEFT)
        
        # Add button sizer to the page
        self.add_to_layout(sizer_buttons)
        

    def button_add_pressed(self, event):
        """
        Button "Add" pressed
        """
        raise NotImplementedError


    def button_remove_pressed(self, event):
        """
        Button "Remove" pressed
        """
        raise NotImplementedError


    def button_edit_pressed(self, event):
        """
        Button "Edit" pressed
        """
        raise NotImplementedError
    
    
    def add_to_layout(self, control, label=None, size=None):
        """
        Add control and label to the current layout
        
        'control'   Widget to be added to the layout
        'label'     Label string on top of the control
        "size"      Size of the control
        """
        # Horizontal sizer
        hsizer =  wx.BoxSizer(wx.HORIZONTAL)
        # Add left spacer
        hsizer.Add((10,-1), proportion=1)
        # Vertical sizer
        vsizer = wx.BoxSizer(wx.VERTICAL)
        # Create label
        if label is not None:
            # Add top spacer
            vsizer.Add((-1,10), proportion=1)
            lab = wx.StaticText(self, label=label)
            vsizer.Add(lab, 0, wx.ALIGN_LEFT)
        # Add control
        if control is not None:
            vsizer.Add(control, 0, wx.ALIGN_LEFT)
        # Add vertical sizer into the horizontal one
        hsizer.Add(vsizer, 0, wx.ALIGN_LEFT)
        # Add right spacer
        hsizer.Add((10,-1), proportion=1)
        # Add sizer to the list
        self._list_sizers.append(hsizer)


    def dolayout(self):
        """
        Layout widgets by means of sizers
        """
        # Add sizers to the main grid sizer
        for sizer in self._list_sizers:
            self.gridsizer.Add(sizer, 0, wx.ALIGN_LEFT)

        # Add button sizer
        self.gridsizer.Add(self.sizer_buttons, 0, wx.ALIGN_RIGHT)

        self.SetSizer(self.gridsizer)


    def add_okcancel_buttons(self):
        """
        Add OK and CANCEL buttons to the dialog
        """
        # Top spacer
        self.add_to_layout(None)
        # Create OK button
        button_ok = wx.Button(self, wx.ID_OK, "OK")
        button_ok.SetDefault()
        # Create CANCEL button
        button_cancel = wx.Button(self, wx.ID_CANCEL, "CANCEL")
        button_cancel.SetDefault()

        # Add left spacer
        self.sizer_buttons.Add((10,-1), proportion=1)
        # Horizontal sizer
        hsizer = wx.BoxSizer(wx.HORIZONTAL)
        # Add buttons
        hsizer.Add(button_ok, 0, wx.ALIGN_LEFT)
        hsizer.Add((5,-1), proportion=1)
        hsizer.Add(button_cancel, 0, wx.ALIGN_LEFT)
        # Add horizontal sizer into sizer_buttons
        self.sizer_buttons.Add(hsizer, 0, wx.ALIGN_LEFT)
        # Add right spacer
        self.sizer_buttons.Add((10,-1), proportion=1)

    
    def __init__(self, parent, title):
        """
        Class constructor

        'parent'    Parent object
        'title'     Title of the dialog
        """
        wx.Dialog.__init__(self, parent, title=title)
        # List of sizers
        self._list_sizers = []
        # Main sizer
        self.gridsizer = wx.FlexGridSizer(rows=3, cols=1, vgap=3, hgap=3)
        # Bottom button sizer
        self.sizer_buttons = wx.BoxSizer(wx.HORIZONTAL)


class RegisterDialog(ConfigDialog):
    """
    Register editor dialog
    """
    last_register_id = 10
    
    def _create_controls(self):
        """
        Create GUI controls
        """
        # Add controls to the layout
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "regname", regex="\S"),
                                       size=(150, 26)), "Register name")
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "regid", regex="^([1][1-9]|[2-9][0-9]|[1][0-9][0-9]|[2][0-1][0-9]|[22][0-5])$"),
                                       size=(70, 26)), "Register ID (10-255)")

        # Create list box
        self.list_parameters = wx.ListCtrl(self, -1, style=wx.LC_REPORT, size=(275, 150))
        self.list_parameters.InsertColumn(0, "Parameter name")
        self.list_parameters.InsertColumn(1, "type")
        self.list_parameters.InsertColumn(2, "dir")
        self.list_parameters.SetColumnWidth(0, 140)
        self.list_parameters.SetColumnWidth(1, 50)
        self.list_parameters.SetColumnWidth(1, 50)

        self.add_to_layout(self.list_parameters, "List of parameters")

        self.add_list_buttons()
        self.add_okcancel_buttons()
        

    def __init__(self, parent=None, regid=None, regname="", parameters=None):
        """
        Class constructor

        @param parent: Parent object
        @param regname: Register name
        @param regid: Register id
        """
        ConfigDialog.__init__(self, parent, title="Register editor")
        # List of parameters
        self.parameters = parameters
        if self.parameters is None:
            self.parameters = []
        # Register name
        self.regname = regname
        # Register ID
        if regid is None:
            RegisterDialog.last_register_id += 1
            regid = str(RegisterDialog.last_register_id)
        self.regid = regid
        # Create widgets
        self._create_controls()
        # Layout widgets
        self.dolayout()
        # Fit dialog size to its contents
        self.Fit()
        # Fill list widget
        for index, param in enumerate(self.parameters):
            self.list_parameters.InsertStringItem(index, param["name"])
            if "dir" not in param:
                self.list_parameters.SetStringItem(index, 1, "out")
            else:
                self.list_parameters.SetStringItem(index, 1, param["dir"])
            self.list_parameters.SetStringItem(index, 2, param["type"])
            self.list_parameters.EnsureVisible(index)


class CfgRegisterDialog(RegisterDialog):
    """
    Editor for configuration registers
    """
    def button_add_pressed(self, event):
        """
        Button "Add" pressed
        """
        dialog = CfgParameterDialog(self)
            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            index = self.list_parameters.GetItemCount()
            self.list_parameters.InsertStringItem(index, dialog.paramname)
            self.list_parameters.SetStringItem(index, 1, dialog.paramtype)
            self.list_parameters.SetStringItem(index, 2, dialog.paramdir)
            self.list_parameters.EnsureVisible(index)
            
            parameter = {"name": dialog.paramname, "type": dialog.paramtype, "size": dialog.bitsize}
            
            if dialog.default != "":
                parameter["default"] = dialog.default
            if dialog.verif != "":
                parameter["verif"] = dialog.verif
            self.parameters.append(parameter)


    def button_remove_pressed(self, event):
        """
        Button "Remove" pressed
        """
        index = self.list_parameters.GetFirstSelected()
        
        if index == -1:
            return
        
        self.list_parameters.DeleteItem(index)
        del self.parameters[index]


    def button_edit_pressed(self, event):
        """
        Button "Edit" pressed
        """
        index = self.list_parameters.GetFirstSelected()
        
        if index == -1:
            return

        dialog = CfgParameterDialog(self, self.parameters[index]["name"], self.parameters[index]["type"], self.parameters[index]["size"])
        if "verif" in self.parameters[index]:
            dialog.verif = self.parameters[index]["verif"]
        if "default" in self.parameters[index]:
            dialog.default = self.parameters[index]["default"]

            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            parameter = {"name": dialog.paramname, "type": dialog.paramtype, "size": dialog.bitsize}
            if dialog.default is not None:
                parameter["default"] = dialog.default
            if dialog.verif is not None:
                parameter["verif"] = dialog.verif        
            self.parameters[index] = parameter
            self.list_parameters.SetStringItem(index, 0, dialog.paramname)
            self.list_parameters.SetStringItem(index, 1, dialog.paramtype)
            
            
    def __init__(self, parent=None, regid=None, regname="", parameters=None):
        """
        Class constructor

        @param parent: Parent object
        @param regid: Register id
        @param regname: Register name
        @param parameters: list of parameters
        """
        RegisterDialog.__init__(self, parent, regid, regname, parameters)
        

class RegRegisterDialog(RegisterDialog):
    """
    Editor for regular registers
    """
    def button_add_pressed(self, event):
        """
        Button "Add" pressed
        """
        dialog = RegParameterDialog(self)
            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            index = self.list_parameters.GetItemCount()
            self.list_parameters.InsertStringItem(index, dialog.paramname)
            self.list_parameters.SetStringItem(index, 1, dialog.paramtype)
            self.list_parameters.SetStringItem(index, 2, dialog.paramdir)
            self.list_parameters.EnsureVisible(index)
            
            parameter = {"name": dialog.paramname, "type": dialog.paramtype, "dir": dialog.paramdir, "size": dialog.bitsize, "units": dialog.units}
            self.parameters.append(parameter)


    def button_remove_pressed(self, event):
        """
        Button "Remove" pressed
        """
        index = self.list_parameters.GetFirstSelected()
        
        if index == -1:
            return
        
        self.list_parameters.DeleteItem(index)
        del self.parameters[index]


    def button_edit_pressed(self, event):
        """
        Button "Edit" pressed
        """
        index = self.list_parameters.GetFirstSelected()
        
        if index == -1:
            return
        
        dialog = RegParameterDialog(self, self.parameters[index]["name"], self.parameters[index]["type"], self.parameters[index]["dir"], self.parameters[index]["size"], self.parameters[index]["units"])
            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            parameter = {"name": dialog.paramname, "type": dialog.paramtype, "dir": dialog.paramdir, "size": dialog.bitsize, "units": dialog.units}       
            self.parameters[index] = parameter
            self.list_parameters.SetStringItem(index, 0, dialog.paramname)
            self.list_parameters.SetStringItem(index, 1, dialog.paramtype)

            
    def __init__(self, parent=None, regid=None, regname="", parameters=None):
        """
        Class constructor

        @param parent: Parent object
        @param regid: Register id
        @param regname: Register name
        @param parameters: list of parameters
        """
        if parameters is None:
            parameters = []
        RegisterDialog.__init__(self, parent, regid, regname, parameters)


class ParameterDialog(ConfigDialog):
    """
    Parameter editor dialog
    """
    def create_controls(self):
        """
        Create GUI controls
        """
        # Add controls to the layout
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "paramname", regex="^[a-zA-Z0-9_-]+$"),
                                       size=(150, 26)), "Name (blank spaces not allowed)")

        self.rb_type_num = wx.RadioButton(self, -1, 'Numeric value', (10, 10), style=wx.RB_GROUP)
        self.rb_type_bin = wx.RadioButton(self, -1, 'Binary value', (10, 30))
        self.rb_type_str = wx.RadioButton(self, -1, 'String value', (10, 50))
        self.Bind(wx.EVT_RADIOBUTTON, self.set_type, id=self.rb_type_num.GetId())
        self.Bind(wx.EVT_RADIOBUTTON, self.set_type, id=self.rb_type_bin.GetId())
        self.Bind(wx.EVT_RADIOBUTTON, self.set_type, id=self.rb_type_str.GetId())
        
        self.add_to_layout(self.rb_type_num, "Data type:")
        self.add_to_layout(self.rb_type_bin)
        self.add_to_layout(self.rb_type_str)
        
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "bitsize", regex="^([1-9]|[1-9][0-9]|[1-3][0-9][0-9]|[4][0-3][0-9]|[440])$"),
                                       size=(70, 26)), "Size in bits")
        

    def set_type(self, event):
        """
        Set type of parameter
        """
        if self.rb_type_num.GetValue():
            self.paramtype = "num"
        elif self.rb_type_bin.GetValue():
            self.paramtype = "bin"
        elif self.rb_type_str.GetValue():
            self.paramtype = "str"
        
        
    def __init__(self, parent=None, paramname="", paramtype="num", paramdir="inp", bitsize=8):
        """
        Class constructor

        @param parent: Parent object
        @param paramname: Parameter name
        @param paramtype: Type of parameter
        @param paramdir: Direction (input or output)
        @param bitsize: Size in bits
        """
        ConfigDialog.__init__(self, parent, title="Parameter editor")
        # Parameter name
        self.paramname = paramname
        # Type of parameter
        self.paramtype = paramtype
        # Direction
        self.paramdir = paramdir
        # Size in bits
        self.bitsize = bitsize


class CfgParameterDialog(ParameterDialog):
    """
    Editor for configuration parameters
    """
    def _create_additional_controls(self):
        """
        Create custom controls
        """
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "verif"), size=(150, 26)), "Regular expression")
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "default"), size=(150, 26)), "Default value")


    def __init__(self, parent=None, paramname="", paramtype="num", bitsize=8, verif="", default=""):
        """
        Class constructor

        @param parent: Parent object
        @param paramname: Parameter name
        @param paramtype: Type of parameter
        @param bitsize: Bit size
        @param verif: regular expression
        @param default: default value
        """
        ParameterDialog.__init__(self, parent, paramname, paramtype, "out", bitsize)
        # Regular expression
        self.verif = verif
        # Default value
        self.default = default

        # Create widgets
        self.create_controls()
        
        # Additional fields
        self._create_additional_controls()
        
        # Add OK-CANCEL buttons
        self.add_okcancel_buttons()
        # Layout widgets
        self.dolayout()
        # Fit dialog size to its contents
        self.Fit()


class RegParameterDialog(ParameterDialog):
    """
    Editor for regular parameters
    """
    def button_add_pressed(self, event):
        """
        Button "Add" pressed
        """
        dialog = UnitDialog(self)
            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            unit = {"name": dialog.unit, "factor": dialog.factor, "offset": dialog.offset}
            self.units.append(unit)
            index = self.list_units.GetItemCount()
            self.list_units.InsertStringItem(index, dialog.unit)
            self.list_units.SetStringItem(index, 1, str(dialog.factor))
            self.list_units.SetStringItem(index, 2, str(dialog.offset))
            self.list_units.EnsureVisible(index)
            

    def button_remove_pressed(self, event):
        """
        Button "Remove" pressed
        """
        index = self.list_units.GetFirstSelected()
        
        if index == -1:
            return
        
        self.list_units.DeleteItem(index)
        del self.units[index]


    def button_edit_pressed(self, event):
        """
        Button "Edit" pressed
        """
        index = self.list_units.GetFirstSelected()
        
        if index == -1:
            return
        
        dialog = UnitDialog(self, self.units[index]["name"], self.units[index]["factor"], self.units[index]["offset"])
            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            unit = {"name": dialog.unit, "factor": dialog.factor, "offset": dialog.offset}            
            self.units[index] = unit
            self.list_units.SetStringItem(index, 0, dialog.unit)
            self.list_units.SetStringItem(index, 1, str(dialog.factor))
            self.list_units.SetStringItem(index, 2, str(dialog.offset))


    def _create_additional_controls(self):
        """
        Create custom controls
        """
        # Direction
        self.rb_dir_inp = wx.RadioButton(self, -1, 'input', (10, 10), style=wx.RB_GROUP)
        self.rb_dir_out = wx.RadioButton(self, -1, 'output', (10, 30))
        self.Bind(wx.EVT_RADIOBUTTON, self.set_direction, id=self.rb_dir_inp.GetId())
        self.Bind(wx.EVT_RADIOBUTTON, self.set_direction, id=self.rb_dir_out.GetId())
        
        self.add_to_layout(self.rb_dir_inp, "Direction:")
        self.add_to_layout(self.rb_dir_out)

        # Create list box
        self.list_units = wx.ListCtrl(self, -1, style=wx.LC_REPORT, size=(260, 100))
        self.list_units.InsertColumn(0, "Unit")
        self.list_units.InsertColumn(1, "Factor")
        self.list_units.InsertColumn(2, "Offset")
        self.list_units.SetColumnWidth(0, 100)
        self.list_units.SetColumnWidth(1, 80)
        self.list_units.SetColumnWidth(1, 80)

        self.add_to_layout(self.list_units, "List of units")

        self.add_list_buttons()


    def set_direction(self, event):
        """
        Set type of parameter
        """
        if self.rb_dir_inp.GetValue():
            self.paramdir = "inp"
        elif self.rb_dir_out.GetValue():
            self.paramdir = "out"
            

    def __init__(self, parent=None, paramname="", paramtype="num", paramdir="inp", bitsize=8, units=None):
        """
        Class constructor

        @param parent: Parent object
        @param paramname: Parameter name
        @param paramtype: Type of parameter
        @param paramdir: Direction (input or output)
        @param bitsize: Bit size
        @param units: list of units
        """
        ParameterDialog.__init__(self, parent, paramname, paramtype, paramdir, bitsize)
        # List of units
        self.units = units
        if self.units is None:
            self.units = []
        # Create widgets
        self.create_controls()
        
        # Additional fields
        self._create_additional_controls()
       
        # Add OK-CANCEL buttons
        self.add_okcancel_buttons()
        # Layout widgets
        self.dolayout()
        # Fit dialog size to its contents
        self.Fit()
        
        # Fill list widget
        #self.list_units.DeleteAllItems()
        for index, unit in enumerate(self.units):
            self.list_units.InsertStringItem(index, unit["name"])
            self.list_units.SetStringItem(index, 1, str(unit["factor"]))
            self.list_units.SetStringItem(index, 2, str(unit["offset"]))
            self.list_units.EnsureVisible(index)


class UnitDialog(ConfigDialog):
    """
    Unit editor dialog
    """
    def create_controls(self):
        """
        Create GUI controls
        """
        # Add controls to the layout
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "unit"), size=(70, 26)), "Unit label")
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "factor", regex="[-+]?[0-9]*\.?[0-9]+"), size=(70, 26)), "Factor modifier")
        self.add_to_layout(wx.TextCtrl(self, validator=TextValidator(self, "offset", regex="[-+]?[0-9]*\.?[0-9]+"), size=(70, 26)), "Offset modifier")
        self.add_to_layout(wx.StaticText(self, label="Final value = value x factor + offset"))
        self.add_okcancel_buttons()
                
        
    def __init__(self, parent=None, unit="", factor=1, offset=0):
        """
        Class constructor

        @param parent: Parent object
        @param unit: Unit string
        @param factor: Factor modifier
        @param offset: Offset modifier
        """
        ConfigDialog.__init__(self, parent, title="Unit editor")
        # Unit string
        self.unit = unit
        # Factor modifer
        self.factor = factor
        # Offset modifier
        self.offset = offset

        # Create widgets
        self.create_controls()
        # Layout widgets
        self.dolayout()
        # Fit dialog size to its contents
        self.Fit()

