# AliRsnLite build system utility macro and fixes

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

macro(AliRsnLite_Sync)

  set(ALIRSNLITE_PARS
    STEER/CMakelibSTEERBase.pkg
    STEER/CMakelibESD.pkg
    STEER/CMakelibAOD.pkg
    ANALYSIS/CMakelibANALYSIS.pkg
    OADB/CMakelibOADB.pkg
    ANALYSIS/CMakelibANALYSISalice.pkg
    ANALYSIS/CMakelibEventMixing.pkg
    CORRFW/CMakelibCORRFW.pkg
    PWGLF/CMakelibPWGLFresonances.pkg)

  set(ALIRSNLITE_DIRS_RM STEER ANALYSIS)

  if(ALIRSNLITE_SYNC STREQUAL "YES")

    # fix for alien classes to be included in project
    set(ROOTHASXML "yes")
    # set sources of AliRoot
    set(ALIRSNLITE_SRC "$ENV{ALICE_ROOT}")
    message(STATUS "ALIRSNLITE_SRC is ${ALIRSNLITE_SRC}")

    # append AliRoot cmake dir
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake/cmake_AliRoot/)
      file(COPY ${ALIRSNLITE_SRC}/cmake/ DESTINATION ${CMAKE_SOURCE_DIR}/cmake/cmake_AliRoot/ PATTERN *.cmake)
      file(COPY ${ALIRSNLITE_SRC}/cmake/ DESTINATION ${CMAKE_SOURCE_DIR}/cmake/cmake_AliRoot/ PATTERN *.tmp)
      file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/cmake/cmake_AliRoot/.svn)
    endif(NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake/cmake_AliRoot/)

    foreach(file ${ALIRSNLITE_PARS})
      get_filename_component(srcdir ${file} PATH)
      string (REPLACE "CMakelib" "" PAR "${file}")
      string (REPLACE ".pkg" "" PAR "${PAR}")
      string (REPLACE "${srcdir}/" "" PAR "${PAR}")

      set(ALIRSNLITE_SRC "$ENV{ALICE_ROOT}")
      if (${PAR})
        message (STATUS "${PAR} custom path : ${${PAR}}")
        set(ALIRSNLITE_SRC "${${PAR}}")
      endif (${PAR})

      if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${file})

#         message(STATUS "${PAR} [ OK ] ${CMAKE_SOURCE_DIR}/${file}")
        message(STATUS "${PAR} [ OK ] ${ALIRSNLITE_SRC}/${file}")

        if(EXISTS ${ALIRSNLITE_SRC}/${file})
          file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${srcdir})
          file(COPY ${ALIRSNLITE_SRC}/${file} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
          execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/cmake/scripts/fix-AliRoot-pkg.sh ${CMAKE_SOURCE_DIR} ${file} ${ALIRSNLITE_SRC})
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

          if(EXISTS ${ALIRSNLITE_SRC}/${srcdir}/PROOF-INF.${PAR})
            file(COPY ${ALIRSNLITE_SRC}/${srcdir}/PROOF-INF.${PAR} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir} PATTERN .svn EXCLUDE)
          else(EXISTS ${ALIRSNLITE_SRC}/${srcdir}/PROOF-INF.${PAR})
            GeneratePROOF_INF(${CMAKE_SOURCE_DIR}/${srcdir}/PROOF-INF.${PAR} ${PAR})
          endif(EXISTS ${ALIRSNLITE_SRC}/${srcdir}/PROOF-INF.${PAR})
          file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${PAR}LinkDef.h DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
          if(EXISTS ${ALIRSNLITE_SRC}/${srcdir}/macros)
            if(EXISTS ${CMAKE_SOURCE_DIR}/${srcdir}/macros)
#               message(STATUS "Macros direcotry ${CMAKE_SOURCE_DIR}/${srcdir}/macros is NOT overwriten ...")
            else(EXISTS ${CMAKE_SOURCE_DIR}/${srcdir}/macros)
              message(STATUS "Copying macro ${ALIRSNLITE_SRC}/${srcdir}/macros to ${CMAKE_SOURCE_DIR}/${srcdir}/macros")
              file(COPY ${ALIRSNLITE_SRC}/${srcdir}/macros DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
            endif(EXISTS ${CMAKE_SOURCE_DIR}/${srcdir}/macros)
          endif(EXISTS ${ALIRSNLITE_SRC}/${srcdir}/macros)
          file(COPY ${CMAKE_SOURCE_DIR}/cmake/scripts/Makefile DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
          execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/cmake/scripts/patch-${PAR}.sh ${PAR} ${ALIRSNLITE_SRC} ${CMAKE_SOURCE_DIR})

        else(EXISTS ${ALIRSNLITE_SRC}/${file})
          message(FATAL_ERROR "${ALIRSNLITE_SRC}/${file} doesn't exists !!!")
        endif(EXISTS ${ALIRSNLITE_SRC}/${file})

      else(NOT EXISTS ${CMAKE_SOURCE_DIR}/${file})
        message(STATUS "${PAR} [ SKIPPED ] ${ALIRSNLITE_SRC}/${file}")
      endif(NOT EXISTS ${CMAKE_SOURCE_DIR}/${file})

    endforeach(file ${ALIRSNLITE_PARS})

    set(SRCS)
    set(HDRS)
    set(ROOTHASXML)

    set(ALIRSNLITE_SRC "$ENV{ALICE_ROOT}")
    execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/cmake/scripts/post-sync.sh ${CMAKE_SOURCE_DIR})

  else(ALIRSNLITE_SYNC STREQUAL "YES")

    if(EXISTS ${CMAKE_SOURCE_DIR}/STEER)
      message(STATUS "AliRsnLite sync is skipped")
    else(EXISTS ${CMAKE_SOURCE_DIR}/STEER)
      message(FATAL_ERROR "First must sync AliRsnLite with AliRoot !!!\ncmake -DALIRSNLITE_SYNC=YES\n")
    endif(EXISTS ${CMAKE_SOURCE_DIR}/STEER)

  endif(ALIRSNLITE_SYNC STREQUAL "YES")

endmacro(AliRsnLite_Sync)

macro(AliRsnLite_KDevelop)

  FIND_PROGRAM(KDEVELOP_EXECUTABLE kdevelop)
  if(KDEVELOP_EXECUTABLE)
    message(STATUS "Generatiing KDevelop settings ...")
    file(COPY ${CMAKE_SOURCE_DIR}/cmake/AliRsnLite.kdev4 DESTINATION ${CMAKE_SOURCE_DIR}/)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/kdev_include_paths.tmp ${CMAKE_SOURCE_DIR}/.kdev_include_paths)
  endif(KDEVELOP_EXECUTABLE)

endmacro(AliRsnLite_KDevelop)

function (GeneratePROOF_INF path parname)
  if(parname)
    message(STATUS "Generating ${parname} ${PARINCLUDE}")
    file(MAKE_DIRECTORY ${path})
    execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/cmake/scripts/generatePARbase.sh ${path} ${parname})
#     execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/cmake/scripts/generateSETUP_C.sh ${path} ${parname} ${PARINCLUDE})
  endif(parname)
endfunction (GeneratePROOF_INF)
