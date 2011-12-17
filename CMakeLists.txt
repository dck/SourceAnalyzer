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
project(SourceAnalyzer)

include( "cmake/common.cmake" )

set( SOURCE_ANALYZER_ROOT_DIR  ${SourceAnalyzer_SOURCE_DIR} )
set( SOURCE_ANALYZER_BUILD_DIR ${SourceAnalyzer_BINARY_DIR} )

set( APPS
	qsagui
	cpar
	link
	view
)
addPrefix( "${SOURCE_ANALYZER_ROOT_DIR}/apps/" APPS )

set( LIBS
	common/sav3
	common/ast
)
addPrefix( "${SOURCE_ANALYZER_ROOT_DIR}/libs/" LIBS )

set( EXAMPLES
)

set( TESTS
	sav3api
)
addPrefix( "${SOURCE_ANALYZER_ROOT_DIR}/tests/" TESTS )

set( ALL_COMPONENTS 
	${APPS}
	${LIBS}
	${EXAMPLES}
	${TESTS}
)

foreach( component ${ALL_COMPONENTS} )
	set( cmakelists ${component}/CMakeLists.txt )
	if( EXISTS ${cmakelists} )
		message( STATUS "Adding component: ${component}" )
		include( ${cmakelists} )
	else( EXISTS ${cmakelists} )
		message( STATUS "Error: Can't find ${cmakelists}" )
	endif( EXISTS ${cmakelists} )
endforeach(component ${ALL_COMPONENTS} )


