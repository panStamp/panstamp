#########################################################################
#
# evnmanager
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
__author__="Daniel Berenguer"
__date__  ="$Jan 23, 2012$"
#########################################################################

import os
import sys
import threading
import time
import json
from xmltools import XmlSettings
from database import DataBase
from maxdefs import MaxDefinitions
sys.path.append(MaxDefinitions.lagarto_dir) 
from lagartocomms import LagartoBroker
from lagartoresources import LagartoEndpoint, LagartoException

from api import TimeAPI, NetworkAPI
from clouding import OpenSense
from bulkcloud import BulkCloud


try:
    import webscripts
except ImportError:
    from webevents import WebEvent
    try:
        WebEvent.create_empty()
        import webscripts
    except:
        raise

from webevents import WebEvent
import scripts.events

    
class PeriodicTrigger(threading.Thread):
    """
    Periodic trigger class
    """
    def run(self):
        """
        Start timer
        """
        """
        tm_year = time.localtime()[0]
        tm_mon = time.localtime()[1]
        tm_mday = time.localtime()[2]
        tm_hour = time.localtime()[3]
        tm_min = time.localtime()[4]
        tm_sec = time.localtime()[5]
        """

        while True:
            tm_sec = time.localtime()[5]
            time.sleep(60.0 - tm_sec)
            
            try:
                # Run time-based events
                TimeEvent()
            except LagartoException as ex:
                ex.log()
    
            try:
                # Update contents in database
                self.database.update_tables()
            except LagartoException as ex:
                ex.log()
                
            try:
                # Upload bulk data to cloud services
                self.cloud.eval_upload()
            except LagartoException as ex:
                ex.log()
                
       
    def __init__(self, database, cloud):
        """
        Constructor
        
        @param database Database object
        @param cloud BulkCloud object
        """
        threading.Thread.__init__(self)
        # Configure thread as daemon
        self.daemon = True
        # Database object
        self.database = database
        # BulkCloud onject
        self.cloud = cloud
        # Run thread
        self.start()


class TimeEvent(threading.Thread):
    """
    Time-based event
    """
    def run(self):
        """
        Run event
        """
        reload(webscripts)
        attributes = dir(webscripts.WebScripts)
        TimeAPI.event = True
        for attr in attributes:
            if attr.startswith("evn_"):
                event = getattr(webscripts.WebScripts, attr)
                event()
        TimeAPI.event = False

        
    def __init__(self):
        """
        Constructor
        """
        threading.Thread.__init__(self)
        NetworkAPI.reset_event()
        TimeAPI.update_time()
        
        # Run event script
        evnscript = EventScript("clock", TimeAPI.current_time)
        
        # Run web script
        self.start()

    
