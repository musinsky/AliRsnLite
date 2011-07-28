#!/bin/bash

sed -i 's/ALICE_CheckModule()/# ALICE_CheckModule()/g' $1/cmake/cmake_AliRoot/ALICE_CMake.cmake
