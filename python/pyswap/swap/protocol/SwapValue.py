#########################################################################
#
# SwapValue
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


class SwapValue(object):
    """
    Multi-format SWAP value class
    """
  
    def getLength(self):
        """
        Get data length
        
        @return Length in bytes of the current value
        """
        return len(self._data)
    
    def toInteger(self):
        """
        Convert SWAP value into number
        
        @return Current value in integer format
        """
        val = 0
        for i, item in enumerate(self._data):
            val |= item << (len(self._data)-1-i)*8
        return val


    def clone(self):
        """
        Get a copy of the current value
        
        @return Copy of the current value
        """
        lstData = self._data[:]
        return SwapValue(lstData)


    def toAscii(self):
        """
        Convert SWAP value into ASCII string. Use this function for sequences of integer numbers
        
        @return Current value in ASCII format
        """
        return "".join(str(item) for item in self._data)
    

    def toAsciiStr(self):
        """
        Convert SWAP value into readable ASCII string. Use this function for real ASCII strings
        
        @return 
        """
        return "".join(chr(item) for item in self._data)

    
    def toAsciiHex(self):
        """
        Convert SWAP value into printable ASCII hex string. Use this function for sequences of
        integer numbers
        """
        out = []
        for item in self._data:
            out.append("{0:02X}".format(item))
        # Return ASCII string
        return "".join(out)

       
    def toList(self):
        """
        Convert SWAP value into list
        
        @return Current value as a list of bytes
        """
        return self._data


    def isEqual(self, value):
        """
        Compare current value with the one passed as argument
        
        @param value: Value to be compared agains the current one
        
        @return True if the value passed as argument is equal to the current one. Return False
        otherwise
        """
        if value is not None:       
            if self.getLength() == value.getLength():
                if self._data[:] == value.toList()[:]:
                    return True
        return False


    def str_to_int(self, str_int):
        """
        Convert string to integer
        
        @param str Integer in string format
        """
        val = 0 
        try:
            val = int(str_int)
        except ValueError:
            if str_int[:2] == "0x":
                try:
                    val = int(str_int[2:], 16)
                except ValueError:
                    raise
            else:
                raise
                    
        return val
    
    
    def __init__(self, value=None, length=0):
        """
        Class constructor

        @param value: Raw value in form of list, boolean, integer, long, string or unicode
        @param length: byte length of the value
        """
        ## Raw value in form of list
        self._data = []
        isAsciiString = False
        if value is not None:
            # In case of list passed in the constructor
            if type(value) is list:
                self._data = value
            # Boolean
            elif type(value) is bool:
                res = int(value)
            # Float
            elif type(value) is float:
                res = int(value*10)
            # In case a string is passed in the constructor
            elif type(value) in [str, unicode]:
                try:
                    # Remove decimal point
                    value = value.replace(".", "")
                    # Convert to integer
                    res = self.str_to_int(value)
                except ValueError:
                    isAsciiString = True
            else:
                res = value

            if isAsciiString:
                # OK, treat value as a pure ASCII string
                strlen = len(value)
                if length == 0:
                    length = strlen
                # Truncate string
                if strlen > length:                    
                    value = value[:length]
                # Copy string
                for ch in value:
                    self._data.append(ord(ch))
                # Trailing zeros
                if length > strlen:
                    for i in range(length - strlen):
                        self._data.append(0)
            # In case of integer or long
            elif 0 < length <= 4:
                for i in range(length):
                    val = (res >> (8 * (length-1-i))) & 0xFF
                    self._data.append(val)

