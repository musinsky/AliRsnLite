#!/bin/bash

MYDIRS="COMMON PWGPP PWG2 PWG3 PWG4"

for MYDIR in $MYDIRS;do
  cp -r $2/$1/$MYDIR $3/$1/
done
