cmake_minimum_required( VERSION 2.8.0 )
project( AST )

set( COMPONENT_NAME AST )
set( ${COMPONENT_NAME}_ROOT_DIR ${AST_SOUCE_DIR} )

aux_source_directory(${AST_SOURCE_DIR}/src AST_SRC)

include_directories( ${AST_SOURCE_DIR}/include  )

add_executable( ast ${AST_SRC})

