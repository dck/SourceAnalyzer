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

cmake_minimum_required( VERSION 2.8.1 )

if( SOURCE_ANALYZER_ROOT_DIR )
	message( STATUS "Building as part of SA. Root dir: ${SOURCE_ANALYZER_ROOT_DIR}" )
	set( SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR ${SOURCE_ANALYZER_ROOT_DIR}/apps/qsagui )
else( SOURCE_ANALYZER_ROOT_DIR )
	message( STATUS "Building QSAGUI as independent component" )
	project( qsagui )
	set( SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR ${SourceAnalyzer_qsagui_SOURCE_DIR} )
endif( SOURCE_ANALYZER_ROOT_DIR )

find_package(Qt4 REQUIRED)
set(QT_USE_QTSVG true)
include( ${QT_USE_FILE} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

###############################################################################
## Variables set ##############################################################
set( docs_dir    ${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}/docs )
set( include_dir ${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}/include  )
set( src_dir     ${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}/src  )
set( forms_dir   ${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}/forms  )
set( res_dir     ${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}/resources )
set( lib_dir     ${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}/lib )

aux_source_directory( ${src_dir} SOURCES)
file (GLOB MOC_HEADERS "${include_dir}/*.h"	)
file (GLOB FORMS       "${forms_dir}/*.ui"	)

qt4_wrap_ui( UI_HEADERS ${FORMS} )

set( RESOURCES
	${res_dir}/resources.qrc
)
###############################################################################

###############################################################################
## Setting the application icon ##############################################################
if( WIN32 )
    if( MINGW )
        add_custom_command( OUTPUT ${CMAKE_BINARY_DIR}/icon_rc.o
        COMMAND windres.exe -I${SOURCE_ANALYZER_QSAGUI_APP_ROOT_DIR}
        -i${res_dir}/icon.rc
        -o ${CMAKE_BINARY_DIR}/icon_rc.o
	    )
        set( SOURCES ${SOURCES} ${CMAKE_BINARY_DIR}/icon_rc.o )
    else( MINGW )
        set( SOURCES ${SOURCES} ${res_dir}/icon.rc )
    endif( MINGW )
endif( WIN32 )
###############################################################################

qt4_add_resources( QRC_SOURCES ${RESOURCES} )
qt4_wrap_cpp( MOC_SRCS ${MOC_HEADERS} )

###############################################################################
## Path to include ############################################################
include_directories( ${include_dir} )
###############################################################################

add_executable( qsagui WIN32 ${SOURCES} ${MOC_SRCS} ${QRC_SOURCES} ${UI_HEADERS} )
target_link_libraries( qsagui ${QT_LIBRARIES} )

###############################################################################
## Installation packages content ##############################################
install(
    TARGETS
        qsagui
    RUNTIME DESTINATION bin
)

install(
    FILES
        ${docs_dir}/license
        ${docs_dir}/qsagui_usermanual_0.6.pdf
    DESTINATION qsaguidocs
)

if( WIN32 )
    Find_File(qtgui QtGui4.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
    Find_File(qtcore QtCore4.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
	Find_File(qtsvg QtSvg4.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
    Find_File(mingwdll mingwm10.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
	Find_File(libgcc libgcc_s_dw2-1.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
    install(
        FILES
		    ${res_dir}/images/icon.ico
            ${qtgui}
			${qtcore}
			${qtsvg}
			${mingwdll}
			${libgcc}
        DESTINATION bin
    )
endif( WIN32 )

###############################################################################
## Installation package settings ##############################################
if( WIN32 )
    set( CPACK_GENERATOR "TGZ;NSIS" )
    set( CPACK_NSIS_COMPRESSOR "/SOLID lzma" )
    set( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
         createShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\qsagui.lnk\\\" \\\"$INSTDIR\\\\bin\\\\qsagui.exe\\\"
		 CreateShortCut \\\"$DESKTOP\\\\qsagui.lnk\\\" \\\"$INSTDIR\\\\bin\\\\qsagui.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\bin\\\\icon.ico\\\"
")
    set( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
         delete \\\"$SMPROGRAMS\\\\QSAGUI\\\\qsagui.lnk\\\"
		 delete \\\"$DESKTOP\\\\qsagui.lnk\\\"
         RMDir /r \\\"$PROGRAMFILES\\\\QSAGUI\\\"
         RMDir /r \\\"$PROFILE\\\\.qsagui\\\"
		 RMDir /r \\\"$TEMP\\\\qsaguitmpdir\\\"
")
endif(WIN32)

if(UNIX)
    set( CPACK_GENERATOR "TGZ;STGZ;DEB;RPM" )
endif(UNIX)

set(CMAKE_BUILD_TYPE "Release")
set(CPACK_PACKAGE_CONTACT "SourceAnalyzer contributors <sa_team@googlegroups.com>")
set(CPACK_PACKAGE_NAME "qsagui")
set(CPACK_PACKAGE_VENDOR "SourceAnalyzer contributors")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "QSAGUI - graphical user interface for SourceAnalyzer")
set(CPACK_PACKAGE_VERSION "1.1.beta")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "qsagui")
set(CPACK_RESOURCE_FILE_LICENSE "${docs_dir}/licensenote")

include(CPack)
###############################################################################
