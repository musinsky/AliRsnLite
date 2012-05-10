#!/bin/bash

ALIRSNLITE_SRC_DIR="$(dirname $(dirname $(readlink -m $0)))"

DIRS_REMOVE="ANALYSIS CORRFW OADB PWGLF STEER lib include pars"

for mydir in $DIRS_REMOVE;do 
  echo "Cleaning $ALIRSNLITE_SRC_DIR/$mydir ..."
  rm -Rf $ALIRSNLITE_SRC_DIR/$mydir
done

if [ -d $ALIRSNLITE_SRC_DIR/build ];then
  rm -Rf $ALIRSNLITE_SRC_DIR/build/*
fi