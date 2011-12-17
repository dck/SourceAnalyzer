# Copyright 2008-2011 SourceAnalyzer team. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
# 
#    1. Redistributions of source code must retain the above copyright notice, this list of
#       conditions and the following disclaimer.
# 
#    2. Redistributions in binary form must reproduce the above copyright notice, this list
#       of conditions and the following disclaimer in the documentation and/or other materials
#       provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY SOURCEANALYZER TEAM ''AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SOURCEANALYZER TEAM OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# The views and conclusions contained in the software and documentation are those of the
# authors and should not be interpreted as representing official policies, either expressed
# or implied, of SourceAnalyzer team.

cmake_minimum_required(VERSION 2.8.1)

# If we run this CMakelist as part of the total SA build
if( SOURCE_ANALYZER_ROOT_DIR )
	message( STATUS "Building as part of SA. Root dir: ${SOURCE_ANALYZER_ROOT_DIR}" )
	set( SOURCE_ANALYZER_SAv3_lib_ROOT_DIR ${SOURCE_ANALYZER_ROOT_DIR}/libs/common/sav3 )
else( SOURCE_ANALYZER_ROOT_DIR )
	message( STATUS "Building SAv3 as independent component" )
	project(SourceAnalyzer_sav3_lib)
	set( SOURCE_ANALYZER_SAv3_lib_ROOT_DIR ${SourceAnalyzer_sav3_lib_SOURCE_DIR} )
endif( SOURCE_ANALYZER_ROOT_DIR )

# common includes
set(COMMON_INCLUDE ${SOURCE_ANALYZER_SAv3_lib_ROOT_DIR}/include)
file (GLOB COMMON_HEADERS "${COMMON_INCLUDE}/*.h")
set(LOCAL_INCLUDE ${SOURCE_ANALYZER_SAv3_lib_ROOT_DIR}/src/include)
file (GLOB LOCAL_HEADERS "${LOCAL_INCLUDE}/*.h")
                   
include_directories(${SOURCE_ANALYZER_SAv3_lib_ROOT_DIR}/include
		    ${SOURCE_ANALYZER_SAv3_lib_ROOT_DIR}/src
		    ${SOURCE_ANALYZER_SAv3_lib_ROOT_DIR}/src/include
		    /usr/local/BerkeleyDB.5.1/include)
		    
link_directories(/usr/local/BerkeleyDB.5.1/lib)
aux_source_directory(${SOURCE_ANALYZER_SAv3_lib_ROOT_DIR}/src SRC)

#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/api  SRC_API_TEST)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/link SRC_LINK_TEST)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/view SRC_VIEW_TEST)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/cpar SRC_CPAR_TEST)

add_library(sa3 ${SRC} ${COMMON_HEADERS} ${LOCAL_HEADERS})
#add_executable(apitest ${SRC_API_TEST})
#add_executable(link ${SRC_LINK_TEST})
#add_executable(view ${SRC_VIEW_TEST})
#add_executable(cpar ${SRC_CPAR_TEST})
#target_link_libraries(apitest db)
#target_link_libraries(link db)
#target_link_libraries(view db)
#target_link_libraries(cpar db)

