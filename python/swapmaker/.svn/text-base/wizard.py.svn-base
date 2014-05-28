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
__date__ ="$Apr 23, 2012"
#########################################################################

from validators import TextValidator, BoolValidator
from dialogs import CfgRegisterDialog, RegRegisterDialog
from xmlfiles import XmlDeviceDir, XmlDevice
from errors import SwapMakerException
from sources import ProductFile, RegtableHFile, RegtableFile, MainInoFile, ReadmeFile

import os
import wx.wizard


class SwapWizardPage(wx.wizard.WizardPageSimple):
    """
    Wizard page
    """
    def _Warning(self, message, caption="Warning!"):
        """
        Display warning message
        """
        dialog = wx.MessageDialog(None, message, caption, wx.OK | wx.ICON_WARNING)
        dialog.ShowModal()
        dialog.Destroy()


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
        # Add top spacer
        vsizer.Add((-1,10), proportion=1)
        # Create label
        if label is not None:
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
        self.sizer.Add(hsizer)
  
    
    def __init__(self, parent, title):
        """
        Constructor
        
        @param parent: parent wizard
        @param title: title of the wizard page
        """
        wx.wizard.WizardPageSimple.__init__(self, parent)
        
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(self.sizer)
        
        title = wx.StaticText(self, -1, title)
        title.SetFont(wx.Font(12, wx.SWISS, wx.NORMAL, wx.BOLD))
        self.sizer.Add(title, 0, wx.ALIGN_CENTRE|wx.ALL, 5)
        self.sizer.Add(wx.StaticLine(self, -1), 0, wx.EXPAND|wx.ALL, 5)


class ProductInfoPage(SwapWizardPage):
    """
    Product information wizard page
    """
    
    def onpage_changing(self, event):
        """
        Verify fields
        """
        if self.developer_name == "":
            self._Warning("Please enter a develper name")
        elif self.developer_id == "":
            self._Warning("Please enter a develper ID")
        elif self.product_name == "":
            self._Warning("Please enter a product name")
        elif self.product_descr == "":
            self._Warning("Please enter a product description")
        elif self.product_id == "":
            self._Warning("Please enter a product ID")
        else:
            event.Allow()
            return
        
        event.Veto()
      
           
    def __init__(self, parent, developer_id=None, developer_name=None, product_id=None, product_name=None, product_descr=None, pwrdownmode=False):
        """
        Constructor
        
        @param parent: parent wizard
        @param developer_id: developer ID
        @param developer_nanme: developer name
        @param product_id: product ID
        @param product_nanme: product name
        @param product_descr: product description
        @param pwrdownmode: power-down mode
        """
        SwapWizardPage.__init__(self, parent, title="Product information")
               
        # Developer name
        self.developer_name = developer_name
        if self.developer_name is None:
            self.developer_name = ""
        # SWAP develoepr id
        self.developer_id = developer_id
        if self.developer_id is None:
            self.developer_id = ""     
        # Product name
        self.product_name = product_name
        if self.product_name is None:
            self.product_name = ""
        # Product description
        self.product_descr = product_descr
        if self.product_descr is None:
            self.product_descr = ""
        # SWAP peoduct id
        self.product_id = product_id
        if self.product_id is None:
            self.product_id = ""
        # Battery-powered device
        self.low_power = pwrdownmode

        # Add widgets to the page
        self.add_to_layout(wx.TextCtrl(self, -1, validator=TextValidator(self, "developer_name"), size=(150, 26)), "Developer name")        
        self.add_to_layout(wx.TextCtrl(self, -1, validator=TextValidator(self, "developer_id", regex="^[0-9]*$"), size=(70, 26)), "Developer ID number (DEC)")
        self.add_to_layout(wx.TextCtrl(self, -1, validator=TextValidator(self, "product_name", regex="^[a-zA-Z0-9]*$"), size=(150, 26)), "Product name (single word)")
        self.add_to_layout(wx.TextCtrl(self, -1, validator=TextValidator(self, "product_descr"), size=(250, 26)), "Product description")
        self.add_to_layout(wx.TextCtrl(self, -1, validator=TextValidator(self, "product_id", regex="^[0-9]*$"), size=(70, 26)), "Product ID number (DEC)")
        self.add_to_layout(wx.CheckBox(self, -1, "Battery-powered device?", validator=BoolValidator(self, "low_power")))

        self.Bind(wx.wizard.EVT_WIZARD_PAGE_CHANGING, self.onpage_changing)
        

