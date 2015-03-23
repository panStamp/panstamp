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
__appname__= "swapdmt-cmd"
__version__= "0.1.0"
#########################################################################

from swap.SwapException import SwapException
from swap.xmltools.XmlSettings import XmlSettings
from swap.xmltools.XmlSerial import XmlSerial
from swap.xmltools.XmlNetwork import XmlNetwork
from swapmanager import SwapManager

import sys
import os
import signal
import argparse


swap_manager = None


def signal_handler(signal, frame):
    """
    Handle signal received
    """
    swap_manager.stop()
    sys.exit(0)


if __name__ == '__main__':
    
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', help="Serial port connected to modem")
    parser.add_argument('--speed', help="Serial baud rate (bps)")
    parser.add_argument('--channel', help="RF channel")
    parser.add_argument('--netid', help="Network ID")
    parser.add_argument('--address', help="SWAP address of modem")
    parser.add_argument('--security', help="Security flag")
    parser.add_argument('--macro', help="Macro")
    opts = parser.parse_args()

    direc = os.path.join(os.path.dirname(sys.argv[0]), "config")
    settings = os.path.join(direc, "settings.xml")
    
    # General settings
    general_cfg = XmlSettings(settings)
    # Serial settings
    serial_cfg = XmlSerial(general_cfg.serial_file)
    # Network settings
    network_cfg = XmlNetwork(general_cfg.network_file)

    save_file = False
    # Save serial parameters
    if opts.port is not None:
        serial_cfg.port = opts.port
        save_file = True
    if opts.speed is not None:
        serial_cfg.speed = SwapManager.str_to_int(opts.speed)
        save_file = True

    if save_file:
        serial_cfg.save()

    save_file = False
    # Save network parameters
    if opts.channel is not None:
        channel = SwapManager.str_to_int(opts.channel)
        if channel is not None:
            SwapManager.str_to_int = channel
            save_file = True
    if opts.netid is not None:
        netid = SwapManager.str_to_int(opts.netid)
        if netid is not None:
            network_cfg.network_id = netid
            save_file = True
    if opts.address is not None:
        addr = SwapManager.str_to_int(opts.address)
        if addr is not None:
            network_cfg.devaddress = addr
            save_file = True
    if opts.security is not None:
        secu = SwapManager.str_to_int(opts.security)
        if secu is not None:
            network_cfg.security = secu
            save_file = True
        
    if save_file:
        network_cfg.save()

    # Catch possible SIGINT signals
    signal.signal(signal.SIGINT, signal_handler)

    try:        
        # SWAP manager
        swap_manager = SwapManager(settings)
    except SwapException as ex:
        ex.display()
        ex.log()

    while True:
        if swap_manager.server_started == True and swap_manager.prog_address is None:
            
            # Launch Macro
            if opts.macro is not None:        
                swap_manager.dispatch_macro(opts.macro)
                opts.macro = None
                
            cmd = raw_input(">> ")
            
            command = []
            for item in cmd.split(" "):
                command.append(item.strip())
                
            swap_manager.dispatch_user_command(command)

    signal.pause()
