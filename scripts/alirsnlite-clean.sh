#!/bin/bash

ALIRSNLITE_SRC_DIR=""

function SetAliRsnLiteSrcDir() {
  if [ "${0:0:1}" == "." ];then
    ALIRSNLITE_SRC_DIR="`cd ../;pwd`"
  elif [ "${0:0:1}" == "/" ];then
    ALIRSNLITE_SRC_DIR="`cd $(dirname $0)/../;pwd`"
  else
    ALIRSNLITE_SRC_DIR="`cd $(dirname ${PWD}/$0)/../;pwd`"
  fi
}

SetAliRsnLiteSrcDir

DIRS_REMOVE="ANALYSIS CORRFW OADB PWG2 STEER lib include"

for mydir in $DIRS_REMOVE;do 
  echo "Cleaning $ALIRSNLITE_SRC_DIR/$mydir ..."
  rm -Rf $ALIRSNLITE_SRC_DIR/$mydir
done