class RegisterPage(SwapWizardPage):
    """
    Register definition page
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
        button_add.SetToolTip(wx.ToolTip("Add new register"))
        button_add.SetDefault()
        # Create "Edit" button
        button_edit = wx.Button(self, -1, "Edit")
        button_edit.Bind(wx.EVT_BUTTON, self.button_edit_pressed)
        button_edit.SetToolTip(wx.ToolTip("Edit register"))
        # Create "Remove" button
        button_remove = wx.Button(self, -1, "Delete")
        button_remove.Bind(wx.EVT_BUTTON, self.button_remove_pressed)
        button_remove.SetToolTip(wx.ToolTip("Delete register"))

        sizer_buttons = wx.BoxSizer(wx.HORIZONTAL)
        # Horizontal sizer
        hsizer = wx.BoxSizer(wx.HORIZONTAL)
        # Add left spacer
        hsizer.Add((10,-1), proportion=1)
        # Add buttons
        hsizer.Add(button_add, 0, wx.ALIGN_LEFT)
        hsizer.Add((5,-1), proportion=1)
        hsizer.Add(button_edit, 0, wx.ALIGN_LEFT)
        hsizer.Add((5,-1), proportion=1)
        hsizer.Add(button_remove, 0, wx.ALIGN_LEFT)
        # Add right spacer
        hsizer.Add((10,-1), proportion=1)
        # Add horizontal sizer into sizer_buttons
        sizer_buttons.Add(hsizer, 0, wx.ALIGN_LEFT)
        
        # Add button sizer to the page
        self.sizer.Add(sizer_buttons)


    def button_add_pressed(self, event):
        """
        Button "Add" pressed
        """
        if self.is_config:
            dialog = CfgRegisterDialog(self)
        else:
            dialog = RegRegisterDialog(self)
            
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:          
            index = self.list_registers.GetItemCount()
            self.list_registers.InsertStringItem(index, dialog.regid)
            self.list_registers.SetStringItem(index, 1, dialog.regname)
            self.list_registers.EnsureVisible(index)
            
            register = {"id": dialog.regid, "name": dialog.regname}
            if self.is_config:
                register["parameters"] = dialog.parameters
            else:
                register["endpoints"] = dialog.parameters
            self.registers.append(register)


    def button_remove_pressed(self, event):
        """
        Button "Remove" pressed
        """
        index = self.list_registers.GetFirstSelected()
        
        if index == -1:
            return
        
        self.list_registers.DeleteItem(index)
        del self.registers[index]


    def button_edit_pressed(self, event):
        """
        Button "Edit" pressed
        """
        # Read selected register
        index = self.list_registers.GetFirstSelected()
        
        if index == -1:
            return
               
        # Open dialog
        if self.is_config:
            dialog = CfgRegisterDialog(self, self.registers[index]["id"], self.registers[index]["name"], self.registers[index]["parameters"])
        else:
            dialog = RegRegisterDialog(self, self.registers[index]["id"], self.registers[index]["name"], self.registers[index]["endpoints"])

        res = dialog.ShowModal()
        
        if res == wx.ID_OK:          
            self.list_registers.SetStringItem(index, 0, dialog.regid)
            self.list_registers.SetStringItem(index, 1, dialog.regname)
            
            register = {"id": dialog.regid, "name": dialog.regname}
            
            if self.is_config:
                register["parameters"] = dialog.parameters
            else:
                register["endpoints"] = dialog.parameters
                
            self.registers[index] = register


    def __init__(self, parent, title="Register editor", is_config=False, registers=None):
        """
        Constructor
        
        @param parent: parent wizard object
        @param title: title of the wizard page
        @param is_config: True in case of configuration registers
        @param registers: list of registers 
        """
        SwapWizardPage.__init__(self, parent, title)

        # Configuration registers?
        self.is_config = is_config

        # List of registers
        self.registers = registers
        if self.registers is None:
            self.registers = []

        # Create list box
        self.list_registers = wx.ListCtrl(self, -1, style=wx.LC_REPORT, size=(275, 150))
        self.list_registers.InsertColumn(0, "Reg ID")
        self.list_registers.InsertColumn(1, "Register name")
        self.list_registers.SetColumnWidth(0, 50)
        self.list_registers.SetColumnWidth(1, 225)

        self.add_to_layout(self.list_registers, "List of registers")
        
        self.add_list_buttons()
        
        # Fill list
        if self.registers is not None:
            for index, reg in enumerate(self.registers):
                self.list_registers.InsertStringItem(index, str(reg["id"]))
                self.list_registers.SetStringItem(index, 1, reg["name"])
                self.list_registers.EnsureVisible(index)


class CfgRegisterPage(RegisterPage):
    """
    Wizard page for configuration registers
    """
    def __init__(self, parent, registers=[]):
        """
        Constructor
        
        @param parent: parent wizard object
        @param registers: list of registers
        """
        RegisterPage.__init__(self, parent, title="Configuration registers", is_config=True, registers=registers)


class RegRegisterPage(RegisterPage):
    """
    Wizard page for regular registers
    """
    def __init__(self, parent, registers=[]):
        """
        Constructor
        
        @param parent: parent wizard object
        @param registers: list of registers 
        """
        RegisterPage.__init__(self, parent, title="Regular registers", is_config=False, registers=registers)


class SwapWizard(wx.wizard.Wizard):
    '''
    SWAPmaker wizard
    '''       
    def __init__(self, title="SWAPmaker wizard"):
        '''
        Constructor
        
        @param title: Title of the GUI frame
        '''
        wx.wizard.Wizard.__init__(self, None, -1, title)
        
        working_dir = os.path.abspath(os.path.dirname(__file__))
        
        dir_file = os.path.join(working_dir, "output", "devices.xml")
        src_folder = os.path.join(working_dir, "output", "sources")
            
        try:
            dir_info = XmlDeviceDir(dir_file)
            def_file = "mydevice.xml"
            if dir_info is not None:
                if dir_info.prodname is not None:
                    def_file = os.path.join(os.path.abspath(os.path.dirname(__file__)), "output", dir_info.prodname + ".xml")
                def_info = XmlDevice(def_file)
        
            product_info = ProductInfoPage(self, dir_info.develid, dir_info.develname, dir_info.prodid, dir_info.prodname, dir_info.prodlabel, def_info.pwrdownmode)
            cfg_register_info = CfgRegisterPage(self, def_info.config_registers)
            reg_register_info = RegRegisterPage(self, def_info.regular_registers)
         
            wx.wizard.WizardPageSimple.Chain(product_info, cfg_register_info)
            wx.wizard.WizardPageSimple.Chain(cfg_register_info, reg_register_info)
            self.FitToPage(product_info)
            res = self.RunWizard(product_info)
      
            if res:
                # Generate XML files
                label = os.path.split(def_file)[1]
                label = label.split(os.extsep)[0]
                info = XmlDeviceDir(filename=dir_file,
                                    develid=product_info.developer_id,
                                    develname=product_info.developer_name,
                                    prodid=product_info.product_id,
                                    prodname=product_info.product_name,
                                    prodlabel=product_info.product_descr)
                info.save()
                
                def_file = os.path.join(os.path.abspath(os.path.dirname(__file__)), "output", product_info.product_name + ".xml")
                info = XmlDevice(filename=def_file,
                                 developer=product_info.developer_name,
                                 product=product_info.product_name,
                                 pwrdownmode=product_info.low_power,
                                 cfgregs=cfg_register_info.registers,
                                 regregs=reg_register_info.registers)
                info.save()
                
                # Generate source code
                # product.h
                file_path = os.path.join(working_dir, src_folder, "product.h")
                ProductFile(filename=file_path,
                            developer=product_info.developer_name,
                            product=product_info.product_name + " (" + product_info.product_descr + ")",
                            hwversion="0x00000100",
                            fwversion="0x00000100",
                            develid=product_info.developer_id,
                            productid=product_info.product_id)
                
                # regtable.h
                file_path = os.path.join(working_dir, src_folder, "regtable.h")
                registers = cfg_register_info.registers + reg_register_info.registers
                RegtableHFile(filename=file_path,
                              developer=product_info.developer_name,
                              product=product_info.product_name + " (" + product_info.product_descr + ")",                        
                              registers=registers)
                
                # regtable.ino
                file_path = os.path.join(working_dir, src_folder, "regtable.ino")
                RegtableFile(filename=file_path,
                             developer=product_info.developer_name,
                             product=product_info.product_name + " (" + product_info.product_descr + ")",                        
                             registers=registers)
                
                # Main file
                file_path = os.path.join(working_dir, src_folder, product_info.product_name + ".ino")
                MainInoFile(filename=file_path,
                            developer=product_info.developer_name,
                            product=product_info.product_name + " (" + product_info.product_descr + ")",                        
                            lowpower=product_info.low_power)
    
                # README file
                # Main file
                file_path = os.path.join(working_dir, src_folder, product_info.product_name + ".ino")
                ReadmeFile(filename=file_path,
                           developer=product_info.developer_name,
                           product=product_info.product_name + " (" + product_info.product_descr + ")")
    
                msg = wx.MessageDialog(None,
                                       "SWAPmaker completed successfully.\n Open \"output\" subfolder for results.",
                                       "Process completed",
                                       wx.OK | wx.ICON_INFORMATION)
                msg.ShowModal()
                msg.Destroy()
                
        except:
            raise

        self.Destroy()
