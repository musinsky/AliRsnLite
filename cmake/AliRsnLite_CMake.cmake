cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)



macro(AliRsnLite_Sync)

  set(ALIRSNLITE_DIRS_RM STEER ANALYSIS)

  if(ALIRSNLITE_SYNC STREQUAL "YES")
    message(STATUS "Yes i want to sync")

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

    foreach(myrmdir ${ALIRSNLITE_DIRS_RM})
      file(REMOVE_RECURSE ${myrmdir})
    endforeach(myrmdir ${ALIRSNLITE_DIRS_RM})

    foreach(file ${ALIRSNLITE_PARS})
      if(EXISTS ${ALIRSNLITE_SRC}/${file})
#       message(STATUS "${ALIRSNLITE_SRC}/${file} is there")
	get_filename_component(srcdir ${file} PATH)
	file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${srcdir})
	file(COPY ${ALIRSNLITE_SRC}/${file} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
# 	message(STATUS "myfile  ${CMAKE_SOURCE_DIR}/${file}")

	include(${CMAKE_SOURCE_DIR}/${file})
	
	foreach(myfile ${SRCS})
	   get_filename_component(myfiledir ${myfile} PATH)
	   file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${myfile} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir}/${myfiledir})
# 	   message(STATUS "*.cxx ${ALIRSNLITE_SRC}/${srcdir}/${myfile}")
	endforeach(myfile ${SRCS})

	string (REPLACE ".cxx" ".h" HDRS "${SRCS}")
	foreach(myfile ${HDRS})
	   get_filename_component(myfiledir ${myfile} PATH)
	   file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${myfile} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir}/${myfiledir})
# 	   message(STATUS "*.h ${ALIRSNLITE_SRC}/${srcdir}/${myfile}")
	endforeach(myfile ${HDRS})

	# take PROOF-INF
	string (REPLACE "CMakelib" "" PAR "${file}")
	string (REPLACE ".pkg" "" PAR "${PAR}")
	string (REPLACE "${srcdir}/" "" PAR "${PAR}")
# 	message(STATUS "PAR ${PAR}")
	file(COPY ${ALIRSNLITE_SRC}/${srcdir}/PROOF-INF.${PAR} DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
	file(COPY ${ALIRSNLITE_SRC}/${srcdir}/${PAR}LinkDef.h DESTINATION ${CMAKE_SOURCE_DIR}/${srcdir})
#   echo "set ( EXPORT \${HDRS})" >> $2/PWG2/CMakelibPWG2resonances.pkg
	execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/scripts/patch-${PAR}.sh ${PAR} ${CMAKE_SOURCE_DIR})
      endif(EXISTS ${ALIRSNLITE_SRC}/${file})
    endforeach(file ${ALIRSNLITE_PARS})

    # remove tmp dir
#     file(REMOVE_RECURSE ${MYTMPDIR})
 

  endif(ALIRSNLITE_SYNC STREQUAL "YES")
#   execute_process(COMMAND find ${CMAKE_SOURCE_DIR} -name "*~" -exec rm -f {} \; )
  execute_process(COMMAND "find ./ -name \".svn\" | xargs rm -Rf")

  message(STATUS "ALIRSNLITE_SRC is ${ALIRSNLITE_SRC}")

  set(SRCS)
  set(HDRS)
  set(ROOTHASXML)

endmacro(AliRsnLite_Sync)
