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
__date__  ="$Oct 18, 2012$"
#########################################################################

import sqlite3
from datetime import datetime, timedelta
datetime.strptime("2012-12-12", "%Y-%m-%d")   # This avoids thread locks
import os.path
import sys
import json
import threading
import Queue

from maxdefs import MaxDefinitions
from api import NetworkAPI as network

sys.path.append(MaxDefinitions.lagarto_dir) 
from lagartoresources import LagartoException


class LogTable:
    """
    Logging table class
    """
    def query(self, columns=None, start=None, end=None, samples=None):
        """
        Query table and return contents in JSON format
        
        @param columns columns to be queried
        @param start start date/time
        @param end end date/time
        @param samples number of samples requested
        
        @return table contents in JSON format
        """
        if columns is None:            
            cols = "*"            
            # Query column names
            query = "PRAGMA table_info(" + self.name + ")"
            columns = self.database.runquery(query)
        else:
            cols = "timestamp, " + ', '.join(columns)
            columns.insert(0, "timestamp")
        
        range_condition = ""
        if start is not None or end is not None:
            range_condition += " WHERE "        
        if start is not None:
            range_condition += "timestamp >= '" + start + "'"
        if end is not None:
            if start is not None:
                range_condition += " and "
            range_condition += "timestamp <= '" + end + "'"
        
        query = "SELECT " + cols + " FROM " + self.name + range_condition
        
        reverse_result = False
        if samples is not None:
            query += " ORDER BY timestamp DESC LIMIT " + str(samples)
            reverse_result = True

        try:
            # Run query
            res = self.database.runquery(query)
            if reverse_result:
                res.reverse()
            # Convert response to JSON format
            formatted_res = {"columns": columns}
            formatted_res["data"] = res
            
        except LagartoException:
            raise
        
        return formatted_res
        
        
    def get_last_timestamp(self):
        """
        Get latest timestamp from database
        
        @return datetime object
        """
        query = "SELECT timestamp FROM " + self.name
        query += " ORDER BY timestamp DESC LIMIT 1"

        try:
            # Run query
            res = self.database.runquery(query)
            if len(res) > 0:
                if len(res[0]) > 0:
                    return datetime.strptime(res[0][0], "%Y-%m-%d %H:%M:%S")
        except LagartoException:
            raise
       
        return None

        
    def update(self, row):
        """
        Add row to the table
        
        @param row new row to be added to teh table, excluding timestamp
        """
        self.get_last_timestamp()
        query = "INSERT INTO " + self.name + " VALUES ('"
        current_date = datetime.now()
        query += current_date.strftime("%Y-%m-%d %H:%M:%S")
        query += "'"
        
        for item in row:
            query += ", " + str(item)
        query += ")"
        
        try:
            # Run query
            self.database.runquery(query)
        except LagartoException:
            raise


    def __init__(self, database, name, endpoints=None):
        """
        Constructor
        
        @param database database object
        @param name table name
        @param endpoints list of endpoints
        """
        ## Database object
        self.database = database
        ## Table name
        self.name = name

        if endpoints is not None:
            query = "CREATE TABLE " + self.name + " (timestamp text"
            
            for col in endpoints:
                if type(col) is float:
                    query += ", " + col + " real"
                if type(col) is int:
                    query += ", " + col + " integer"
                else:
                    query += ", " + col + " text"
                
            query += ")"
            
            try:
                # Run query
                self.database.runquery(query)
            except LagartoException:
                raise


