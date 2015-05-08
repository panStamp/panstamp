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

working_dir = os.path.dirname(__file__)
lagarto_dir = os.path.split(working_dir)[0]
config_dir = os.path.join(working_dir, "config")
lagarto_lagarto_dir = os.path.join(lagarto_dir, "lagarto")
config_dir = os.path.join(working_dir, "config")
sys.path.append(lagarto_lagarto_dir) 

from swap.SwapInterface import SwapInterface
from swap.protocol.SwapDefs import SwapRegId, SwapState
from swap.protocol.SwapValue import SwapValue
from swap.xmltools.XmlSettings import XmlSettings
from swap.xmltools.XmlSerial import XmlSerial
from swap.xmltools.XmlNetwork import XmlNetwork

from lagartocomms import LagartoServer
from lagartoresources import LagartoException
from lagartoconfig import XmlLagarto


class SwapManager(SwapInterface, LagartoServer):
    """
    SWAP Management Class
    """
    def swapServerError(self, ex):
        """
        SWAP server error detected
        
        @param ex exception
        """
        ex.display()
        ex.log()
        
    
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
            if self.lagarto_config.publish == "always" or (self.lagarto_config.publish == "event" and endp.valueChanged):
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
    
    
    def config_mote(self, address, new_address, txinterval):
        """
        Configure device
        
        @param address current device address
        @param new_address new address to be configured for the mote
        @param txinterval Tx interval
        """
        try:
            # Get mote object
            mote = self.server.network.get_mote(address=int(address))
            if mote is not None:
                # Save commands
                if txinterval != "":                
                    res = mote.save_txinterval_command(int(txinterval))
                
                if new_address != "":
                    newaddr = int(new_address)
                    if mote.address != newaddr:
                        res = mote.save_address_command(newaddr)
                
                return res
        except:
            raise LagartoException("Unable to config device with address = " + str(address))
                    

    def delete_mote(self, address):
        """
        Delete device
        
        @param address current device address
        """
        try:
            self.network.delete_mote(int(address))
            self.network.save()
        except:
            raise LagartoException("Unable to delete device with address = " + str(address))
        
        
    def config_endpoint(self, id, name, location, unit=None):
        """
        Configure endpoint
        
        @param id unique id
        @param name name of endpoint
        @param location location of endpoint
        @param unit units shown for the endpoint value
        """
        try:
            endp = self.get_endpoint(endpid=id)
            endp.name = name
            endp.location = location
            if unit is not None:
                endp.setUnit(unit)
            self.network.save()
        except:
            raise LagartoException("Unable to save endpoint settings")


    def config_modem(self, port, speed):
        """
        Configure serial modem
        
        @param port serial port
        @param serial baudrate
        """
        try:
            # Open network configuration
            config = XmlSerial(self.main_settings.serial_file)
            # Change parameters
            config.port = port
            config.speed = int(speed)
            # Save config file
            config.save()
        except:
            raise LagartoException("Unable to save modem serial settings")
        

    def config_network(self, channel, netid, address, security, password):
        """
        Configure network parameters from the serial modem
        
        @param channel RF channel
        @param netid SWAP network ID
        @param address network address
        @param security wireless security flag
        @param password password for wireless security
        """
        try:
            # Open network configuration
            config = XmlNetwork(self.main_settings.network_file)
            # Change parameters
            config.freq_channel = int(channel)
            config.network_id = int(netid, 16)
            config.devaddress = int(address)
            config.security = int(security)
            config.password = password                   
            config.save()
        except:
            raise LagartoException("Unable to save modem network settings")
        

    def general_settings(self, debug, local, remote, update, serial, network, swapnet):
        """
        Configure serial modem
        
        @param debug debug level
        @param local local device definition folder
        @param remote remote device definition folder
        @param update update local device definition folder on start-up
        @param serial serial gateway config file
        @param network wireless config file
        @param swapnet SWAP network file
        """
        try:            
            config = XmlSettings(self.swap_settings)
            config.debug = int(debug)
            config.device_localdir = local
            config.device_remote = remote
            if update is not None:
                config.updatedef = update == "true"
            config.serial_file = serial
            config.network_file = network
            config.swap_file = swapnet                    
            # Save config file
            config.save()
        except:
            raise LagartoException("Unable to save general settings")
              
    
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
                self.config_endpoint(params["id"],
                                     params["name"],
                                     params["location"],
                                     params["unit"])
                return "endpoint_panel.html"
            elif command == "delete_mote":
                self.delete_mote(params["address"])
                return "device_panel.html"              
            elif command == "config_mote":                
                res = self.config_mote(params["address"],
                                       params["newaddr"],
                                       params["txinterval"])
                if res is None:
                    return "command_saved.html"
                elif res:
                    return "command_received.html"
                else:
                    return "command_not_received.html"
            else:
                # Save gateway's wireless settings
                if command == "modem_network":
                    self.config_network(params["channel"],
                                        params["netid"],
                                        params["address"],
                                        params["security"],
                                        params["password"])                  
                # Save gateway's port settings
                elif command == "modem_serial":
                    self.config_modem(params["port"],
                                      params["speed"])  
                # Configure general settings
                elif command == "general_settings":
                    self.general_settings(params["debug"],
                                          params["local"],
                                          params["remote"],
                                          params["update"],
                                          params["serial"],
                                          params["network"],
                                          params["swapnet"])
                    
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
       
        # Main configuration file
        self.swap_settings = swap_settings
        # Print SWAP activity
        self._print_swap = False
        
        try:
            self.main_settings = XmlSettings(self.swap_settings)
            # Set log file to trace lagarto exceptions
            LagartoException.error_file = XmlSettings.error_file
            # Superclass call
            SwapInterface.__init__(self, swap_settings)
        except:
            raise

        # Lagarto server constructor
        LagartoServer.__init__(self, working_dir)

        if XmlSettings.debug == 2:
            self._print_swap = True
            
        self.lagarto_config = XmlLagarto(os.path.join(config_dir, "lagarto.xml"))
        
