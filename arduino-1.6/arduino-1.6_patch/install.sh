#!/bin/bash
# Install panStamp files for Arduino-1.5

ARDUINO_DIR=..

printf "Installing patch...\n"

cp -R libraries $ARDUINO_DIR/
cp -R hardware $ARDUINO_DIR/
cp -R app $ARDUINO_DIR/
cp -R lin/platform.txt $ARDUINO_DIR/hardware/panstamp/msp430/

if [ "$(uname)" == "Darwin" ]; then
  printf "MacOS detected\n"
  cp -R mac/tools/msp430 $ARDUINO_DIR/hardware/tools/
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
  printf "Linux detected\n"
  cp -R lin/tools/msp430 $ARDUINO_DIR/hardware/tools/
else
  printf "OS not supported by the installer\n"
fi

printf "Installation completed\n"

