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
__date__  ="$Feb 24, 2012$"
#########################################################################

#import logging
#import os
import ephem
import time
import sys

from maxdefs import MaxDefinitions
sys.path.append(MaxDefinitions.lagarto_dir)
from lagartoresources import LagartoEndpoint, LagartoException
from xmltools import XmlSettings

from clouding import PachubePacket, ThingSpeakPacket, OpenSensePacket, OpenSense, TwitterMessage, AutoRemotePacket, GroveStreamsPacket


class TimeAPI:
    """
    Time API providing static methods
    """
    # Current time
    current_time = time.localtime()

    # Clock event: True or False
    event = False
       
        
    @staticmethod
    def time():
        """
        Return hhmm value
        """
        return TimeAPI.current_time.tm_hour * 100 + TimeAPI.current_time.tm_min


    @staticmethod
    def date():
        """
        Return MMdd value
        """
        return TimeAPI.current_time.tm_mon * 100 + TimeAPI.current_time.tm_mday


    @staticmethod
    def month():
        """
        Return month [1-12] value
        """
        return TimeAPI.current_time.tm_mon


    @staticmethod
    def monthday():
        """
        Return month day [1-31] value
        """
        return TimeAPI.current_time.tm_mday


    @staticmethod
    def weekday():
        """
        Return week day [0-6] value
        """
        return TimeAPI.current_time.tm_wday


    @staticmethod
    def repeat_time(start_time, interval):
        """
        Repeat time condition a given amount of minutes
        
        @param start_time: initial time condition in hhmm format
        @param interval: periodic interval in hhmm format
        """
        if interval == 0:
            return start_time

        now = TimeAPI.time()
        
        t = start_time
        while True:
            if t == now:
                return now
            elif t == 2400 and now == 0:
                return 0
            elif t > 2400:
                return -1
            t += interval
            

    @staticmethod
    def sunrise():
        """
        Return sunrise time in hhmm format
        """
        observ = ephem.Observer()  
        observ.lat = str(XmlSettings.latitude)  
        observ.long = str(XmlSettings.longitude)  
        sun = ephem.Sun()  
        sun.compute()  
        st = ephem.localtime(observ.next_rising(sun))

        factor = 1
        if st.minute < 10:
            factor = 10
        return st.hour * 100 * factor + st.minute


    @staticmethod
    def sunset():
        """
        Return sunrise time in hhmm format
        """
        observ = ephem.Observer()  
        observ.lat = str(XmlSettings.latitude)  
        observ.long = str(XmlSettings.longitude)  
        sun = ephem.Sun()  
        sun.compute()  
        st = ephem.localtime(observ.next_setting(sun))

        factor = 1
        if st.minute < 10:
            factor = 10
        return st.hour * 100 * factor + st.minute

        
    @staticmethod
    def update_time():
        """
        Update current tine
        """
        TimeAPI.current_time = time.localtime()


class NetworkAPI:
    """
    Static toolbox for network data
    """
    # Lagarto client object
    lagarto_client = None
    # Current network event tuple (endpoint, value)
    event = [None, None]


    @staticmethod
    def reset_event():
        """
        Reset event variable
        """
        NetworkAPI.event = [None, None]

    @staticmethod
    def get_endpoint(endp):
        """
        Get Lagarto endpoint
        
        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id
        
        @return lagarto endpoint object
        """
        epd = endp.split('.')
        length = len(epd)
        epid = None
        eploc = None
        epname = None
        if length == 3:
            eploc = epd[1]
            epname = epd[2]
        else:
            epd = endp.partition('.')
            epid = epd[2]
        
        procname = epd[0]
        lagarto_endp = LagartoEndpoint(endp_id=epid, location=eploc, name=epname, procname=procname)

        try:
            status = NetworkAPI.lagarto_client.request_status(procname, [lagarto_endp.dumps(enable_value=False)])
            if status is not None:
                if len(status) > 0:
                    lagarto_endp = LagartoEndpoint(endpstr=status[0], procname=procname)
                    return lagarto_endp
            return None
        except:
            raise


    @staticmethod
    def get_value(endp):
        """
        Get endpoint value
        
        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id
        
        @return endpoint value
        """
        epd = NetworkAPI.get_endpoint(endp)
        if epd is not None:
            return epd.get_value()
        return None


    @staticmethod
    def set_value(endp, value):
        """
        Set endpoint value
        
        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id
        @param value: new endpoint value
        
        @return endpoint value
        """
        try:
            lagarto_endp = NetworkAPI.get_endpoint(endp)
            if lagarto_endp is not None:
                lagarto_endp.value = value
                
                status = NetworkAPI.lagarto_client.request_status(lagarto_endp.procname, [lagarto_endp.dumps()])
                if status is not None:
                    if len(status) > 0:
                        if "value" in status[0]:
                            return status[0]["value"]
    
            return None
        except LagartoException as ex:
            ex.log()
            


    def __init__(self, lagarto_client):
        """
        Constructor
        
        @param lagarto_client: lagarto client object
        """
        NetworkAPI.lagarto_client = lagarto_client
        LagartoEndpoint.lagarto_client = lagarto_client


