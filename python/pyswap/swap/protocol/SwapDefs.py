#########################################################################
#
# SwapDefs
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

class SwapAddress:
    """
    SWAP reserved addresses
    """
    ## Broadcast address
    BROADCAST_ADDR = 0x00
    
    
class SwapFunction:
    """
    SWAP function codes
    """
    ## SWAP STATUS type
    STATUS = 0x00
    ## SWAP QUERY type
    QUERY = 0x01
    ## SWAP COMMAND type
    COMMAND = 0x02
    

class SwapRegId:
    """
    Standard register ID's
    """
    ID_PRODUCT_CODE = 0
    ID_HW_VERSION = 1
    ID_FW_VERSION = 2
    ID_SYSTEM_STATE = 3
    ID_FREQ_CHANNEL = 4
    ID_SECU_OPTION = 5
    ID_SECU_PASSWORD = 6
    ID_SECU_NONCE = 7
    ID_NETWORK_ID = 8
    ID_DEVICE_ADDR = 9
    ID_TX_INTERVAL = 10
    

class SwapState:
    """
    System states
    """
    RESTART = 0
    RXON = 1
    RXOFF = 2
    SYNC = 3
    LOWBAT = 4

    @staticmethod
    def toString(state):
        """
        Return string defining the system state
        
        @param state: SWAP state to be converted to string
        
        @return State in string format
        """
        if state == SwapState.RESTART:
            return "Device restarting"
        elif state == SwapState.RXON:
            return "RF reception enabled"
        elif state == SwapState.RXOFF:
            return "RF reception disabled"
        elif state == SwapState.SYNC:
            return "Synchronization mode"
        elif state == SwapState.LOWBAT:
            return "Device battery is low"
        else:
            return str(state)

class SwapType:
    """
    Data types
    """
    BINARY = "bin"
    NUMBER = "num"
    STRING = "str"
    INPUT = "inp"
    OUTPUT = "out"
    
    @staticmethod
    def toString(type):
        """
        Return complete name of the type
        
        @param type: Type of parameter
        
        @return Type of parameter in string format
        """
        if type == SwapType.BINARY:
            return "binary"
        elif type == SwapType.NUMBER:
            return "number"
        elif type == SwapType.STRING:
            return "string"
        elif type == SwapType.INPUT:
            return "input"
        elif type == SwapType.OUTPUT:
            return "output"