class DataBase(threading.Thread):
    """
    Data Base class
    """
    def query_table(self, name, columns=None, start=None, end=None, samples=None):
        """
        Query table
        
        @param name table name
        @param columns table columns to be queried
        @param start start of timestamp range
        @param end end of timestamp range
        @param samples number of samples requested
        """
        try:
            table = self.get_table(name)
            return table.query(columns, start, end, samples)
        except LagartoException:
            raise
        
        return None
    
        
    def get_table(self, name):
        """
        Get table object
        
        @param name table name
        
        @return table object
        """
        return LogTable(self, name)

        
    def edit_table(self, name, endpoints, interval="5min"):
        """
        Edit or create table in data base
        
        @param name table name
        @param endpoints list of endpoints contained in this table
        @param interval update interval ("1m", "5m", "1d", etc)
        @return table created
        """
        tables = self.db_config["database"]
        
        if name in tables:
            # Update config information
            # Endpoints changed?
            if tables[name]["endpoints"][:-1] != endpoints[:-1]:
                # Delete table
                self.delete_table(name)
            else:
                tables[name]["interval"] = interval
                # Save new config
                self.save_config()
        try:
            # Rename table columns
            columns = []
            for endp in endpoints:
                columns.append(endp.rpartition('.')[2])
                
            # Create table in database
            table = LogTable(self, name, columns)
            # Add config information
            tables[name] = {}
            tables[name]["endpoints"] = endpoints
            tables[name]["interval"] = interval            
            # Save new config
            self.save_config()
        except LagartoException:
            raise

    
    def delete_table(self, name):
        """
        Delete table from data base
        
        @param name table name
        """
        query = "DROP TABLE if exists " + name
        
        # Run query
        self.runquery(query)

        # Delete table from config file
        tables = self.db_config["database"]
        if name in tables:
            del tables[name]
            # Save config
            self.save_config()
   
       
    def close(self):
        """
        Close database connection
        """
        self._run_db = False
        self._dbconn.close()

    
    def read_config(self):
        """
        Read configuration file containing metadata
        
        @return dictionary containing configuration
        """       
        try:
            db_cfg_file = open(self.config_path)   
            db_config = json.load(db_cfg_file)
            db_cfg_file.close()
            return db_config
        except IOError as ex:
            return {"database": {}}


    def save_config(self):
        """
        Save configuration file containing metadata
        """        
        try:
            db_cfg_file = open(self.config_path, 'w')     
            json.dump(self.db_config, db_cfg_file, sort_keys=False, indent=2)
            db_cfg_file.close()
        except IOError:
            raise LagartoException("Unable to save database configuration in " + self.config_path)


    def update_tables(self):
        """
        Update table contents
        """
        if self.db_config is None:
            return
        
        tables = self.db_config["database"]
        
        current = datetime.now()

        for name, table in tables.iteritems():
            # Get table object from database
            dbtable = self.get_table(name)            
            update = False
            # Get latest timestamp fromdatabase
            last = dbtable.get_last_timestamp()
            if last is None:
                update = True
            else:
                elapsed = current - last
            if not update:
                if table["interval"] == "1m":
                    if elapsed >= timedelta(minutes=1):
                        update = True
                elif table["interval"] == "5m":
                    if elapsed >= timedelta(minutes=5):
                        update = True
                elif table["interval"] == "15m":
                    if elapsed >= timedelta(minutes=15):
                        update = True
                elif table["interval"] == "30m":
                    if elapsed >= timedelta(minutes=30):
                        update = True           
                elif table["interval"] == "1h":
                    if elapsed >= timedelta(hours=1):
                        update = True
                elif table["interval"] == "12h":
                    if elapsed >= timedelta(hours=12):
                        update = True
                elif table["interval"] == "1d":
                    if elapsed >= timedelta(days=1):
                        update = True
                elif table["interval"] == "1w":
                    if elapsed >= timedelta(weeks=1):
                        update = True
                elif table["interval"] == "2w":
                    if elapsed >= timedelta(weeks=2):
                        update = True
                elif table["interval"] == "1M":
                    if current.month != last.month:
                        update = True
            
            if update:                      
                # Fill endpoint data
                endpoint_values = []
                for endp_id in table["endpoints"]:
                    endp = network.get_endpoint(endp_id)
                    if endp is None:
                        raise LagartoException("Unable to find a matching endpoint for " + endp_id + " before updating " + name + " from database")
                    if endp.value is None:
                        raise LagartoException(endp_id + " has an invalid value. Unable to update " + name + " from database")                        
                    endpoint_values.append(endp.value)
                # Update table            
                dbtable.update(endpoint_values)
        
    
    def runquery(self, query):
        """
        Run database query
        
        @param query SQL query to be run
        
        @return response from database
        """
        # Put query in queue
        self._queue.put(query, block=True)
        # Block until the query is run
        self._queue.join()
        
        if self._db_error is not None:
            raise LagartoException(self._db_error)
        
        return self._db_response

    
    def run(self):
        """
        Run database manager on its own thread
        """
        try:
            # Connection to SQLite database
            self._dbconn = sqlite3.connect(self.database_path)
        except sqlite3.Error, e:
            raise LagartoException(e.args[0])

        while self._run_db:
            # Gets the SQL query from the queue
            query = self._queue.get()
            self._db_response = None
            self._db_error = None           
            
            try:
                c = self._dbconn.cursor()
                # Create table
                c.execute(query)
                
                if (query.startswith("SELECT")):
                    # Get response
                    self._db_response = c.fetchall()
                else:
                    # Commit changes
                    self._dbconn.commit()
            except sqlite3.Error, e:
                self._db_error = e.args[0]
            finally:
                # Job done
                self._queue.task_done()
    

    def edit_graph(self, graph_name, table_name, columns=None, title=None, graphtype=None, samples=None,
                   start=None, end=None, miny=None, maxy=None, show_grid=False):
        """
        Edit graph config settings
        
        @param graph_name name of the graph
        @param table_name name of the table from the database
        @param columns columns to be plotted
        @param title title of the graph
        @param graphtype type of graph ("lines", "bars")
        @param samples amount of samples to be ploted. None for all
        @param start optional start date/time
        @param end optional end date/time
        @param miny minimum Y value to show in the graph
        @param maxy maximum Y value to show in the graph
        @param show_grid display grid on graph
        """
        try:
            graph = Graph(graph_name, table_name, columns, title, graphtype, samples, start, end, miny, maxy, show_grid)
            # Save new config
            graph.save()
        except LagartoException:
            raise


    def delete_graph(self, graph_name):
        """
        Remove graph from config file
        
        @param graph_name name of the graph to be deleted
        """
        try:
            graph = Graph(graph_name)
            # Save new config
            graph.delete()
        except LagartoException:
            raise
        

    def __init__(self, name="lagarto.db"):
        '''
        Constructor
        
        @param name name of the data base
        '''
        self.database_path = os.path.join(MaxDefinitions.working_dir, "database", name)
        self.config_path = os.path.join(MaxDefinitions.working_dir, "config", "database.json")
        
        threading.Thread.__init__(self)
        
        self._queue = Queue.Queue()
        self.daemon = True
        self._run_db = True
        self._db_response = None
        self._db_error = None
        
        ## Database config metadata
        self.db_config = self.read_config()
        
        try:
            # Run thread
            self.start()
            pass
        except:
            raise


