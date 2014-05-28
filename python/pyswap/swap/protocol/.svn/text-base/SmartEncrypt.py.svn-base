#########################################################################
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
__date__ ="$Jun 6, 2012$"
#########################################################################

class Password(object):
    """
    Encryption password
    """
    
    def to_string(self):
        """
        Convert password (list of bytes) to string
        
        @return password in string format
        """
        # Convert list of bytes to list of strings
        strlist = []
        for item in self.data:
            strlist.append("{0:02X}".format(int(item)))
        # Convert list of strings to string
        strpwd = "".join(item for item in strlist)
        # Return string
        return strpwd

    
    def __init__(self, password):
        """
        Class constructor
        
        @param password: password formated as a list or string
        """
        ## Password bytes
        self.data = []

        if type(password) is list:
            self.data[:] = password[:]
        elif type(password) in [str, unicode]:
            for i in range(0, len(password), 2):
                byte = int(password[i:i + 2], 16)
                self.data.append(byte)
