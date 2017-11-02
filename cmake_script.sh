#!/bin/bash
#
# A simple helper script to run the proper cmake for G4Emma

GEANT4_DIR=~/GEANT4/geant4.9.6-install/lib/Geant4-9.6.4/

cmake -DGeant4_DIR=$GEANT4_DIR .

#Assumes you have a quad core processor
make -j4

