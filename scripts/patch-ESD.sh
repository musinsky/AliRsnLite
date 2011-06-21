#!/bin/bash

cp $2/patches/ESD/AliESDHeader.patch $2/STEER/
MYPWD=`pwd`
cd $2/STEER/
patch -p1 -i AliESDHeader.patch
rm -f AliESDHeader.patch
cd $MYPWD
