#!/bin/bash

clear

echo "------------------------------------"
echo " INSTALL PYSWAP"
echo "------------------------------------"
cd pyswap
python setup.py install
cd ..

echo "------------------------------------"
echo " UPDATE APT SOURCES"
echo "------------------------------------"
apt-get update

echo ""
echo "------------------------------------"
echo " INSTALL PYTHON-DEV"
echo "------------------------------------"
apt-get --yes --force-yes install python-dev

echo ""
echo "------------------------------------"
echo " INSTALL PYTHON-DEV"
echo "------------------------------------"
apt-get --yes --force-yes install python-setuptools

echo ""
echo "------------------------------------"
echo " INSTALL LIBZMQ-DEV"
echo "------------------------------------"
apt-get --yes --force-yes install libzmq-dev

echo ""
echo "------------------------------------"
echo " INSTALL PYZMQ"
echo "------------------------------------"
easy_install pyzmq

echo ""
echo "------------------------------------"
echo " INSTALL PYSERIAL"
echo "------------------------------------"
easy_install pyserial

echo ""
echo "------------------------------------"
echo " INSTALL CHERRYPY"
echo "------------------------------------"
easy_install cherrypy

echo ""
echo "------------------------------------"
echo " INSTALL PYEPHEM"
echo "------------------------------------"
easy_install pyephem

echo ""
echo "------------------------------------"
echo " INSTALL PYCRYPTO"
echo "------------------------------------"
easy_install pycrypto

echo ""
echo "------------------------------------"
echo " INSTALL TWITTER MODULE FOR PYTHON"
echo "------------------------------------"
easy_install twitter
