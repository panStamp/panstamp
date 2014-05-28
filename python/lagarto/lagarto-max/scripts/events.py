#########################################################################
#
# Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
#
# This file is part of the lagarto project.
#
# lagarto  is free software; you can redistribute it and/or modify
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

from api import TimeAPI as clock, NetworkAPI as network, CloudAPI as cloud
import time


def startup():
    """
    Startup function
    
    Place here your custom initializations
    """
    print "Custom objects being initialized..."
        
    
def event_handler(evnsrc, evnobj):
    """
    Event handling function
    
    @param evnsrc: event source ("network", "clock")
    @param evnobj: event object
    
    evnsrc = "network" -> evnobj = lagarto endpoint
    evnsrc = "clock" -> evnobj = time.localtime() object
    """
    if evnsrc == "network":
        print time.strftime("%d %b %Y %H:%M:%S", time.localtime()), evnobj.location + "." + evnobj.name, evnobj.value
