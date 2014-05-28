#########################################################################
#
# Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
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
__date__ ="May 2, 2012"
#########################################################################

from dialogs import RegisterDialog
from errors import SwapMakerException

import os
import xml.etree.ElementTree as xml

working_dir = os.path.dirname(__file__)


class XmlDeviceDir(object):
    """
    Class implementing directory files linking device names with
    its corresponding description files
    """

    def read(self):
        """
        Read configuration file
        """
        try:
            # Parse XML file
            tree = xml.parse(self.file_name)
            if tree is None:
                return
            # Get the root node
            root = tree.getroot()
            # List of developers
            devel = root.find("developer")
            if devel is not None:
                # Get developer id
                strid = devel.get("id")
                if strid is None:
                    raise SwapMakerException("Developer section needs a valid ID in " + self.file_name)
                    return
                self.develid = int(strid)
                # Get developer name
                self.develname = devel.get("name")
                if self.develname is None:
                    raise SwapMakerException("Developer section needs a name in " + self.file_name)
                    return
    
                # Parse device belonging to this developer
                dev = devel.find("dev")
                if dev is not None:
                    # Get product id
                    strid = dev.get("id")
                    if strid is None:
                        raise SwapMakerException("Device section needs a valid ID in " + self.file_name)
                        return
                    self.prodid = int(strid)
                    # Get folder name / command-line option
                    self.prodname = dev.get("name")
                    if self.prodname is None:
                        raise SwapMakerException("Device section needs a comman-line option in " + self.file_name)
                        return
                    # Get GUI label
                    self.prodlabel = dev.get("label")
                    if self.prodlabel is None:
                        raise SwapMakerException("Device section needs a label in " + self.file_name)
                        return
        except IOError:
            return
   
    
    def save(self):
        """
        Save file
        """
        try:
            f = open(self.file_name, 'w')
            f.write("<?xml version=\"1.0\"?>\n")
            f.write("<devices>\n")
            f.write("\t<developer id=\"" + str(self.develid) + "\" name=\"" + self.develname + "\">\n")
            f.write("\t\t<dev id=\"" + str(self.prodid) + "\" name=\"" + self.prodname + "\" label=\"" + self.prodlabel + "\"/>\n")
            f.write("\t</developer>\n")
            f.write("</devices>\n")
            f.close()
        except:
            raise SwapMakerException("Unable to save " + self.file_name)
    
    
    def __init__(self, filename, develid=None, develname=None, prodid=None, prodname=None, prodlabel=None):
        """
        Class constructor
        
        @param filename: path to the directory file
        @param develid: developer id
        @param develname: developer name
        @param prodid: product id
        @param prodname: product name
        @param prodlabel: product label (description)
        """
        # Path to the configuration file
        self.file_name = filename
        # Developer id
        self.develid = develid
        # Developer name
        self.develname = develname
        # Product id
        self.prodid = prodid
        # Product name
        self.prodname = prodname
        # Product label (description)
        self.prodlabel = prodlabel
        # Parse document
        self.read()

        
