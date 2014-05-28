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
__date__  ="$Mar 31, 2012$"
#########################################################################

from maxdefs import MaxDefinitions

import logging
import os
import httplib
import urllib
import json
import subprocess
from time import strftime


class ApiLog:
    """
    API logging class
    """
    # Logging
    logger = logging.getLogger('Lagarto-Max')
    logging.basicConfig(format='%(asctime)s %(module)s %(levelname)s %(message)s',filename='lagarto-max-cloud.log',level=logging.INFO)


class PachubePacket:
    """
    Generic Pachube packet class
    """
    def push(self):
        """
        Push values to Pachube
        
        @return response from Pachube
        """
        header = {'X-PachubeApiKey': self.sharing_key}
        url = "api.pachube.com"
        res = None

        try:
            conn = httplib.HTTPConnection(url, timeout=8)
            conn.request('PUT', "/v2/feeds/" + self.feed_id, json.dumps(self.packet), header)
            response = conn.getresponse()
            res = response.reason
        except:
            pass
        
        conn.close()

        return res


    def __init__(self, sharing_key, feed_id, endpoints):
        """
        Constructor
        
        @param sharing_key: Pachube sharing key
        @param feed_id: Pachube feed ID
        @param endpoints: list of (datastream, value) pairs
        """
        # Sharing key
        self.sharing_key = sharing_key
        # Feed ID
        self.feed_id = feed_id
        
        datastreams = []
        for endp in endpoints:
            dstream = {"id": endp[0], "current_value": str(endp[1])}
            datastreams.append(dstream)
        
        self.packet = {"version": "1.0.0", "datastreams": datastreams}


class ThingSpeakPacket:
    """
    Generic ThingSpeak packet class
    """
    def push(self):
        """
        Push values to ThingSpeak
        
        @return response from ThingSpeak
        """
        headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
        url = "api.thingspeak.com"
        res = None
        
        try:
            conn = httplib.HTTPConnection(url, timeout=5)
            conn.request("POST", "/update", self.params, headers)       
            response = conn.getresponse()
            res = response.reason
        except:
            pass
        
        conn.close()

        return res


    def __init__(self, api_key, endpoints):
        """
        Constructor
        
        @param api_key: ThingSpeak write API key
        @param endpoints: list of (field ID, value) pairs
        """
        params_dict = {'key': api_key}
        
        for endp in endpoints:
            params_dict[endp[0]] = endp[1]
        
        # Parameters
        self.params = urllib.urlencode(params_dict)


class OpenSense:
    """
    open.sen.se static data
    """
    # List of feed id's
    feed_ids = {}


class OpenSensePacket:
    """
    Generic open.sen.se packet class
    """
    def push(self):
        """
        Push values to open.sen.se
        
        @return response from open.sen.se
        """
        headers = {"Content-type": "application/json", "sense_key": self.sense_key}
        url = "api.sen.se"
        res = None

        try:
            conn = httplib.HTTPConnection(url, timeout=8)
            conn.request('POST', "/events/", json.dumps(self.events), headers)
            response = conn.getresponse()
            res = response.reason
        except:
            pass
        
        conn.close()

        return res


    def __init__(self, sense_key, endpoints):
        """
        Constructor
        
        @param sense_key: open.sen.se authentication key
        @param endpoints: list of (feed_ID, value) pairs
        """
        # Sense key
        self.sense_key = sense_key

        self.events = []
        for endp in endpoints:
            event = {"feed_id": endp[0], "value": endp[1]}
            self.events.append(event)


class TwitterMessage:
    """
    Text message ready to be sent to a Twitter account
    """
    def send(self):
        """
        Send message to Twitter
        """
        command = "twitter set " + self.message
        subprocess.call(command, shell=True)


    def __init__(self, message):
        """
        Constructor
        
        @param message: message text
        """
        timestring = message+' '+strftime("%Y-%m-%d %H:%M:%S")
        #truncate to the maximum chars allowed by twitter that is 160
        self.message=  timestring[:160]

class AutoRemotePacket:
    """
    Generic AutoRemote packet class
    """
    def push(self):
        """
        Push values to AutoRemote

        @return response from AutoRemote
        """
        headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
        url = "autoremotejoaomgcd.appspot.com"
        res = None

        try:
            conn = httplib.HTTPSConnection(url, timeout=5)
            ApiLog.logger.debug('AutoRemotePacket.push.conn.request( \"POST\", \"/sendmessage\", '+str(self.params)+', '+str(headers)+')')
            conn.request("POST", "/sendmessage", self.params, headers)
            response = conn.getresponse()
            res = response.reason
        except:
            pass

        conn.close()
        ApiLog.logger.debug('AutoRemotePacket.push.res: '+str(res))

        return res


    def __init__(self, key, message, target, sender, channel, password):
        """
        Constructor

        @param key: AutoRemote key
        @param message: Formatted message template
        @param target: Target field
        @param sender: Sending device
        @param channel: Act as Sender device channel
        @param password: Device password
        
        """
        ApiLog.logger.info('AutoRemotePacket('+key+', '+message+', '+target+', '+sender+', '+channel+', '+password+')')
        params_dict = {'key': key, 'message': message}
        if target:
                params_dict['target']=target
        if sender:
                params_dict['sender']=sender
        if channel:
                params_dict['channel']=channel
        if password:
                params_dict['password']=password

        # Parameters
        self.params = urllib.urlencode(params_dict)
        ApiLog.logger.debug('AutoRemotePacket.params: '+self.params)


class GroveStreamsPacket:
    """
    Generic GroveStreams packet class
    """
    def push(self):
        """
        Push values to GroveStreams
        
        @return response from GroveStreams
        """        

        header = {"Connection" : "close", "Content-type": "application/json",
                       "X-Forwarded-For": self.endpoints[0][0], "Cookie" : "api_key=" + self.api_key}

        url = "grovestreams.com"
        res = None

        try:
            conn = httplib.HTTPConnection(url, timeout=8)
            if self.template_id is not None:
                conn.request('PUT', "/api/feed?compTmplId=" + self.template_id, json.dumps(self.datastreams), header)
            else:
                conn.request('PUT', "/api/feed", json.dumps(self.datastreams), header)
            response = conn.getresponse()
            res = response.reason
        except:
            pass
        
        conn.close()

        return res


    def __init__(self, api_key, template_id, endpoints):
        """
        Constructor
        
        @param api_key: GroveStreams API key
        @param template_id: component template id
        @param endpoints: list of (datastream, value) pairs
        """
        # API key
        self.api_key = api_key
        
        # Component ID
        self.template_id = None
        if template_id != "":
            self.template_id = template_id        

        # List of endpoints
        self.endpoints = endpoints

        self.datastreams = []   

        for endp in endpoints:
            dstream = {"compId" : endp[0]}
            dstream["streamId"] = "data"
            dstream["data"] = endp[1]
            self.datastreams.append(dstream)


