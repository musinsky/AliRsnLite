#!/bin/bash

if [ ! -f Makefile ];then
  cmake ../ -DALIRSNLITE_SYNC=YES $USER_CMAKE_EXTRA_OPTIONS
fi

make $*