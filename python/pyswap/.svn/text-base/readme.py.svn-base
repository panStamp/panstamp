## @mainpage Python SWAP library
# @section intro_sec Introduction
# pyswap is a Python library made to communicate with SWAP networks through a serial panStamp. This library is the core of every
# application written for computers in the scope of the panStamp project; SWAPdmt and the SWAP plugin for HouseAgent? are indeed
# based on this library.
#
# pyswap is more than a communication library in fact. It takes care of discovering motes, registering resources in memory and
# maintaining configuration files. You'll find that this library does the most part of whatever application you may need for your
# computer. At the end, you'll need to subscribe to some callback functions that will alert your application about any event in
# the network.
#
# @section descr_sec Description
# In order to use pyswap you should follow these steps:
#
# - Download this library and add it to your PYTHONPATH variable.
# - Copy settings.xml, serial.xml and network.xml into your application source folder and change their contents according to
#  your settings (serial port, Network Id, gateway address, etc).
# - Download "devices" from our repositories and place it wherever you want in your computer. Then set device to the correct
#  location in settings.xml. Next time you run your application, it will be able to find static descriptions about any device
#  in your SWAP network.
# - Write your application 
#
# @section interf_sec Interface
# pyswap provides an event-based interface that frees your application from having to store devices, registers or parameters by
# itself. This interface just notifies your application about any event occurred in the SWAP network, including value changes and
# detection of new motes and their endpoints. In order to use this interface, you must create a Python module containing a class
# deriving from SwapInterface?. Then, according to your needs, you'll end by defining different callback methods that will notify
# your application about the occurrence of certain events.
#
# Visit our wiki for more information: @link http://code.google.com/p/panstamp/wiki/pyswap

