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

cmake_minimum_required( VERSION 2.8.0 )

if( SOURCE_ANALYZER_ROOT_DIR )
    message( STATUS "Building as part of SA. Root dir: ${SOURCE_ANALYZER_ROOT_DIR}" )
    set( COMPONENT_NAME SA_AST )
    set( COMPONENT_ROOT ${SOURCE_ANALYZER_ROOT_DIR}/libs/common/ast )
endif( SOURCE_ANALYZER_ROOT_DIR )

if( NOT COMPONENT_NAME )
    set( COMPONENT_NAME SA_AST )
    project( ${COMPONENT_NAME} )
    set( COMPONENT_ROOT ${${COMPONENT_NAME}_SOURCE_DIR} )
    message( STATUS "Independent build - Setting COMPONENT_NAME to ${COMPONENT_NAME}" )
endif( NOT COMPONENT_NAME )

set( ${COMPONENT_NAME}_OUTPUT_NAME "ast" )

###############################################################################
## Setting paths to the component's directories
set( ${COMPONENT_NAME}_root_dir 	${COMPONENT_ROOT}    		            )
set( ${COMPONENT_NAME}_src_dir		${${COMPONENT_NAME}_root_dir}/src 		)
set( ${COMPONENT_NAME}_include_dir	${${COMPONENT_NAME}_root_dir}/include	)
set( ${COMPONENT_NAME}_test_dir     ${${COMPONENT_NAME}_root_dir}/tests 	)

###############################################################################
## Setting list of src and headers
file (GLOB ${COMPONENT_NAME}_EXTERNAL_HEADERS "${${COMPONENT_NAME}_include_dir}/*.h")
file (GLOB ${COMPONENT_NAME}_INTERNAL_HEADERS "${${COMPONENT_NAME}_src_dir}/*.h"	)
aux_source_directory(${${COMPONENT_NAME}_src_dir} ${COMPONENT_NAME}_SRC)

aux_source_directory(${${COMPONENT_NAME}_test_dir} ${COMPONENT_NAME}_TEST_SRC)
file (GLOB ${COMPONENT_NAME}_TEST_HEADERS "${${COMPONENT_NAME}_test_dir}/*.h"	)

###############################################################################
## Doing real work :)
include_directories( ${${COMPONENT_NAME}_src_dir} ${${COMPONENT_NAME}_include_dir})

add_library( ${${COMPONENT_NAME}_OUTPUT_NAME}
    ${${COMPONENT_NAME}_SRC}
    ${${COMPONENT_NAME}_EXTERNAL_HEADERS}
    ${${COMPONENT_NAME}_INTERNAL_HEADERS}
)

link_directories(/usr/local/lib)
add_executable( test_${${COMPONENT_NAME}_OUTPUT_NAME}
    ${${COMPONENT_NAME}_EXTERNAL_HEADERS}
    ${${COMPONENT_NAME}_INTERNAL_HEADERS}
    ${${COMPONENT_NAME}_TEST_HEADERS}
    ${${COMPONENT_NAME}_TEST_SRC}
)
target_link_libraries(test_${${COMPONENT_NAME}_OUTPUT_NAME}
    ${${COMPONENT_NAME}_OUTPUT_NAME}
    cppunit
    dl
)
