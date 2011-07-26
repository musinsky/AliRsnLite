cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

macro(AliRsnLite_Sync)

  set(ALIRSNLITE_PARS STEER/CMakelibSTEERBase.pkg
                      STEER/CMakelibESD.pkg
                      STEER/CMakelibAOD.pkg
                      ANALYSIS/CMakelibANALYSIS.pkg
                      OADB/CMakelibOADB.pkg
                      ANALYSIS/CMakelibANALYSISalice.pkg
                      ANALYSIS/CMakelibEventMixing.pkg
                      CORRFW/CMakelibCORRFW.pkg
                      PWG2/CMakelibPWG2resonances.pkg)

  set(ALIRSNLITE_DIRS_RM STEER ANALYSIS)

  if(ALIRSNLITE_SYNC STREQUAL "YES")

    # fix for alien classes to be included in project
    set(ROOTHASXML "yes")

    # check for ALIRSNLITE_TAG
    if(ALIRSNLITE_TAG)
      message(STATUS "ALICE_TAG is ${ALIRSNLITE_TAG}")
      set(ALIRSNLITE_SRC "http://alisoft.cern.ch/AliRoot/tags/${ALIRSNLITE_TAG}")
    else(ALIRSNLITE_TAG)
      set(ALIRSNLITE_SRC "$ENV{ALICE_ROOT}")
    endif(ALIRSNLITE_TAG)

    # append aliroot cmake dir
    file(COPY ${ALIRSNLITE_SRC}/cmake/ DESTINATION ${CMAKE_SOURCE_DIR}/cmake PATTERN *.cmake)
    file(COPY ${ALIRSNLITE_SRC}/cmake/ DESTINATION ${CMAKE_SOURCE_DIR}/cmake PATTERN *.tmp)
    file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/cmake/.svn)

    foreach(myrmdir ${ALIRSNLITE_DIRS_RM})
      file(REMOVE_RECURSE ${myrmdir})
    endforeach(myrmdir ${ALIRSNLITE_DIRS_RM})

    foreach(file ${ALIRSNLITE_PARS})
      if(EXISTS ${ALIRSNLITE_SRC}/${file})
        get_filename_component(srcdir ${file} PATH)
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${srcdir})
        file(COPY ${ALIRSNLITE_SRC}/${file} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
# 	if(file STREQUAL "OADB/CMakelibOADB.pkg")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/fixAliRootInstall.sh ${CMAKE_SOURCE_DIR}/${file} ${file})
# 	endif(file STREQUAL "OADB/CMakelibOADB.pkg")
        include(${CMAKE_SOURCE_DIR}/${file})
        foreach(myfile ${SRCS})
          get_filename_component(myfiledir ${myfile} PATH)
          file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${myfile} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir}/${myfiledir})
        endforeach(myfile ${SRCS})

        string (REPLACE ".cxx" ".h" HDRS "${SRCS}")
        foreach(myfile ${HDRS})
          get_filename_component(myfiledir ${myfile} PATH)
          file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${myfile} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir}/${myfiledir})
        endforeach(myfile ${HDRS})

        # take PROOF-INF
        string (REPLACE "CMakelib" "" PAR "${file}")
        string (REPLACE ".pkg" "" PAR "${PAR}")
        string (REPLACE "${srcdir}/" "" PAR "${PAR}")
        file(COPY ${ALIRSNLITE_SRC}/${srcdir}/PROOF-INF.${PAR} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir} PATTERN .svn EXCLUDE)
        file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${PAR}LinkDef.h DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
        file(COPY ${ALIRSNLITE_SRC}/${srcdir}/Makefile DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
        execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/scripts/patch-${PAR}.sh ${PAR} ${CMAKE_SOURCE_DIR})
      else(EXISTS ${ALIRSNLITE_SRC}/${file})
        message(FATAL_ERROR "${ALIRSNLITE_SRC}/${file} doesn't exists !!!")
      endif(EXISTS ${ALIRSNLITE_SRC}/${file})
    endforeach(file ${ALIRSNLITE_PARS})
    message(STATUS "ALIRSNLITE_SRC is ${ALIRSNLITE_SRC}")
    set(SRCS)
    set(HDRS)
    set(ROOTHASXML)

    execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/scripts/post-sync.sh ${CMAKE_SOURCE_DIR})

  else(ALIRSNLITE_SYNC STREQUAL "YES")
    if(EXISTS ${CMAKE_SOURCE_DIR}/STEER)
      message(STATUS "Sync is skipped")
    else(EXISTS ${CMAKE_SOURCE_DIR}/STEER)
      message(FATAL_ERROR "  cmake -DALIRSNLITE_SYNC=YES  ../")
    endif(EXISTS ${CMAKE_SOURCE_DIR}/STEER)
    
  endif(ALIRSNLITE_SYNC STREQUAL "YES")


endmacro(AliRsnLite_Sync)
