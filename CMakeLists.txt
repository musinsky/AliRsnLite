# -*- mode: cmake -*-

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

#--------------------------------------------------------------------------------#
# Set Basic CMake Configuration                                                  #
#--------------------------------------------------------------------------------#

project(AliRsnLite CXX C Fortran)

get_filename_component(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}" REALPATH)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})


include(ALICE_CMake)

# ROOT
# ------------------------------
find_package(ROOT REQUIRED)

# IRST Rule Checker 
# ------------------------------

find_package(RuleChecker)



#--------------------------------------------------------------------------------#
# Environment/Compiler Configuration                                             #
#--------------------------------------------------------------------------------#

ALICE_CheckOutOfSourceBuild()
ALICE_RootConfig(CMAKE_CXX_COMPILER --cxx)
ALICE_RootConfig(CMAKE_C_COMPILER --cc)
ALICE_RootConfig(CMAKE_Fortran_COMPILER --f77)
ALICE_ConfigurePlatform()
ALICE_ConfigureCompiler()

ALICE_RootConfig(CMAKE_ALICE_TARGET --arch)


#--------------------------------------------------------------------------------#
# AliRoot Path Settings                                                          #
#--------------------------------------------------------------------------------#

ALICE_DevFlagsOutput()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/tgt_${CMAKE_ALICE_TARGET}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/tgt_${CMAKE_ALICE_TARGET}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/tgt_${CMAKE_ALICE_TARGET}")
set(CMAKE_INCLUDE_EXPORT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
file(MAKE_DIRECTORY ${CMAKE_INCLUDE_EXPORT_DIRECTORY})
set(EXPORTDIRS "${ALICE_INSTALL}/include")
set(BINLIBDIRS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")



#--------------------------------------------------------------------------------#
# AliRoot Modules                                                                #
#--------------------------------------------------------------------------------#

# Standard Modules
# ------------------------------
set(ALIROOTMODULES STEER ANALYSIS OADB PWG2)
#  PWG2resonances
# Set Final list of modules 
set(MODULES ${ALIROOTMODULES} )

# Set Module directories
set(MODDIRS ${MODULES})

# Libraries for Linking
# ------------------------------
# set(ALILIBS STEERBase ESD AOD)
set(LIBS ${ROOTCLIBS} ${ROOTPLIBS} ${SYSLIBS})

#--------------------------------------------------------------------------------#
# Build Modules                                                                  #
#--------------------------------------------------------------------------------#

# Module Variables
# ------------------------------
set(ALLLIBS)
set(ALLEXECS)
set(INCLUDEFILES)
set(BINLIBS)
set(EXPORTFILES)
set(DEPLIB)

add_custom_target(alilibs-static)
add_custom_target(include-headers ALL)
# add_custom_target(DA-all)
# add_custom_target(check-all) 
# add_custom_target(check-hxml)
# add_custom_target(smell-all) 
# add_custom_target(htmldoc) 
add_custom_target(par-all)
add_custom_target(test-par-all)


# Add Modules	
# ------------------------------
foreach(MODULE ${MODDIRS})
  set(${MODULE}ALIBS)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Module.tmp ${PROJECT_SOURCE_DIR}/${MODULE}/CMakeLists.txt @ONLY)
  add_custom_target(${MODULE}-all ALL)
  add_custom_target(${MODULE}-par-all)
  add_custom_target(test-${MODULE}-par-all)
  add_custom_target(${MODULE}-all-static)
  add_dependencies(alilibs-static "${MODULE}-static")
  add_subdirectory(${MODULE})
endforeach(MODULE)

add_dependencies(PWG2resonances-headers "STEERBase-headers ESD-headers AOD-headers ANALYSIS-headers OADB-headers ANALYSISalice-headers")
add_dependencies(ANALYSISalice-headers "STEERBase-headers ESD-headers AOD-headers ANALYSIS-headers OADB-headers")
add_dependencies(OADB-headers "STEERBase-headers ESD-headers AOD-headers ANALYSIS-headers")
add_dependencies(ANALYSIS-headers "STEERBase-headers ESD-headers AOD-headers")
add_dependencies(AOD-headers "STEERBase-headers")
add_dependencies(ESD-headers "STEERBase-headers")

include (InstallRequiredSystemLibraries)
# include (CPack)

