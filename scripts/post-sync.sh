#!/bin/bash
# MYPWD=`pwd`
# cd $1
find $1 -name ".svn" -exec rm -Rf {} \; > /dev/null 2>&1
find $1 -name "*~" -exec rm -Rf {} \; > /dev/null 2>&1
# cd $PWD