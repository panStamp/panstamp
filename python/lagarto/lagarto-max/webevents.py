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
__date__  ="$Feb 26, 2012$"
#########################################################################

import inspect
import os


class WebEvent:
    """
    Web event
    """
    working_dir = os.path.dirname(__file__)
    template_str = "from api import TimeAPI as clock, NetworkAPI as network, CloudAPI as cloud\n\nclass WebScripts:"
    script_file = os.path.join(working_dir, "webscripts.py")
    tmp_file = os.path.join(working_dir, "webscripts.tmp")
    
    @staticmethod
    def get_event_code(evnid):
        """
        Retrieve source code from event
        
        @param evnid: event id
        
        @return: list containing every line of source code
        """
        code = []
        event_found = False
        file = open(WebEvent.script_file)
        for line in file:
            if not event_found:
                l = line.strip(" ")
                if l == "def " + evnid + "():\n":
                    event_found = True
                    code.append(" " * 4 + "@staticmethod\n")
                    code.append(line)
            else:
                if line == "\n":
                    return code
                else:
                    code.append(line)
        return code


    @staticmethod
    def get_events(showcode=False):
        """
        Get list of events
        
        @param showcode: add source code information if True
        
        @return list of events
        """
        events = []
        import webscripts
        reload(webscripts)
        attributes = dir(webscripts.WebScripts)
        for attr in attributes:
            if attr.startswith("evn_"):
                event = getattr(webscripts.WebScripts, attr)
                # Retrieve event name from method's doc
                evndescr = inspect.getdoc(event)
                event_data = {"id": attr, "name": evndescr}
                if showcode:
                    body = WebEvent.get_event_code(attr)
                    if body is not None:
                        event_data["code"] = body
                
                events.append(event_data)
        
        return {"events": events}
        
    
    @staticmethod
    def create_empty():
        try:
            f = open(WebEvent.script_file, 'w')
            f.write(WebEvent.template_str)
            f.write("\n    pass\n")
        except IOError:
            raise MaxException("Unable to create " + WebEvent.script_file)
        finally:
            f.close()

          
    def delete(self):
        """
        Delete current event from script file
        """
        # Read all events
        origin = WebEvent.get_events(True)
        if origin is not None:
            target = None
            try:
                # Write events in temporary file
                target = open(WebEvent.tmp_file, 'w')
                target.write(WebEvent.template_str)
                for event in origin["events"]:
                    if event["id"] != self.id:
                        target.write("\n")
                        for line in event["code"]:
                            target.write(line)
            
                target.close()
                # Rename to definitive file
                os.rename(WebEvent.tmp_file, WebEvent.script_file)
            except IOError:
                if target is not None:
                    target.close()
                raise MaxException("Unable to create " + WebEvent.tmp_file)

        
    def dumps(self):
        """
        Serialize event in JSON format
        """
        evn_data = {"id": self.id,
                    "name": self.name,
                    "code": [line.strip() for line in self.source_code]}

        return {"event": evn_data}


    def save(self):
        """
        Save event data in script
        """
        # Read all events
        origin = WebEvent.get_events(True)
        if origin is not None:
            target = None
            try:
                # Write events in temporary file
                target = open(WebEvent.tmp_file, 'w')
                target.write(WebEvent.template_str)
                event_found = False
                for event in origin["events"]:
                    target.write("\n")
                    if event["id"] == self.id:
                        event_found = True
                        target.write(" " * 4 + "@staticmethod\n")
                        target.write(" " * 4 + "def " + self.id + "():\n")
                        target.write(" " * 8 + "\"\"\"\n")
                        target.write(" " * 8 + self.name + "\n")
                        target.write(" " * 8 + "\"\"\"\n")
                        
                        for line in self.source_code[5:]:
                            target.write(line)                      
                    else:
                        for line in event["code"]:
                            target.write(line)
           
                if not event_found:
                    target.write("\n")                    
                    target.write(" " * 4 + "@staticmethod\n")
                    target.write(" " * 4 + "def " + self.id + "():\n")
                    target.write(" " * 8 + "\"\"\"\n")
                    target.write(" " * 8 + self.name + "\n")
                    target.write(" " * 8 + "\"\"\"\n")
                    
                    for line in self.source_code[5:]:
                        target.write(line)                      
                    
                target.close()
                
                # For Windows only
                if os.name == "nt":
                    # Remove webscripts.py
                    os.remove(WebEvent.script_file)
                # Rename to definitive file
                os.rename(WebEvent.tmp_file, WebEvent.script_file)
            except IOError:
                if target is not None:
                    target.close()
                raise MaxException("Unable to create temporary file")
            except OSError:
                if target is not None:
                    target.close()
                raise MaxException("Unable to remove or rename temporary file webscripts.tmp")
            

    def set_line(self, line, linenb=None, ltype="trigger"):
        """
        Set event line
        
        @param line: new line of code
        @param linenb: line number to be modified. 0 in case of new line
        @param ltype: type of statement ("trigger", "condition", "action")
        """                  
        linenb = int(linenb)        
        # Add new line?
        if linenb == 0:
            # browse existing lines
            new_trigger_line = 0
            new_condition_line = 0
            section = None
            i = 0
            for l in self.source_code:
                if section is None:
                    if l.startswith(" " * 8 + "if False:"):
                        section = "trigger"
                elif section == "trigger":
                    if l.startswith(" " * 8 + "else:"):
                        new_trigger_line = i
                    if l.startswith(" " * 12 + "return"):
                        new_condition_line = i + 1
                i += 1
            
            if ltype == "trigger":
                statement = " " * 8 + "elif " + line + ":\n"
                self.source_code.insert(new_trigger_line, statement)
                statement = " " * 12 + "pass\n"
                self.source_code.insert(new_trigger_line + 1, statement)
            elif ltype == "condition":
                statement = " " * 8 + "if not (" + line + "):\n"
                self.source_code.insert(new_condition_line, statement)
                statement = " " * 12 + "return\n"
                self.source_code.insert(new_condition_line + 1, statement)
            else:
                statement = " " * 8 + line + "\n"
                self.source_code.append(statement)
        # Modify existing line?
        else:
            if ltype == "trigger":
                statement = " " * 8 + "elif " + line + ":\n"
            elif ltype == "condition":
                statement = " " * 8 + "if not (" + line + "):\n"
            else:
                statement = " " * 8 + line + "\n"
            self.source_code[linenb-1] = statement  
            
            # Save event
            self.save()


    def delete_line(self, linenb):
        """
        Delete event line
        
        @param linenb: line number to be deleted
        """                
        linenb = int(linenb)        
        if linenb > 0:
            index = linenb - 1
            del self.source_code[index]
            if index < len(self.source_code):
                line = self.source_code[index]
                if line.startswith(" " * 12 + "pass") or line.startswith(" " * 12 + "return"):
                    del self.source_code[index]


    def _read(self):
        """
        Read event from script
        """
        import webscripts
        reload(webscripts)
        # Get method
        method = getattr(webscripts.WebScripts, self.id)
        if method is None:
            raise MaxException("Event " + self.id + " is not available")
        # Retrieve event name from method's doc
        self.name = inspect.getdoc(method)
        # Source code
        body = WebEvent.get_event_code(self.id)
        if body is not None:
            self.source_code = body


    def _create(self):
        """
        Create new event
        """
        self.source_code = []
        self.source_code.append(" " * 4 + "@staticmethod\n")
        self.source_code.append(" " * 4 + "def " + self.id + "():\n")
        self.source_code.append(" " * 8 + "\"\"\"\n")
        self.source_code.append(" " * 8 + self.name + "\n")
        self.source_code.append(" " * 8 + "\"\"\"\n")
        self.source_code.append(" " * 8 + "if False:\n")
        self.source_code.append(" " * 12 + "pass\n")
        self.source_code.append(" " * 8 + "else:\n")
        self.source_code.append(" " * 12 + "return\n")
        

    def __init__(self, evnid):
        """
        Constructor
        
        @param evnid: event id
        """
        # Event id
        self.id = evnid
        # Event name
        self.name = evnid
        # List of source code lines
        self.source_code = None
                    
        try:
            # Read event from script
            self._read()
        except:
            try:
                # Create new event
                self._create()
            except:
                raise