class XmlDevice(object):
    """
    Device configuration settings
    """

    def get_definition(self):
        """
        Read current configuration file
        """
        if self.file_name is not None:
            # Parse XML file
            try:
                tree = xml.parse(self.file_name)
            except:
                os.remove(self.file_name)
                return

            if tree is None:
                raise IOError(self.file_name  + " does not exist")
            # Get the root node
            root = tree.getroot()
            # Get manufacturer
            elem = root.find("developer")
            if elem is not None:
                self.developer = elem.text
            # Get product name
            elem = root.find("product")
            if elem is not None:
                self.product = elem.text
            # Get Power Down flag
            elem = root.find("pwrdownmode")
            if elem is not None:
                self.pwrdownmode = (elem.text.lower() in ["true", "1", "enable", "yes"])


    def get_registers(self, config=False):
        """
        Return list of registers

        @param config: Set to True if Configuration register are required. False for regular ones

        @return List of registers
        """
        if self.file_name is None:
            return None

        # List of registers belonging to the current device
        registers = []

        try:
            # Parse XML file
            tree = xml.parse(self.file_name)
            if tree is None:
                return []
            # Get the root node
            root = tree.getroot()
            # Get manufacturer
    
            # List of register elements belonging to the device
            regtype = "regular"
            if config == True:
                regtype = "config"
            regs = root.findall(regtype + "/reg")
            if regs is not None:
                for reg in regs:
                    # Get register id
                    strid = reg.get("id")
                    if strid is not None:
                        regid = int(strid)
                        if regid > RegisterDialog.last_register_id:
                            RegisterDialog.last_register_id = regid
                        # Get register name
                        regname = reg.get("name", default="")
                        # Create register entry
                        register = {"id": regid, "name": regname}
    
                        # List of endpoints belonging to the register
                        parameters = []
                        if config == True:
                            elementName = "param"
                        else:
                            elementName = "endpoint"
                        params = reg.findall(elementName)
                        
                        if params is not None:
                            for param in params:
                                # Read XML fields
                                paramtype = param.get("type", default="num")
                                paramdir = param.get("dir", default="out")
                                paramname = param.get("name", default="")
                                if paramtype == "bin":
                                    paramsize = "1"
                                else:
                                    paramsize = "8"
                                elem = param.find("size")
                                if elem is not None:
                                    tuplesize = self.get_bytebits(strsize=elem.text)
                                    paramsize = tuplesize[0] * 8 + tuplesize[1]
                                paramdef = None
                                elem = param.find("default")
                                if elem is not None:
                                    paramdef = elem.text
                                if paramdef is not None:
                                    if paramtype in ["num", "bin"]:
                                        try:
                                            paramdef = int(paramdef)
                                        except ValueError:
                                            try:
                                                paramdef = float(paramdef)
                                            except ValueError:
                                                raise SwapMakerException("Default value " + str(paramdef) + " is not a number")
                                                return
                                paramverif = None
                                elem = param.find("verif")
                                if elem is not None:
                                    paramverif = elem.text
                                # Get list of units
                                uts = param.findall("units/unit")
                                units = []
                                if uts is not None and len(uts) > 0:
                                    for ut in uts:
                                        name = ut.get("name", default=None)
                                        factor = ut.get("factor", default=1)
                                        offset = ut.get("offset", default=0)
                                        unit = {"name": name, "factor": factor, "offset": offset}
                                        units.append(unit)
        
                                parameter = {"name": paramname, "type": paramtype, "size": paramsize, "units": units}
                                
                                parameter["dir"] = paramdir
                                if config:
                                    if paramdef is not None:
                                        parameter["default"] = paramdef
                                    if paramverif is not None:
                                        parameter["verif"] = paramverif
                                else:
                                    parameter["units"] = units
        
                                # Add current parameter to the list
                                parameters.append(parameter)
                 
                        # Add list of parameters to the register
                        if config:
                            register["parameters"] = parameters
                        else:
                            register["endpoints"] = parameters
    
                        # Append register to the list of registers
                        registers.append(register)
        except IOError:
            pass

        # Return list of registers
        return registers
   

    def get_bytebits(self, bits=None, strsize=None):
        """
        Return (byte, bit) tuple
        
        @param bits: number of bits to be converted
        @param strsize: bytes.bits string
        
        @return (byte, bit) tuple
        """
        if bits is not None:
            return (bits/8, bits%8)
        if strsize is not None:
            res = strsize.split(".")
            if len(res) == 1:
                res[1] = 0
            return map(int, res)
    
    
    def save(self):
        """
        Save file
        """
        try:
            f = open(self.file_name, 'w')
            f.write("<?xml version=\"1.0\"?>\n")
            f.write("<device>\n")
            f.write("\t<developer>" + self.developer + "</developer>\n")
            f.write("\t<product>" + self.product + "</product>\n")
            f.write("\t<pwrdownmode>" + str(self.pwrdownmode).lower() + "</pwrdownmode>\n")

            # Configuration registers
            if self.config_registers is not None:
                if len(self.config_registers) > 0:
                    f.write("\t<config>\n")
                    for reg in self.config_registers:
                        curr_bitpos = 0
                        f.write("\t\t<reg name=\"" + reg["name"] + "\" id=\"" + str(reg["id"]) + "\">\n")                    
                        for param in reg["parameters"]:
                            bitsize = int(param["size"])
                            f.write("\t\t\t<param name=\"" + param["name"] + "\" type=\"" + param["type"] + "\">\n")
                            if len(reg["parameters"]) > 1:
                                pos = self.get_bytebits(curr_bitpos)
                                strpos = ".".join(map(str, pos))
                                curr_bitpos = pos[0] * 8 + pos[1]
                                f.write("\t\t\t\t<position>" + strpos + "</position>\n")
                                param["bytepos"] = pos[0]
                                param["bitpos"] = pos[1]
                            curr_bitpos += bitsize
                            siz = self.get_bytebits(bitsize)
                            strsize = ".".join(map(str, siz))
                            param["bytesize"] = siz[0]
                            param["bitsize"] = siz[1]
                            f.write("\t\t\t\t<size>" + strsize + "</size>\n")
                            if "default" in param:
                                if param["default"] != "":
                                    f.write("\t\t\t\t<default>" + param["default"] + "</default>\n")
                            if "verif" in param:
                                if param["verif"] != "":
                                    f.write("\t\t\t\t<verif>" + param["verif"] + "</verif>\n")
                            f.write("\t\t\t</param>\n")
                        reg["size"] = curr_bitpos/8
                        if curr_bitpos%8 > 0:
                            reg["size"] += 1                    
                        f.write("\t\t</reg>\n")
                    f.write("\t</config>\n")

            # Regular registers
            if self.regular_registers is not None:
                if len(self.regular_registers) > 0:
                    f.write("\t<regular>\n")
                    for reg in self.regular_registers:
                        curr_bitpos = 0
                        f.write("\t\t<reg name=\"" + reg["name"] + "\" id=\"" + str(reg["id"]) + "\">\n")                    
                        for param in reg["endpoints"]:
                            bitsize = int(param["size"])
                            f.write("\t\t\t<endpoint name=\"" + param["name"] + "\" type=\"" + param["type"] + "\" dir=\"" + param["dir"] + "\">\n")
                            if len(reg["endpoints"]) > 1:
                                pos = self.get_bytebits(curr_bitpos)
                                strpos = ".".join(map(str, pos))
                                curr_bitpos = pos[0] * 8 + pos[1]
                                f.write("\t\t\t\t<position>" + strpos + "</position>\n")
                                param["bytepos"] = pos[0]
                                param["bitpos"] = pos[1]
                            curr_bitpos += bitsize
                            siz = self.get_bytebits(bitsize)                        
                            strsize = ".".join(map(str, siz))
                            param["bytesize"] = siz[0]
                            param["bitsize"] = siz[1]
                            f.write("\t\t\t\t<size>" + strsize + "</size>\n")
                            if "default" in param:
                                f.write("\t\t\t\t<default>" + param["default"] + "</default>\n")
                            if "units" in param:
                                f.write("\t\t\t\t<units>\n")
                                for unit in param["units"]:
                                    f.write("\t\t\t\t\t<unit name=\"" + unit["name"] + "\" factor=\"" + unit["factor"] + "\" offset=\"" + unit["offset"] + "\"/>\n")
                                f.write("\t\t\t\t</units>\n")
                            f.write("\t\t\t</endpoint>\n")
                        reg["size"] = curr_bitpos/8
                        if curr_bitpos%8 > 0:
                            reg["size"] += 1                    
                        f.write("\t\t</reg>\n")
                    f.write("\t</regular>\n")
            f.write("</device>\n")
            f.close()
        except:
            raise SwapMakerException("Unable to save " + self.file_name)


    def __init__(self, filename="mydevice.xml", developer=None, product=None, pwrdownmode=False, cfgregs=None, regregs=None):
        """
        Class constructor
        
        @param filename: file name
        @param developer: developer name
        @param product: brief product description
        @param pwrdownmode: low-power flag
        @param cfgregs: list of config registers
        @param regregs: list of regular registers
        """
       
        ## Name/path of the current configuration file
        self.file_name = filename
        
        file_exists = True
        try:
            f = open(self.file_name)
            f.close()
        except IOError:
            file_exists = False

        ## Name of the developer
        self.developer = developer
        ## Name of the Product
        self.product = product
        ## Power down mode (True or False). If True, the mote sleeps most of the times
        self.pwrdownmode = pwrdownmode
        ## Lists of registers
        self.config_registers = cfgregs
        self.regular_registers = regregs
        
        if developer is None:
            if file_exists:
                # Read definition parameters from XML file
                self.get_definition()
                if cfgregs is None:
                    # Read config registers
                    self.config_registers = self.get_registers(True)
                if regregs is None:
                    # Read regular registers
                    self.regular_registers = self.get_registers(False)
