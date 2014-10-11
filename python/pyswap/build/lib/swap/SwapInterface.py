#########################################################################
#
# SwapInterface
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
__date__  ="Sep 28, 2011 1:09:12 PM$"
__version__ = "0.1.3"
#########################################################################

from SwapServer import SwapServer


class SwapInterface:
    """
    SWAP Interface superclass. Any SWAP application should derive from this one
    """
    def swapServerStarted(self):
        """
        SWAP server started successfully
        """
        pass


    def swapPacketReceived(self, packet):
        """
        New SWAP packet received
        
        @param packet: SWAP packet received
        """
        pass
    

    def swapPacketSent(self, packet):
        """
        SWAP packet transmitted
        
        @param packet: SWAP packet transmitted
        """
        pass
    

    def newMoteDetected(self, mote):
        """
        New mote detected by SWAP server
        
        @param mote: mote detected
        """
        pass


    def newParameterDetected(self, parameter):
        """
        New configuration parameter detected by SWAP server
        
        @param parameter: Endpoint detected
        """
        pass
    
    
    def newEndpointDetected(self, endpoint):
        """
        New endpoint detected by SWAP server
        
        @param endpoint: Endpoint detected
        """
        pass


    def moteStateChanged(self, mote):
        """
        Mote state changed
        
        @param mote: Mote having changed
        """
        pass
 

    def moteAddressChanged(self, mote):
        """
        Mote address changed
        
        @param mote: Mote having changed
        """
        pass


    def registerValueChanged(self, register):
        """
        Register value changed
        
        @param register: Register having changed
        """
        pass
    
    
    def endpointValueChanged(self, endpoint):
        """
        Endpoint value changed
        
        @param endpoint: Endpoint having changed
        """
        pass
    
    
    def parameterValueChanged(self, parameter):
        """
        Configuration parameter changed
        
        @param parameter: configuration parameter having changed
        """
        pass


    def getNbOfMotes(self):
        """
        @return the amount of motes available in lstMotes
        """
        return self.network.get_nbof_motes()


    def getMote(self, index=None, address=None):
        """
        Return mote from list
        
        @param index: Index of the mote within lstMotes
        @param address: SWAP address of the mote
        
        @return mote
        """
        return self.server.network.get_mote(index, address)


    def setMoteRegister(self, mote, regId, value):
        """
        Set new register value on wireless mote
        
        @param mote: Mote targeted by this command
        @param regId: Register ID
        @param value: New register value
        
        @return True if the command is correctly ack'ed. Return False otherwise
        """
        return self.server.setMoteRegister(mote, regId, value)


    def queryMoteRegister(self, mote, regId):
        """
        Query mote register, wait for response and return value
        Non re-entrant method!!
        
        @param mote: Mote to be queried
        @param regID: Register ID
        
        @return register value
        """
        return self.server.queryMoteRegister(mote, regId)


    def create_server(self):
        """
        Create server object
        """
        self.server = SwapServer(self, self.verbose)
        return self.server
        

    def start_server(self):
        """
        Start SWAP server
        """
        self.server.start()
        

    def stop(self):
        """
        Stop SWAP server
        """
        self.server.stop()


    def get_endpoint(self, endpid=None, location=None, name=None):
        """
        Get endpoint given its unique id or location.name pair
        
        @param endpid endpoint id
        @param location endpoint location
        @param name endpoint name
        
        @return endpoint object
        """    
        for mote in self.network.motes:
            for register in mote.regular_registers:
                for endpoint in register.parameters:
                    if endpid is not None:
                        if endpid == endpoint.id:
                            return endpoint
                    elif name == endpoint.name and location == endpoint.location:
                        return endpoint
        return None


    def update_definition_files(self):
        """
        Update Device Definition Files from Internet server
        """
        self.server.update_definition_files()


    def __init__(self, settings=None, start=True):
        """
        Class constructor

        @param settings: path to the main configuration file
        @param verbose: Print out SWAP frames
        @param start: Start SWAP server if True
        """
        ## SWAP server
        self.server = None
        ## Network data
        self.network = None
                       
        if start:
            print "SWAP server starting... "
        self.server = SwapServer(self, settings, start)
        self.network = self.server.network
        if start:
            print "SWAP server is now running... "
