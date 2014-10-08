#########################################################################
#
# SwapPacket
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
__date__ ="$Aug 20, 2011 10:36:00 AM$"
#########################################################################

from swap.modem.CcPacket import CcPacket
from SwapValue import SwapValue
from SwapDefs import SwapAddress, SwapFunction
from swap.SwapException import SwapException

import copy


class SwapPacket(CcPacket):
    """
    SWAP packet class
    """
    smart_encrypt_pwd = None

    def smart_encryption(self, password, decrypt=False):
        """
        Encrypt/Decrypt packet using the Smart Encryption mechanism
        
        @param password: Smart Encryption password
        @param decrypt:  Decrypt packet if True. Encrypt otherwise
        """
        # Update password
        SwapPacket.smart_encrypt_pwd = password

        # Encryot SwapPacket and CcPacket fields               
        if decrypt:
            self.nonce ^= password.data[9]
        
        self.function ^= password.data[11] ^ self.nonce
        self.srcAddress ^= password.data[10] ^ self.nonce
        self.regAddress ^= password.data[8] ^ self.nonce
        self.regId ^= password.data[7] ^ self.nonce
        
        if self.value is not None:
            pos = 0
            newarray = []
            for byte in self.value.toList():
                byte ^= password.data[pos] ^ self.nonce
                newarray.append(byte)
                pos += 1
                if pos == 11:
                    pos = 0
            self.value = SwapValue(newarray)

        if not decrypt:
            self.nonce ^= password.data[9]

        self._update_ccdata()
            
        
    def send(self, server):
        """
        Overriden send method
        
        @param server: SWAP server object to be used for transmission
        """
        self.srcAddress = server.devaddress
        self.data[1] = self.srcAddress
        
        # Update security option according to server's one
        self.security = server.security
        self.data[2] |= self.security & 0x0F

        # Keep copy of the current packet before encryption
        packet_before_encrypt = copy.copy(self)
        
        # Smart encryption enabled?
        if self.security & 0x02:
            # Encrypt packet
            self.smart_encryption(server.password)
        
        CcPacket.send(self, server.modem)
        # Notify event        
        server._eventHandler.swapPacketSent(packet_before_encrypt)
        
        
    def _update_ccdata(self):
        """
        Update ccPacket data bytes
        """
        if self.extended_address:
            """
            self.data[0] = (self.destAddress >> 8) & 0x0F
            self.data[1] = self.destAddress & 0x0F
            self.data[5] = (self.srcAddress >> 8) & 0x0F
            self.data[6] = self.srcAddress & 0x0F
            self.data[7] = (self.regAddress >> 8) & 0x0F
            self.data[8] = self.regAddress & 0x0F
            self.data[9] = self.regId
            """
            self.data.append((self.destAddress >> 8) & 0x0F)
            self.data.append(self.destAddress & 0x0F)
            
            self.data.append((self.hop << 4) | (self.security & 0x0F))
            self.data.append(self.nonce)
            self.data.append(self.function | (self.extended_address * (0x80)))
            
            self.data.append((self.srcAddress >> 8) & 0x0F)
            self.data.append(self.srcAddress & 0x0F)
            self.data.append((self.regAddress >> 8) & 0x0F)
            self.data.append(self.regAddress & 0x0F)
            self.data.append(self.regId)
        else:
            """
            self.data[0] = self.destAddress
            self.data[1] = self.srcAddress
            self.data[5] = self.srcAddress
            self.data[6] = self.regId
            """

            self.data.append(self.destAddress)
            self.data.append(self.srcAddress)       
            self.data.append((self.hop << 4) | (self.security & 0x0F))
            self.data.append(self.nonce)
            self.data.append(self.function | (self.extended_address * (0x80)))
            self.data.append(self.srcAddress)
            self.data.append(self.regId)
            
        """
        self.data[2] = (self.hop << 4) | (self.security & 0x0F)
        self.data[3] = self.nonce
        self.data[4] = self.function | (self.extended_address * (0x80))       
        """
        
        if self.value is not None:
            for item in self.value.toList():
                self.data.append(item)


    def __init__(self, ccPacket=None, destAddr=SwapAddress.BROADCAST_ADDR, hop=0, nonce=0, function=SwapFunction.STATUS, regAddr=0, regId=0, value=None, extended_addr=False):
        """
        Class constructor
        
        @param ccPacket: Raw CcPacket where to take the information from
        @param destAddr: Destination address
        @param hop: Transmission hop count
        @param nonce: Security nonce
        @param function: SWAP function code (see SwapDefs.SwapFunction for more details)
        @param regAddr: Register address (address of the mote where the register really resides)   
        @param regId: Register ID
        @param value: Register value  
        """
        CcPacket.__init__(self)

        ## Extended address disabled by default
        self.extended_address = extended_addr

        ## Destination address
        self.destAddress = destAddr
        ## Source address
        self.srcAddress = regAddr
        ## Hop count for repeating purposes
        self.hop = hop
        ## Security option
        self.security = 0
        ## Security nonce
        self.nonce = nonce
        ## Function code
        self.function = function
        ## Register address
        self.regAddress = regAddr
        ## Register ID
        self.regId = regId
        ## SWAP value
        self.value = value

        if ccPacket is not None:
            if len(ccPacket.data) < 7:
                raise SwapException("Packet received is too short")
            
            # Function code
            self.function = ccPacket.data[4] & 0x7F
            # Extended address indicator
            self.extended_address = (ccPacket.data[4] & 0x80) != 0
            # Superclass attributes
            ## RSSI byte
            self.rssi = ccPacket.rssi
            ## LQI byte
            self.lqi = ccPacket.lqi
            ## CcPacket data field
            self.data = ccPacket.data
                        
            if self.extended_address:
                # Destination address
                self.destAddress = (ccPacket.data[0] << 8) | ccPacket.data[1]
                # Source address
                self.srcAddress = (ccPacket.data[5] << 8) | ccPacket.data[6]
                # Register address
                self.regAddress = (ccPacket.data[7] << 8) | ccPacket.data[8]
                # Register ID
                self.regId = ccPacket.data[9]
            else:
                # Destination address
                self.destAddress = ccPacket.data[0]
                # Source address
                self.srcAddress = ccPacket.data[1]
                # Register address
                self.regAddress = ccPacket.data[5]
                # Register ID
                self.regId = ccPacket.data[6]
                
            # Hop count for repeating purposes
            self.hop = (ccPacket.data[2] >> 4) & 0x0F
            # Security option
            self.security = ccPacket.data[2] & 0x0F
            # Security nonce
            self.nonce = ccPacket.data[3]
                       
            if len(ccPacket.data) >= 8:
                self.value = SwapValue(ccPacket.data[7:])   
            # Encryption enabled?
            if self.security & 0x02 and SwapPacket.smart_encrypt_pwd is not None:
                # Decrypt packet
                self.smart_encryption(SwapPacket.smart_encrypt_pwd, decrypt=True)
        
        else:
            self._update_ccdata()
            

