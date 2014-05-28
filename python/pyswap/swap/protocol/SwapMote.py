#########################################################################
#
# SwapMote
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
# Author: Daniel Berenguer
# Creation date: 20-Aug-2011
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Aug 20, 2011 10:36:00 AM$"
#########################################################################

from SwapPacket import SwapStatusPacket, SwapCommandPacket, SwapQueryPacket
from SwapDefs import SwapRegId, SwapState
from SwapValue import SwapValue
from swap.SwapException import SwapException
from swap.xmltools.XmlDevice import XmlDevice

import time


class SwapMote(object):
    """
    SWAP mote class
    """
    def cmdRegister(self, regId, value):
        """
        Send command to register and return expected response
        
        @param regId: Register ID
        @param value: New value
        
        @return Expected SWAP status packet sent from mote after reception of this command
        """
        # Expected response from mote
        infPacket = SwapStatusPacket(self.address, regId, value)
        # Command to be sent to the mote
        cmdPacket = SwapCommandPacket(self.address, regId, value, self.nonce)
        # Send command
        cmdPacket.send(self.server)
        # Return expected response
        return infPacket;


    def qryRegister(self, regId):
        """
        Send query to register
        
        @param regId: Register ID
        """
        # Query packet to be sent
        qryPacket = SwapQueryPacket(self.address, regId)
        # Send query
        qryPacket.send(self.server)


    def staRegister(self, regId):
        """
        Send SWAP status packet about the current value of the register passed as argument
        
        @param regId: Register ID
        @param value: New value
        """
        # Get register
        reg = self.getRegister(regId)
        # Status packet to be sent
        infPacket = SwapStatusPacket(self.address, regId, reg.value)
        # Send SWAP status packet
        infPacket.send(self.server)


    def cmdRegisterWack(self, regId, value):
        """
        Send SWAP command to remote register and wait for confirmation
        
        @param regId: Register ID
        @param value: New value
        
        @return True if ACK is received from mote. Return False otherwise
        """
        return self.server.setMoteRegister(self, regId, value)


    def setAddress(self, address):
        """
        Set mote address
        
        @param address: New mote address
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(address, length=1)
        return self.cmdRegisterWack(SwapRegId.ID_DEVICE_ADDR, val)


    def setNetworkId(self, netId):
        """
        Set mote's network id. Return true if ACK received from mote
        
        @param netId: New Network ID
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(netId, length=2)
        return self.cmdRegisterWack(SwapRegId.ID_NETWORK_ID, val)


    def setFreqChannel(self, channel):
        """
        Set mote's frequency channel. Return true if ACK received from mote
        
        @param channel: New frequency channel
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(channel, length=1)
        return self.cmdRegisterWack(SwapRegId.ID_FREQ_CHANNEL, val)


    def setSecurity(self, secu):
        """
        Set mote's security option. Return true if ACK received from mote
        
        @param secu: Security option
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(secu, length=1)
        return self.cmdRegisterWack(SwapRegId.ID_SECU_OPTION, val)


    def setTxInterval(self, interval):
        """
        Set periodic Tx interval. Return true if ACK received from mote
        
        @param interval: New Tx interval
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(interval, length=2)
        return self.cmdRegisterWack(SwapRegId.ID_TX_INTERVAL, val)
    
    
    def restart(self):
        """
        Ask mote to restart
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(SwapState.RESTART, length=1)
        return self.cmdRegisterWack(SwapRegId.ID_SYSTEM_STATE, val)


    def leaveSync(self):
        """
        Ask mote to leave SYNC mode (RXON state)
        
        @return True if this command is confirmed from the mote. Return False otherwise
        """
        val = SwapValue(SwapState.RXOFF, length=1)
        return self.cmdRegisterWack(SwapRegId.ID_SYSTEM_STATE, val)

    
    def updateTimeStamp(self):
        """
        Update time stamp
        """
        self.timestamp = time.time()
    
    
    def getRegister(self, regId):
        """
        Get register given its ID
        
        @param regId: Register ID
        
        @return SwapRegister object
        """
        # Regular registers
        for reg in self.regular_registers:
            if reg.id == regId:
                return reg            
        # Configuration registers
        for reg in self.config_registers:
            if reg.id == regId:
                return reg

        return None


    def getParameter(self, name):
        """
        Get parameter given its name
        
        @param name: name of the parameter belonging to this mote
        
        @return: SwapParam object
        """
        # Regular registers
        for reg in self.regular_registers:
            for param in reg.parameters:
                if param.name == name:
                    return param
        # Configuration registers
        for reg in self.config_registers:
            for param in reg.parameters:
                if param.name == name:
                    return param
                
        return None
    
    
    def dumps(self, include_units=False):
        """
        Serialize mote data to a JSON formatted string
        
        @param include_units: if True, include list of units for each endpoint
        within the serialized output
        """
        data = {}
        data["pcode"] = self.product_code
        data["manufacturer"] = self.definition.manufacturer 
        data["name"] = self.definition.product
        data["address"] = self.address
        
        regs = []
        try:
            for reg in self.regular_registers:
                regs.append(reg.dumps(include_units))
        except SwapException:
            raise
            
        data["registers"] = regs
        
        return data
    
        
    def __init__(self, server=None, product_code=None, address=0xFF, security=0, nonce=0):
        """
        Class constructor
        
        @param server: SWAP server object
        @param product_code: Product Code
        @param address: Mote address
        """
        if server is None:
            raise SwapException("SwapMote constructor needs a valid SwapServer object")
        ## Swap server object
        self.server = server
        ## Product code
        self.product_code = product_code
        ## Product ID
        self.product_id = 0
        ## Manufacturer ID
        self.manufacturer_id = 0
        ## Definition settings
        self.config = None

        # Get manufacturer and product id from product code
        """
        if product_code is not None:
            for i in range(4):
                self.manufacturer_id = self.manufacturer_id | (product_code[i] << 8 * (3-i))
                self.product_id = self.product_id | (product_code[i + 4] << 8 * (3-i))
        """
        try:
            self.manufacturer_id = long(product_code[0:8], 16)
            self.product_id = long(product_code[8:], 16)
        except:
            raise SwapException("Incorrect product code received")
        
        # Definition file
        ## Definition settings
        try:
            self.definition = XmlDevice(self)
        except:
            raise
            
        ## Device address
        self.address = address
        ## Security option
        self.security = security
        ## Current mote's security nonce
        self.nonce = nonce
        ## State of the mote
        self.state = SwapState.RXOFF
        ## List of regular registers provided by this mote
        self.regular_registers = None
        ## List of config registers provided by this mote
        self.config_registers = None
        if self.definition is not None:
            # List of regular registers
            self.regular_registers = self.definition.getRegList()
            # List of config registers
            self.config_registers = self.definition.getRegList(config=True)
        ## Time stamp of the last update received from mote
        self.timestamp = time.time()
        ## Powerdown mode
        self.pwrdownmode = self.definition.pwrdownmode
        ## Interval between periodic transmissions
        self.txinterval = self.definition.txinterval

