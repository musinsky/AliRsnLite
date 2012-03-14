#!/bin/bash

if [ "$2" = "OADB/CMakelibOADB.pkg" ]; then
  MY_SRC_DIR="$1"
  MY_ALIRSNLITE_SRC="$3"
  MY_PWD=`pwd`
  cd $MY_ALIRSNLITE_SRC/OADB
  MY_GROUPS=""
  for MY_DIR in `ls -1 `;do
    if [ -d $MY_DIR ];then
      if [ "$MY_DIR" != "macros" -a "$MY_DIR" != "PROOF-INF.OADB" ];then
        MY_GROUPS="$MY_GROUPS $MY_DIR"
      fi
    fi
  done
  MY_INSTALL_ALL="install (DIRECTORY"
  cd $MY_PWD
  MY_SRC_DIR=${MY_SRC_DIR//\//\\\/}
  for MY_GROUP in $MY_GROUPS;do
    MY_INSTALL_ALL="$MY_INSTALL_ALL $MY_SRC_DIR\/OADB\/$MY_GROUP"
  done
  sed -i "s/^install.*/$MY_INSTALL_ALL/" $1/$2
fi
