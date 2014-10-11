#########################################################################
#
# SwapRegister
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
__date__ ="$Aug 20, 2011 10:36:00 AM$"
#########################################################################

from SwapValue import SwapValue
from swap.SwapException import SwapException


class SwapRegister(object):
    """
    SWAP register class
    """

    def getAddress(self):
        """
        Return address of the current register
        
        @return Register address
        """
        return self.mote.address

    
    def sendSwapCmd(self, value):
        """
        Send SWAP command to the current register
        
        @param value: New register value
        
        @return Expected SWAP status response to be received from the mote
        """
        return self.mote.cmdRegister(self.id, value)


    def sendSwapQuery(self):
        """
        Send SWAP query to the current register
        """
        self.mote.qryRegister(self.id)


    def sendSwapStatus(self):
        """
        Send SWAP status packet about this register
        """
        self.mote.staRegister(self.id)


    def cmdValueWack(self, value):
        """
        Send command to register value and wait for mote's confirmation

        @param value: New register value
        
        @return True if the command is successfully acknowledged
        """
        return self.mote.cmdRegisterWack(self.id, value)


    def add(self, item):
        """
        Add item (endpoint or parameter) to the associated list of items
        
        @param item: Item to be added to the list
        """
        self.parameters.append(item)


    def getNbOfItems(self):
        """
        Return the amount of items belonging to the current register
        
        @return Amount of items (endpoints or parameters) contained into the current register
        """
        return len(self.parameters)


    def getLength(self):
        """
        Return data length in bytes
        
        @return Length in bytes of the current register
        """
        maxByteSize = 0
        maxBytePos = 0
        maxBitSize = 0
        maxBitPos = 0
        # Iterate along the contained parameters
        for param in self.parameters:
            if param.bytePos > maxBytePos:
                maxBytePos = param.bytePos
                maxBitPos = param.bitPos
                maxByteSize = param.byteSize
                maxBitSize = param.bitSize
            elif param.bytePos == maxBytePos and param.bitPos >= maxBitPos:
                maxBitPos = param.bitPos
                maxByteSize = param.byteSize
                maxBitSize = param.bitSize

        # Calculate register length
        bitLength = maxBytePos * 8 + maxByteSize * 8 + maxBitPos + maxBitSize
        byteLength = bitLength / 8
        if (bitLength % 8) > 0:
            byteLength += 1

        return byteLength
    

    def update(self):
        """
        Update register value according to the values of its contained parameters
        """
        # Return if value is None?
        if self.value is None:
            return

        # Current register value converted to list
        lstRegVal = self.value.toList()

        # For every parameter contained in this register
        for param in self.parameters:
            indexReg = param.bytePos
            shiftReg = 7 - param.bitPos
            # Total bits to be copied from this parameter
            bitsToCopy = param.byteSize * 8 + param.bitSize
            # Parameter value in list format
            lstParamVal = param.value.toList()
            indexParam = 0
            shiftParam = param.bitSize - 1
            if shiftParam < 0:
                shiftParam = 7

            if len(lstParamVal) > 0:                
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
                    
        # Update mote's time stamp
        if self.mote is not None:
            self.mote.updateTimeStamp()

                
    def setValue(self, value):
        """
        Set register value

        @param value: New register value
        """
        if value.__class__ is not SwapValue:
            raise SwapException("setValue only accepts SwapValue objects")
            return
 
        # Set register value
        self.value = value
        
        # Update mote's time stamp
        self.mote.updateTimeStamp()
        
        # Now update the value in every endpoint or parameter contained in this register
        for param in self.parameters:
            param.update()
               
               
    def isConfig(self):
        """
        This method tells us whether the current register contains configuration paramters or not
        
        @return True if this register contains configuration parameters. Return False otherwise
        """
        if len(self.parameters) > 0:
            if self.parameters[0].__class__.__name__ == "SwapCfgParam":
                return True
        return False
    
    
    def dumps(self, include_units=False):
        """
        Serialize register data to a JSON formatted string
        
        @param include_units: if True, include list of units for each endpoint
        within the serialized output
        """
        if self.isConfig():
            return None
        
        data = {}
        data["id"] = self.id
        data["name"] = self.name
        
        endpoints_data = []
        
        try:
            for item in self.parameters:
                endpoints_data.append(item.dumps(include_units))
        except SwapException:
            raise
            
        data["endpoints"] = endpoints_data
        
        return data
        
        
    def __init__(self, mote=None, id=None, description=None):
        """
        Class constructor

        @param mote: Mote containing the current register
        @param id: Register ID
        @param name: Generic name of the current register
        """
        ## Mote owner of the current register
        self.mote = mote
        ## Register ID
        self.id = id
        ## SWAP value contained in the current register
        self.value = None
        ## Brief name
        self.name = description
        ## List of endpoints or configuration parameters belonging to the current register
        self.parameters = []

