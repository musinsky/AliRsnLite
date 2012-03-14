#!/bin/bash

MY_GROUPS=""
MY_SRC_DIR="$3"
MY_ALIRSNLITE_SRC="$2"
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

cd $MY_PWD

for MYDIR in $MY_GROUPS;do
  cp -r $2/$1/$MYDIR $3/$1/
done
