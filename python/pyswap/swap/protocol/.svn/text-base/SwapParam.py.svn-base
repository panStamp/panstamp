#########################################################################
#
# SwapParam
#
# Copyright (c) 2011 panStamp <contact@panstamp.com>
# 
# This file is part of the panStamp project.
# 
# panStamp  is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# any later version.
# 
# panStamp is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with panStamp; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__ ="$Aug 26, 2011 8:56:27 AM$"
#########################################################################

from SwapDefs import SwapType
from SwapValue import SwapValue
from swap.SwapException import SwapException

import time
import math


class SwapParam:
    """
    Generic SWAP parameter, integrated into a SWAP register
    """
    def getRegAddress(self):
        """
        Return register address of the current parameter
        
        @return Register address
        """
        return self.register.getAddress()


    def getRegId(self):
        """
        Return register ID of the current parameter
        
        @return Register ID
        """
        return self.register.id
    
    
    def update(self):
        """
        Update parameter's value, posibly after a change in its parent register
        """
        self.valueChanged = False
        if self.register is None:
            raise SwapException("Register not specified for current endpoint")
            return

        # Current register value converted to list
        lstRegVal = self.register.value.toList()
        # Total bits to be copied
        indexReg = self.bytePos
        shiftReg = 7 - self.bitPos
        bitsToCopy = self.byteSize * 8 + self.bitSize
        # Current parameter value in list format
        lstParamVal = self.value.toList()

        if len(lstParamVal) == 0:
            return

        # Keep old value
        oldParamVal = self.value.clone()
        indexParam = 0
        shiftParam = self.bitSize - 1

        if shiftParam < 0:
            shiftParam = 7
        for i in range(bitsToCopy):
            if indexReg >= len(lstRegVal):
                break            
            if (lstRegVal[indexReg] >> shiftReg) & 0x01 == 0:
                mask = ~(1 << shiftParam)
                lstParamVal[indexParam] &= mask
            else:
                mask = 1 << shiftParam
                lstParamVal[indexParam] |= mask

            shiftReg -= 1
            shiftParam -= 1

            # Register byte over?
            if shiftReg < 0:
                indexReg += 1
                shiftReg = 7

            # Parameter byte over?
            if shiftParam < 0:
                indexParam += 1
                shiftParam = 7

        # Did the value change?
        if not self.value.isEqual(oldParamVal):
            self.valueChanged = True
            
        # Update time stamp
        self.lastupdate = time.time()


    def setValue(self, value):
        """
        Set parameter value

        @param value: New parameter value
        """
        # Convert to SwapValue
        if value.__class__ is SwapValue:
            # Incorrect length?
            if self.value.getLength() != value.getLength():
                return
            self.value = value
        else:
            # Byte length
            length = self.byteSize
            if self.bitSize > 0:
                length += 1
                       
            if type(value) is list:
                res = value
            elif type(value) in [str, unicode]:
                if self.type == SwapType.NUMBER:
                    try:
                        # Possible integer number
                        res = int(value)
                    except ValueError:
                        try:
                            # Possible float number
                            res = float(value)
                        except ValueError:
                            raise SwapException(value + " is not a valid numeric value for " + self.name)
                elif self.type == SwapType.BINARY:
                    if value.lower() in ["on", "open", "1", "true", "enabled"]:
                        res = 1
                    else:
                        res = 0
                else:   # SwapType.STRING
                    res = value
            else:
                res = value
                
            if type(res) in [int, float]:
                if self.unit is not None:
                    res -= self.unit.offset
                    res /= self.unit.factor
                    # Convert to integer
                    res = int(res)
                
            self.value = SwapValue(res, length)
            
        # Update current value
        self.value = SwapValue(res, length)
        # Update time stamp
        self.lastupdate = time.time()

        # Update register value
        self.register.update()
      
        
    def getValueInAscii(self):
        """
        Return value in ASCII string format
        
        @return Value in ASCII format
        """
        if self.type == SwapType.NUMBER:
            val = self.value.toInteger()
            # Add units
            if self.unit is not None:                
                if self.unit.calc is not None:
                    oper = self.unit.calc.replace("${val}", str(val))
                    try:    
                        val = eval("math." + oper)
                    except ValueError as ex:
                        raise SwapException("Math exception for " + oper + ". " + str(ex))
                                         
                strVal = str(val * self.unit.factor + self.unit.offset)
            else:
                strVal = str(val)
        elif self.type == SwapType.BINARY:
            strVal = self.value.toAscii()
            if strVal == "1":
                strVal = "on"
            elif strVal == "0":
                strVal = "off"
        else:
            strVal = self.value.toAsciiStr()
        
        return strVal
    
    
    def setUnit(self, strunit):
        """
        Set unit for the current parameter
        
        @param strunit: new unit in string format
        """
        if self.lstunits is None:
            raise SwapException("Parameter " + self.name + " does not support units")
        
        for unit in self.lstunits:
            if unit.name == strunit:
                self.unit = unit
                return
            
        raise SwapException("Unit " + strunit + " not found")
    
    
    def __init__(self, register=None, pType=SwapType.NUMBER, direction=SwapType.INPUT, name="", position="0", size="1", default=None, verif=None, units=None):
        """
        Class constructor

        @param register: Register containing this parameter
        @param pType: Type of SWAP endpoint (see SwapDefs.SwapType)
        @param direction: Input or output (see SwapDefs.SwapType)
        @param name: Short description about the parameter
        @param position: Position in bytes.bits within the parent register
        @param size: Size in bytes.bits
        @param default: Default value in string format
        @param verif: Verification string
        @param units: List of units
        """
        ## Parameter name
        self.name = name
        ## Register where the current parameter belongs to
        self.register = register
             
        ## Data type (see SwapDefs.SwapType for more details)
        self.type = pType       
        ## Direction (see SwapDefs.SwapType for more details)
        self.direction = direction
        ## Position (in bytes) of the parameter within the register
        self.bytePos = 0
        ## Position (in bits) after bytePos
        self.bitPos = 0
        # Get true positions
        dot = position.find('.')
        if dot > -1:
            self.bytePos = int(position[:dot])
            self.bitPos = int(position[dot+1:])
        else:
            self.bytePos = int(position)

        ## Size (in bytes) of the parameter value
        self.byteSize = 1
        ## Size in bits of the parameter value after byteSize
        self.bitSize = 0
        # Get true sizes
        dot = size.find('.')
        if dot > -1:
            self.byteSize = int(size[:dot])
            self.bitSize = int(size[dot+1:])
        else:
            self.byteSize = int(size)

        ## Current value
        self.value = None
        ## Time stamp of the last update
        self.lastupdate = None
        
        ## List of units
        self.lstunits = units
        ## Selected unit
        self.unit = None
        if self.lstunits is not None and len(self.lstunits) > 0:
            self.unit = self.lstunits[0]

        # Set initial value
        if default is not None:
            self.setValue(default)

        ## Flag that tells us whether this parameter changed its value during the last update or not
        self.valueChanged = False
        
        ## Verification string. This can be a macro or a regular expression
        self.verif = verif
        
        ## Display this parameter from master app
        self.display = True
        

