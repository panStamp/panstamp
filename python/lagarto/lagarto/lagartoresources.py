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
# along with panLoader; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Feb 6, 2012$"
#########################################################################

import time
import datetime


class LagartoMessage:
    """
    Lagarto message class
    """
    def dumps(self):
        """
        Serialize message in form of JSON string
        """       
        data = {}
        data["procname"] = self.proc_name
        if self.http_server is not None:
            data["httpserver"] = self.http_server   
        
        if self.status is not None:
            endp_data = []
            for endp in self.status:
                endp_data.append(endp)
                
            data["status"] = endp_data
            
        return {"lagarto" : data}

        
    def __init__(self, msg=None, proc_name=None, http_server=None, status=None):
        """
        Constructor
        
        @param msg: serialized message string
        @param proc_name: process name
        @param http_server: HTTP server address
        @param status: list of endpoint data
        """
        ## Process name
        self.proc_name = proc_name
        ## HTTP server address
        self.http_server = http_server
        ## List of endpoint data
        self.status = status
        
        if msg is not None:
            if "lagarto" not in msg:
                raise LagartoException("Incorrect packet header")
            
            data = msg["lagarto"]
            
            if "procname" not in data:
                raise LagartoException("Status message must contain a proces name")
            
            self.proc_name = data["procname"]
            
            if "httpserver" in data:
                self.http_server = data["httpserver"]
            
            if "status" in data:
                self.status = data["status"]

        
class LagartoEndpoint:
    """
    Lagarto endpoint class
    """
    # Lagarto client (if any)
    lagarto_client = None
    
    @staticmethod
    def strtovalue(strval, vtype):
        """
        Return value in native format given its string representation and type
        
        @param strval: value in string format
        @param vtype: type of value ("str", "num", "bin")
        
        @return value in native format
        """
        res = strval
        
        if vtype == "num":
            try:
                res = int(strval)
            except ValueError:
                try:
                    res = float(strval)
                except ValueError:
                    raise LagartoException(strval + " is not a valid numeric value")
        elif vtype == "bin":
            if strval.lower() in ["on", "open", "1", "true", "enabled"]:
                res = True
            else:
                res = False
                
        return res

        
    def get_value(self):
        """
        Request endpoint value from remote process
        
        @param value: new endpoint value
        
        @return actual endpoint value, returned by the associated lagarto server
        """
        status = LagartoEndpoint.lagarto_client.request_status(self.procname, [self.dumps()])
        if status is not None:
            if len(status) > 0:
                if "value" in status[0]:
                    self.value = status[0]["value"]
                    if "unit" in status[0]:
                        self.unit = status[0]["unit"]
                    if self.name != status[0]["name"]:
                        self.name = status[0]["name"]
                    if self.location != status[0]["location"]:
                        self.location = status[0]["location"]
                    return LagartoEndpoint.strtovalue(status[0]["value"], status[0]["type"])
        return None


    def set_value(self, value):
        """
        Set endpoint value on remote process
        
        @param value: new endpoint value
        
        @return actual endpoint value, returned by the associated lagarto server
        """
        res = None
        status = LagartoEndpoint.lagarto_client.request_status(self.procname, [self.dumps()])
        if status is not None:
            if len(status) > 0:
                if "value" in status[0]:
                    res = LagartoEndpoint.strtovalue(status[0]["value"], status[0]["type"])
        
        if res is not None:
            self.value = res
            
        return res
        

    def dumps(self, enable_value=True):
        """
        Serialize address in form of JSON string
        
        @param enable_value add value information
        """
        endpoint = {}
        endpoint["id"] = self.id
        endpoint["name"] = self.name
        endpoint["location"] = self.location
        
        if self.direction is not None:
            endpoint["direction"] = self.direction
        if self.type is not None:
            endpoint["type"] = self.type
        if enable_value and self.value is not None:
            endpoint["value"] = str(self.value)
            if self.unit is not None:
                endpoint["unit"] = self.unit
                
        return endpoint


    def __init__(self, endpstr=None, endp_id=None, location=None, name=None, vtype=None, direction=None, value=None, unit=None, procname=None):
        """
        Constructor
        
        @param enspstr: endpoint in string format
        @param endp_id: endpoint unique id
        @param location: endpoint location
        @param name: endpoint name
        @param type: type of endpoint ("num", "bin", "str")
        @param direction: endpoint direction
        @param value: endpoint value
        @param unit: optional unit
        @param procname: process name
        """
        
        ## Endpoint id
        self.id = endp_id
        ## Endpoint name
        self.name = name
        ## Endpoint location
        self.location = location
        ## Type of value
        self.type = vtype
        ## Direction (input or output)
        self.direction = direction
        ## Endpoint value
        if value is not None:
            if type(value) in ["str", "unicode"] and vtype is not None:
                self.value = LagartoEndpoint.strtovalue(value, vtype)
            else:
                self.value = value
        else:
            self.value = "?"

        ## Unit
        self.unit = unit
        ## Process name
        self.procname = procname
        
        if endpstr is not None:
            if "id" not in endpstr:
                raise LagartoException("Lacking id information in endpoint")
            if "location" not in endpstr:
                raise LagartoException("Lacking location information in endpoint")
            if "name" not in endpstr:
                raise LagartoException("Lacking name information in endpoint")
            
            self.id = endpstr["id"]
            self.location = endpstr["location"]
            self.name = endpstr["name"]
                           
            if "unit" in endpstr:
                self.unit = endpstr["unit"]

            if "type" in endpstr:
                self.type = endpstr["type"]
                
            if "direction" in endpstr:
                self.direction = endpstr["direction"]

            if "value" in endpstr:
                if self.type is not None:
                    self.value = LagartoEndpoint.strtovalue(endpstr["value"], self.type)
                else:
                    self.value = endpstr["value"]
                
                
class LagartoException(Exception):
    """
    Main exception class for lagarto comms
    """
    error_file = "lagarto.err"
    
    def display(self):
        """
        Print exception description
        """
        print datetime.datetime.fromtimestamp(self.timestamp).strftime("%d-%m-%Y %H:%M:%S"), self.description
              
                  
    def __str__(self):
        """
        String representation of the exception
        """
        return repr(self.description)


    def log(self):
        """
        Write exception in log file
        """
        f = open(LagartoException.error_file, 'a')
        f.write(datetime.datetime.fromtimestamp(self.timestamp).strftime("%d-%m-%Y %H:%M:%S") + ": " + self.description + "\n")
        f.close()


    def __init__(self, value):
        """
        Class constructor
        
        @param value: Description about the error
        """
        self.timestamp = time.time()
        # Exception description
        self.description = "LagartoException occurred: " + value
