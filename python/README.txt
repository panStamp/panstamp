-----------------------------------
Python applications for panStamp
-----------------------------------

pyswap : Python module for SWAP networks. This module can be used by
any applications requiring wireless communication with panStamps and other SWAP devices

SWAPdmt-cmd : Command-line utility for configuring, controlling and upgrading SWAP nodes.
https://github.com/panStamp/panstamp/wiki/SWAPdmt-(command-line-version)

SWAPdmt-gui : GUI version of SWAPdmt. Similar as SWAPdmt-cmd but with fewer features.
https://github.com/panStamp/panstamp/wiki/SWAPdmt-(GUI-version)

lagarto : Open Automation Platform with TCP/IP and SWAP connectivities.
https://github.com/panStamp/panstamp/wiki/lagarto

--------------------------
 INSTALLATION GUIDE
--------------------------

If your OS is Debian, Ubuntu, Raspbian or any other Debian-derivative distribution then you
can easily install the Python applications by running "debian-install.sh":

# sudo sh debian-install.sh

Otherwise, you can install the tools manually by following this step-by-step guide:

Initial Requirements:

1. Python 2.7 and Python development files (python-dev)
2. Python "setuptools" (python-setuptools)

Manual installation:

1. Install ZeroMQ on your OS. Download the installer from the web page:

    http://zeromq.org/intro:get-the-software
    
2. Install the Python bindings for ZeroMQ:

    easy_install pyzmq

3. Install pyserial (Python module for serial comms):

    easy_install pyserial
    
4. Install Cherrypy (Web server)

    easy_install cherrypy
    
5. Install Pyephem (Used to calculate sunrise and sunset times)

    easy_install pyephem
    
6. Install pycrypto (AES encryption and others)

    easy_install pycrypto
    
7. Install twitter module

    easy_install twitter
    
8. Install pyswap

    cd pyswap
    python setup.py install

9. If you want to run the graphic version of SWAPdmt, then you will have to install
   wxPython: http://www.wxpython.org/
