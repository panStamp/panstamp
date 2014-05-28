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
__date__ ="May 13, 2012"
#########################################################################

from errors import SwapMakerException

import os
import time


working_dir = os.path.dirname(__file__)


class SourceFile(object):
    """
    Source file class
    """

    def __init__(self, filename, description="", developer="", product=""):
        """
        Class constructor
        
        @param filename: file name
        @param description: file description
        @param developer: developer name
        @param product: product name 
        """
       
        # Name/path of the source code file
        self.file_name = filename
                      
        try:
            target = open(self.file_name,"w")
            
            file_path = os.path.join(working_dir, "templates", "header.txt")
            header = open(file_path,"r")
            for line in header:
                newline = line.replace("${FILE_NAME}", os.path.split(self.file_name)[1])
                if newline == line:
                    newline = line.replace("${FILE_DESCRIPTION}", description)
                if newline == line:
                    newline = line.replace("${SWAP_PRODUCT_NAME}", product)
                if newline == line:
                    newline = line.replace("${SWAP_MANUFACT_NAME}", developer)
                if newline == line:
                    newline = line.replace("${DATE}", time.strftime("%d %b %Y", time.localtime()))
                target.write(newline)
            header.close()
           
            target.close()
        except:
            raise SwapMakerException("Unable to process header for " + self.file_name)


class ProductFile(SourceFile):
    """
    Product file class
    """
    def __init__(self, filename="product.h", developer="", product="",  hwversion="", fwversion="", develid="", productid=""):
        """
        Class constructor
        
        @param filename: file name
        @param developer: developer name
        @param product: product name
        @param hwversion: Hardware version
        @param fwversion: firmware version
        @param develid: developer id
        @param productid: product id 
        """
        SourceFile.__init__(self, filename,
                            description="Product information",
                            developer=developer,
                            product=product)
        
        try:
            target = open(filename,"a")
            
            file_path = os.path.join(working_dir, "templates", "product.h")
            templ = open(file_path,"r")
            for line in templ:
                newline = line.replace("${HARDWARE_VERSION}", hwversion)
                if newline == line:
                    newline = line.replace("${FIRMWARE_VERSION}", fwversion)
                if newline == line:
                    newline = line.replace("${SWAP_MANUFACT_ID}", develid)
                if newline == line:
                    newline = line.replace("${SWAP_PRODUCT_ID}", productid)
                target.write(newline)
            templ.close()
            
            target.close()
        except:
            raise SwapMakerException("Unable to process template for " + self.file_name)


class RegtableHFile(SourceFile):
    """
    Regtable header file class
    """
    def __init__(self, filename="regtable.h", developer="", product="", registers=None):
        """
        Class constructor
        
        @param filename: file name
        @param developer: developer name
        @param product: product name
        @param registers: list of registers
        """
        SourceFile.__init__(self, filename,
                            description="List of registers. Header file.",
                            developer=developer,
                            product=product)
        
        if registers is None:
            return
        
        buf = ""
        for i, reg in enumerate(registers):
            rname = reg["name"].replace(" ", "")
            buf += "  REGI_" + rname.upper()
            if i < len(registers) - 1:
                buf += ",\n"
        
        try:
            target = open(filename,"a")
            
            file_path = os.path.join(working_dir, "templates", "regtable.h")
            templ = open(file_path,"r")
            for line in templ:
                newline = line.replace("${REGTABLE_INDEX}", buf)
                target.write(newline)
            templ.close()
            
            target.close()
        except:
            raise SwapMakerException("Unable to process template for " + self.file_name)


