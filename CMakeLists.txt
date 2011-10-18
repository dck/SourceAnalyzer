cmake_minimum_required(VERSION 2.6)
project(SourceAnalyzer)

set(COMMON_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GLOBAL_HEADERS  ${COMMON_INCLUDE}/sa3parsers.h
                    ${COMMON_INCLUDE}/sa3status.h
                    ${COMMON_INCLUDE}/sa3tools.h
                    ${COMMON_INCLUDE}/sa3types.h )
set(LOCAL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(LOCAL_HEADERS   ${LOCAL_INCLUDE}/api.h
                    ${LOCAL_INCLUDE}/apicall.h
                    ${LOCAL_INCLUDE}/badarg.h
                    ${LOCAL_INCLUDE}/cg.h
                    ${LOCAL_INCLUDE}/malloc.h
                    ${LOCAL_INCLUDE}/types.h
                    ${LOCAL_INCLUDE}/wrapdb.h )
                   
                    



include_directories(${SourceAnalyzer_SOURCE_DIR}/include
		    ${SourceAnalyzer_SOURCE_DIR}/src
		    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
		    /usr/local/BerkeleyDB.5.1/include)
link_directories(/usr/local/BerkeleyDB.5.1/lib)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/api  SRC_API_TEST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/link SRC_LINK_TEST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/view SRC_VIEW_TEST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/cpar SRC_CPAR_TEST)

add_library(sa3 ${SRC} ${GLOBAL_HEADERS} ${LOCAL_HEADERS})
add_executable(apitest ${SRC_API_TEST})
add_executable(link ${SRC_LINK_TEST})
add_executable(view ${SRC_VIEW_TEST})
add_executable(cpar ${SRC_CPAR_TEST})
target_link_libraries(apitest db)
target_link_libraries(link db)
target_link_libraries(view db)
target_link_libraries(cpar db)

