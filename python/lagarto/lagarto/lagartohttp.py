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
__date__  ="$Jan 26, 2014$"
#########################################################################

from lagartoresources import LagartoEndpoint, LagartoMessage
import lagartoauth as auth
from lagartoauth import LagartoAuth

import cherrypy
from cherrypy.lib import auth_basic

import threading
import json
import sys
import os


class LagartoHttpServer(threading.Thread):
    """
    Lagarto HTTP/GET server
    """
    ## Lagarto directory
    lagarto_dir = os.path.dirname(__file__)
    
    ## Working directory
    working_dir = ""
    

    @cherrypy.expose
    def default(self,*args,**kwargs):
        """
        Catch default HTTP request
        """        
        if args[0] == "command":
            if len(args) == 2:
                command = args[1]
                params = kwargs 
                res = self.data_server.http_command_received(command, params)

                if res == True:
                    raise cherrypy.HTTPRedirect("../lagarto/command_ok.html")
                elif res == False:
                    raise cherrypy.HTTPRedirect("../lagarto/command_nok.html")
                elif type(res) in [str, unicode]:
                    raise cherrypy.HTTPRedirect("../" + res)
                else:
                    cherrypy.response.headers['Content-Type'] = "application/json"
                    return json.dumps(res)
        return None


    @cherrypy.expose
    def values(self, id=None, location=None, name=None, value=None, format=None):
        """
        HTTP call to /values
        """        
        if format is not None:
            # Simple representation?
            if format == "simple":
                if value is None:
                    return _http_simple_get_value(self, id, location, name)
                else:
                    return _http_simple_set_value(self, id, location, name, value)
        
        # Otherwise keep JSON format
        cherrypy.response.headers['Content-Type'] = "application/json"               
        if value is None:                    
            return self._http_json_get_value(id, location, name)
        else:
            return self._http_json_set_value(id, location, name, value)


    @cherrypy.expose
    def config_server(self, procname, broadcast, httpport):
        """
        Configure endpoint
        
        @param procname process name
        @param broadcast ZeroMQ briadcast and port
        @param httpport HTTP port
        """
        if procname is not None:
            self.data_server.config.procname = procname
        if broadcast is not None:
            self.data_server.config.broadcast = broadcast
        if httpport is not None:
            self.data_server.config.httpport = httpport
        self.data_server.config.save()
        
        raise cherrypy.HTTPRedirect("lagarto/command_ok.html")
    

    @cherrypy.expose
    def edit_account(self, user, olduser=None, password=None):
        """
        Edit user account for web authentication
        
        @param user new user name
        @param olduser old user name
        @param password user password
        """
        if olduser == "":
            olduser = None
        
        if auth.LagartoAuth.set_account(username=user, password=password, oldusername=olduser):            
            raise cherrypy.HTTPRedirect("lagarto/account_panel.html")
        else:
            raise cherrypy.HTTPRedirect("lagarto/command_nok.html")


    @cherrypy.expose
    def delete_account(self, user):
        """
        Edit delete account for web authentication
        
        @param user user name
        @param olduser old user name
        @param password user password
        """
        if auth.LagartoAuth.delete_account(user):
            raise cherrypy.HTTPRedirect("lagarto/command_ok.html")
        else:
            raise cherrypy.HTTPRedirect("lagarto/command_nok.html")
        

    @cherrypy.expose
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
        self.data_server.general_settings(debug, local, remote, update, serial, network, swapnet)
        raise cherrypy.HTTPRedirect("lagarto/command_ok.html")    
    

    @cherrypy.expose
    def get_accounts(self):
        """
        Return list of user accounts

        @return list of user accounts
        """
        accounts = auth.LagartoAuth.get_user_names()
        res = json.dumps({"accounts": accounts})          
        cherrypy.response.headers['Content-Type'] = "application/json"
        return res 
           
        
    @cherrypy.expose
    def core(self, command=None, params=None):
        """
        HTTP call to /values
        """
        if command is None:
            return None

        if command == "restart_process":
            python = sys.executable
            os.execl(python, python, * sys.argv)
            return None


    def _http_simple_get_value(self, uid, location, name):
        """
        Get endpoint value from data source
        
        @param uid: endpoint unique id
        @param location: endpoint location
        @param name: endpoint name
        
        @return endpoint value in raw string format
        """
        endp = LagartoEndpoint(endp_id=uid, location=location, name=name)

        status = LagartoHttpServer.data_server.get_status([endp.dumps()])

        if status is not None:
            if len(status) == 1:
                if "value" in status[0]:
                    res = str(status[0]["value"])
                    if "unit" in status[0]:
                        res += " " + status[0]["unit"]
                    return res
                
        return None


    @staticmethod
    def _http_simple_set_value(self, uid, location, name, value):
        """
        Set endpoint value on data source
        
        @param uid: endpoint unique id
        @param location: endpoint location
        @param name: endpoint name
        @param value: new endpoint value
        
        @return endpoint value in raw string format
        """
        endp = LagartoEndpoint(endp_id=uid, location=location, name=name, value=value)
        status = LagartoHttpServer.data_server.set_status([endp.dumps()])
        
        if status is not None:
            if len(status) == 1:
                if "value" in status[0]:   
                    res = str(status[0]["value"])
                    if "unit" in status[0]:
                        res += " " + status[0]["unit"]
                    return res
                
        return None
    

    def _http_json_get_value(self, uid=None, location=None, name=None):
        """
        Get single or multiple endpoint values from data source
        
        @param uid: endpoint uid
        @param location: endpoint locations
        @param name: endpoint name
        
        @return list of endpoint values in JSON format
        """
        # Create list of JSON endpoint data
        req = []
        if uid is not None:
            endp = LagartoEndpoint(endp_id=uid)
            req.append(endp.dumps())
        elif location is not None and name is not None:
            endp = LagartoEndpoint(location=location, name=name)
            req.append(endp.dumps())
        else:  # Return whole network status
            req = None
                    
        status = self.data_server.get_status(req)
        
        if status is not None:
            if len(status) > 0:
                # Create complete Lagarto status message
                http_server = self.address + ":" + str(self.port)
                response = LagartoMessage(proc_name=self.proc_name, http_server=http_server, status=status)
                return json.dumps(response.dumps())
                
        return None
    

    def _http_json_set_value(self, uid, location, name, value):
        """
        Set single or multiple endpoint values from data source
        
        @param uids: endpoint id
        @param locations: endpoint location
        @param names: endpoint name
        @param values: endpoint value
        
        @return list of endpoint values in JSON format
        """
        # Create list of JSON endpoint data
        req = []
        if value is not None:
            if uid is not None:
                endp = LagartoEndpoint(endp_id=uid, value=value)
            elif location is not None and name is not None:
                endp = LagartoEndpoint(location = location, name=name, value=value)

            req.append(endp.dumps())
        else:
            return None
                    
        status = self.data_server.set_status(req)
        
        if status is not None:
            if len(status) > 0:
                # Create complete Lagarto status message
                http_server = self.address + ":" + str(self.port)
                response = LagartoMessage(proc_name=self.proc_name, http_server=http_server, status=status)
                return json.dumps(response.dumps())
                
        return None


    def stop(self):
        """
        Stop HTTP server
        """        
        cherrypy.engine.exit()    


    def run(self):
        """
        Start web server
        """       
        conffile = os.path.join(LagartoHttpServer.working_dir, "config", "webserver.conf")
        cherrypy.config.update(conffile)
        
        ## Lagarto directory
        lagarto_dir = os.path.dirname(__file__)
        lagarto_web_dir = os.path.join(lagarto_dir, "www")
        
        ## For basic auth
        basic_auth = LagartoAuth()
        
        confdict = {"global" : {
                            "server.socket_port": self.port},
                    "/" : {
                           "tools.staticdir.root" : LagartoHttpServer.working_dir},
                    "/lagarto" : {
                            "tools.staticdir.dir" : lagarto_web_dir},
                    }
        
        if LagartoAuth.is_security_enabled():
            confdict["/lagarto"] = {
                            "tools.staticdir.dir" : lagarto_web_dir,
                            "tools.auth_basic.on": True,
                            'tools.auth_basic.realm': 'localhost',
                            'tools.auth_basic.checkpassword': LagartoAuth.valid_user
                            }
            
            confdict["/config"] = {
                            "tools.auth_basic.on": True,
                            'tools.auth_basic.realm': 'localhost',
                            'tools.auth_basic.checkpassword': LagartoAuth.valid_user
                            }
        
        cherrypy.config.update(confdict)
        
        app = cherrypy.tree.mount(self, '/', conffile)
        app.merge(confdict)
        
        if hasattr(cherrypy.engine, "signal_handler"):
            cherrypy.engine.signal_handler.subscribe()
        if hasattr(cherrypy.engine, "console_control_handler"):
            cherrypy.engine.console_control_handler.subscribe()
        cherrypy.engine.start()
        cherrypy.engine.block()
        
        
    def __init__(self, data_server, config, working_dir):
        """
        Constructor
        
        @param data_server: Data server
        @param config: Lagarto config object
        @param working_dir: working directory
        """
        threading.Thread.__init__(self)
        
        ## Data server, probably the parent object
        self.data_server = data_server
        ## Process name
        self.proc_name = config.procname
        ## HTTP server port
        self.port = config.httpport
        ## Local IP address
        self.address = config.address
        ## Working directory
        LagartoHttpServer.working_dir = working_dir