class RegtableFile(SourceFile):
    """
    Regtable file class
    """
    def __init__(self, filename="regtable.ino", developer="", product="", registers=None):
        """
        Class constructor
        
        @param filename: file name
        @param developer: developer name
        @param product: product name
        @param registers: list of registers
        """
        SourceFile.__init__(self, filename,
                            description="List of registers. Definition and handlers.",
                            developer=developer,
                            product=product)
        
        if registers is None:
            return
        
        # Definition of custom registers
        customregs = ""
        # Table of registers
        tableregs = ""
        # Callback functions
        callbackfuncts = ""


        for i, reg in enumerate(registers):
            nm = reg["name"].replace(" ", "")
            dname = "dt" + nm
            rname = "reg" + nm
            updtcallback = "updt" + nm
            setcallback = "set" + nm
            customregs += "static byte " + dname + "[" + str(reg["size"]) + "];\n"
            customregs += "REGISTER " + rname + "(" + dname + ", sizeof(" + dname + "), "
            callbackfuncts += "/**\n * "

            direction = "out"
            if "endpoints" in reg:
                if len(reg["endpoints"]) > 0:
                    direction = reg["endpoints"][0]["dir"]
        
            if direction == "out":
                customregs += "NULL, &" + setcallback + ");\n"
                callbackfuncts += setcallback
                callbackfuncts += "\n *\n * Set " + nm + "\n *\n * rId: register ID\n * value: new value\n*/\n"
                callbackfuncts += "const void " + setcallback + "(byte rId, byte *value)\n{\n"
                
                callbackfuncts += "  // Update register value:\n"
                callbackfuncts += "  memcpy(regTable[rId]->value, value, sizeof(regTable[rId]->value));\n\n"
                callbackfuncts += "  // Change your outputs or variables here:\n"
                
            else:
                customregs += "&" + updtcallback + ", NULL);\n"
                callbackfuncts += updtcallback
                callbackfuncts += "\n *\n * Update " + nm + "\n *\n * rId: register ID\n */\n"
                callbackfuncts += "const void " + updtcallback + "(byte rId)\n{\n"
                
                callbackfuncts += "  // Update register from:\n"
                for endp in reg["endpoints"]:
                    bytepos = 0
                    bitpos = 0
                    if "bytepos" in endp:
                        bytepos = endp["bytepos"]
                        if "bitpos" in endp:
                            bitpos = endp["bitpos"]
                    callbackfuncts += "  // " + endp["name"] + " (Position = byte " + str(bytepos)
                    callbackfuncts += " bit " + str(bitpos) + " - Size = " + str(endp["size"]) + " bits)\n"                    
            
            callbackfuncts += "}\n\n"
            
            tableregs += "  &" + rname
            if i < len(registers) - 1:
                tableregs += ",\n"

        # Replace fields from template        
        try:
            target = open(filename,"a")
            
            file_path = os.path.join(working_dir, "templates", "regtable.ino")
            templ = open(file_path,"r")
            for line in templ:
                newline = line.replace("${DEFINITIONS_CUSTOM_REGISTERS}", customregs)
                if newline == line:
                    newline = line.replace("${TABLE_CUSTOM_REGISTERS}", tableregs)
                if newline == line:
                    newline = line.replace("${DEFINITION_CALLBACKS}", callbackfuncts)
                target.write(newline)

            templ.close()
            
            target.close()
        except:
            raise SwapMakerException("Unable to process template for " + self.file_name)

class MainInoFile(SourceFile):
    """
    Main source file class
    """
    def __init__(self, filename="main.ino", developer="", product="", lowpower=False):
        """
        Class constructor
        
        @param filename: file name
        @param developer: developer name
        @param product: product name
        @param lowpower: true if this is a low-power mote
        """
        SourceFile.__init__(self, filename,
                            description="Main source file.",
                            developer=developer,
                            product=product)
               
        try:
            target = open(filename,"a")
            
            if lowpower:
                templ_name = "main_lowpower.ino"
            else:
                templ_name = "main.ino"
                
            file_path = os.path.join(working_dir, "templates", templ_name)
            templ = open(file_path,"r")
            for line in templ:
                target.write(line)
            templ.close()
            
            target.close()
        except:
            raise SwapMakerException("Unable to process template for " + self.file_name)


class ReadmeFile(SourceFile):
    """
    README file class
    """
    def __init__(self, filename="README.txt", developer="", product=""):
        """
        Class constructor
        
        @param filename: file name
        @param developer: developer name
        @param product: product name
        """
        SourceFile.__init__(self, filename,
                            description="Instructions",
                            developer=developer,
                            product=product)
        
        try:
            target = open(filename,"a")
            
            file_path = os.path.join(working_dir, "templates", "README.txt")
            templ = open(file_path,"r")
            for line in templ:
                newline = line.replace("${SWAP_PRODUCT_NAME}", product)
                target.write(newline)
            templ.close()
            
            target.close()
        except:
            raise SwapMakerException("Unable to process template for " + self.file_name)
