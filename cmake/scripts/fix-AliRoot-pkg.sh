#!/bin/bash

if [ "$2" = "OADB/CMakelibOADB.pkg" ]; then
    sed -i 's/COMMON PWG1 PWG2 PWG3 PWG4/${ALIRSNLITE_SRC}\/OADB\/COMMON ${ALIRSNLITE_SRC}\/OADB\/PWG1 ${ALIRSNLITE_SRC}\/OADB\/PWG2 ${ALIRSNLITE_SRC}\/OADB\/PWG3 ${ALIRSNLITE_SRC}\/OADB\/PWG4/' $1
fi