class SwapCfgParam(SwapParam):
    """
    Class representing a configuration parameter for a given mote
    """

    def __init__(self, register=None, pType=SwapType.NUMBER, name="",
                position="0", size="1", default=None, verif=None):
        """
        Class constructor

        @param register: Register containing this parameter
        @param pType: Type of SWAP endpoint (see SwapDefs.SwapType)
        @param direction: Input or output (see SwapDefs.SwapType)
        @param name: Short name about the parameter
        @param description: Short description about hte parameter
        @param position: Position in bytes.bits within the parent register
        @param size: Size in bytes.bits
        @param default: Default value in string format
        @param verif: Verification string
        """
        SwapParam.__init__(self, register, pType, None, name, position, size, default, verif)
        ## Default value
        self.default = default
        
        
class SwapEndpoint(SwapParam):
    """
    SWAP endpoint class
    """
    def cmdWack(self, value):
        """
        Send SWAP command to remote endpoint and wait for confirmation
        
        @param value: New value
        
        @return True if ACK is received from mote. Return False otherwise
        """
        return self.register.mote.server.setEndpointValue(self, value)
    
    
    def sendSwapCmd(self, value):
        """
        Send SWAP command for the current endpoint
        
        @param value: New endpoint value
        
        @return Expected SWAP status response to be received from the mote
        """
        # Convert to SwapValue
        if value.__class__ is SwapValue:
            swap_value = value
        else:
            # Byte length
            length = self.byteSize
            if self.bitSize > 0:
                length += 1
                
            if type(value) is list:
                res = value
            elif type(value) in [str, unicode]:
                if self.type == SwapType.NUMBER:
                    try:
                        # Possible integer number
                        res = int(value)
                    except ValueError:
                        try:
                            # Possible float number
                            res = float(value)
                        except ValueError:
                            raise SwapException(value + " is not a valid numeric value for " + self.name)
                elif self.type == SwapType.BINARY:
                    if value.lower() in ["on", "open", "1", "true", "enabled"]:
                        res = 1
                    else:
                        res = 0
                else:   # SwapType.STRING
                    res = value
            else:
                res = value
                
            if type(res) in [int, float]:
                if self.unit is not None:
                    res -= self.unit.offset
                    res /= self.unit.factor
                    # Take integer part only
                    res = math.modf(res)
                
            swap_value = SwapValue(res, length)

        # Register value in list format
        lstRegVal = []
        lstRegVal[:] = self.register.value.toList()
        
        # Build register value
        indexReg = self.bytePos
        shiftReg = 7 - self.bitPos
        # Total bits to be copied from this parameter
        bitsToCopy = self.byteSize * 8 + self.bitSize
        # Parameter value in list format
        lstParamVal = swap_value.toList()
        indexParam = 0
        shiftParam = self.bitSize - 1
        if shiftParam < 0:
            shiftParam = 7

        for i in range(bitsToCopy):
            if (lstParamVal[indexParam] >> shiftParam) & 0x01 == 0:
                mask = ~(1 << shiftReg)
                lstRegVal[indexReg] &= mask
            else:
                mask = 1 << shiftReg
                lstRegVal[indexReg] |= mask

            shiftReg -= 1
            shiftParam -= 1

            # Register byte over?
            if shiftReg < 0:
                indexReg += 1
                shiftReg = 7

            # Parameter byte over?
            if shiftParam < 0:
                indexParam += 1
                shiftParam = 7
        
        
        # Convert to SWapValue
        newRegVal = SwapValue(lstRegVal)

        # Send SWAP command
        return self.register.sendSwapCmd(newRegVal)


    def sendSwapQuery(self):
        """
        Send SWAP query for the current endpoint
        """
        self.register.sendSwapQuery()

    
    def sendSwapStatus(self):
        """
        Send SWAP status packet about this endpoint
        """
        self.register.sendSwapStatus()
  
   
    def dumps_units(self):
        """
        Serialize list of units available for this endpoint
        """
        data = []
        for unit in self.lstunits:
            data.append(unit.name)
            
        return data
    
        
    def dumps(self, include_units=False):
        """
        Serialize endpoint data to a JSON formatted string
        
        @param include_units: if True, include list of units within the serialized output
        """
        try:
            val = self.getValueInAscii()
        except SwapException:
            raise
        
        data = {}
        data["id"] = self.id.replace(" ", "_")
        data["name"] = self.name.replace(" ", "_")
        data["location"] = self.location.replace(" ", "_")
        data["type"] = self.type
        data["direction"] = self.direction
        
        if self.lastupdate is not None:
            data["timestamp"] = time.strftime("%d %b %Y %H:%M:%S", time.localtime(self.lastupdate))
            
        data["value"] = val
        if self.unit is not None:
            data["unit"] = self.unit.name
            if include_units:
                data["units"] = self.dumps_units()
       
        return data
       
    
    def __init__(self, register=None, pType=SwapType.NUMBER, direction=SwapType.INPUT,
                name="", position="0", size="1", default=None, verif=None, units=None):
        """
        Class constructor
        
        @param register: Register containing this parameter
        @param pType: Type of SWAP endpoint (see SwapDefs.SwapType)
        @param direction: Input or output (see SwapDefs.SwapType)
        @param name: Short description about the parameter
        @param description: Short description about hte parameter
        @param position: Position in bytes.bits within the parent register
        @param size: Size in bytes.bits
        @param default: Default value in string format
        @param verif: Verification string
        @param units: List of units
        """
        SwapParam.__init__(self, register, pType, direction, name, position, size, default, verif, units)

        ## Endpoint unique id
        endp_index = len(self.register.parameters)
        self.id = str(self.getRegAddress()) + "." + str(self.getRegId()) + "." + str(endp_index)

        ## Endpoint locationm
        self.location = "SWAP"
        
        ## Time stamp
        self.lastupdate = None
