#########################################################################
#
# SerialModem
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
__date__ ="$Aug 20, 2011 10:36:00 AM$"
#########################################################################

import time

from SerialPort import SerialPort
from CcPacket import CcPacket
from swap.SwapException import SwapException

class SerialModem:
    """
    Class representing a serial panstamp modem
    """

    class Mode:
        """
        Serial modes
        """
        DATA = 0
        COMMAND = 1


    def stop(self):
        """
        Stop serial gateway
        """
        if self._serport is not None:
            self._serport.stop()


    def _serialPacketReceived(self, buf):
        """
        Serial packet received. This is a callback function called from
        the SerialPort object
        
        @param buf: Serial packet received in String format
        """
        # If modem in command mode
        if self._sermode == SerialModem.Mode.COMMAND:
            self._atresponse = buf
            self.__atresponse_received = True
        # If modem in data mode
        else:
            # Waiting for ready signal from modem?
            if self._wait_modem_start == False:
                if buf == "Modem ready!":
                    self._wait_modem_start = True
            # Create CcPacket from string and notify reception
            elif self._ccpacket_received is not None:
                try:
                    ccPacket = CcPacket(buf)
                    self._ccpacket_received(ccPacket)
                except SwapException:
                    raise


    def setRxCallback(self, cbFunct):
        """
        Set callback reception function. Notify new CcPacket reception
        
        @param cbFunct: Definition of custom Callback function for the reception of packets
        """
        self._ccpacket_received = cbFunct
        

    def goToCommandMode(self):
        """
        Enter command mode (for AT commands)
        
        @return True if the serial gateway does enter Command Mode. Return false otherwise
        """
        if self._sermode == SerialModem.Mode.COMMAND:
            return True
        
        self._sermode = SerialModem.Mode.COMMAND
        response = self.runAtCommand("+++", 5000)

        if response is not None:
            if response[:2] == "OK":
                return True
        
        self._sermode = SerialModem.Mode.DATA
        return False


    def goToDataMode(self):
        """
        Enter data mode (for Rx/Tx operations)
        
        @return True if the serial gateway does enter Data Mode. Return false otherwise
        """
        if self._sermode == SerialModem.Mode.DATA:
            return True
        
        response = self.runAtCommand("ATO\r")
        
        if response is not None:
            if response[0:2] == "OK":
                self._sermode = SerialModem.Mode.DATA;
                return True;
        
        return False;

    
    def reset(self):
        """
        Reset serial gateway
        
        @return True if the serial gateway is successfully restarted
        """
        # Switch to command mode if necessary
        if self._sermode == SerialModem.Mode.DATA:
            self.goToCommandMode()
        # Run AT command
        response = self.runAtCommand("ATZ\r")
        if response is None:
            return False
        
        if response[0:2] == "OK":
            self._sermode = SerialModem.Mode.DATA
            return True
        
        return False


    def runAtCommand(self, cmd="AT\r", timeout=1000):
        """
        Run AT command on the serial gateway
        
        @param cmd: AT command to be run
        @param timeout: Period after which the function should timeout
        
        @return Response received from gateway or None in case of lack of response (timeout)
        """
        self.__atresponse_received = False
        # Send command via serial
        if self._serport is None:
            raise SwapException("Port " + self.portname + " is not open")

        # Skip wireless packets
        self._atresponse = "("
        # Send serial packet
        self._serport.send(cmd)
        
        # Wait for response from modem
        while len(self._atresponse) == 0 or self._atresponse[0] == '(':
            if not self._waitForResponse(timeout):
                return None
        # Return response received from gateway
        return self._atresponse


    def sendCcPacket(self, packet):
        """
        Send wireless CcPacket through the serial gateway
        
        @param packet: CcPacket to be transmitted
        """
        strBuf = packet.toString() + "\r"
        self._serport.send(strBuf)

   
    def setFreqChannel(self, value):
        """
        Set frequency channel for the wireless gateway
        
        @param value: New frequency channel
        """
        # Check format
        if value > 0xFF:
            raise SwapException("Frequency channels must be 1-byte length")
        # Switch to command mode if necessary
        if self._sermode == SerialModem.Mode.DATA:
            self.goToCommandMode()
        # Run AT command
        response =  self.runAtCommand("ATCH=" + "{0:02X}".format(value) + "\r")
        if response is None:
            return False
        if response[0:2] == "OK":
            self.freq_channel = value
            return True
        return False


    def setSyncWord(self, value):
        """
        Set synchronization word for the wireless gateway
        
        @param value: New synchronization word
        """
        # Check format
        if value > 0xFFFF:
            raise SwapException("Synchronization words must be 2-byte length")
        # Switch to command mode if necessary
        if self._sermode == SerialModem.Mode.DATA:
            self.goToCommandMode()
        # Run AT command
        response = self.runAtCommand("ATSW=" + "{0:04X}".format(value) + "\r")
        if response is None:
            return False
        if response[0:2] == "OK":
            self.syncword = value
            return True
        else:
            return False


    def setDevAddress(self, value):
        """
        Set device address for the serial gateway
        
        @param value: New device address
        """
        # Check format
        if value > 0xFF:
            raise SwapException("Device addresses must be 1-byte length")
        # Switch to command mode if necessary
        if self._sermode == SerialModem.Mode.DATA:
            self.goToCommandMode()
        # Run AT command
        response = self.runAtCommand("ATDA=" + "{0:02X}".format(value) + "\r")
        if response is None:
            return False
        if response[0:2] == "OK":
            self.devaddress = value
            return True
        else:
            return False
    
    def _waitForResponse(self, millis):
        """
        Wait a given amount of milliseconds for a response from the serial modem
        
        @param millis: Amount of milliseconds to wait for a response
        """
        loops = millis / 10
        while not self.__atresponse_received:
            time.sleep(0.01)
            loops -= 1
            if loops == 0:
                return False
        return True


    def __init__(self, portname="/dev/ttyUSB0", speed=38400, verbose=False):
        """
        Class constructor
        
        @param portname: Name/path of the serial port
        @param speed: Serial baudrate in bps
        @param verbose: Print out SWAP traffic (True or False)
        """
        # Serial mode (command or data modes)
        self._sermode = SerialModem.Mode.DATA
        # Response to the last AT command sent to the serial modem
        self._atresponse = ""
        # AT response received from modem
        self.__atresponse_received = None
        # "Packet received" callback function. To be defined by the parent object
        self._ccpacket_received = None
        ## Name(path) of the serial port
        self.portname = portname
        ## Speed of the serial port in bps
        self.portspeed = speed
        ## Hardware version of the serial modem
        self.hwversion = None
        ## Firmware version of the serial modem
        self.fwversion = None

        try:
            # Open serial port
            self._serport = SerialPort(self.portname, self.portspeed, verbose)
            # Define callback function for incoming serial packets
            self._serport.setRxCallback(self._serialPacketReceived)
            # Run serial port thread
            self._serport.start()
               
            # This flags switches to True when the serial modem is ready
            self._wait_modem_start = False
            start = time.time()
            soft_reset = False
            while self._wait_modem_start == False:
                elapsed = time.time() - start
                if not soft_reset and elapsed > 5:
                    self.reset()
                    soft_reset = True
                elif soft_reset and elapsed > 10:
                    raise SwapException("Unable to reset serial modem")

            # Retrieve modem settings
            # Switch to command mode
            if not self.goToCommandMode():
                raise SwapException("Modem is unable to enter command mode")
    
            # Hardware version
            response = self.runAtCommand("ATHV?\r")
            if response is None:
                raise SwapException("Unable to retrieve Hardware Version from serial modem")
            self.hwversion = long(response, 16)
    
            # Firmware version
            response = self.runAtCommand("ATFV?\r")
            if response is None:
                raise SwapException("Unable to retrieve Firmware Version from serial modem")
            self.fwversion = long(response, 16)
    
            # Frequency channel
            response = self.runAtCommand("ATCH?\r")
            if response is None:
                raise SwapException("Unable to retrieve Frequency Channel from serial modem")
            ## Frequency channel of the serial gateway
            self.freq_channel = int(response, 16)
    
            # Synchronization word
            response = self.runAtCommand("ATSW?\r")
            if response is None:
                raise SwapException("Unable to retrieve Synchronization Word from serial modem")
            ## Synchronization word of the serial gateway
            self.syncword = int(response, 16)
    
            # Device address
            response = self.runAtCommand("ATDA?\r")
            if response is None:
                raise SwapException("Unable to retrieve Device Address from serial modem")
            ## Device address of the serial gateway
            self.devaddress = int(response, 16)
    
            # Switch to data mode
            self.goToDataMode()
        except:
            raise