class EvnManager(LagartoBroker):
    """
    Lagarto event management class
    """

    def notify_status(self, event):
        """
        Notify status to the master application (callback)
        To be implemented by subclass
        
        @param event: message received from publisher in JSON format
        """
        reload(webscripts)
        attributes = dir(webscripts.WebScripts)
               
        for endp in event["status"]:
            # Create lagarto endpoint
            lagarto_endp = LagartoEndpoint(endpstr=endp, procname=event["procname"])
            # Run event script
            evnscript = EventScript("network", lagarto_endp)
            # Update network event in API
            NetworkAPI.event = [event["procname"] + "." + endp["location"]  + "." + endp["name"], endp["value"]]
               
            # Wait if time event currently running
            while TimeAPI.event:
                time.sleep(0.1)

            # Run web script
            for attr in attributes:
                if attr.startswith("evn_"):
                    event_func = getattr(webscripts.WebScripts, attr)
                    event_func()

            
    def http_command_received(self, command, params):
        """
        Process command sent from HTTP server. Method to be overrided by data consumer.
        Method required by LagartoClient
        
        @param command: command string
        @param params: dictionary of parameters
        
        @return True if command successfukky processed by server.
        Return False otherwise
        """
        try:
            if command == "get_server_list":
                return self.get_servers()
            
            elif command == "get_endpoint_list":
                return self.get_endpoints(params["server"])
    
            elif command == "set_endpoint_value":
                location = None
                name = None
                endp_id = None
                
                if "id" in params:
                    endp_id = params["id"]
                if "location" in params:
                    location = params["location"]
                    if "name" in params:
                        name = params["name"]
    
                endpoint = LagartoEndpoint(endp_id = endp_id, location=location, name=name, value=params["value"], procname=params["procname"])           
                return self.set_endpoint(endpoint)
    
            elif command == "set_from_opensense":
                if "feed_id" not in params:
                    return None
    
                endp = None
                for key, value in OpenSense.feed_ids.iteritems():
                    if value == params["feed_id"]:
                        endp = value
    
                if endp is None:
                    return None
                endp_data = endp.split(".")
                if len(endp_data) != 3:
                    return None
                       
                endpoint = LagartoEndpoint(location=endp_data[1], name=endp_data[2], value=params["value"], procname=endp_data[0])
                return self.set_endpoint(endpoint)
        
            elif command == "get_event_list":
                return WebEvent.get_events()
            
            elif command == "get_event":
                if "id" in params:
                    try:
                        event = WebEvent(params["id"])
                        return event.dumps()
                    except:
                        pass
                
            elif command == "delete_event":
                if "id" in params:
                    event = WebEvent(params["id"])
                    event.delete()
                    return "event_panel.html"
    
            elif command == "config_event_name":
                if "id" in params:
                    event = WebEvent(params["id"])
                    event.name = params["name"]
                    event.save()
                    return True
                    
            elif command == "set_event_line":
                if "id" in params:
                    event = WebEvent(params["id"])
                    linenb = params["linenb"]
                    event.set_line(params["line"], linenb, params["type"])
                    event.save()
                    return "edit_event.html?id=" + params["id"]
    
            elif command == "delete_event_line":
                if "id" in params:
                    event = WebEvent(params["id"])
                    linenb = params["linenb"]
                    event.delete_line(linenb)
                    event.save()
                    return "edit_event.html?id=" + params["id"]
    
            elif command == "edit_db_table":
                self.database.edit_table(params["name"], params["endpoints"].split(','), params["interval"])
                return "db_panel.html"
    
            elif command == "delete_db_table":
                self.database.delete_table(params["name"])
                return "db_panel.html"
            
            elif command == "edit_graph":
                columns = None
                if "columns" in params:
                    columns = params["columns"].split(',')
                title = None
                if "title" in params:
                    title = params["title"]
                graphtype = None
                if "type" in params:
                    graphtype = params["type"]
                samples = None
                if "samples" in params:
                    samples = params["samples"]
                start = None
                if "starttime" in params:
                    start = params["starttime"]
                end = None
                if "endtime" in params:
                    end = params["endtime"]
                miny = None
                if "miny" in params:
                    miny = params["miny"]
                maxy = None
                if "maxy" in params:
                    maxy = params["maxy"]
                show_grid = False
                if "show_grid" in params:
                    show_grid = (params["show_grid"].lower() in ["true", "on", "yes", "enable"])
                self.database.edit_graph(graph_name=params["name"],
                                         table_name=params["table"],
                                         columns=columns,
                                         title=title,
                                         graphtype=graphtype,
                                         samples=samples,
                                         start=start,
                                         end=end,
                                         miny=miny,
                                         maxy=maxy,
                                         show_grid=show_grid)
                return "graph_panel.html"

            elif command == "delete_graph":
                self.database.delete_graph(params["name"])
                return "graph_panel.html"
            
            elif command == "query_db_table":
                columns = None
                start = None
                end = None
                samples = None
                if "table" in params:
                    if "columns" in params:
                        columns = params["columns"].split(',')
                    if "start" in params:
                        start = params["start"].replace('+', ' ')
                    if "end" in params:
                        end = params["end"].replace('+', ' ')
                    if "samples" in params:
                        samples = params["samples"]
                return self.database.query_table(params["table"], columns, start, end, samples)
                
            elif command == "edit_cloud_tx":
                endp_dict = json.loads(params["endpoints"])
                self.bulkcloud.edit_cloud_tx(params["name"], params["service"], endp_dict, params["apikey"], params["interval"])
                return "cloud_panel.html"

            elif command == "delete_cloud_tx":
                self.bulkcloud.delete_cloud_tx(params["name"])
                return "cloud_panel.html"
                
        except LagartoException as ex:
            ex.log()
        except Exception as ex:
            print ex
            pass

        return False


    def stop(self):
        """
        Stop event manager
        """       
        # Stop lagarto client
        LagartoBroker.stop(self)
        
        
    def __init__(self):
        """
        Constructor
        """
        # Read configuration
        config_file = os.path.join(MaxDefinitions.working_dir, "config", "settings.xml")
        XmlSettings(config_file)
        
        # Set log file to trace lagarto exceptions
        LagartoException.error_file = XmlSettings.error_file
        
        # Lagarto broker constructor
        LagartoBroker.__init__(self, MaxDefinitions.working_dir)

        NetworkAPI(lagarto_client=self)

        # Run startup script
        scripts.events.startup()      
        
        # Open database
        self.database = DataBase()
        
        # Cloud transmissions
        self.bulkcloud = BulkCloud()
        
        # Start periodic trigger thread
        PeriodicTrigger(self.database, self.bulkcloud)
        
        # Start Lagarto client
        self.start()
        

class EventScript(threading.Thread):
    """
    Class used to run event handler on independednt thread
    """
    def run(self):
        """
        Run thread
        """
        reload(scripts.events)
        scripts.events.event_handler(self.evnsrc, self.evnobj)

        
    def __init__(self, evnsrc, evnobj):
        """
        Constructor
        
        @param evnsrc: event source ("network", "clock", "startup")
        @param evnobj: event object
        """
        threading.Thread.__init__(self)
        
        # Event source
        self.evnsrc = evnsrc
        
        # Event object
        self.evnobj = evnobj
        
        # Run event handler
        self.start()

