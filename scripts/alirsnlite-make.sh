#!/bin/bash

if [ ! -f Makefile ];then
  cmake ../ -DALIRSNLITE_SYNC=YES $USER_CMAKE_EXTRA_OPTIONS
fi

CMD="nice -n 15"

ISPUMP=`which pump > /dev/null 2>&1`
if [ $? -eq 0 ];then
  CMD="$CMD pump"
fi

CMD="$CMD make $*"

$CMD

