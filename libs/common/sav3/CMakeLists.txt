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

