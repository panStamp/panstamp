#########################################################################
#
# SwapNetwork
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
__date__ ="$Jan 17, 2012 22:56:00 PM$"
#########################################################################

from SwapMote import SwapMote
from swap.SwapException import SwapException

import json


class SwapNetwork:
    """
    Container of SWAP network data
    """
    def read(self):
        """
        Read initial network data from file
        """
        # Clear current list of motes:
        self.clear()
        
        try:
            network_file = open(self.filename)   
            network_data = json.load(network_file)["network"]
            network_file.close()
            # Initialize list of motes
            for mote_data in network_data["motes"]:
                mote = SwapMote(self.server, mote_data["pcode"], mote_data["address"])
                self.motes.append(mote)
                # Initialize endpoints belonging to this mote
                for register in mote.regular_registers:
                    # Find register config
                    for register_data in mote_data["registers"]:
                        if register_data["id"] == register.id:
                            for endpoint in register.parameters:
                                # Find endpoint config                            
                                for endpoint_data in register_data["endpoints"]:
                                    if endpoint_data["id"] == endpoint.id:                      
                                        endpoint.name = endpoint_data["name"]
                                        endpoint.location = endpoint_data["location"]
                                        if "unit" in endpoint_data and endpoint.unit is not None:
                                            endpoint.setUnit(endpoint_data["unit"])
                                        if "value" in endpoint_data:
                                            endpoint.setValue(endpoint_data["value"])

                                        endpoint.direction = endpoint_data["direction"]
                                        endpoint.type = endpoint_data["type"]
                                        
                                        endpoint.display = True
                                        if "display" in endpoint_data:
                                            if endpoint_data["display"].lower() in ["false", "no", 0, "0", "disabled"]:
                                                endpoint.display = False
                                        break
                            break
                
        except IOError as ex:
            pass
        except SwapException as ex:
            ex.display()


    def save(self):
        """
        Save current network data into file
        """
        try:
            network = self.dumps()
            print "Saving", self.filename
            network_file = open(self.filename, 'w')     
            # Write network data into file
            json.dump(network, network_file, sort_keys=False, indent=2)
            network_file.close()
        except SwapException:
            raise
        except IOError:
            raise SwapException("Unable to save SWAP network data in file")
  
    
    def add_mote(self, mote):
        """
        Add mote to the network
        
        @param mote: SWAP mote to be added
        
        @return true if the mote did not exist in the list. False otherwise
        """        
        # Search mote in list
        for m in self.motes:
            if m.address == mote.address:
                return False

        self.motes.append(mote)
        return True


    def delete_mote(self, address):
        """
        Delete mote from network
        
        @param address: address of the mote to be removed
        """        
        # Search mote in list
        for m in self.motes:
            if m.address == address:
                self.motes.remove(m)
                self.save()
                return


    def get_mote(self, index=None, address=None):
        """
        Return mote from list given its index or address

        @param index: Index of hte mote within lstMotes
        @param address: Address of the mote
        
        @return mote
        """
        if index is not None and index >= 0:
            return self.motes[index]
        elif (address is not None) and (address > 0) and (address <= 255):
            for mote in self.motes:
                if mote.address == address:
                    return mote
        return None
    
    
    def get_nbof_motes(self):
        """
        Return number of motes available in the network
        
        @return amount of motes
        """
        return len(self.motes)
    
    
    def get_endpoint(self, usrlocation=None, usrname=None):
        """
        Get endpoint given its user name and location
        
        @param usrlocation: user location
        @param usrname: user name

        @return endpoint object
        """
        if usrlocation is None or usrname is None:
            return None

        for mote in self.motes:
            for reg in mote.regular_registers:
                for endp in reg.parameters:
                    if endp.usrlocation == usrlocation and endp.usrname == usrname:
                        return endp
        return None


    def clear(self):
        """
        Clear list of motes
        """
        self.motes = []
        
    
    def dumps(self):
        """
        Serialize network data to a JSON formatted string
        """
        net_data = {}
        net_data["name"] = "SWAP"
        
        motes_data = []
        
        try:
            for mote in self.motes:
                motes_data.append(mote.dumps(include_units=True))
        except SwapException:
            raise
        
        net_data["motes"] = motes_data

        data = {"network" : net_data}     
        return data
    
    
    def __init__(self, server, filename="swapnet.json"):
        """
        Class constructor
        
        @param filename: Name fo the SWAP network file
        """
        ## SWAP server
        self.server = server
        
        ## File name
        self.filename = filename
                   
        ## List of mote objects
        self.motes = []
        
        # Read config file
        try:
            self.read()
        except:
            pass
