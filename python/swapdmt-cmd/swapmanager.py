#########################################################################
#
# SwapManager
#
# Copyright (c) 2014 Daniel Berenguer <dberenguer@usapiens.com>
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
# along with panStamp and swapdmt; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Jun 4, 2014$"
#########################################################################

import os
import sys

from swap.SwapException import SwapException
from swap.SwapInterface import SwapInterface
from swap.protocol.SwapDefs import SwapAddress, SwapFunction, SwapRegId, SwapState
from swap.protocol.SwapPacket import SwapStatusPacket
from swap.protocol.SwapValue import SwapValue
from progressbar import AnimatedProgressBar
from macro import Macro

from hexfile import HexFile

working_dir = os.path.dirname(__file__)
firmware_dir = os.path.join(working_dir, "firmware")
sys.path.append(firmware_dir) 


class SwapManager(SwapInterface):
    """
    SWAP Management Class
    """
    """
    Product code of firmware server
    """

    ## Version number
    VERSION_NUMBER = "0.4"
    
    ## Product code of Firmware server
    FIRMSERVER_PRODUCT_CODE = [0, 0, 0, 1, 0, 0, 0, 16]

    def swapServerError(self, ex):
        """
        SWAP server error detected
        
        @param ex exception
        """
        ex.show()
        self.stop()
        print "Quitting application"
        sys.exit(0)
    
    
    def swapServerStarted(self):
        """
        SWAP server started successfully
        """               
        self.server_started = True

    
    def swapPacketReceived(self, packet):
        """
        New SWAP packet received
        
        @param packet: SWAP packet received
        """
        if packet.destAddress in [SwapAddress.BROADCAST_ADDR, self.server.devaddress]:
            # Packet broadcasted or explicitely addressed to our server
            if packet.function == SwapFunction.QUERY:
                # SWAP query received
                if packet.destAddress == self.server.devaddress:
                    # Local register being queried
                    if packet.regId == SwapRegId.ID_PRODUCT_CODE:
                        # Server's product code
                        self.transmit_product_code()
                    elif packet.regId == SwapRegId.ID_FIRMWARE:
                        # Query comming from our node in programming mode?
                        if self.prog_address == packet.srcAddress:
                            # Firmware image being queried
                            if packet.value.getLength() == 2:
                                # The query contains 2 bytes of data
                                # Take line number
                                nb_line = (packet.value.toList()[0] << 8) | packet.value.toList()[1]                                                                        
                                line = self.hexfile.get_line(nb_line)
                                
                                if line is not None:
                                    prefix = packet.value.toList()
                                    payload = prefix + line.data
                                    val = SwapValue(payload)
                                    page_packet = SwapStatusPacket(self.server.devaddress, SwapRegId.ID_FIRMWARE, val)
                                    page_packet.send(self.server)
    
                                if nb_line == 0:
                                    self.hexfile_line = 0
                                    print "Erasing flash memory"
                                    self.progress + 1                           
                                elif nb_line == 1:
                                    print "Uploading code to node"
                                elif 1 < nb_line < (self.hexfile.nbof_data_lines - 1):                               
                                    if (nb_line != self.hexfile_line):
                                        self.hexfile_line = nb_line
                                        self.progress + 1
                                        self.progress.show_progress()
                                elif nb_line == (self.hexfile.nbof_data_lines):
                                    self.progress + 2
                                    self.progress.show_progress()
                                    print "\n\rUpload completed"
                                    self.prog_address = None
                                    self.hexfile_line = 0
                                

    def newMoteDetected(self, mote):
        """
        New mote detected by SWAP server
        
        @param mote: Mote detected
        """
        if self.verbose == True:
            print "New node with address " + hex(mote.address) + " : " + mote.definition.product + \
            " (by " + mote.definition.manufacturer + ")"
            
            
    def stop(self):
        """
        Stop SWAP manager
        """
        # Stop SWAP server
        self.server.stop()

    
    def transmit_product_code(self):
        """
        Transmit SWAP status packet containing the product code
        """
        pcode = SwapValue(self.FIRMSERVER_PRODUCT_CODE)
        status = SwapStatusPacket(self.server.devaddress, SwapRegId.ID_PRODUCT_CODE, pcode)
        status.send(self.server)

    
    @staticmethod
    def str_to_int(str_int):
        """
        Convert string to integer
        
        @param str Integer in string format
        """
        try:
            val = int(str_int)
        except ValueError:
            if str_int[:2] == "0x":
                try:
                    val = int(str_int[2:], 16)
                except ValueError:
                    print str_int + " is not a correct value"
                    return None
                    
        return val
        

    def dispatch_user_command(self, command):
        """
        Apply user command
        
        @param command : user command
        """
        if len(command) == 2 and command[0] == "macro":
            self.dispatch_macro(command[1])
        else:
            self.dispatch_command(command)
            
            
    def dispatch_macro(self, path):
        """
        Execute custom macro
        
        @path to the custom macro file
        """
        macro = Macro(path)

        for cmd in macro.commands:
            self.dispatch_command(cmd)
        
        
    def dispatch_command(self, command):
        """
        Apply user command
        
        @param command : user command
        """
        if len(command) == 1:
            if command[0] == "help":
                self.print_help()
            elif command[0] == "version":
                print self.VERSION_NUMBER
            elif command[0] == "quit":
                self.stop()
                sys.exit(0)
        elif len(command) < 2:
            print "Incorrect command. Run \"help\" to get more information"
        # Print SWAP traffic
        elif command[0] == "traffic":
            if command[1].lower() == "on":
                self.verbose = True
                self.server.verbose = True
                self.server.modem.verbose = True
                self.server.modem._serport._verbose = True                
            elif command[1].lower() == "off":
                self.verbose = False
                self.server.verbose = False
                self.server.modem.verbose = False
                self.server.modem._serport._verbose = False
            else:
                print command[1] + " is not a correct value"
        # Set HEX file
        elif command[0] == "hexfile":
            # HEX file object
            self.hexfile = HexFile(command[1])
        # Print list of nodes
        elif command[0] == "list":
            if command[1] == "nodes":
                if len(self.network.motes) == 0:
                    print "No nodes detected"
                else:
                    for mote in self.network.motes:
                        print "Addr: " + hex(mote.address) + " - " + mote.definition.product
        # Clear list list of nodes
        elif command[0] == "clear":
            if command[1] == "nodes":
                self.server.network.clear()
                self.server.network.save()
        # Change device device property
        elif command[0] == "node":
            if len(command) < 3:
                print "Insufficient arguments"
                print "Correct format is: node <address> <options...>"                
            else:
                addr = self.str_to_int(command[1])
                if addr is None:
                    print "Incorrect address format"
                else:                    
                    # Get mote object
                    mote = self.server.network.get_mote(address=addr)
                    
                    if mote is None:
                        print "Node not found in data base"
                        print "Clear your list of nodes and restart them again"
                    else:
                        # Program node
                        if command[2] == "program":
                            # Create progress bar    
                            self.progress = AnimatedProgressBar(end=self.hexfile.nbof_data_lines, width=50)
                            # Save address of node being programmed
                            self.prog_address = addr
                            # Transmit product code
                            self.transmit_product_code()
                            
                            # Put node in upgrade mode
                            val = SwapValue(SwapState.UPGRADE, 1)
                            if mote.cmdRegisterWack(SwapRegId.ID_SYSTEM_STATE, val):
                                print "Node now in programming mode"
                            elif self.hexfile_line == 0:
                                print "Unable to put node in progamming mode"
                                self.prog_address = None
                        # Restart node (if not sleeping)
                        elif command[2] == "restart":
                            if mote.restart():
                                print "Node restarting"
                            else:
                                print "Got no response from node. It's probably sleeping"
                        # Show details of device
                        elif command[2] == "details":
                            print "SWAP address : " + hex(mote.address)
                            print "Developer : " + mote.definition.manufacturer
                            print "Product name : " + mote.definition.product
                            if mote.config_registers is not None:
                                print "Config registers :"
                                for reg in mote.config_registers:
                                    print "Register ID : " + hex(reg.id)
                                    print "Register name : " + reg.name
                                    print "Register value : 0x" + reg.value.toAsciiHex()
                                    for param in reg.parameters:
                                        print "  Parameter name : " + param.name
                                        print "  Parameter value : 0x" + param.value.toAsciiHex()
                            if mote.regular_registers is not None:
                                print "Regular registers :"
                                for reg in mote.regular_registers:
                                    print "Register ID : " + hex(reg.id)
                                    print "Register name : " + reg.name
                                    print "Register value : 0x" + reg.value.toAsciiHex()
                                    for endp in reg.parameters:
                                        print "  Endpoint name : " + endp.name
                                        print "  Endpoint value : 0x" + endp.value.toAsciiHex()                      
                        # Change device address
                        elif command[2] == "address":
                            if len(command) == 3:
                                self.print_response(mote.qryRegisterWack(SwapRegId.ID_DEVICE_ADDR), command[2])
                            elif len(command) > 4:
                                self.print_format_error(command[2])
                            else:
                                new_addr = self.str_to_int(command[3])                        
                                if new_addr is not None:
                                    self.print_confirmation(mote.setAddress(new_addr), command[2], command[3])
                        # Change Tx interval
                        elif command[2] == "txinterval":
                            if len(command) == 3:
                                self.print_response( mote.qryRegisterWack(SwapRegId.ID_TX_INTERVAL), command[2])
                            elif len(command) > 4:
                                self.print_format_error(command[2])                            
                            else:
                                new_interval = self.str_to_int(command[3])
                                if new_interval is not None:
                                    self.print_confirmation(mote.setTxInterval(new_interval), command[2], command[3])                   
                        # Change network id
                        elif command[2] == "netid":
                            if len(command) == 3:
                                self.print_response(mote.qryRegisterWack(SwapRegId.ID_NETWORK_ID), "network ID")
                            elif len(command) > 4:
                                self.print_format_error(command[2])
                            else:
                                new_netid = self.str_to_int(command[3])
                                if new_netid is not None:
                                    self.print_confirmation(mote.setNetworkId(new_netid), command[2], command[3])                                  
                        # Change frequency channel
                        elif command[2] == "channel":
                            if len(command) == 3:
                                self.print_response(mote.qryRegisterWack(SwapRegId.ID_FREQ_CHANNEL), command[2])
                            elif len(command) > 4:
                                self.print_format_error(command[2])
                            else:
                                new_channel = self.str_to_int(command[3])
                                if new_channel is not None:
                                    self.print_confirmation(mote.setFreqChannel(new_channel), command[2], command[3])                        
                        # Read/write register
                        elif command[2] == "reg":                               
                            if len(command) > 3:
                                reg_id = self.str_to_int(command[3])
                                if reg_id is not None:
                                    if len(command) == 4:
                                        self.print_response(mote.qryRegisterWack(reg_id), "register value")
                                    elif len(command) == 5:
                                        reg = mote.getRegister(reg_id)
                                        val = SwapValue(command[4], reg.value.getLength())
                                        self.print_confirmation(mote.cmdRegisterWack(reg_id, val), "register value", command[4])
                                    else:
                                        print "Too many arguments"
                                        print "Correct format is: node <address> reg <reg_id> [<reg_val>]"
                            else:
                                print "Insufficient arguments"
                                print "Correct format is: node <address> reg <reg_id> [<reg_val>]"
                        else:
                            print "Command not supported"
        else:
            print "Command not supported"
                                       

    def print_format_error(self, command):
        """
        Print format error message
        
        @param command Command executed in string format
        """
        print "Too many arguments"
        print "Correct format is: node <address> " + command + " <new_" + command + ">"
                            

    def print_confirmation(self, success, command, strval=None):
        """
        Print confirmation or or lack of response message, depending on the parameters
        passed as arguments, after having sent a command to a node
        
        @param success True for positive message. False for negative
        @param command Command executed in string format
        @param strval Value received fom node in string format
        """
        if success:
            print "Got confirmation from node. New " + command + " is " + strval
        else:
            print "No confirmation received from node. " + command + " probably did not change"
    

    def print_response(self, regval, command):
        """
        Print response or or lack of response message, depending on the parameters
        passed as arguments, after having sent a query to a node
        
        @param regval Register value returned by node
        @param command Command executed in string format
        """
        if regval is not None:
            print "Current " + command + " is " + str(regval.toInteger()) + " (" + hex(regval.toInteger()) + ")"
        else:
            print "No response received from node"
            

    def print_help(self):
        """
        Print help
        """
        print "SWAPdmt commands and syntax:"
        print ""
        print "help                         Print help"
        print "version                      Print version number"
        print "quit                         Quit application"
        print "traffic <on|off>             Print or hide SWAP traffic"
        print "macro <path to macro file>   Run commands listed in macro file"
        print "hexfile <path to hex file>   Enter hex file for SWAP firmware upgrade"
        print "list nodes                   Print list of nodes detected in the SWAP network"
        print "clear nodes                  Clear list of nodes detected in the SWAP network"
        print ""
        print "node <addr> channel [<new_channel>]          Print or set RF channel of node"
        print "node <addr> netid [<new_netid>]              Print or set SWAP network ID of node"
        print "node <addr> address [<new_addr>]             Print or set SWAP address of node"
        print "node <addr> txinterval [<new_interval>]      Print or set Tx interval of node"
        print "node <addr> reg <reg_id> [<new_reg_value>]   Print or set register value"
        print "node <addr> details                          Print product details of node"
        print "node <addr> restart                          Restart node (if it's not sleeping)"
        print "node <addr> program                          Program node wirelessly with hex file"
        print ""
        

    def __init__(self, swap_settings=None):
        """
        Class constructor

        @param file_name: name of the HEX file        
        @param swap_settings: path to the main SWAP configuration file
        """             
        # Main configuration file
        self.swap_settings = swap_settings
        # Print SWAP activity
        self._print_swap = False
        # Server started flag
        self.server_started = False

        try:
            # Superclass call
            SwapInterface.__init__(self, swap_settings)
        except SwapException as ex:
            ex.display()
        except:
            pass

        # Disable verbose
        self.server.verbose = False
        self.verbose = False
                       
        # Firmware image
        self.hexfile = None        
        # Progress bar
        self.progress = None
        # Number of line being queried
        self.hexfile_line = 0
        # Address of node being programmed
        self.prog_address = None
