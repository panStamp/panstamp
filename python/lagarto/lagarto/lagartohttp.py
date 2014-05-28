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
__date__  ="$Feb 04, 2012$"
#########################################################################

from lagartoresources import LagartoEndpoint, LagartoMessage
import lagartoauth as auth

from wsgiref.simple_server import make_server
from cgi import parse_qs, escape
import threading
import json
import sys
import os


class LagartoHttpServer(threading.Thread):
    """
    Lagarto HTTP/GET server
    """
    # Data server
    data_server = None
    ## Process name
    proc_name = None
    ## HTTP server port
    port = None
    ## Local IP address
    address = None
    ## Working directory
    working_dir = None
    ## Lagarto directory
    lagarto_dir = os.path.dirname(__file__)
    
    path_info = None
    query_string = None
    
    mime_types = {
                  "htm" : "text/html",
                  "html" : "text/html",
                  "css" : "text/css",
                  "txt" : "text/plain",
                  "xml" : "application/xml",
                  "json" : "application/json",
                  "jpg" : "image/jpeg",
                  "jpeg" : "image/jpeg",
                  "gif" : "image/gif",
                  "js" : "application/javascript"
                  }
   
    
    @staticmethod
    def _http_simple_get_value(uid, location, name):
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
    def _http_simple_set_value(uid, location, name, value):
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


    @staticmethod
    def _http_json_get_value(uids=None, locations=None, names=None):
        """
        Get single or multiple endpoint values from data source
        
        @param uids: list of endpoint ids
        @param locations: list of endpoint locations
        @param names: list of endpoint names
        
        @return list of endpoint values in JSON format
        """
        # Create list of JSON endpoint data
        req = []
        if uids is not None and locations is not None and names is not None:
            if len(uids) > 0:
                for uid in uids:
                    endp = LagartoEndpoint(endp_id=uid)
                    req.append(endp.dumps())
            elif len(locations) == len(names) > 0:
                for i, loc in enumerate(locations):
                    endp = LagartoEndpoint(location=loc, name=names[i])
                    req.append(endp.dumps())
        else:  # Return whole network status
            req = None
                    
        status = LagartoHttpServer.data_server.get_status(req)
        
        if status is not None:
            if len(status) > 0:
                # Create complete Lagarto status message
                http_server = LagartoHttpServer.address + ":" + str(LagartoHttpServer.port)
                response = LagartoMessage(proc_name=LagartoHttpServer.proc_name, http_server=http_server, status=status)
                return json.dumps(response.dumps())
                
        return None
    

    @staticmethod
    def _http_json_set_value(uids, locations, names, values):
        """
        Set single or multiple endpoint values on data source
        
        @param uids: list of endpoint ids
        @param locations: list of endpoint locations
        @param names: list of endpoint names
        @param values: list of endpoint values
        
        @return list of endpoint values in JSON format
        """
        # Create list of JSON endpoint data
        req = []
        if values is not None:
            if len(values) > 0:
                for i, val in enumerate(values):
                    if len(locations) == len(names) > i:
                        endp = LagartoEndpoint(location = locations[i], name=names[i], value=val)
                    elif len(uids) > i:
                        endp = LagartoEndpoint(endp_id=uids[i], value=val)
                    req.append(endp.dumps())
        else:
            return None
                    
        status = LagartoHttpServer.data_server.set_status(req)
        
        if status is not None:
            if len(status) > 0:
                # Create complete Lagarto status message
                http_server = LagartoHttpServer.address + ":" + str(LagartoHttpServer.port)
                response = LagartoMessage(proc_name=LagartoHttpServer.proc_name, http_server=http_server, status=status)
                return json.dumps(response.dumps())
                
        return None
  
    
    @staticmethod
    def _get_endpoint_id(path):
        """
        Read endpoint id (if any) from http path
        
        @param path: tokenized ('/') PATH_INFO read from HTTP/GET request
        """
        if len(path) != 3:
            return None
        
        return path[1]
    
    
    @staticmethod
    def _get_endpoint_location(path):
        """
        Read endpoint location (if any) from http path
        
        @param path: tokenized ('/') PATH_INFO read from HTTP/GET request
        """
        if len(path) < 4:
            return None
        
        return path[1]
    
    
    @staticmethod
    def _get_endpoint_name(path):
        """
        Read endpoint name (if any) from HTTP path
        
        @param path: tokenized ('/') PATH_INFO read from HTTP/GET request
        """
        if len(path) < 4:
            return None
        
        return path[2]


    @staticmethod
    def _process_request(environ, start_response):
        """
        Process http request
        """
        # The environment variable CONTENT_LENGTH may be empty or missing
        request_method = "POST"
        try:
            request_body_size = int(environ.get('CONTENT_LENGTH', 0))
            if request_body_size == 0:
                request_method = "GET"
        except (ValueError):
            request_method = "GET"
 
        # Read PATH_INFO string
        LagartoHttpServer.path_info = environ["PATH_INFO"] 
        
        # HTTP POST request?
        if request_method == "POST":
            # When the method is POST the query string will be sent
            # in the HTTP request body which is passed by the WSGI server
            # in the file like wsgi.input environment variable.
            LagartoHttpServer.query_string = environ['wsgi.input'].read(request_body_size)
        # HTTP GET request?
        else:
            LagartoHttpServer.query_string = environ["QUERY_STRING"]

        path = escape(LagartoHttpServer.path_info).split('/')
        path = path[1:]
        
        # Read/Write endpoint?
        if path[0] == "values":
            (status, response_headers, response_body) = LagartoHttpServer._serve_values(LagartoHttpServer.query_string, path)
        elif path[0] == "command":
            (status, response_headers, response_body) = LagartoHttpServer._send_command(LagartoHttpServer.query_string, path)
        else:
            # Process request with basic auth enabled
            return LagartoHttpServer._process_request_secu(environ, start_response)
            
        start_response(status, response_headers)
        return [response_body]


    @staticmethod
    @auth.lagartoauth
    def _process_request_secu(environ, start_response):
        """
        Process http request
        """       
        path = escape(LagartoHttpServer.path_info).split('/')
        path = path[1:]
            
        response_body = ""
        
        if path[0] == "command":
            (status, response_headers, response_body) = LagartoHttpServer._send_command(LagartoHttpServer.query_string, path)
        elif path[0] == "core":
            (status, response_headers, response_body) = LagartoHttpServer._request_core(LagartoHttpServer.query_string, path)
        # Serve static file
        else:
            (status, response_headers, response_body) = LagartoHttpServer._serve_file(LagartoHttpServer.path_info)
          
        start_response(status, response_headers)
    
        return [response_body]


    @staticmethod
    def _serve_values(query_string, path):
        """
        Serve endpoint values
        
        @param query_string: HTTP query string
        @param path: HTTP path info as a list
        
        @return response (status, headers, body) tuple
        """
        # Dictionary containing lists as values.
        d = parse_qs(query_string)
        
        locations = d.get("location", [])
        names = d.get("name", [])
        uids = d.get("id", [])
        values = d.get("value", [])
    
        # Simple format / single endpoint?
        if len(locations) == 0 and len(uids) == 0:
            # Endpoint information containied in the url:
            endpoint_id = LagartoHttpServer._get_endpoint_id(path)
            endpoint_location = LagartoHttpServer._get_endpoint_location(path)
            endpoint_name = LagartoHttpServer._get_endpoint_name(path)
            
            if endpoint_id is None and endpoint_location is None:
                body = LagartoHttpServer._http_json_get_value()
                mime_type = "application/json"
            else:            
                if len(values) == 0:
                    body = LagartoHttpServer._http_simple_get_value(endpoint_id, endpoint_location, endpoint_name)
                else:
                    body = LagartoHttpServer._http_simple_set_value(endpoint_id, endpoint_location, endpoint_name, values[0])
                mime_type = "text/html"            
        # JSON format?
        else:
            locations = [escape(loc) for loc in locations]
            names = [escape(name) for name in names]
            uids = [escape(uid) for uid in uids]
            values = [escape(val) for val in values]
        
            if len(values) == 0:
                body = LagartoHttpServer._http_json_get_value(uids, locations, names)
            else:
                body = LagartoHttpServer._http_json_set_value(uids, locations, names, values)
            mime_type = "application/json"
        
        if body is None or body == "":
            mime_type = "text/html"
            status = "404 Not Found"
            body = "Endpoint or endpoints not found"
        else:
            status = "200 OK"
        
        headers = [("Content-Type", mime_type), ("Content-Length", str(len(body)))]
        
        return (status, headers, body)


    @staticmethod
    def _serve_file(file_path):
        """
        Serve static file
        
        @param file_path: path to the file being requested
        
        @return response (status, headers, body) tuple
        """
        file_path = file_path.strip('/')      
        fpath = file_path.split("/")
        
        # Redirect to the corrrect path
        if fpath[0] == "lagarto":
            file_path = os.path.join(LagartoHttpServer.lagarto_dir, "www", os.sep.join(fpath[1:]))
        elif fpath[0] == "config":
            file_path = os.path.join(LagartoHttpServer.working_dir, "config", os.sep.join(fpath[1:]))
        elif fpath[0] == "":
            file_path = os.path.join(LagartoHttpServer.working_dir, "www", "index.html")
        else:
            file_path = os.path.join(LagartoHttpServer.working_dir, "www", os.sep.join(fpath))
               
        try:
            # Open and read file
            f = open(file_path, 'rb')
            body = f.read()
            f.close()
        
            # Get file extension
            file_extension = file_path.rpartition('.')[2]
            # Get mime-type
            if file_extension in LagartoHttpServer.mime_types:
                mtype = LagartoHttpServer.mime_types[file_extension]
            else:
                mtype = "text/plain"

            status = "200 OK"
        except:
            status = "404 Not Found"
            mtype = "text/plain"
            body = "Unable to open file " + file_path
               
        # Set response headers
        headers = [("Content-Type", mtype), ("Content-Length", str(len(body)))]
        
        return (status, headers, body)


    @staticmethod
    def _send_command(query_string, path):
        """
        Send command to data server
        
        @param query_string: HTTP query string
        @param path: HTTP path info as a list
        
        @return response (status, headers, body) tuple
        """
        # Get file extension
        command = path[1]
        # Dictionary containing lists as values.
        query_list = parse_qs(query_string)
        params = {}
        
        for key, val in query_list.iteritems():
            params[key] = val[0]
 
        res = LagartoHttpServer.data_server.http_command_received(command, params)
 
        if res == True:
            return LagartoHttpServer._serve_file("lagarto/command_ok.html")
        elif res == False:
            return LagartoHttpServer._serve_file("lagarto/command_nok.html")
        elif type(res) == str:
            return LagartoHttpServer._serve_file(res)
        else:
            mtype = "application/json"
            status = "200 OK"
            body = json.dumps(res)
            headers = [("Content-Type", mtype), ("Content-Length", str(len(body)))]
            return (status, headers, body)
    

    @staticmethod
    def _request_core(query_string, path):
        """
        Request core function on data server
        
        @param query_string: HTTP query string
        @param path: HTTP path info as a list
        
        @return response (status, headers, body) tuple
        """
        # Get file extension
        command = path[1]
        # Dictionary containing keyword:value pairs
        query_list = parse_qs(query_string)
        params = {}
        
        for key, val in query_list.iteritems():
            params[key] = val[0]
 
        if command == "restart_process":
            python = sys.executable
            os.execl(python, python, * sys.argv)

        elif command == "get_accounts":
            accounts = auth.LagartoAuth.get_user_names()
            body = json.dumps({"accounts": accounts})
            mime_type = "application/json"
            status = "200 OK"
            headers = [("Content-Type", mime_type), ("Content-Length", str(len(body)))]
            
            return (status, headers, body)
        
        elif command == "server_config":
            if "procname" in params:
                LagartoHttpServer.data_server.config.procname = params["procname"]
            if "broadcast" in params:
                LagartoHttpServer.data_server.config.broadcast = params["broadcast"]
            if "httpport" in params:
                LagartoHttpServer.data_server.config.httpport = params["httpport"]
            LagartoHttpServer.data_server.config.save()
            
            return LagartoHttpServer._serve_file("lagarto/command_ok.html")

        elif command == "edit_account":
            if "olduser" in params:
                old_user = params["olduser"]
            else:
                old_user = None
            if auth.LagartoAuth.set_account(username=params["user"],
                                    password=params["password"],
                                    oldusername=old_user):
                return LagartoHttpServer._serve_file("lagarto/command_ok.html")
        
        elif command == "delete_account":
            if auth.LagartoAuth.delete_account(params["user"]):
                return LagartoHttpServer._serve_file("lagarto/command_ok.html")

        return LagartoHttpServer._serve_file("lagarto/command_nok.html")


    def run(self):
        """
        Run HTTP aserver
        """
        print "Starting HTTP server"
        self.httpd = make_server("", LagartoHttpServer.port, LagartoHttpServer._process_request)
        self.httpd.serve_forever(poll_interval=0.5)
        print "Closing HTTP server..."
        

    def stop(self):
        """
        Stop HTTP server
        """        
        self.httpd.shutdown()
        self.httpd.server_close()     
        

    def __init__(self, data_server, config, working_dir):
        """
        Constructor
        
        @param data_server: Data server
        @param config: Lagarto config object
        @param working_dir: working directory
        """
        threading.Thread.__init__(self)
        
        ## Data server, probably the parent object
        LagartoHttpServer.data_server = data_server
        ## Process name
        LagartoHttpServer.proc_name = config.procname
        ## HTTP server port
        LagartoHttpServer.port = config.httpport
        ## Local IP address
        LagartoHttpServer.address = config.address
        ## Working directory
        LagartoHttpServer.working_dir = working_dir
