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
__date__  ="$Nov 06, 2014$"
#########################################################################

import time
import sys
import os
import json
from datetime import datetime

from maxdefs import MaxDefinitions
from api import NetworkAPI as network
sys.path.append(MaxDefinitions.lagarto_dir)
from lagartoresources import LagartoEndpoint, LagartoException
from clouding import ThingSpeakPacket


class BulkCloud:
    """
    Periodic bulk upload of values to cloud services
    """

    def edit_cloud_tx(self, name, service, endpoints, apikey, interval="5min"):
        """
        Edit or create cloud transmission
        
        @param name cloud tx name
        @param service cloud service
        @param endpoints list of endpoints to be uploaded
        @param apikey APY key from cloud account
        @param interval update interval ("1m", "5m", "1d", etc)
        """
        transmissions = self.tx_config["cloud"]
        
        new_entry = {"service" : service, "apikey" : apikey, "interval" : interval, "endpoints" : endpoints}
        
        # Update config information
        transmissions[name] = new_entry

        # Save new config
        self.save_config()

    
    def delete_cloud_tx(self, name):
        """
        Delete cloud transmission
        
        @param name cloud tx name
        """
        # Delete cloud tansmission from config file
        transmissions = self.tx_config["cloud"]
        if name in transmissions:
            del transmissions[name]
            # Save config
            self.save_config()
            

    def read_config(self):
        """
        Read configuration file containing metadata
        
        @return dictionary containing configuration
        """       
        try:
            tx_cfg_file = open(self.config_path)   
            tx_config = json.load(tx_cfg_file)
            tx_cfg_file.close()
            return tx_config
        except IOError as ex:
            return {"cloud": {}}


    def save_config(self):
        """
        Save configuration file containing metadata
        """        
        try:
            tx_cfg_file = open(self.config_path, 'w')
            json.dump(self.tx_config, tx_cfg_file, sort_keys=False, indent=2)
            tx_cfg_file.close()
        except IOError:
            raise LagartoException("Unable to save cloud configuration in " + self.config_path)
            

    def eval_upload(self):
        """
        Evaluate possible transmissions. To be called once per minute
        """
        if self.tx_config is None:
            return
        
        transmissions = self.tx_config["cloud"]
        
        update = False
        current = datetime.now()

        for name, tx in transmissions.iteritems():
            interval = tx["interval"]         
            if interval == "1m":
                update = True
            elif interval == "5m":
                if current.minute % 5 == 0:
                    update = True
            elif interval == "15m":
                if current.minute % 15 == 0:
                    update = True
            elif interval == "30m":
                if current.minute % 30 == 0:
                    update = True           
            elif interval == "1h":
                if current.minute == 0:
                    update = True
            elif interval == "12h":
                if current.hour % 12 == 0:
                    update = True
            elif interval == "1d":
                if current.hour == 0:
                    update = True
            
            if update:
                update = False                 
                # Fill endpoint data
                endpoint_values = []
                for field_id in tx["endpoints"]:
                    endp_name = tx["endpoints"][field_id]
                    endp = network.get_endpoint(endp_name)
                    if endp is None:
                        raise LagartoException("Unable to find a matching endpoint for " + endp_name + " before updating " + name + " from database")
                    elif endp.value is None:
                        raise LagartoException(endp_name + " has an invalid value. Unable to upload " + name + " to " + tx["service"])
                    else:                        
                        endpoint_values.append((field_id, endp.value))
                        update = True
                
                if update:
                    if tx["service"] == "ThingSpeak":
                        thinkspeak = ThingSpeakPacket(tx["apikey"], endpoint_values)        
                        thinkspeak.push()
                
                
    def __init__(self):
        """
        Class constructor
        """       
        ## Configuration file
        self.config_path = os.path.join(MaxDefinitions.working_dir, "config", "cloud.json")
        
        ## Transmission config metadata
        self.tx_config = self.read_config()