class SwapStatusPacket(SwapPacket):
    """
    SWAP status packet class
    """
    def __init__(self, rAddr, rId, val, extended_addr=False):
        """
        Class constructor
        
        @param rAddr: Register address
        @param rId: Register ID
        @param val: New value
        """
        SwapPacket.__init__(self, regAddr=rAddr, regId=rId, value=val, extended_addr=extended_addr)
 

class SwapQueryPacket(SwapPacket):
    """
    SWAP Query packet class
    """
    def __init__(self, rAddr=SwapAddress.BROADCAST_ADDR, rId=0, extended_addr=False):
        """
        Class constructor
        
        @param rAddr: Register address
        @param rId: Register ID
        """
        SwapPacket.__init__(self, destAddr=rAddr, function=SwapFunction.QUERY, regAddr=rAddr, regId=rId, extended_addr=extended_addr)
        
        
class SwapCommandPacket(SwapPacket):
    """
    SWAP Command packet class
    """
    def __init__(self, rAddr, rId, val, nonce=0, extended_addr=False):
        """
        Class constructor
        
        @param rAddr: Register address
        @param rId: Register ID
        @param val: New value
        @param nonce: Security nonce
        """
        SwapPacket.__init__(self, destAddr=rAddr, nonce=nonce, function=SwapFunction.COMMAND, regAddr=rAddr, regId=rId, value=val, extended_addr=extended_addr)
