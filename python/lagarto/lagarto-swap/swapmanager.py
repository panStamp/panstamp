#########################################################################
#
# SwapManager
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
# along with panLoader; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Jan 23, 2012$"
#########################################################################

import os
import sys

from swap.SwapInterface import SwapInterface
from swap.protocol.SwapDefs import SwapState
from swap.xmltools.XmlSettings import XmlSettings
from swap.xmltools.XmlSerial import XmlSerial
from swap.xmltools.XmlNetwork import XmlNetwork

working_dir = os.path.dirname(__file__)
panstamp_dir = os.path.split(working_dir)[0]
lagarto_dir = os.path.join(panstamp_dir, "lagarto")
sys.path.append(lagarto_dir) 
from lagartocomms import LagartoServer


class SwapManager(SwapInterface, LagartoServer):
    """
    SWAP Management Class
    """
    def newMoteDetected(self, mote):
        """
        New mote detected by SWAP server
        
        @param mote: Mote detected
        """
        if self._print_swap == True:
            print "New mote with address " + str(mote.address) + " : " + mote.definition.product + \
            " (by " + mote.definition.manufacturer + ")"


    def newEndpointDetected(self, endpoint):
        """
        New endpoint detected by SWAP server
        
        @param endpoint: Endpoint detected
        """
        if self._print_swap == True:
            print "New endpoint with Reg ID = " + str(endpoint.getRegId()) + " : " + endpoint.name


    def moteStateChanged(self, mote):
        """
        Mote state changed
        
        @param mote: Mote having changed
        """
        if self._print_swap == True:
            print "Mote with address " + str(mote.address) + " switched to \"" + \
            SwapState.toString(mote.state) + "\""     


    def moteAddressChanged(self, mote):
        """
        Mote address changed
        
        @param mote: Mote having changed
        """
        if self._print_swap == True:
            print "Mote changed address to " + str(mote.address)


    def registerValueChanged(self, register):
        """
        Register value changed
        
        @param register: register object having changed
        """
        # Skip config registers
        if register.isConfig():
            return
        
        if self._print_swap == True:
            print  "Register addr= " + str(register.getAddress()) + " id=" + str(register.id) + " changed to " + register.value.toAsciiHex()
        
        status = []
        # For every endpoint contained in this register
        for endp in register.parameters:
            strval = endp.getValueInAscii()
            if endp.valueChanged:
                if self._print_swap:
                    if endp.unit is not None:
                        strval += " " + endp.unit.name
                    print endp.name + " in address " + str(endp.getRegAddress()) + " changed to " + strval
                               
                if endp.display:
                    endp_data = endp.dumps()
                    if endp_data is not None:
                        status.append(endp_data)
        
        if len(status) > 0:        
            self.publish_status(status)
                      
        
    def get_status(self, endpoints):
        """
        Return network status as a list of endpoints in JSON format
        Method required by LagartoServer
        
        @param endpoints: list of endpoints being queried
        
        @return list of endpoints in JSON format
        """
        status = []
        if endpoints is None:
            for mote in self.network.motes:
                for reg in mote.regular_registers:
                    for endp in reg.parameters:
                        status.append(endp.dumps())
        else:
            for item in endpoints:
                endp = self.get_endpoint(item["id"], item["location"], item["name"])
                if endp is not None:
                    status.append(endp.dumps()) 
        
        return status


    def set_status(self, endpoints):
        """
        Set endpoint status
        Method required by LagartoServer
        
        @param endpoints: list of endpoints in JSON format
        
        @return list of endpoints being controlled, with new values
        """
        status = []
        for item in endpoints:
            endp = self.get_endpoint(item["id"], item["location"], item["name"])
            if endp is not None:
                if "value" in item:
                    if endp.direction == "out":
                        new_value = item["value"]
                        if endp.type == "bin" and item["value"].lower() == "toggle":
                            if endp.getValueInAscii() == "on":
                                new_value = "off"
                            else:
                                new_value = "on"
                        endp.cmdWack(new_value)
                        # Build new JSON structure
                        status.append(endp.dumps())

        return status
    
    
    def http_command_received(self, command, params):
        """
        Process command sent from HTTP server. Method to be overrided by data server.
        Method required by LagartoServer
        
        @param command: command string
        @param params: dictionary of parameters
        
        @return True if command successfully processed by server.
        Return False otherwise
        """
        try:
            # Configure endpoint
            if command == "config_endpoint":
                endp = self.get_endpoint(endpid=params["id"])
                endp.name = params["name"]
                endp.location = params["location"]
                if "unit" in params:
                    endp.setUnit(params["unit"])
                self.network.save()
            elif command == "delete_mote":
                self.network.delete_mote(int(params["address"]))
            else:
                # Save gateway's wireless settings
                if command == "modem_network":
                    main_settings = XmlSettings(self.swap_settings)
                    # Open network configuration
                    config = XmlNetwork(main_settings.network_file)
                    # Change parameters
                    config.freq_channel = int(params["channel"])
                    config.network_id = int(params["netid"], 16)
                    config.devaddress = int(params["address"])
                    config.security = int(params["security"])
                    config.password = params["password"]                   
                # Save gateway's port settings
                elif command == "modem_serial":
                    main_settings = XmlSettings(self.swap_settings)
                    # Open network configuration
                    config = XmlSerial(main_settings.serial_file)
                    # Change parameters
                    config.port = params["port"]
                    config.speed = int(params["speed"])   
                # Configure general settings
                elif command == "general_settings":
                    config = XmlSettings(self.swap_settings)
                    config.debug = int(params["debug"])
                    config.device_localdir = params["local"]
                    config.device_remote = params["remote"]
                    if "update" in params:
                        config.updatedef = params["update"] == "true"
                    config.serial_file = params["serial"]
                    config.network_file = params["network"]
                    config.swap_file = params["swapnet"]
                    
                # Save config file
                config.save()
                # Save current network information
                self.network.save()
                # Restart server
                self.server.stop()
                self.server.start()
                
        except:
            return False
        
        return True
  
    
    def stop(self):
        """
        Stop SWAP manager
        """
        # Stop SWAP server
        self.server.stop()
        # Stop Lagarto server
        LagartoServer.stop(self)

    
    def __init__(self, swap_settings=None):
        """
        Class constructor
        
        @param swap_settings: path to the main SWAP configuration file
        @param verbose: Print out SWAP frames or not
        @param monitor: Print out network events or not
        """
        # MAin configuration file
        self.swap_settings = swap_settings
        # Print SWAP activity
        self._print_swap = False
        
        try:
            # Superclass call
            SwapInterface.__init__(self, swap_settings)
        except:
            raise

        # Lagarto server constructor
        LagartoServer.__init__(self, working_dir)

        if XmlSettings.debug == 2:
            self._print_swap = True
