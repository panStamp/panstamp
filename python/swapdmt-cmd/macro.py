#!/usr/bin/env python
#########################################################################
#
# Copyright (c) 2014 Daniel Berenguer <dberenguer@usapiens.com>
#
# This file is part of the lagarto project.
#
# lagarto  is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# lagarto is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with panStamp and swapdmt; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Oct 8, 2014$"
#########################################################################

class Macro:
    """
    Custom macros running multiple swapdmt commands, one after the other
    """
    
    def __init__(self, path):
        """
        Class constructor
        
        @param Path to the custom macro file
        """
        self.commands = []
        
        try:
            # Read file
            f = open(path, "r")
            for line in f:
                # Remove leading and trailing spaces
                line = line.strip();
                if len(line) > 0:
                    if line[0] not in ['\n', '#']:
                        cmd = []
                        for item in line.split(" "):
                            cmd.append(item.strip())
                        self.commands.append(cmd)
        except IOError:
            print "Error: can\'t find file " + path
