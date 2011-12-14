cmake_minimum_required(VERSION 2.8.1)
project(SourceAnalyzer)

set( SOURCE_ANALYZER_ROOT_DIR ${SourceAnalyzer_SOURCE_DIR} )
set( SOURCE_ANALYZER_BUILD_DIR ${SourceAnalyzer_BINARY_DIR} )

set( APPS
	${SOURCE_ANALYZER_ROOT_DIR}/apps/qsagui
)

set( LIBS
	${SOURCE_ANALYZER_ROOT_DIR}/libs/common/sav3
)

set( EXAMPLES
)

set( TESTS
)

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