class Graph:
    """
    Graph/chart management class
    """
    def read(self):
        """
        Read configuration file containing metadata
        
        @return dictionary containing configuration
        """       
        try:
            cfg_file = open(self.config_path)   
            config = json.load(cfg_file)
            cfg_file.close()
            return config
        except IOError as ex:
            return {"graphs": {}}
        

    def save(self):
        """
        Save properties in configuration file
        """
        try:
            cfg_file = open(self.config_path, 'w')     
            json.dump(self.graph_config, cfg_file, sort_keys=False, indent=2)
            cfg_file.close()
        except IOError:
            raise LagartoException("Unable to save graph configuration in " + self.config_path)


    def delete(self):
        """
        Delete current graph from config file
        """
        if self.name in self.graph_config["graphs"]:
            del self.graph_config["graphs"][self.name]
            self.save()
        else:
            raise LagartoException("Unable to delete " + self.name + " from " + self.config_path)

        
    def __init__(self, name, table=None, columns=None, title=None, graphtype=None, samples=None,
                 start=None, end=None, miny=None, maxy=None, show_grid=False):
        """
        Constructor
        
        @param name name of the graph
        @param table name of the table from the database
        @param columns list of columns to be plotted
        @param title title of the graph
        @param graphtype type of graph ("lines", "bars")
        @param samples amount of samples to be ploted. None for all
        @param start optional start date/time
        @param end optional end date/time
        @param miny minimum Y value to show in the graph
        @param maxy maximum Y value to show in the graph
        @param show_grid display grid on graph
        """
        ## Name of the graph
        self.name = name
        
        ## Configuration file
        self.config_path = os.path.join(MaxDefinitions.working_dir, "config", "graphs.json")
        
        ## Properties
        self.properties = {}
        if table is not None:
            self.properties["table"] = table
        if columns is not None:
            self.properties["columns"] = columns
        if title is not None:
            self.properties["title"] = title
        if graphtype is not None:
            self.properties["type"] = graphtype
        if samples is not None:
            self.properties["samples"] = samples
        if start is not None:
            self.properties["start"] = start
        if end is not None:
            self.properties["end"] = end
        if miny is not None:
            self.properties["miny"] = miny
        if maxy is not None:
            self.properties["maxy"] = maxy
        if show_grid is True:
            self.properties["show_grid"] = "on"
            
        ## Config settings
        self.graph_config = self.read()

        if "graphs" not in self.graph_config:
            self.graph_config = {"graphs" : {}}
        # Update local properties in config
        self.graph_config["graphs"][self.name] = self.properties
