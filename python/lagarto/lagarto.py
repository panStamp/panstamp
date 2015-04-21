#########################################################################
#
# lagarto-max
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
# lagarto is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with lagarto; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="Jan 28, 2015"
__appname__= "lagarto-max"
__version__ = "2.4"
#########################################################################

import os
import sys
import signal
import time
import subprocess

lagarto_max_process = None
lagarto_swap_process = None

def signal_handler(signal, frame):
    """
    Handle signal received
    """
    print "Terminating lagarto processes"
    if lagarto_max_process is not None:
      lagarto_swap_process.terminate()
    if lagarto_swap_process is not None:
      lagarto_max_process.terminate()
    sys.exit(0)
    
    
if __name__ == '__main__':
  
    # Catch possible SIGINT signals
    signal.signal(signal.SIGINT, signal_handler)

    current_dir = os.path.dirname(sys.argv[0])
    
    if current_dir == "":
        lagarto_max = "lagarto-max" + os.sep + "lagarto-max.py"
        lagarto_swap = "lagarto-swap" + os.sep + "lagarto-swap.py"
    else:
        lagarto_max = current_dir + os.sep + "lagarto-max" + os.sep + "lagarto-max.py"
        lagarto_swap = current_dir + os.sep + "lagarto-swap" + os.sep + "lagarto-swap.py"
    
    print "lagarto version " + __version__ + " (" + __date__ + ")"
    
    try:
        print "Running lagarto-max"
        lagarto_max_process = subprocess.Popen(["python", lagarto_max])
        
        time.sleep(6)
        
        print "Running lagarto-swap"
        lagarto_swap_process = subprocess.Popen(["python", lagarto_swap])
    except Exception as ex:
        print ex
        
    signal.pause()