class CloudAPI:
    """
    Static toolbox for clouding tasks
    """

    @staticmethod
    def push_pachube(endp, sharing_key, feed_id, datastream_id):
        """
        Push data to pachube

        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id        
        @param sharing_key: Pachube sharing key
        @param feed_id: Pachube feed ID
        @param datastream_id: Pachube datastream ID
        
        @return HTTP response from Pachube
        """
        try:
            endpoint = NetworkAPI.get_endpoint(endp)
            if endpoint is not None:
                packet = PachubePacket(sharing_key, feed_id, [(datastream_id, endpoint.value)])
                return packet.push()
            return None
        except LagartoException as ex:
            ex.log()


    @staticmethod
    def push_thingspeak(endp, api_key, field_id):
        """
        Push data to ThingSpeak

        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id        
        @param api_key: ThingSpeak API key
        @param field_id: ThingSpeak field ID
        
        @return HTTP response from ThingSpeak
        """
        try:
            endpoint = NetworkAPI.get_endpoint(endp)
            if endpoint is not None:
                packet = ThingSpeakPacket(api_key, [(field_id, endpoint.value)])
                return packet.push()
            return None
        except LagartoException as ex:
            ex.log()


    @staticmethod
    def push_opensense(endp, sense_key, feed_id):
        """
        Push data to open.sen.se

        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id        
        @param sense_key: open.sen.se API key
        @param feed_id: open.sen.se feed ID
        
        @return HTTP response from open.sen.se
        """
        try:
            endpoint = NetworkAPI.get_endpoint(endp)
            if endpoint is not None:
                endpstr = endpoint.procname + "." + endpoint.location + "." + endpoint.name
                if endpstr not in OpenSense.feed_ids:
                    OpenSense.feed_ids[endpstr] = feed_id
                packet = OpenSensePacket(sense_key, [(feed_id, endpoint.value)])
                return packet.push()
            return None
        except LagartoException as ex:
            ex.log()


    @staticmethod
    def push_twitter(endp):
        """
        Push data to open.sen.se

        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id        
        """
        try:
            endpoint = NetworkAPI.get_endpoint(endp)
            if endpoint is not None:
                endpstr = endpoint.procname + "." + endpoint.location + "." + endpoint.name + " = " + str(endpoint.value)
                if endpoint.unit is not None:
                    endpstr += " " + endpoint.unit
                message = TwitterMessage(endpstr)
                return message.send()
            return None
        except LagartoException as ex:
            ex.log()

    @staticmethod
    def push_autoremote(endp, key, message, target, sender, channel, password):
        """
        Push data to AutoRemote

        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id
        @param key: AutoRemote key
        @param message: Formatted message template
        @param target: Target field
        @param sender: Sending device
        @param channel: Act as Sender device channel
        @param password: Device password

        @return HTTP response from AutoRemote
        """
        try:
            endpoint = NetworkAPI.get_endpoint(endp)
            if endpoint is not None:
                populated_message= message.replace("${id}", str(endpoint.id))
                populated_message= populated_message.replace("${location}", str(endpoint.location))
                populated_message= populated_message.replace("${name}", str(endpoint.name))
                populated_message= populated_message.replace("${type}", str(endpoint.type))
                populated_message= populated_message.replace("${direction}", str(endpoint.direction))
                populated_message= populated_message.replace("${value}", str(endpoint.value))
                populated_message= populated_message.replace("${unit}", str(endpoint.unit))
                packet = AutoRemotePacket(key, populated_message, target, sender, channel, password)
                return packet.push()
            return None
        except LagartoException as ex:
            ex.log()

    @staticmethod
    def push_grovestreams(endp, api_key, template_id):
        """
        Push data to GroveStreams

        @param endp: endpoint identification string
        format 1: process.location.name
        format 2: process.id        
        @param api_key: GroveStreams API key
        @param template_id: GroveStreams template component ID
        
        @return HTTP response from GroveStreams
        """
        try:
          endpoint = NetworkAPI.get_endpoint(endp)
          if endpoint is not None:
              endpdata = (endpoint.location + "." + endpoint.name,  str(endpoint.value))
              packet = GroveStreamsPacket(api_key, template_id, [endpdata])
              return packet.push()
          return None
        except LagartoException as ex:
            ex.log()

